/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_MediaSource_h_
#define mozilla_dom_MediaSource_h_

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

  SourceBufferList* ActiveSourceBuffers()
  {
    // TODO get from mImpl.
    RefPtr<SourceBufferList> list =
      new SourceBufferList(GeckoMediaSourceBufferListImpl());
    return list;
  }

  double Duration();
  void DurationChange(const double aDuration);

  MediaSourceReadyState ReadyState();

  void Detach() { /* TODO */}

  bool HasLiveSeekableRange();
  media::TimeInterval LiveSeekableRange();

  void DecoderEnded(const bool aEnded);

  void EndOfStreamError(const GeckoMediaEndOfStreamError aError);

  MediaSourceDecoder* GetDecoder() { return mDecoder; }

  static bool IsTypeSupported(const char* aType);

private:
  ~MediaSource();

  GeckoMediaSourceImpl mImpl;

  RefPtr<MediaSourceDecoder> mDecoder;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_MediaSource_h_
