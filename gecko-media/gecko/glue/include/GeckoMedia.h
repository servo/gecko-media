/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMedia_h_
#define GeckoMedia_h_

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

struct RustServicesFnTable
{
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
}

#endif // GeckoMedia_h_
