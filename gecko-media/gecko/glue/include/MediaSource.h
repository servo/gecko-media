/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_MediaSource_h_
#define mozilla_dom_MediaSource_h_

#include "GeckoMediaMacros.h"
#include "GeckoMediaSource.h"
#include "MediaSourceDecoder.h"
#include "nsISupportsImpl.h"
#include "SourceBufferList.h"
#include "TimeUnits.h"

namespace mozilla {
namespace dom {

class MediaSource final
{
public:
  NS_INLINE_DECL_REFCOUNTING(MediaSource)

  MediaSource(GeckoMediaSourceImpl aGeckoMediaSourceImpl);

  SourceBufferList* SourceBuffers();
  SourceBufferList* ActiveSourceBuffers();

  double Duration();
  void DurationChange(const double aDuration);

  MediaSourceReadyState ReadyState();

  // Attach this MediaSource to Decoder aDecoder.  Returns false if already attached.
  // FIXME This will likely get a GeckoMediaSourceDecoder at some point.
  bool Attach(MediaSourceDecoder* aDecoder);
  void Detach();

  bool HasLiveSeekableRange();
  media::TimeInterval LiveSeekableRange();

  void DecoderEnded(const bool aEnded);

  void EndOfStreamError(const GeckoMediaEndOfStreamError aError);

  MediaSourceDecoder* GetDecoder() { return mDecoder; }

  static bool IsTypeSupported(const char* aType);

private:
  ~MediaSource();

  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetReadyState, MediaSourceReadyState);

  GeckoMediaSourceImpl mImpl;

  RefPtr<MediaSourceDecoder> mDecoder;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_MediaSource_h_
