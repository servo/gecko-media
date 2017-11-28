/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "ImageContainer.h"
#include <string.h>                     // for memcpy, memset
// #include "GLImages.h"                   // for SurfaceTextureImage
// #include "gfx2DGlue.h"
// #include "gfxPlatform.h"                // for gfxPlatform
// #include "gfxUtils.h"                   // for gfxUtils
// #include "libyuv.h"
#include "mozilla/RefPtr.h"             // for already_AddRefed
// #include "mozilla/ipc/CrossProcessMutex.h"  // for CrossProcessMutex, etc
// #include "mozilla/layers/CompositorTypes.h"
// #include "mozilla/layers/ImageBridgeChild.h"  // for ImageBridgeChild
// #include "mozilla/layers/ImageClient.h"  // for ImageClient
// #include "mozilla/layers/LayersMessages.h"
// #include "mozilla/layers/SharedPlanarYCbCrImage.h"
// #include "mozilla/layers/SharedRGBImage.h"
// #include "mozilla/layers/TextureClientRecycleAllocator.h"
// #include "mozilla/gfx/gfxVars.h"
#include "nsISupportsUtils.h"           // for NS_IF_ADDREF
#include "YCbCrUtils.h"                 // for YCbCr conversions
// #include "gfx2DGlue.h"
#include "mozilla/gfx/2D.h"
#include "mozilla/CheckedInt.h"

// #ifdef XP_MACOSX
// #include "mozilla/gfx/QuartzSupport.h"
// #endif

// #ifdef XP_WIN
// #include "gfxWindowsPlatform.h"
// #include <d3d10_1.h>
// #include "mozilla/gfx/DeviceManagerDx.h"
// #include "mozilla/layers/D3D11YCbCrImage.h"
// #endif

#include "mozilla/AbstractThread.h"
#include "GeckoMediaDecoderOwner.h"
#include "RustServices.h"
#include "nsRefPtrHashtable.h"
#include "mozilla/Assertions.h"

namespace mozilla {

namespace gfx {

class gfxVars {
public:
  static gfxImageFormat OffscreenFormat() {
    #ifdef XP_WIDGET_ANDROID
    #error Someone needs to figure out the screen pixel depth for gfxVars::OffscreenFormat()!
    #else
    return mozilla::gfx::SurfaceFormat::X8R8G8B8_UINT32;
    #endif
  }
};

} // namespacec gfx

namespace layers {

// using namespace mozilla::ipc;
// using namespace android;
using namespace mozilla::gfx;

Atomic<int32_t> Image::sSerialCounter(0);

Atomic<uint32_t> ImageContainer::sGenerationCounter(0);

RefPtr<PlanarYCbCrImage>
ImageFactory::CreatePlanarYCbCrImage(const gfx::IntSize& aScaleHint, BufferRecycleBin *aRecycleBin)
{
  return new RecyclingPlanarYCbCrImage(aRecycleBin);
}

BufferRecycleBin::BufferRecycleBin()
  : mLock("mozilla.layers.BufferRecycleBin.mLock")
  // This member is only valid when the bin is not empty and will be properly
  // initialized in RecycleBuffer, but initializing it here avoids static analysis
  // noise.
  , mRecycledBufferSize(0)
{
}

void
BufferRecycleBin::RecycleBuffer(UniquePtr<uint8_t[]> aBuffer, uint32_t aSize)
{
  MutexAutoLock lock(mLock);

  if (!mRecycledBuffers.IsEmpty() && aSize != mRecycledBufferSize) {
    mRecycledBuffers.Clear();
  }
  mRecycledBufferSize = aSize;
  mRecycledBuffers.AppendElement(Move(aBuffer));
}

UniquePtr<uint8_t[]>
BufferRecycleBin::GetBuffer(uint32_t aSize)
{
  MutexAutoLock lock(mLock);

  if (mRecycledBuffers.IsEmpty() || mRecycledBufferSize != aSize)
    return MakeUnique<uint8_t[]>(aSize);

  uint32_t last = mRecycledBuffers.Length() - 1;
  UniquePtr<uint8_t[]> result = Move(mRecycledBuffers[last]);
  mRecycledBuffers.RemoveElementAt(last);
  return result;
}

void
BufferRecycleBin::ClearRecycledBuffers()
{
  MutexAutoLock lock(mLock);
  if (!mRecycledBuffers.IsEmpty()) {
    mRecycledBuffers.Clear();
  }
  mRecycledBufferSize = 0;
}

// ImageContainerListener::ImageContainerListener(ImageContainer* aImageContainer)
//   : mLock("mozilla.layers.ImageContainerListener.mLock")
//   , mImageContainer(aImageContainer)
// {
// }

// ImageContainerListener::~ImageContainerListener()
// {
// }

// void
// ImageContainerListener::NotifyComposite(const ImageCompositeNotification& aNotification)
// {
//   MutexAutoLock lock(mLock);
//   if (mImageContainer) {
//     mImageContainer->NotifyComposite(aNotification);
//   }
// }

// void
// ImageContainerListener::ClearImageContainer()
// {
//   MutexAutoLock lock(mLock);
//   mImageContainer = nullptr;
// }

// void
// ImageContainerListener::DropImageClient()
// {
//   MutexAutoLock lock(mLock);
//   if (mImageContainer) {
//     mImageContainer->DropImageClient();
//   }
// }

// already_AddRefed<ImageClient>
// ImageContainer::GetImageClient()
// {
//   RecursiveMutexAutoLock mon(mRecursiveMutex);
//   EnsureImageClient();
//   RefPtr<ImageClient> imageClient = mImageClient;
//   return imageClient.forget();
// }

// void
// ImageContainer::DropImageClient()
// {
//   RecursiveMutexAutoLock mon(mRecursiveMutex);
//   if (mImageClient) {
//     mImageClient->ClearCachedResources();
//     mImageClient = nullptr;
//   }
// }

// void
// ImageContainer::EnsureImageClient()
// {
//   // If we're not forcing a new ImageClient, then we can skip this if we don't have an existing
//   // ImageClient, or if the existing one belongs to an IPC actor that is still open.
//   if (!mIsAsync) {
//     return;
//   }
//   if (mImageClient && mImageClient->GetForwarder()->GetLayersIPCActor()->IPCOpen()) {
//     return;
//   }

//   RefPtr<ImageBridgeChild> imageBridge = ImageBridgeChild::GetSingleton();
//   if (imageBridge) {
//     mImageClient = imageBridge->CreateImageClient(CompositableType::IMAGE, this);
//     if (mImageClient) {
//       mAsyncContainerHandle = mImageClient->GetAsyncHandle();
//     } else {
//       // It's okay to drop the async container handle since the ImageBridgeChild
//       // is going to die anyway.
//       mAsyncContainerHandle = CompositableHandle();
//     }
//   }
// }

ImageContainer::ImageContainer(GeckoMediaDecoderOwner* aOwner)
: mRecursiveMutex("ImageContainer.mRecursiveMutex"),
  mGenerationCounter(++sGenerationCounter),
  mPaintCount(0),
  mDroppedImageCount(0),
  mImageFactory(new ImageFactory()),
  mRecycleBin(new BufferRecycleBin()),
  // mIsAsync(flag == ASYNCHRONOUS),
  mCurrentProducerID(-1),
  mOwner(aOwner)
{
  // if (flag == ASYNCHRONOUS) {
  //   mNotifyCompositeListener = new ImageContainerListener(this);
  //   EnsureImageClient();
  // }
}

// ImageContainer::ImageContainer(const CompositableHandle& aHandle)
//   : mRecursiveMutex("ImageContainer.mRecursiveMutex"),
//   mGenerationCounter(++sGenerationCounter),
//   mPaintCount(0),
//   mDroppedImageCount(0),
//   mImageFactory(nullptr),
//   mRecycleBin(nullptr),
//   mIsAsync(true),
//   // mAsyncContainerHandle(aHandle),
//   mCurrentProducerID(-1)
// {
//   // MOZ_ASSERT(mAsyncContainerHandle);
// }

ImageContainer::~ImageContainer()
{
  // if (mNotifyCompositeListener) {
  //   mNotifyCompositeListener->ClearImageContainer();
  // }
  // if (mAsyncContainerHandle) {
  //   if (RefPtr<ImageBridgeChild> imageBridge = ImageBridgeChild::GetSingleton()) {
  //     imageBridge->ForgetImageContainer(mAsyncContainerHandle);
  //   }
  // }
}

RefPtr<PlanarYCbCrImage>
ImageContainer::CreatePlanarYCbCrImage()
{
  RecursiveMutexAutoLock lock(mRecursiveMutex);
  // EnsureImageClient();
  // if (mImageClient && mImageClient->AsImageClientSingle()) {
  //   return new SharedPlanarYCbCrImage(mImageClient);
  // }
  return mImageFactory->CreatePlanarYCbCrImage(mScaleHint, mRecycleBin);
}

// RefPtr<SharedRGBImage>
// ImageContainer::CreateSharedRGBImage()
// {
//   RecursiveMutexAutoLock lock(mRecursiveMutex);
//   EnsureImageClient();
//   if (!mImageClient || !mImageClient->AsImageClientSingle()) {
//     return nullptr;
//   }
//   return new SharedRGBImage(mImageClient);
// }

void
ImageContainer::SetCurrentImageInternal(const nsTArray<NonOwningImage>& aImages)
{
  RecursiveMutexAutoLock lock(mRecursiveMutex);

  mGenerationCounter = ++sGenerationCounter;

  if (!aImages.IsEmpty()) {
    NS_ASSERTION(mCurrentImages.IsEmpty() ||
                 mCurrentImages[0].mProducerID != aImages[0].mProducerID ||
                 mCurrentImages[0].mFrameID <= aImages[0].mFrameID,
                 "frame IDs shouldn't go backwards");
    if (aImages[0].mProducerID != mCurrentProducerID) {
      mFrameIDsNotYetComposited.Clear();
      mCurrentProducerID = aImages[0].mProducerID;
    } else if (!aImages[0].mTimeStamp.IsNull()) {
      // Check for expired frames
      for (auto& img : mCurrentImages) {
        if (img.mProducerID != aImages[0].mProducerID ||
            img.mTimeStamp.IsNull() ||
            img.mTimeStamp >= aImages[0].mTimeStamp) {
          break;
        }
        if (!img.mComposited && !img.mTimeStamp.IsNull() &&
            img.mFrameID != aImages[0].mFrameID) {
          mFrameIDsNotYetComposited.AppendElement(img.mFrameID);
        }
      }

      // Remove really old frames, assuming they'll never be composited.
      const uint32_t maxFrames = 100;
      if (mFrameIDsNotYetComposited.Length() > maxFrames) {
        uint32_t dropFrames = mFrameIDsNotYetComposited.Length() - maxFrames;
        mDroppedImageCount += dropFrames;
        mFrameIDsNotYetComposited.RemoveElementsAt(0, dropFrames);
      }
    }
  }

  nsTArray<OwningImage> newImages;

  for (uint32_t i = 0; i < aImages.Length(); ++i) {
    NS_ASSERTION(aImages[i].mImage, "image can't be null");
    NS_ASSERTION(!aImages[i].mTimeStamp.IsNull() || aImages.Length() == 1,
                 "Multiple images require timestamps");
    if (i > 0) {
      NS_ASSERTION(aImages[i].mTimeStamp >= aImages[i - 1].mTimeStamp,
                   "Timestamps must not decrease");
      NS_ASSERTION(aImages[i].mFrameID > aImages[i - 1].mFrameID,
                   "FrameIDs must increase");
      NS_ASSERTION(aImages[i].mProducerID == aImages[i - 1].mProducerID,
                   "ProducerIDs must be the same");
    }
    OwningImage* img = newImages.AppendElement();
    img->mImage = aImages[i].mImage;
    img->mTimeStamp = aImages[i].mTimeStamp;
    img->mFrameID = aImages[i].mFrameID;
    img->mProducerID = aImages[i].mProducerID;
    for (auto& oldImg : mCurrentImages) {
      if (oldImg.mFrameID == img->mFrameID &&
          oldImg.mProducerID == img->mProducerID) {
        img->mComposited = oldImg.mComposited;
        break;
      }
    }
  }

  mCurrentImages.SwapElements(newImages);

  NotifyOwnerOfNewImages();
}

class UpdateCurrentImagesRunnable : public Runnable
{
public:
  UpdateCurrentImagesRunnable(GeckoMediaDecoderOwner* aOwner,
                              nsTArray<GeckoPlanarYCbCrImage>&& aImages)
    : Runnable("UpdateCurrentImagesRunnable")
    , mOwner(aOwner)
    , mImages(Move(aImages))
  {
  }
  NS_IMETHOD Run() {
    MOZ_ASSERT(NS_IsMainThread());
    mOwner->UpdateCurrentImages(Move(mImages));
    // Release owner on main thread.
    mOwner = nullptr;
    return NS_OK;
  }
private:
  nsTArray<GeckoPlanarYCbCrImage> mImages;
  RefPtr<GeckoMediaDecoderOwner> mOwner;
};

StaticMutex sImageMutex;
static nsRefPtrHashtable<nsUint32HashKey, Image> sImages;
static nsDataHashtable<nsUint32HashKey, uint32_t> sImageFFIRefCnt;

void PlanarYCbCrImage_AddRefPixelData(uint32_t aFrameID)
{
  StaticMutexAutoLock lock(sImageMutex);
  MOZ_ASSERT(sImages.Contains(aFrameID));
  MOZ_ASSERT(sImageFFIRefCnt.Contains(aFrameID));

  uint32_t* refcnt = sImageFFIRefCnt.GetValue(aFrameID);
  MOZ_ASSERT(refcnt && *refcnt);
  *refcnt += 1;
}

void PlanarYCbCrImage_FreeData(uint32_t aFrameID) {
  StaticMutexAutoLock lock(sImageMutex);
  MOZ_ASSERT(sImages.Contains(aFrameID));
  MOZ_ASSERT(sImageFFIRefCnt.Contains(aFrameID));

  uint32_t* refcnt = sImageFFIRefCnt.GetValue(aFrameID);
  MOZ_ASSERT(refcnt && *refcnt);
  *refcnt -= 1;

  if (*refcnt == 0) {
    sImageFFIRefCnt.Remove(aFrameID);
    sImages.Remove(aFrameID);
  }
}

const uint8_t*
PlanarYCbCrImage_GetPixelData(uint32_t aFrameID, PlaneType aPlaneType)
{
  RefPtr<Image> img;
  {
    StaticMutexAutoLock lock(sImageMutex);
    MOZ_ASSERT(sImages.Contains(aFrameID));
    if (!sImages.Get(aFrameID, getter_AddRefs(img))) {
      return nullptr;
    }
  }
  PlanarYCbCrImage* planarImage = img->AsPlanarYCbCrImage();
  MOZ_ASSERT(planarImage);
  if (!planarImage) {
    return nullptr;
  }
  const PlanarYCbCrData* data = planarImage->GetData();
  switch (aPlaneType) {
    case PlaneType::Y: return data->mYChannel;
    case PlaneType::Cb: return data->mCbChannel;
    case PlaneType::Cr: return data->mCrChannel;
  }
  return nullptr;
}

void
ImageContainer::NotifyOwnerOfNewImages()
{
  if (!mOwner) {
    return;
  }

  nsTArray<GeckoPlanarYCbCrImage> images;
  for (const OwningImage& owningImage : mCurrentImages) {

    PlanarYCbCrImage* planarImage = owningImage.mImage->AsPlanarYCbCrImage();
    MOZ_ASSERT(planarImage);
    if (!planarImage) {
      continue;
    }
    const PlanarYCbCrData* data = planarImage->GetData();

    GeckoPlanarYCbCrImage* img = images.AppendElement();

    img->mYStride = data->mYStride;
    img->mYWidth = data->mYSize.width;
    img->mYHeight = data->mYSize.height;
    img->mYSkip = data->mYSkip;
    img->mCbCrStride = data->mCbCrStride;
    img->mCbCrWidth = data->mCbCrSize.width;
    img->mCbCrHeight = data->mCbCrSize.height;
    img->mCbSkip = data->mCbSkip;
    img->mCrSkip = data->mCrSkip;
    img->mPicX = data->mPicX;
    img->mPicY = data->mPicY;
    img->mPicWidth = data->mPicSize.width;
    img->mPicHeight = data->mPicSize.height;
    img->mFrameID = owningImage.mFrameID;

    // Calculate a TimeStamp in the external frame of reference.
    // Note the OwningImage can have a null TimeStamp if we're
    // in the middle of shutting down. TimeStamp arithmetic asserts
    // if performed on a null TimeStamp, so guard against having
    // a null TimeStamp here.
    uint64_t rustTime = RustServices::TimeNow();
    TimeStamp now = TimeStamp::Now();
    TimeStamp frameTime = !owningImage.mTimeStamp.IsNull() ? owningImage.mTimeStamp : now;
    img->mTimeStamp = rustTime + (frameTime - now).ToMicroseconds() * 1000.0;

    {
      StaticMutexAutoLock lock(sImageMutex);
      if (!sImages.Contains(img->mFrameID)) {
        sImages.Put(img->mFrameID, owningImage.mImage);
        sImageFFIRefCnt.Put(img->mFrameID, 1);
      } else {
        uint32_t* refcnt = sImageFFIRefCnt.GetValue(img->mFrameID);
        MOZ_ASSERT(refcnt && *refcnt);
        *refcnt += 1;
      }
    }

    img->mAddRefPixelData = &PlanarYCbCrImage_AddRefPixelData;
    img->mFreePixelData = &PlanarYCbCrImage_FreeData;
    img->mGetPixelData = &PlanarYCbCrImage_GetPixelData;
  }

  RefPtr<Runnable> task = new UpdateCurrentImagesRunnable(mOwner, Move(images));
  RefPtr<AbstractThread> thread = AbstractThread::MainThread();
  thread->Dispatch(task.forget());
}

// void
// ImageContainer::ClearImagesFromImageBridge()
// {
//   RecursiveMutexAutoLock lock(mRecursiveMutex);
//   SetCurrentImageInternal(nsTArray<NonOwningImage>());
// }

void
ImageContainer::SetCurrentImages(const nsTArray<NonOwningImage>& aImages)
{
  MOZ_ASSERT(!aImages.IsEmpty());
  RecursiveMutexAutoLock lock(mRecursiveMutex);
  // if (mIsAsync) {
  //   if (RefPtr<ImageBridgeChild> imageBridge = ImageBridgeChild::GetSingleton()) {
  //     imageBridge->UpdateImageClient(this);
  //   }
  // }
  SetCurrentImageInternal(aImages);
}

void
ImageContainer::ClearAllImages()
{
  // if (mImageClient) {
  //   // Let ImageClient release all TextureClients. This doesn't return
  //   // until ImageBridge has called ClearCurrentImageFromImageBridge.
  //   if (RefPtr<ImageBridgeChild> imageBridge = ImageBridgeChild::GetSingleton()) {
  //     imageBridge->FlushAllImages(mImageClient, this);
  //   }
  //   return;
  // }

  RecursiveMutexAutoLock lock(mRecursiveMutex);
  SetCurrentImageInternal(nsTArray<NonOwningImage>());
}

void
ImageContainer::ClearCachedResources()
{
  RecursiveMutexAutoLock lock(mRecursiveMutex);
  // if (mImageClient && mImageClient->AsImageClientSingle()) {
  //   if (!mImageClient->HasTextureClientRecycler()) {
  //     return;
  //   }
  //   mImageClient->GetTextureClientRecycler()->ShrinkToMinimumSize();
  //   return;
  // }
  return mRecycleBin->ClearRecycledBuffers();
}

void
ImageContainer::SetCurrentImageInTransaction(Image *aImage)
{
  AutoTArray<NonOwningImage,1> images;
  images.AppendElement(NonOwningImage(aImage));
  SetCurrentImagesInTransaction(images);
}

void
ImageContainer::SetCurrentImagesInTransaction(const nsTArray<NonOwningImage>& aImages)
{
  NS_ASSERTION(NS_IsMainThread(), "Should be on main thread.");
  // NS_ASSERTION(!mImageClient, "Should use async image transfer with ImageBridge.");

  SetCurrentImageInternal(aImages);
}

// bool ImageContainer::IsAsync() const
// {
//   return mIsAsync;
// }

// CompositableHandle ImageContainer::GetAsyncContainerHandle()
// {
//   NS_ASSERTION(IsAsync(), "Shared image ID is only relevant to async ImageContainers");
//   NS_ASSERTION(mAsyncContainerHandle, "Should have a shared image ID");
//   RecursiveMutexAutoLock mon(mRecursiveMutex);
//   EnsureImageClient();
//   return mAsyncContainerHandle;
// }

bool
ImageContainer::HasCurrentImage()
{
  RecursiveMutexAutoLock lock(mRecursiveMutex);

  return !mCurrentImages.IsEmpty();
}

void
ImageContainer::GetCurrentImages(nsTArray<OwningImage>* aImages,
                                 uint32_t* aGenerationCounter)
{
  RecursiveMutexAutoLock lock(mRecursiveMutex);

  *aImages = mCurrentImages;
  if (aGenerationCounter) {
    *aGenerationCounter = mGenerationCounter;
  }
}

gfx::IntSize
ImageContainer::GetCurrentSize()
{
  RecursiveMutexAutoLock lock(mRecursiveMutex);

  if (mCurrentImages.IsEmpty()) {
    return gfx::IntSize(0, 0);
  }

  return mCurrentImages[0].mImage->GetSize();
}

// void
// ImageContainer::NotifyComposite(const ImageCompositeNotification& aNotification)
// {
//   RecursiveMutexAutoLock lock(mRecursiveMutex);

//   // An image composition notification is sent the first time a particular
//   // image is composited by an ImageHost. Thus, every time we receive such
//   // a notification, a new image has been painted.
//   ++mPaintCount;

//   if (aNotification.producerID() == mCurrentProducerID) {
//     uint32_t i;
//     for (i = 0; i < mFrameIDsNotYetComposited.Length(); ++i) {
//       if (mFrameIDsNotYetComposited[i] <= aNotification.frameID()) {
//         if (mFrameIDsNotYetComposited[i] < aNotification.frameID()) {
//           ++mDroppedImageCount;
//         }
//       } else {
//         break;
//       }
//     }
//     mFrameIDsNotYetComposited.RemoveElementsAt(0, i);
//     for (auto& img : mCurrentImages) {
//       if (img.mFrameID == aNotification.frameID()) {
//         img.mComposited = true;
//       }
//     }
//   }

//   if (!aNotification.imageTimeStamp().IsNull()) {
//     mPaintDelay = aNotification.firstCompositeTimeStamp() -
//         aNotification.imageTimeStamp();
//   }
// }

#ifdef XP_WIN
D3D11YCbCrRecycleAllocator*
ImageContainer::GetD3D11YCbCrRecycleAllocator(KnowsCompositor* aAllocator)
{
  if (mD3D11YCbCrRecycleAllocator &&
      aAllocator == mD3D11YCbCrRecycleAllocator->GetAllocator()) {
    return mD3D11YCbCrRecycleAllocator;
  }

  RefPtr<ID3D11Device> device = gfx::DeviceManagerDx::Get()->GetContentDevice();
  if (!device) {
    device = gfx::DeviceManagerDx::Get()->GetCompositorDevice();
  }

  LayersBackend backend = aAllocator->GetCompositorBackendType();
  if (!device || backend != LayersBackend::LAYERS_D3D11) {
    return nullptr;
  }

  RefPtr<ID3D10Multithread> multi;
  HRESULT hr =
    device->QueryInterface((ID3D10Multithread**)getter_AddRefs(multi));
  if (FAILED(hr) || !multi) {
    gfxWarning() << "Multithread safety interface not supported. " << hr;
    return nullptr;
  }
  multi->SetMultithreadProtected(TRUE);

  mD3D11YCbCrRecycleAllocator =
    new D3D11YCbCrRecycleAllocator(aAllocator, device);
  return mD3D11YCbCrRecycleAllocator;
}
#endif

PlanarYCbCrImage::PlanarYCbCrImage()
  : Image(nullptr, ImageFormat::PLANAR_YCBCR)
  , mOffscreenFormat(SurfaceFormat::UNKNOWN)
  , mBufferSize(0)
{
}

RecyclingPlanarYCbCrImage::~RecyclingPlanarYCbCrImage()
{
  if (mBuffer) {
    mRecycleBin->RecycleBuffer(Move(mBuffer), mBufferSize);
  }
}

size_t
RecyclingPlanarYCbCrImage::SizeOfExcludingThis(MallocSizeOf aMallocSizeOf) const
{
  // Ignoring:
  // - mData - just wraps mBuffer
  // - Surfaces should be reported under gfx-surfaces-*:
  //   - mSourceSurface
  // - Base class:
  //   - mImplData is not used
  // Not owned:
  // - mRecycleBin
  size_t size = aMallocSizeOf(mBuffer.get());

  // Could add in the future:
  // - mBackendData (from base class)

  return size;
}

UniquePtr<uint8_t[]>
RecyclingPlanarYCbCrImage::AllocateBuffer(uint32_t aSize)
{
  return mRecycleBin->GetBuffer(aSize);
}

static void
CopyPlane(uint8_t *aDst, const uint8_t *aSrc,
          const gfx::IntSize &aSize, int32_t aStride, int32_t aSkip)
{
  if (!aSkip) {
    // Fast path: planar input.
    memcpy(aDst, aSrc, aSize.height * aStride);
  } else {
    for (int i = 0; i < aSize.height; i++) {
      const uint8_t* rowSrc = aSrc + aStride * i;
      for (int j = 0; j < aSize.width; j++) {
        *aDst = *rowSrc;
        aDst++;
        rowSrc += aSkip + 1;
      }
    }
  }
}

bool
RecyclingPlanarYCbCrImage::CopyData(const Data& aData)
{
  mData = aData;

  // update buffer size
  // Use uint32_t throughout to match AllocateBuffer's param and mBufferSize
  const auto checkedSize =
    CheckedInt<uint32_t>(mData.mCbCrStride) * mData.mCbCrSize.height * 2 +
    CheckedInt<uint32_t>(mData.mYStride) * mData.mYSize.height;

  if (!checkedSize.isValid())
    return false;

  const auto size = checkedSize.value();

  // get new buffer
  mBuffer = AllocateBuffer(size);
  if (!mBuffer)
    return false;

  // update buffer size
  mBufferSize = size;

  mData.mYChannel = mBuffer.get();
  mData.mCbChannel = mData.mYChannel + mData.mYStride * mData.mYSize.height;
  mData.mCrChannel = mData.mCbChannel + mData.mCbCrStride * mData.mCbCrSize.height;

  CopyPlane(mData.mYChannel, aData.mYChannel,
            mData.mYSize, mData.mYStride, mData.mYSkip);
  CopyPlane(mData.mCbChannel, aData.mCbChannel,
            mData.mCbCrSize, mData.mCbCrStride, mData.mCbSkip);
  CopyPlane(mData.mCrChannel, aData.mCrChannel,
            mData.mCbCrSize, mData.mCbCrStride, mData.mCrSkip);

  mSize = aData.mPicSize;
  mOrigin = gfx::IntPoint(aData.mPicX, aData.mPicY);
  return true;
}

gfxImageFormat
PlanarYCbCrImage::GetOffscreenFormat()
{
  return mOffscreenFormat == SurfaceFormat::UNKNOWN ?
    gfxVars::OffscreenFormat() :
    mOffscreenFormat;
}

bool
PlanarYCbCrImage::AdoptData(const Data &aData)
{
  mData = aData;
  mSize = aData.mPicSize;
  mOrigin = gfx::IntPoint(aData.mPicX, aData.mPicY);
  return true;
}

uint8_t*
RecyclingPlanarYCbCrImage::AllocateAndGetNewBuffer(uint32_t aSize)
{
  // get new buffer
  mBuffer = AllocateBuffer(aSize);
  if (mBuffer) {
    // update buffer size
    mBufferSize = aSize;
  }
  return mBuffer.get();
}

// already_AddRefed<gfx::SourceSurface>
// PlanarYCbCrImage::GetAsSourceSurface()
// {
//   if (mSourceSurface) {
//     RefPtr<gfx::SourceSurface> surface(mSourceSurface);
//     return surface.forget();
//   }

//   gfx::IntSize size(mSize);
//   gfx::SurfaceFormat format = gfx::ImageFormatToSurfaceFormat(GetOffscreenFormat());
//   gfx::GetYCbCrToRGBDestFormatAndSize(mData, format, size);
//   if (mSize.width > PlanarYCbCrImage::MAX_DIMENSION ||
//       mSize.height > PlanarYCbCrImage::MAX_DIMENSION) {
//     NS_ERROR("Illegal image dest width or height");
//     return nullptr;
//   }

//   RefPtr<gfx::DataSourceSurface> surface = gfx::Factory::CreateDataSourceSurface(size, format);
//   if (NS_WARN_IF(!surface)) {
//     return nullptr;
//   }

//   DataSourceSurface::ScopedMap mapping(surface, DataSourceSurface::WRITE);
//   if (NS_WARN_IF(!mapping.IsMapped())) {
//     return nullptr;
//   }

//   gfx::ConvertYCbCrToRGB(mData, format, size, mapping.GetData(), mapping.GetStride());

//   mSourceSurface = surface;

//   return surface.forget();
// }

// NVImage::NVImage()
//   : Image(nullptr, ImageFormat::NV_IMAGE)
//   , mBufferSize(0)
// {
// }

// NVImage::~NVImage() = default;

// IntSize
// NVImage::GetSize()
// {
//   return mSize;
// }

// IntRect
// NVImage::GetPictureRect()
// {
//   return mData.GetPictureRect();
// }

// already_AddRefed<SourceSurface>
// NVImage::GetAsSourceSurface()
// {
//   if (mSourceSurface) {
//     RefPtr<gfx::SourceSurface> surface(mSourceSurface);
//     return surface.forget();
//   }

//   // Convert the current NV12 or NV21 data to YUV420P so that we can follow the
//   // logics in PlanarYCbCrImage::GetAsSourceSurface().
//   const int bufferLength = mData.mYSize.height * mData.mYStride +
//                            mData.mCbCrSize.height * mData.mCbCrSize.width * 2;
//   auto *buffer = new uint8_t[bufferLength];

//   Data aData = mData;
//   aData.mCbCrStride = aData.mCbCrSize.width;
//   aData.mCbSkip = 0;
//   aData.mCrSkip = 0;
//   aData.mYChannel = buffer;
//   aData.mCbChannel = aData.mYChannel + aData.mYSize.height * aData.mYStride;
//   aData.mCrChannel = aData.mCbChannel + aData.mCbCrSize.height * aData.mCbCrStride;

//   if (mData.mCbChannel < mData.mCrChannel) {  // NV12
//     libyuv::NV12ToI420(mData.mYChannel, mData.mYStride,
//                        mData.mCbChannel, mData.mCbCrStride,
//                        aData.mYChannel, aData.mYStride,
//                        aData.mCbChannel, aData.mCbCrStride,
//                        aData.mCrChannel, aData.mCbCrStride,
//                        aData.mYSize.width, aData.mYSize.height);
//   } else {  // NV21
//     libyuv::NV21ToI420(mData.mYChannel, mData.mYStride,
//                        mData.mCrChannel, mData.mCbCrStride,
//                        aData.mYChannel, aData.mYStride,
//                        aData.mCbChannel, aData.mCbCrStride,
//                        aData.mCrChannel, aData.mCbCrStride,
//                        aData.mYSize.width, aData.mYSize.height);
//   }

//   // The logics in PlanarYCbCrImage::GetAsSourceSurface().
//   gfx::IntSize size(mSize);
//   gfx::SurfaceFormat format =
//     gfx::ImageFormatToSurfaceFormat(gfxPlatform::GetPlatform()->GetOffscreenFormat());
//   gfx::GetYCbCrToRGBDestFormatAndSize(aData, format, size);
//   if (mSize.width > PlanarYCbCrImage::MAX_DIMENSION ||
//       mSize.height > PlanarYCbCrImage::MAX_DIMENSION) {
//     NS_ERROR("Illegal image dest width or height");
//     return nullptr;
//   }

//   RefPtr<gfx::DataSourceSurface> surface = gfx::Factory::CreateDataSourceSurface(size, format);
//   if (NS_WARN_IF(!surface)) {
//     return nullptr;
//   }

//   DataSourceSurface::ScopedMap mapping(surface, DataSourceSurface::WRITE);
//   if (NS_WARN_IF(!mapping.IsMapped())) {
//     return nullptr;
//   }

//   gfx::ConvertYCbCrToRGB(aData, format, size, mapping.GetData(), mapping.GetStride());

//   mSourceSurface = surface;

//   // Release the temporary buffer.
//   delete[] buffer;

//   return surface.forget();
// }

// bool
// NVImage::IsValid()
// {
//   return !!mBufferSize;
// }

// uint32_t
// NVImage::GetBufferSize() const
// {
//   return mBufferSize;
// }

// NVImage*
// NVImage::AsNVImage()
// {
//   return this;
// };

// bool
// NVImage::SetData(const Data& aData)
// {
//   MOZ_ASSERT(aData.mCbSkip == 1 && aData.mCrSkip == 1);
//   MOZ_ASSERT((int)std::abs(aData.mCbChannel - aData.mCrChannel) == 1);

//   // Calculate buffer size
//   // Use uint32_t throughout to match AllocateBuffer's param and mBufferSize
//   const auto checkedSize =
//     CheckedInt<uint32_t>(aData.mYSize.height) * aData.mYStride +
//     CheckedInt<uint32_t>(aData.mCbCrSize.height) * aData.mCbCrStride;

//   if (!checkedSize.isValid())
//     return false;

//   const auto size = checkedSize.value();

//   // Allocate a new buffer.
//   mBuffer = AllocateBuffer(size);
//   if (!mBuffer) {
//     return false;
//   }

//   // Update mBufferSize.
//   mBufferSize = size;

//   // Update mData.
//   mData = aData;
//   mData.mYChannel = mBuffer.get();
//   mData.mCbChannel = mData.mYChannel + (aData.mCbChannel - aData.mYChannel);
//   mData.mCrChannel = mData.mYChannel + (aData.mCrChannel - aData.mYChannel);

//   // Update mSize.
//   mSize = aData.mPicSize;

//   // Copy the input data into mBuffer.
//   // This copies the y-channel and the interleaving CbCr-channel.
//   memcpy(mData.mYChannel, aData.mYChannel, mBufferSize);

//   return true;
// }

// const NVImage::Data*
// NVImage::GetData() const
// {
//   return &mData;
// }

// UniquePtr<uint8_t>
// NVImage::AllocateBuffer(uint32_t aSize)
// {
//   UniquePtr<uint8_t> buffer(new uint8_t[aSize]);
//   return buffer;
// }

// SourceSurfaceImage::SourceSurfaceImage(const gfx::IntSize& aSize, gfx::SourceSurface* aSourceSurface)
//   : Image(nullptr, ImageFormat::CAIRO_SURFACE),
//     mSize(aSize),
//     mSourceSurface(aSourceSurface),
//     mTextureFlags(TextureFlags::DEFAULT)
// {}

// SourceSurfaceImage::SourceSurfaceImage(gfx::SourceSurface* aSourceSurface)
//   : Image(nullptr, ImageFormat::CAIRO_SURFACE),
//     mSize(aSourceSurface->GetSize()),
//     mSourceSurface(aSourceSurface),
//     mTextureFlags(TextureFlags::DEFAULT)
// {}

// SourceSurfaceImage::~SourceSurfaceImage() = default;

// TextureClient*
// SourceSurfaceImage::GetTextureClient(KnowsCompositor* aForwarder)
// {
//   if (!aForwarder) {
//     return nullptr;
//   }

//   auto entry = mTextureClients.LookupForAdd(aForwarder->GetSerial());
//   if (entry) {
//     return entry.Data();
//   }

//   RefPtr<TextureClient> textureClient;
//   RefPtr<SourceSurface> surface = GetAsSourceSurface();
//   MOZ_ASSERT(surface);
//   if (surface) {
//     // gfx::BackendType::NONE means default to content backend
//     textureClient =
//       TextureClient::CreateFromSurface(aForwarder,
//                                        surface,
//                                        BackendSelector::Content,
//                                        mTextureFlags,
//                                        ALLOC_DEFAULT);
//   }
//   if (textureClient) {
//     textureClient->SyncWithObject(aForwarder->GetSyncObject());
//     entry.OrInsert([&textureClient](){ return textureClient; });
//     return textureClient;
//   }

//   // Remove the speculatively added entry.
//   mTextureClients.Remove(aForwarder->GetSerial());
//   return nullptr;
// }

ImageContainer::ProducerID
ImageContainer::AllocateProducerID()
{
  // Callable on all threads.
  static Atomic<ImageContainer::ProducerID> sProducerID(0u);
  return ++sProducerID;
}

} // namespace layers
} // namespace mozilla
