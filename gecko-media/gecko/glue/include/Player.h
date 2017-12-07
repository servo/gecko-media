/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef Player_h_
#define Player_h_

#include "GeckoMediaTypes.h"
#include <stddef.h>
#include <stdint.h>

struct RustVecU8Object
{
  uint8_t* mData;
  size_t mLength;
  void (*mFree)(uint8_t* mData, size_t aLength);
};

struct GeckoMediaMetadata
{
  double mDuration;
  int mVideoWidth;
  int mVideoHeight;
};

enum PlaneType
{
  Y,
  Cb,
  Cr
};

struct ByteSlice {
  const uint8_t* mData;
  size_t mLength;
};

struct GeckoPlanarYCbCrImage
{
  // Luminance buffer
  int32_t mYStride;
  int32_t mYWidth;
  int32_t mYHeight;
  int32_t mYSkip;

  // Chroma buffers
  int32_t mCbCrStride;
  int32_t mCbCrWidth;
  int32_t mCbCrHeight;
  int32_t mCbSkip;
  int32_t mCrSkip;

  // Picture region
  uint32_t mPicX;
  uint32_t mPicY;
  int32_t mPicWidth;
  int32_t mPicHeight;

  uint64_t mTimeStamp;
  uint32_t mFrameID;

  void (*mAddRefPixelData)(uint32_t aFrameID);
  void (*mFreePixelData)(uint32_t aFrameID);
  const uint8_t* (*mGetPixelData)(uint32_t aFrameID, PlaneType aPlaneType);
  ByteSlice (*mGetSliceData)(uint32_t aFrameID);
};

struct PlayerCallbackObject
{
  void* mContext;
  void (*mPlaybackEnded)(void*);
  void (*mDecodeError)(void*);
  void (*mAsyncEvent)(void*, const char*);
  void (*mMetadataLoaded)(void*, GeckoMediaMetadata);
  void (*mDurationChanged)(void*, double);
  void (*mLoadedData)(void*);
  void (*mSeekStarted)(void*);
  void (*mSeekCompleted)(void*);
  void (*mTimeUpdate)(void*, double);
  void (*mUpdateCurrentImages)(void*, size_t, GeckoPlanarYCbCrImage*);
  void (*mNotifyBuffered)(void*, size_t, GeckoMediaTimeInterval*);
  void (*mNotifySeekable)(void*, size_t, GeckoMediaTimeInterval*);
  void (*mFree)(void*);
};

struct GeckoMediaByteRange {
  uint64_t mStart;
  uint64_t mEnd;
};

struct CachedRangesObserverObject {
  void (*mUpdate)(uint32_t, const GeckoMediaByteRange*, size_t);
  uint32_t mResourceID;
};

struct NetworkResourceObject {
  void (*mSetRangesObserver)(void*, CachedRangesObserverObject);
  bool (*mReadAt)(void* aData, uint64_t aOffset, uint8_t* aBytes, uint32_t aLength, uint32_t* aOutBytesRead);
  void (*mPin)(void* aData);
  void (*mUnPin)(void* aData);
  int64_t (*mLength)(void* aData);
  bool (*mReadFromCache)(void* aData, uint64_t aOffset, uint8_t* aBytes, uint32_t aLength);
  void (*mFree)(void*);
  void* mData;
};

extern "C" {
void
GeckoMedia_Player_CreateBlobPlayer(size_t aId,
                                   RustVecU8Object aMediaData,
                                   const char* aMimeType,
                                   PlayerCallbackObject aCallback);

void
GeckoMedia_Player_CreateNetworkPlayer(size_t aId,
                                      NetworkResourceObject aMediaData,
                                      const char* aMimeType,
                                      PlayerCallbackObject aCallback);

void
GeckoMedia_Player_Play(size_t aId);

void
GeckoMedia_Player_Pause(size_t aId);

void
GeckoMedia_Player_Seek(size_t aId, double aTimeOffsetSeconds);

void
GeckoMedia_Player_Shutdown(size_t aId);

void
GeckoMedia_Player_SetVolume(size_t aId, double volume);
}

#endif // Player_h_