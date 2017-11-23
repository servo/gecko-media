/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "RustMediaResource.h"

namespace mozilla {

RustMediaResource::RustMediaResource(NetworkResourceObject aNetworkResource)
  : mResource(aNetworkResource)
{
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
  if (!mResource.mCachedRanges || !mResource.mData) {
    return NS_ERROR_FAILURE;
  }
  GeckoMediaByteIntervalSet intervalSet{ &AddByteRangeToIntervalSet,
                                         reinterpret_cast<void*>(&aRanges) };
  (*mResource.mCachedRanges)(mResource.mData, &intervalSet);
  return NS_OK;
}

} // namespace mozilla
