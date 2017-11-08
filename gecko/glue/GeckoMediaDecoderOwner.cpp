/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaDecoderOwner.h"
#include "mozilla/AbstractThread.h"

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

nsresult
GeckoMediaDecoderOwner::DispatchAsyncEvent(const nsAString& aName)
{
  nsAutoCString dst;
  CopyUTF16toUTF8(aName, dst);
  if (mCallback.mContext && mCallback.mAsyncEvent) {
    (*mCallback.mAsyncEvent)(mCallback.mContext, (const int8_t*)dst.get());
  }
  return NS_OK;
};

void
GeckoMediaDecoderOwner::UpdateReadyState()
{
}

void
GeckoMediaDecoderOwner::FireTimeUpdate(bool aPeriodic)
{
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
    (*mCallback.mMetadataLoaded)(mCallback.mContext);
  }
}

void
GeckoMediaDecoderOwner::FirstFrameLoaded()
{
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
}

void
GeckoMediaDecoderOwner::SeekCompleted()
{
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

bool
GeckoMediaDecoderOwner::IsActive() const
{
  return true;
}

bool
GeckoMediaDecoderOwner::IsHidden() const
{
  return false;
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
  return nullptr;
}

already_AddRefed<GMPCrashHelper>
GeckoMediaDecoderOwner::CreateGMPCrashHelper()
{
  return nullptr;
}

void
GeckoMediaDecoderOwner::SetDecoder(GeckoMediaDecoder* aDecoder)
{
  mDecoder = aDecoder;
}

} // namespace mozilla
