/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Player.h"

#include "BufferMediaResource.h"
#include "GeckoMediaDecoder.h"
#include "GeckoMediaDecoderOwner.h"
#include "GeckoMediaMacros.h"
#include "RustMediaResource.h"
#include "UniquePtr.h"
#include "mozilla/RefPtr.h"
#include "nsTArray.h"

using namespace mozilla;

class RustVecU8BufferMediaResource : public BufferMediaResource
{
public:
  RustVecU8BufferMediaResource(RustVecU8Object aVec)
    : BufferMediaResource(aVec.mData, aVec.mLength)
    , mRustVecU8(aVec)
  {
  }

private:
  ~RustVecU8BufferMediaResource()
  {
    (*mRustVecU8.mFree)(mRustVecU8.mData, mRustVecU8.mLength);
  }
  RustVecU8Object mRustVecU8;
};

struct Player
{
  Player(size_t aId,
         PlayerCallbackObject aCallback,
         FrameAllocatorObject aAllocator)
    : mDecoderOwner(new GeckoMediaDecoderOwner(aCallback, aAllocator))
    , mId(aId)
  {
  }
  RefPtr<GeckoMediaDecoder> mDecoder;
  RefPtr<GeckoMediaDecoderOwner> mDecoderOwner;
  const size_t mId;
};

DEF_GECKO_MEDIA_REFLECTOR_CONTAINER(Player)

IMPL_GECKO_MEDIA_REFLECTOR_GETTER(Player)

IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN_BEGIN(Player, Player)
reflector->mDecoderOwner->Shutdown();
reflector->mDecoder->Shutdown();
IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN_END

static void
CreatePlayer(size_t aId,
             MediaResource* aResource,
             const char* aMimeType,
             PlayerCallbackObject aCallback,
             FrameAllocatorObject aAllocator)
{
  mozilla::Maybe<MediaContainerType> mime = MakeMediaContainerType(aMimeType);
  if (!mime) {
    return;
  }

  Player* reflector =
    sReflectors.AppendElement(Player(aId, aCallback, aAllocator));
  MOZ_ASSERT(GetReflector(aId) == reflector);

  MediaDecoderInit decoderInit(reflector->mDecoderOwner.get(),
                               1.0,   // volume
                               true,  // mPreservesPitch
                               1.0,   // mPlaybackRate
                               false, // mMinimizePreroll
                               false, // mHasSuspendTaint
                               false, // mLooping
                               mime.value());
  reflector->mDecoder = new GeckoMediaDecoder(decoderInit);
  reflector->mDecoderOwner->SetDecoder(reflector->mDecoder);
  reflector->mDecoder->Load(aResource);
}

void
GeckoMedia_Player_CreateBlobPlayer(size_t aId,
                                   RustVecU8Object aMediaData,
                                   const char* aMimeType,
                                   PlayerCallbackObject aCallback,
                                   FrameAllocatorObject aAllocator)
{
  RefPtr<BufferMediaResource> resource =
    new RustVecU8BufferMediaResource(aMediaData);
  CreatePlayer(aId, resource, aMimeType, aCallback, aAllocator);
}

void
GeckoMedia_Player_CreateNetworkPlayer(size_t aId,
                                      NetworkResourceObject aNetworkResource,
                                      const char* aMimeType,
                                      PlayerCallbackObject aCallback,
                                      FrameAllocatorObject aAllocator)
{
  RefPtr<RustMediaResource> resource = new RustMediaResource(aNetworkResource);
  CreatePlayer(aId, resource, aMimeType, aCallback, aAllocator);
  IMPL_GECKO_MEDIA_REFLECTOR_GET(Player)
  resource->SetDecoder(reflector->mDecoder);
}

void
GeckoMedia_Player_Play(size_t aId)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(Player)
  reflector->mDecoder->Play();
}

void
GeckoMedia_Player_Pause(size_t aId)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(Player)
  reflector->mDecoder->Pause();
}

void
GeckoMedia_Player_Seek(size_t aId, double aTimeOffsetSeconds)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(Player)
  MOZ_ASSERT(reflector->mDecoder);
  reflector->mDecoder->Seek(aTimeOffsetSeconds, SeekTarget::Accurate);
}

void
GeckoMedia_Player_SetVolume(size_t aId, double volume)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(Player)
  reflector->mDecoder->SetVolume(volume);
}

void
GeckoMedia_Player_SetPlaybackRate(size_t aId, double rate)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(Player)
    reflector->mDecoder->SetPlaybackRate(rate);
}
