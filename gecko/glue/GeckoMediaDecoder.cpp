/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "GeckoMediaDecoder.h"

#include "DecoderTraits.h"
#include "MediaDecoderStateMachine.h"
#include "MediaShutdownManager.h"
#include "VideoUtils.h"
#include "mozilla/Logging.h"
#include <algorithm>

mozilla::LogModule*
GetGeckoMediaLog()
{
  static mozilla::LazyLogModule sLogModule("GeckoMedia");
  return sLogModule;
}

#define GECKO_DEBUG(arg, ...)                                                  \
  MOZ_LOG(GetGeckoMediaLog(),                                                  \
          mozilla::LogLevel::Debug,                                            \
          ("GeckoMediaDecoder(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))
#define GECKO_DEBUGV(arg, ...)                                                 \
  MOZ_LOG(GetGeckoMediaLog(),                                                  \
          mozilla::LogLevel::Verbose,                                          \
          ("GeckoMediaDecoder(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))

using namespace mozilla::media;

namespace mozilla {

GeckoMediaDecoder::GeckoMediaDecoder(MediaDecoderInit& aInit)
  : MediaDecoder(aInit)
  , mGeckoWatchManager(this, aInit.mOwner->AbstractMainThread())
{
  mExplicitDuration.emplace(UnspecifiedNaN<double>());
  mGeckoWatchManager.Watch(mBuffered, &GeckoMediaDecoder::NotifyBuffered);
}

MediaDecoderStateMachine*
GeckoMediaDecoder::CreateStateMachine()
{
  MOZ_ASSERT(NS_IsMainThread());
  MediaFormatReaderInit init;
  init.mVideoFrameContainer = GetVideoFrameContainer();
  init.mCrashHelper = GetOwner()->CreateGMPCrashHelper();
  init.mFrameStats = mFrameStats;
  init.mResource = mResource;
  init.mMediaDecoderOwnerID = mOwner;
  mReader = DecoderTraits::CreateReader(ContainerType(), init);
  return new MediaDecoderStateMachine(this, mReader);
}

nsresult
GeckoMediaDecoder::Load(BufferMediaResource* aResource)
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!GetStateMachine());
  AbstractThread::AutoEnter context(AbstractMainThread());

  mResource = aResource;
  if (!mResource) {
    return NS_ERROR_FAILURE;
  }

  nsresult rv;
  rv = MediaShutdownManager::Instance().Register(this);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  SetStateMachine(CreateStateMachine());
  if (!GetStateMachine()) {
    NS_WARNING("Failed to create state machine!");
    return NS_ERROR_FAILURE;
  }

  rv = GetStateMachine()->Init(this);
  NS_ENSURE_SUCCESS(rv, rv);

  SetStateMachineParameters();
  return NS_OK;
}

void
GeckoMediaDecoder::Shutdown()
{
  MOZ_ASSERT(NS_IsMainThread());
  AbstractThread::AutoEnter context(AbstractMainThread());
  GECKO_DEBUG("Shutdown");

  mGeckoWatchManager.Shutdown();
  MediaDecoder::Shutdown();
}

void
GeckoMediaDecoder::AddSizeOfResources(ResourceSizes* aSizes)
{
  MOZ_ASSERT(NS_IsMainThread());
  AbstractThread::AutoEnter context(AbstractMainThread());
}

bool
GeckoMediaDecoder::CanPlayThroughImpl()
{
  MOZ_ASSERT(NS_IsMainThread());
  NS_ENSURE_TRUE(GetStateMachine(), false);
  return true;
}

double
GeckoMediaDecoder::GetDuration()
{
  if (mInfo && mInfo->mMetadataDuration.isSome()) {
    return mInfo->mMetadataDuration.value().ToSeconds();
  } else {
    return MediaDecoder::GetDuration();
  }
}

void
GeckoMediaDecoder::Pause()
{
  mOwnerPaused = true;
  MediaDecoder::Pause();
}

nsresult
GeckoMediaDecoder::Play()
{
  mOwnerPaused = false;
  return MediaDecoder::Play();
}

bool
GeckoMediaDecoder::IsOwnerPaused() const
{
  return mOwnerPaused;
}

TimeInterval
GeckoMediaDecoder::ClampIntervalToEnd(const TimeInterval& aInterval)
{
  MOZ_ASSERT(NS_IsMainThread());
  AbstractThread::AutoEnter context(AbstractMainThread());

  TimeUnit duration = TimeUnit::FromSeconds(GetDuration());
  if (duration < aInterval.mStart) {
    return aInterval;
  }
  return TimeInterval(
    aInterval.mStart, std::min(aInterval.mEnd, duration), aInterval.mFuzz);
}

already_AddRefed<nsIPrincipal>
GeckoMediaDecoder::GetCurrentPrincipal()
{
  MOZ_ASSERT(NS_IsMainThread());
  return nullptr;
}

void
GeckoMediaDecoder::NotifyBuffered()
{
  mOwner->NotifyBuffered();
}

#undef GECKO_DEBUG
#undef GECKO_DEBUGV

} // namespace mozilla
