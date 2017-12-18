/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MediaSource.h"

#if MOZ_AV1
#include "AOMDecoder.h"
#endif
#include "DecoderTraits.h"
#include "MediaContainerType.h"
#include "MediaMIMETypes.h"
#include "mozilla/Logging.h"
#include "mozilla/Preferences.h"
#include "nsThreadManager.h"

mozilla::LogModule*
GetMediaSourceLog()
{
  static mozilla::LazyLogModule sLogModule("MediaSource");
  return sLogModule;
}

#define MSE_DEBUG(arg, ...)                                                    \
  MOZ_LOG(GetMediaSourceLog(),                                                 \
          mozilla::LogLevel::Debug,                                            \
          ("MediaSource(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))

namespace mozilla {

// Returns true if we should enable MSE webm regardless of preferences.
// 1. If MP4/H264 isn't supported:
//   * Windows XP
//   * Windows Vista and Server 2008 without the optional "Platform Update
//   Supplement"
//   * N/KN editions (Europe and Korea) of Windows 7/8/8.1/10 without the
//     optional "Windows Media Feature Pack"
// 2. If H264 hardware acceleration is not available.
// 3. The CPU is considered to be fast enough
static bool
IsWebMForced(DecoderDoctorDiagnostics* aDiagnostics)
{
  bool mp4supported = DecoderTraits::IsMP4SupportedType(
    MediaContainerType(MEDIAMIMETYPE("video/mp4")), aDiagnostics);
  /* TODO (gecko-media) This requires importing mozilla/gfx/gfxVars.h and
   *                    dependencies
  bool hwsupported = gfx::gfxVars::CanUseHardwareVideoDecoding();
#ifdef MOZ_WIDGET_ANDROID
  return !mp4supported || !hwsupported || VP9Benchmark::IsVP9DecodeFast() ||
         java::HardwareCodecCapabilityUtils::HasHWVP9();
#else
  return !mp4supported || !hwsupported || VP9Benchmark::IsVP9DecodeFast();
#endif
  */
  return !mp4supported;
}

namespace dom {

MediaSource::MediaSource(GeckoMediaSourceImpl aImpl)
  : mImpl(aImpl)
  , mDecoder(nullptr)
{
}

MediaSource::~MediaSource()
{
  MOZ_ASSERT(NS_IsMainThread());

  if (mDecoder) {
    mDecoder->DetachMediaSource();
  }

  if (NS_WARN_IF(!mImpl.mContext || !mImpl.mFree)) {
    return;
  }
  (*mImpl.mFree)(mImpl.mContext);
}

SourceBufferList*
MediaSource::SourceBuffers()
{
  MOZ_ASSERT(NS_IsMainThread());

  CALLBACK_GUARD(GetSourceBuffers, nullptr);
  size_t* id = (*mImpl.mGetSourceBuffers)(mImpl.mContext);
  if (NS_WARN_IF(!id)) {
    return nullptr;
  }

  return GetSourceBufferList(*id);
}

SourceBufferList*
MediaSource::ActiveSourceBuffers()
{
  MOZ_ASSERT(NS_IsMainThread());

  CALLBACK_GUARD(GetActiveSourceBuffers, nullptr);
  size_t* id = (*mImpl.mGetActiveSourceBuffers)(mImpl.mContext);
  if (NS_WARN_IF(!id)) {
    return nullptr;
  }

  return GetSourceBufferList(*id);
}

double
MediaSource::Duration()
{
  MOZ_ASSERT(NS_IsMainThread());
  CALLBACK_GUARD(GetDuration, 0);
  return CALLBACK_CALL(GetDuration);
}

void
MediaSource::DurationChange(double aDuration)
{
  MOZ_ASSERT(NS_IsMainThread());
  MSE_DEBUG("DurationChange(aDuration=%f", aDuration);

  // Update the media duration to the new duration and run the HTMLMediaElement
  // duration change algorithm.
  mDecoder->SetMediaSourceDuration(aDuration);
}

MediaSourceReadyState
MediaSource::ReadyState()
{
  MOZ_ASSERT(NS_IsMainThread());
  CALLBACK_GUARD(GetReadyState, MediaSourceReadyState::Unknown);
  return CALLBACK_CALL(GetReadyState);
}

bool
MediaSource::Attach(MediaSourceDecoder* aDecoder)
{
  MOZ_ASSERT(NS_IsMainThread());
  MSE_DEBUG("Attach(aDecoder=%p) owner=%p", aDecoder, aDecoder->GetOwner());
  MOZ_ASSERT(aDecoder);
  MOZ_ASSERT(aDecoder->GetOwner());
  if (ReadyState() != MediaSourceReadyState::Closed) {
    return false;
  }
  MOZ_ASSERT(!mDecoder);
  mDecoder = aDecoder;
  mDecoder->AttachMediaSource(this);
  SetReadyState(MediaSourceReadyState::Open);
  return true;
}

void
MediaSource::Detach()
{
  MOZ_ASSERT(NS_IsMainThread());
  CALLBACK_GUARD_VOID(ClearActiveSourceBuffers);
  CALLBACK_GUARD_VOID(ClearSourceBuffers);
  // TODO MOZ_RELEASE_ASSERT(mCompletionPromises.IsEmpty());
  MSE_DEBUG("mDecoder=%p owner=%p",
            mDecoder.get(), mDecoder ? mDecoder->GetOwner() : nullptr);
  if (!mDecoder) {
    MOZ_ASSERT(ReadyState() == MediaSourceReadyState::Closed);
    MOZ_ASSERT(ActiveSourceBuffers()->Length() ==0 &&
               SourceBuffers()->Length() == 0);
    return;
  }
  SetReadyState(MediaSourceReadyState::Closed);
  CALLBACK_CALL(ClearActiveSourceBuffers);
  CALLBACK_CALL(ClearSourceBuffers);
  mDecoder->DetachMediaSource();
  mDecoder = nullptr;
}

bool
MediaSource::HasLiveSeekableRange()
{
  MOZ_ASSERT(NS_IsMainThread());
  CALLBACK_GUARD(HasLiveSeekableRange, false);
  return CALLBACK_CALL(HasLiveSeekableRange);
}

media::TimeInterval
MediaSource::LiveSeekableRange()
{
  MOZ_ASSERT(NS_IsMainThread());
  media::TimeInterval interval(media::TimeUnit::Zero(),
                               media::TimeUnit::Zero());
  if (NS_WARN_IF(!mImpl.mContext || !mImpl.mGetLiveSeekableRange)) {
    return interval;
  }

  GeckoMediaTimeInterval interval_ =
    (*mImpl.mGetLiveSeekableRange)(mImpl.mContext);
  interval.mStart = media::TimeUnit::FromSeconds(interval_.mStart);
  interval.mEnd = media::TimeUnit::FromSeconds(interval_.mEnd);
  return interval;
}

void
MediaSource::DecoderEnded(const bool aEnded)
{
  MOZ_ASSERT(NS_IsMainThread());
  MSE_DEBUG("DecoderEnded(aEnded=%d)", aEnded);

  // Notify reader whether more data may come or not.
  mDecoder->Ended(aEnded);
}

void
MediaSource::EndOfStreamError(const GeckoMediaEndOfStreamError aError)
{
  MOZ_ASSERT(NS_IsMainThread());
  MSE_DEBUG("EndOfStreamError");

  switch (aError) {
    case GeckoMediaEndOfStreamError::Network:
      mDecoder->NetworkError(MediaResult(NS_ERROR_FAILURE, "MSE network"));
      break;
    case GeckoMediaEndOfStreamError::Decode:
      mDecoder->DecodeError(NS_ERROR_DOM_MEDIA_FATAL_ERR);
      break;
    default:
      break;
  }
}

/* static */
bool
MediaSource::IsTypeSupported(const char* aType)
{
  MOZ_ASSERT(NS_IsMainThread());
  auto containerType = MakeMediaContainerType(aType);
  if (containerType.isNothing()) {
    return false;
  }

  if (DecoderTraits::CanHandleContainerType(containerType.value(), nullptr) ==
      CANPLAY_NO) {
    return false;
  }

  // Now we know that this media type could be played.
  // MediaSource imposes extra restrictions, and some prefs.
  const MediaMIMEType& mimeType = containerType->Type();

  if (mimeType == MEDIAMIMETYPE("video/mp4") ||
      mimeType == MEDIAMIMETYPE("audio/mp4")) {
    if (!Preferences::GetBool("media.mediasource.mp4.enabled", false)) {
      return false;
    }
    return true;
  }

  if (mimeType == MEDIAMIMETYPE("video/webm")) {
    if (!(Preferences::GetBool("media.mediasource.webm.enabled", false) ||
          containerType->ExtendedType().Codecs().Contains(
            NS_LITERAL_STRING("vp8")) ||
#ifdef MOZ_AV1
          // FIXME: Temporary comparison with the full codecs attribute.
          // See bug 1377015.
          AOMDecoder::IsSupportedCodec(
            containerType->ExtendedType().Codecs().AsString()) ||
#endif
          IsWebMForced(nullptr))) {
      return false;
    }
    return true;
  }

  if (mimeType == MEDIAMIMETYPE("audio/webm")) {
    if (!(Preferences::GetBool("media.mediasource.webm.enabled", false) ||
          Preferences::GetBool("media.mediasource.webm.audio.enabled", true))) {
      return false;
    }
    return true;
  }

  return false;
}

} // namespace dom
} // namespace mozilla
