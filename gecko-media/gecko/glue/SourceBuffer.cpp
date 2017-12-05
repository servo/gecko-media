/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SourceBuffer.h"

#include "GeckoMediaSource.h"
#include "MediaSource.h"
#include "MediaSourceDemuxer.h"
#include "mozilla/Logging.h"

extern mozilla::LogModule* GetMediaSourceLog();

#define MSE_DEBUG(arg, ...)                                                    \
  MOZ_LOG(GetMediaSourceLog(),                                                 \
          mozilla::LogLevel::Debug,                                            \
          ("SourceBuffer(%p)::%s: " arg,                                       \
           this,                                                               \
           __func__,                                                           \
           ##__VA_ARGS__))

namespace mozilla {
namespace dom {

SourceBuffer::SourceBuffer(GeckoMediaSourceBufferImpl aImpl,
                           size_t aParentId,
                           const char* aMimeType)
  : mImpl(aImpl)
{
  mozilla::Maybe<MediaContainerType> mime = MakeMediaContainerType(aMimeType);
  if (NS_WARN_IF(!mime)) {
    return;
  }

  auto parent = GetMediaSource(aParentId);
  if (NS_WARN_IF(!parent)) {
    return;
  }

  mTrackBuffersManager =
    new TrackBuffersManager(parent->GetDecoder(), mime.value());

  MSE_DEBUG("Create mTrackBuffersManager=%p", mTrackBuffersManager.get());

  parent->GetDecoder()->GetDemuxer()->AttachSourceBuffer(mTrackBuffersManager);
}

SourceBuffer::~SourceBuffer()
{
  if (mImpl.mContext && mImpl.mFree) {
    (*mImpl.mFree)(mImpl.mContext);
  }
}

media::TimeIntervals
SourceBuffer::GetTimeIntervals()
{
  return mTrackBuffersManager->Buffered();
}

} // namespace dom
} // namespace mozilla