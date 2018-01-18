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

  // Attach this MediaSource to Decoder aDecoder.  Returns false if already
  // attached.
  bool Attach(MediaSourceDecoder* aDecoder);
  void Detach();

  bool HasLiveSeekableRange();
  media::TimeInterval LiveSeekableRange();

  void DecoderEnded(const bool aEnded);

  void EndOfStreamError(const GeckoMediaEndOfStreamError aError);

  MediaSourceDecoder* GetDecoder() { return mDecoder; }

  // Resolve all CompletionPromise pending.
  void CompletePendingTransactions();

  static bool IsTypeSupported(const char* aType);

private:
  // SourceBuffer uses SourceBufferIsActive.
  friend class SourceBuffer;

  ~MediaSource();

  SourceBufferList* GetSourceBuffers();
  SourceBufferList* GetActiveSourceBuffers();

  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetReadyState, MediaSourceReadyState);

  typedef MozPromise<bool, MediaResult, /* IsExclusive = */ true>
    ActiveCompletionPromise;
  // Return a MozPromise that will be resolved once all related operations are
  // completed, or can't progress any further.
  // Such as, transition of readyState from HAVE_NOTHING to HAVE_METADATA.
  RefPtr<ActiveCompletionPromise> SourceBufferIsActive(
    SourceBuffer* aSourceBuffer);

  GeckoMediaSourceImpl mImpl;

  RefPtr<SourceBufferList> mSourceBuffers;
  RefPtr<SourceBufferList> mActiveSourceBuffers;

  RefPtr<MediaSourceDecoder> mDecoder;

  nsTArray<MozPromiseHolder<ActiveCompletionPromise>> mCompletionPromises;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_MediaSource_h_
