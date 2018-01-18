/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SourceBuffer.h"

#include "AbstractThread.h"
#include "GeckoMediaSource.h"
#include "MediaSourceDemuxer.h"
#include "mozilla/Logging.h"

extern mozilla::LogModule*
GetMediaSourceLog();

#define MSE_DEBUG(arg, ...)                                                    \
  MOZ_LOG(GetMediaSourceLog(),                                                 \
          mozilla::LogLevel::Debug,                                            \
          ("SourceBuffer(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))

namespace mozilla {
namespace dom {

using namespace media;

SourceBuffer::SourceBuffer(GeckoMediaSourceBufferImpl aImpl,
                           size_t aParentId,
                           const char* aMimeType,
                           bool aGenerateTimestamps)
  : mCurrentAttributes(aImpl, aGenerateTimestamps)
{
  mozilla::Maybe<MediaContainerType> mime = MakeMediaContainerType(aMimeType);
  if (NS_WARN_IF(!mime)) {
    return;
  }

  mMediaSource = GetMediaSource(aParentId);
  MOZ_ASSERT(mMediaSource);

  RefPtr<MediaSourceDecoder> decoder = mMediaSource->GetDecoder();
  if (NS_WARN_IF(!decoder)) {
    return;
  }

  mTrackBuffersManager = new TrackBuffersManager(decoder, mime.value());

  MSE_DEBUG("Create mTrackBuffersManager=%p", mTrackBuffersManager.get());

  decoder->GetDemuxer()->AttachSourceBuffer(mTrackBuffersManager);
}

media::TimeIntervals
SourceBuffer::GetTimeIntervals()
{
  MOZ_ASSERT(NS_IsMainThread());
  return mTrackBuffersManager->Buffered();
}

void
SourceBuffer::EvictData(size_t aLength, bool* aBufferFull)
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(mMediaSource);

  typedef TrackBuffersManager::EvictDataResult Result;

  Result evicted = mTrackBuffersManager->EvictData(
    media::TimeUnit::FromSeconds(mMediaSource->GetDecoder()->GetCurrentTime()),
    aLength);

  *aBufferFull = (evicted == Result::BUFFER_FULL);
}

void
SourceBuffer::AppendData(const uint8_t* aData, size_t aLength)
{
  MSE_DEBUG("AppendData(aLength=%zu)", aLength);

  RefPtr<MediaByteBuffer> data = new MediaByteBuffer();
  if (NS_WARN_IF(!data->AppendElements(aData, aLength, fallible))) {
    return mCurrentAttributes.OnDataAppended(
      uint32_t(NS_ERROR_DOM_QUOTA_EXCEEDED_ERR));
  }

  if (NS_WARN_IF(!mTrackBuffersManager)) {
    return mCurrentAttributes.OnDataAppended(
      uint32_t(NS_ERROR_NOT_INITIALIZED));
  }

  mTrackBuffersManager->AppendData(data.forget(), mCurrentAttributes)
    ->Then(AbstractThread::MainThread(),
           __func__,
           this,
           &SourceBuffer::AppendDataCompletedWithSuccess,
           &SourceBuffer::AppendDataErrored)
    ->Track(mPendingAppend);
}

void
SourceBuffer::AppendDataCompletedWithSuccess(
  const SourceBufferTask::AppendBufferResult& aResult)
{
  MOZ_ASSERT(mCurrentAttributes.GetUpdating());
  mPendingAppend.Complete();

  if (aResult.first()) {
    if (!mCurrentAttributes.GetActive()) {
      mCurrentAttributes.SetActive(true);
      MSE_DEBUG("Init segment received");
      mMediaSource->SourceBufferIsActive(this)
        ->Then(AbstractThread::MainThread(),
               __func__,
               [this]() {
                 MSE_DEBUG("Complete AppendBuffer operation");
                 mCompletionPromise.Complete();
                 mCurrentAttributes.OnDataAppended(0 /* success */);
               })
        ->Track(mCompletionPromise);
    }
  }
  if (mCurrentAttributes.GetActive()) {
    // Tell our parent decoder that we have received new data
    // and send progress event.
    mMediaSource->GetDecoder()->NotifyDataArrived();
  }

  mCurrentAttributes = aResult.second();

  CheckEndTime();

  if (!mCompletionPromise.Exists()) {
    mCurrentAttributes.OnDataAppended(0 /* success */);
  }
}

void
SourceBuffer::AppendDataErrored(const MediaResult& aError)
{
  MOZ_ASSERT(mCurrentAttributes.GetUpdating());
  mPendingAppend.Complete();

  switch (aError.Code()) {
    case NS_ERROR_DOM_MEDIA_CANCELED:
      // Nothing further to do as the trackbuffer has been shutdown.
      // or append was aborted and abort() has handled all the events.
      break;
    default:
      ResetParserState();
      mCurrentAttributes.OnDataAppended(uint32_t(NS_ERROR_DOM_MEDIA_CANCELED));
      break;
  }
}

void
SourceBuffer::CheckEndTime()
{
  MOZ_ASSERT(NS_IsMainThread());
  // Check if we need to update mMediaSource duration
  double endTime = mCurrentAttributes.GetGroupEndTimestamp().ToSeconds();
  double duration = mMediaSource->Duration();
  if (endTime > duration) {
    mMediaSource->DurationChange(endTime);
  }
}

void
SourceBuffer::AbortBufferAppend()
{
  MOZ_ASSERT(NS_IsMainThread());

  if (mCurrentAttributes.GetUpdating()) {
    mCompletionPromise.DisconnectIfExists();
    if (mPendingAppend.Exists()) {
      mPendingAppend.Disconnect();
      mTrackBuffersManager->AbortAppendData();
    }

    mCurrentAttributes.SetUpdating(false);
  }
}

void
SourceBuffer::ResetParserState()
{
  mTrackBuffersManager->ResetParserState(mCurrentAttributes);
}

void
SourceBuffer::RangeRemoval(double aStart, double aEnd)
{
  MOZ_ASSERT(NS_IsMainThread());

  mTrackBuffersManager
    ->RangeRemoval(TimeUnit::FromSeconds(aStart), TimeUnit::FromSeconds(aEnd))
    ->Then(AbstractThread::MainThread(),
           __func__,
           [this](bool) {
             mPendingRemoval.Complete();
             mCurrentAttributes.OnRangeRemoved();
           },
           []() { MOZ_ASSERT(false); })
    ->Track(mPendingRemoval);
}

} // namespace dom
} // namespace mozilla