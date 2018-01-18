/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMediaSource_h_
#define GeckoMediaSource_h_

#include "GeckoMediaTypes.h"
#include <stddef.h>
#include <stdint.h>

struct GeckoMediaSource;

namespace mozilla {
namespace dom {

class MediaSource;

enum class MediaSourceReadyState
{
  Closed = 0,
  Open,
  Ended,
  Unknown,
  EndGuard_
};

} // namespace dom
} // namespace mozilla

struct GeckoMediaSourceImpl
{
  void* mContext;
  void (*mFree)(void*);
  mozilla::dom::MediaSourceReadyState (*mGetReadyState)(void*);
  void (*mSetReadyState)(void*, mozilla::dom::MediaSourceReadyState);
  double (*mGetDuration)(void*);
  bool (*mHasLiveSeekableRange)(void*);
  GeckoMediaTimeInterval (*mGetLiveSeekableRange)(void*);
  size_t* (*mGetSourceBuffers)(void*);
  size_t* (*mGetActiveSourceBuffers)(void*);
};

mozilla::dom::MediaSource*
GetMediaSource(size_t aId);

extern "C" {
void
GeckoMedia_MediaSource_Create(size_t aId, GeckoMediaSourceImpl aImpl);

void
GeckoMedia_MediaSource_Shutdown(size_t aId);

bool
GeckoMedia_MediaSource_IsTypeSupported(const char* aMimeType);

void
GeckoMedia_MediaSource_DurationChange(size_t aId, const double aDuration);

void
GeckoMedia_MediaSource_DecoderEnded(size_t aId, const bool aEnded);

void
GeckoMedia_MediaSource_EndOfStreamError(size_t aId,
                                        GeckoMediaEndOfStreamError aError);
}

#endif // GeckoMediaSource_h