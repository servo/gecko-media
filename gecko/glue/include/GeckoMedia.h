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

bool
GeckoMedia_Initialize(ThreadObserverObject aObject);

void
GeckoMedia_Shutdown();

enum CanPlayTypeResult
{
  No = 0,
  Maybe = 1,
  Probably = 2,
};

CanPlayTypeResult
GeckoMedia_CanPlayType(const char* aMimeType);

void
GeckoMedia_ProcessEvents();

struct RustRunnable
{
  void* mData;
  void (*mFunction)(void* aData);
};

void
GeckoMedia_QueueRustRunnable(RustRunnable aRunnable);

struct RustVecU8Object
{
  uint8_t* mData;
  size_t mLength;
  void (*mFree)(uint8_t* mData, size_t aLength);
};

struct PlayerCallbackObject
{
  void* mContext;
  void (*mPlaybackEnded)(void*);
  void (*mDecodeError)(void*);
  void (*mAsyncEvent)(void*, const int8_t*);
  void (*mMetadataLoaded)(void*);
  void (*mLoadedData)(void*);
  void (*mSeekStarted)(void*);
  void (*mSeekCompleted)(void*);
  void (*mTimeUpdate)(void*, double);
  void (*mFree)(void*);
};

void
GeckoMedia_Player_Create(size_t aId, PlayerCallbackObject aCallback);

void
GeckoMedia_Player_LoadBlob(size_t aId,
                           RustVecU8Object aMediaData,
                           const char* aMimeType);

void
GeckoMedia_Player_Play(size_t aId);

void
GeckoMedia_Player_Pause(size_t aId);

void
GeckoMedia_Player_Shutdown(size_t aId);

void
GeckoMedia_Player_SetVolume(size_t aId, double volume);

double
GeckoMedia_Player_GetDuration(size_t aId);

#endif // GeckoMedia_h_
