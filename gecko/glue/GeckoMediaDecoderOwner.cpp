/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaDecoderOwner.h"
#include "mozilla/AbstractThread.h"
#include "VideoFrameContainer.h"
#include "ImageContainer.h"

namespace mozilla {

GeckoMediaDecoderOwner::GeckoMediaDecoderOwner(PlayerCallbackObject aCallback)
  : mCallback(aCallback)
{
}

GeckoMediaDecoderOwner::~GeckoMediaDecoderOwner()
{
  if (mCallback.mContext && mCallback.mFree) {
    (*mCallback.mFree)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::DownloadProgressed()
{
}

void
GeckoMediaDecoderOwner::DispatchAsyncEvent(const nsAString& aName)
{
  nsAutoCString dst;
  CopyUTF16toUTF8(aName, dst);
  if (dst.EqualsLiteral("durationchange")) {
    if (mCallback.mContext && mCallback.mDurationChanged) {
      (*mCallback.mDurationChanged)(mCallback.mContext,
                                    mDecoder->GetDuration());
      return;
    }
  }
  if (mCallback.mContext && mCallback.mAsyncEvent) {
    (*mCallback.mAsyncEvent)(mCallback.mContext, (const int8_t*)dst.get());
  }
};

void
GeckoMediaDecoderOwner::UpdateReadyState()
{
}

void
GeckoMediaDecoderOwner::FireTimeUpdate(bool aPeriodic)
{
  if (mCallback.mContext && mCallback.mTimeUpdate) {
    double time = mDecoder->GetCurrentTime();
    (*mCallback.mTimeUpdate)(mCallback.mContext, time);
  }
}

bool
GeckoMediaDecoderOwner::GetPaused()
{
  return mDecoder && mDecoder->IsOwnerPaused();
}

void
GeckoMediaDecoderOwner::MetadataLoaded(const MediaInfo* aInfo,
                                       UniquePtr<const MetadataTags> aTags)
{
  // FIXME: serialize aInfo and aTags somehow to callback.
  if (mCallback.mContext && mCallback.mMetadataLoaded) {
    GeckoMediaMetadata metadata = { 0 };
    metadata.mDuration = mDecoder->GetDuration();
    (*mCallback.mMetadataLoaded)(mCallback.mContext, metadata);
  }
}

void
GeckoMediaDecoderOwner::FirstFrameLoaded()
{
  if (mCallback.mContext && mCallback.mLoadedData) {
    (*mCallback.mLoadedData)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::NetworkError()
{
}

void
GeckoMediaDecoderOwner::DecodeError(const MediaResult& aError)
{
  mHasError = true;
  if (mCallback.mContext && mCallback.mDecodeError) {
    (*mCallback.mDecodeError)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::DecodeWarning(const MediaResult& aError)
{
}

bool
GeckoMediaDecoderOwner::HasError() const
{
  return mHasError;
}

void
GeckoMediaDecoderOwner::LoadAborted()
{
}

void
GeckoMediaDecoderOwner::PlaybackEnded()
{
  if (mCallback.mContext && mCallback.mPlaybackEnded) {
    (*mCallback.mPlaybackEnded)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::SeekStarted()
{
  if (mCallback.mContext && mCallback.mSeekStarted) {
    (*mCallback.mSeekStarted)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::SeekCompleted()
{
  if (mCallback.mContext && mCallback.mSeekCompleted) {
    (*mCallback.mSeekCompleted)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::DownloadSuspended()
{
}

void
GeckoMediaDecoderOwner::NotifySuspendedByCache(bool aSuspendedByCache)
{
}

void
GeckoMediaDecoderOwner::NotifyDecoderPrincipalChanged()
{
}

void
GeckoMediaDecoderOwner::SetAudibleState(bool aAudible)
{
}

void
GeckoMediaDecoderOwner::NotifyXPCOMShutdown()
{
}

void
GeckoMediaDecoderOwner::DispatchEncrypted(const nsTArray<uint8_t>& aInitData,
                                          const nsAString& aInitDataType)
{
}

void
GeckoMediaDecoderOwner::ConstructMediaTracks(const MediaInfo* aInfo)
{
}

void
GeckoMediaDecoderOwner::RemoveMediaTracks()
{
}

void
GeckoMediaDecoderOwner::AsyncResolveSeekDOMPromiseIfExists()
{
}

void
GeckoMediaDecoderOwner::AsyncRejectSeekDOMPromiseIfExists()
{
}

void
GeckoMediaDecoderOwner::NotifyWaitingForKey()
{
}

AbstractThread*
GeckoMediaDecoderOwner::AbstractMainThread() const
{
  return AbstractThread::MainThread();
}

dom::HTMLMediaElement*
GeckoMediaDecoderOwner::GetMediaElement()
{
  return nullptr;
}

VideoFrameContainer*
GeckoMediaDecoderOwner::GetVideoFrameContainer()
{
  RefPtr<layers::ImageContainer> container =
    new layers::ImageContainer(layers::ImageContainer::SYNCHRONOUS);
  mVideoFrameContainer =
    new VideoFrameContainer(this, container.forget());

  return mVideoFrameContainer;
}

already_AddRefed<GMPCrashHelper>
GeckoMediaDecoderOwner::CreateGMPCrashHelper()
{
  return nullptr;
}

void
GeckoMediaDecoderOwner::Invalidate(bool aImageSizeChanged,
                                   Maybe<nsIntSize>& aNewIntrinsicSize,
                                   bool aForceInvalidate)
{
}

void
GeckoMediaDecoderOwner::PrincipalHandleChangedForVideoFrameContainer(
  VideoFrameContainer* aContainer,
  const PrincipalHandle& aNewPrincipalHandle)
{
}

void
GeckoMediaDecoderOwner::SetDecoder(GeckoMediaDecoder* aDecoder)
{
  mDecoder = aDecoder;
}

} // namespace mozilla
