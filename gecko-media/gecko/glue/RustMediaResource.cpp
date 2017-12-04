/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "RustMediaResource.h"

#include "mozilla/Atomics.h"
#include "mozilla/Assertions.h"
#include "mozilla/StaticMutex.h"

namespace mozilla {

static Atomic<uint32_t> sNextResourceId(0u);
static nsTArray<RustMediaResource*> sResources;
StaticMutex sResourceMutex;

static void
UpdateCachedRanges(uint32_t aResourceID,
                   const GeckoMediaByteRange* aRanges,
                   size_t aLength)
{
  RefPtr<RustMediaResource> resource;
  {
    StaticMutexAutoLock lock(sResourceMutex);
    for (auto r : sResources) {
      if (r->ID() == aResourceID) {
        resource = r;
        break;
      }
    }
  }
  if (!resource) {
    NS_WARNING("Received cached ranges update for dead resource.");
    return;
  }
  resource->SetCachedRanges(aRanges, aLength);
}

void
RustMediaResource::SetCachedRanges(const GeckoMediaByteRange* aRanges, size_t aLength)
{
  {
    MutexAutoLock lock(mRangesLock);
    mRanges.Clear();
    mRanges.AppendElements(aRanges, aLength);
  }
  if (mDecoder) {
    RefPtr<GeckoMediaDecoder> decoder = mDecoder;
    RefPtr<AbstractThread> thread = AbstractThread::MainThread();
    nsCOMPtr<nsIRunnable> task = NS_NewRunnableFunction(
      "RustMediaResource::SetCachedRanges",
      [decoder]() {
        decoder->NotifyDataArrivedIfNotShutdown();
      });
   thread->Dispatch(task.forget());
  }
}

RustMediaResource::RustMediaResource(NetworkResourceObject aNetworkResource)
  : mResource(aNetworkResource)
  , mResourceID(++sNextResourceId)
  , mRangesLock("RustMediaResource::mRanges")
{
  MOZ_ASSERT(mResourceID > 0);
  {
    StaticMutexAutoLock lock(sResourceMutex);
    sResources.AppendElement(this);
  }
  if (mResource.mSetRangesObserver && mResource.mData) {
    (*mResource.mSetRangesObserver)(
      mResource.mData,
      CachedRangesObserverObject{
        &UpdateCachedRanges,
        mResourceID
      });
  }
}

RustMediaResource::~RustMediaResource()
{
  MOZ_ASSERT(!mDecoder);
}

nsresult
RustMediaResource::Close()
{
  mDecoder = nullptr;
  {
    StaticMutexAutoLock lock(sResourceMutex);
    sResources.RemoveElement(this);
  }
  if (mResource.mFree && mResource.mData) {
    (*mResource.mFree)(mResource.mData);
    mResource.mData = nullptr;
  }
  return NS_OK;
}

void
RustMediaResource::SetDecoder(GeckoMediaDecoder* aDecoder)
{
  mDecoder = aDecoder;
}

nsresult
RustMediaResource::ReadAt(int64_t aOffset,
                          char* aBuffer,
                          uint32_t aCount,
                          uint32_t* aBytes)
{
  if (!mResource.mReadAt || !mResource.mData) {
    return NS_ERROR_FAILURE;
  }
  bool ok = (*mResource.mReadAt)(mResource.mData,
                                 aOffset,
                                 reinterpret_cast<uint8_t*>(aBuffer),
                                 aCount,
                                 aBytes);
  return ok ? NS_OK : NS_ERROR_FAILURE;
}

bool
RustMediaResource::ShouldCacheReads()
{
  return true;
}

void
RustMediaResource::Pin()
{
  if (!mResource.mPin || !mResource.mData) {
    return;
  }
  return (*mResource.mPin)(mResource.mData);
}

void
RustMediaResource::Unpin()
{
  if (!mResource.mUnPin || !mResource.mData) {
    return;
  }
  return (*mResource.mUnPin)(mResource.mData);
}

int64_t
RustMediaResource::GetLength()
{
  if (!mResource.mLength || !mResource.mData) {
    return -1;
  }
  return (*mResource.mLength)(mResource.mData);
}

int64_t
RustMediaResource::GetNextCachedData(int64_t aOffset)
{
  MediaByteRangeSet ranges;
  nsresult rv = GetCachedRanges(ranges);
  if (NS_FAILED(rv)) {
    return false;
  }

  int64_t length = GetLength();
  if (length == -1) {
    return false;
  }

  for (const auto& range : ranges) {
    if (range.mStart >= aOffset) {
      return std::max<int64_t>(range.mStart, aOffset);
    }
  }
  return -1;
}

int64_t
RustMediaResource::GetCachedDataEnd(int64_t aOffset)
{
  MediaByteRangeSet ranges;
  nsresult rv = GetCachedRanges(ranges);
  if (NS_FAILED(rv)) {
    return false;
  }

  int64_t length = GetLength();
  if (length == -1) {
    return false;
  }

  for (const auto& range : ranges) {
    if (range.mStart >= aOffset) {
      return range.mEnd;
    }
  }
  return aOffset;
}

bool
RustMediaResource::IsDataCachedToEndOfResource(int64_t aOffset)
{
  MediaByteRangeSet ranges;
  nsresult rv = GetCachedRanges(ranges);
  if (NS_FAILED(rv)) {
    return false;
  }

  int64_t length = GetLength();
  if (length == -1) {
    return false;
  }

  for (const auto& range : ranges) {
    if (range.mStart >= aOffset) {
      return range.mEnd == length;
    }
  }
  return false;
}

nsresult
RustMediaResource::ReadFromCache(char* aBuffer,
                                 int64_t aOffset,
                                 uint32_t aCount)
{
  if (!mResource.mReadFromCache || !mResource.mData) {
    return NS_ERROR_FAILURE;
  }
  bool ok = (*mResource.mReadFromCache)(
    mResource.mData, aOffset, reinterpret_cast<uint8_t*>(aBuffer), aCount);
  return ok ? NS_OK : NS_ERROR_FAILURE;
}

extern "C" void
AddByteRangeToIntervalSet(void* aData, uint64_t aStart, uint64_t aEnd)
{
  MediaByteRangeSet* intervalSet = reinterpret_cast<MediaByteRangeSet*>(aData);
  *intervalSet += MediaByteRange(aStart, aEnd);
}

nsresult
RustMediaResource::GetCachedRanges(MediaByteRangeSet& aRanges)
{
  MutexAutoLock lock(mRangesLock);
  for (const auto& range : mRanges) {
    aRanges += MediaByteRange(range.mStart, range.mEnd);
  }
  return NS_OK;
}

} // namespace mozilla
