/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SourceBuffer.h"

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

} // namespace dom
} // namespace mozilla