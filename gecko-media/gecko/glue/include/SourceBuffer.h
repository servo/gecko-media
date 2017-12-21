/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SourceBuffer_h_
#define mozilla_dom_SourceBuffer_h_

#include "GeckoMediaSourceBuffer.h"
#include "MediaSource.h"
#include "nsISupportsImpl.h"
#include "SourceBufferAttributes.h"
#include "TimeUnits.h"
#include "TrackBuffersManager.h"

namespace mozilla {
namespace dom {

class SourceBuffer final
{
public:
  NS_INLINE_DECL_REFCOUNTING(SourceBuffer)

  SourceBuffer(GeckoMediaSourceBufferImpl aImpl,
               size_t aParentId,
               const char* aMimeType,
               bool aGenerateTimestamps);

  media::TimeIntervals GetTimeIntervals();

  void EvictData(size_t aLength, bool* aBufferFull);

  void AppendData(const uint8_t* aData,
                  size_t aLength,
                  success_callback_t aSuccessCb,
                  void* aSuccessCbContext,
                  error_callback_t aErrorCb,
                  void* aErrorCbContext);

  void AbortBufferAppend();

  void ResetParserState();

private:
  friend class MediaSource;

  ~SourceBuffer(){};

  // If the media segment contains data beyond the current duration,
  // then run the duration change algorithm with new duration set to the
  // maximum of the current duration and the group end timestamp.
  void CheckEndTime();

  void AppendDataCompletedWithSuccess(
    const SourceBufferTask::AppendBufferResult& aResult,
    success_callback_t aSuccessCb,
    void* aSuccessCbContext);
  void AppendDataErrored(const MediaResult& aError,
                         error_callback_t aErrorCb,
                         void* aErrorCbContext);

  bool GetActive() { return mCurrentAttributes.GetActive(); }
  void SetActive(bool aActive) { mCurrentAttributes.SetActive(aActive); }

  SourceBufferAttributes mCurrentAttributes;

  RefPtr<MediaSource> mMediaSource;

  RefPtr<TrackBuffersManager> mTrackBuffersManager;

  MozPromiseRequestHolder<SourceBufferTask::AppendPromise> mPendingAppend;
  MozPromiseRequestHolder<MediaSource::ActiveCompletionPromise>
    mCompletionPromise;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_SourceBuffer_h_