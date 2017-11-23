/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMediaSource_h_
#define GeckoMediaSource_h_

#include <stddef.h>
#include <stdint.h>

struct GeckoMediaSource;

enum class MediaSourceReadyState {
  Closed = 0,
  Open,
  Ended,
  Unknown,
  EndGuard_
};

struct GeckoMediaSourceImpl
{
  void* mContext;
  void (*mFree)(void*);
  MediaSourceReadyState (*mGetReadyState)(void*);
};

extern "C" {
  void GeckoMedia_MediaSource_Create(size_t aId,
                                     GeckoMediaSourceImpl aImpl);

  void GeckoMedia_MediaSource_Shutdown(size_t aId);

  bool GeckoMedia_MediaSource_IsTypeSupported(const char* aMimeType);
}

#endif // GeckoMediaSource_h
