/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMedia_h_
#define GeckoMedia_h_

#include <stddef.h>
#include <stdint.h>

struct ThreadObserverVtable
{
  void (*mOnDispatchedEvent)(void* aData);
  void (*mFree)(void* aData);
};

struct ThreadObserverObject
{
  void* mData;
  const ThreadObserverVtable* mVtable;
};

typedef uint64_t (*RustGetTimeNowFn_t)();

struct RustServicesFnTable {
  RustGetTimeNowFn_t mGetTimeNowFn;
};

enum CanPlayTypeResult
{
  No = 0,
  Maybe = 1,
  Probably = 2,
};

struct RustRunnable
{
  void* mData;
  void (*mFunction)(void* aData);
};

struct RustVecU8Object
{
  uint8_t* mData;
  size_t mLength;
  void (*mFree)(uint8_t* mData, size_t aLength);
};

struct GeckoMediaTimeInterval {
  double mStart;
  double mEnd;
};

struct GeckoMediaMetadata {
  double mDuration;
};

struct GeckoPlanarYCbCrImage {
  // Luminance buffer
  uint8_t* mYChannel;
  int32_t mYStride;
  int32_t mYWidth;
  int32_t mYHeight;
  int32_t mYSkip;

  // Chroma buffers
  uint8_t* mCbChannel;
  uint8_t* mCrChannel;
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

  void* mContext;
  void (*mFree)(void* aContext);
};

struct PlayerCallbackObject
{
  void* mContext;
  void (*mPlaybackEnded)(void*);
  void (*mDecodeError)(void*);
  void (*mAsyncEvent)(void*, const int8_t*);
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

extern "C" {
bool
GeckoMedia_Initialize(ThreadObserverObject aObject,
                      RustServicesFnTable aRustServices);

void
GeckoMedia_Shutdown();

CanPlayTypeResult
GeckoMedia_CanPlayType(const char* aMimeType);

void
GeckoMedia_ProcessEvents();

void
GeckoMedia_QueueRustRunnable(RustRunnable aRunnable);

void
GeckoMedia_FreeImage(GeckoPlanarYCbCrImage image);

void
GeckoMedia_Player_CreateBlobPlayer(size_t aId,
                                   RustVecU8Object aMediaData,
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

#endif // GeckoMedia_h_
