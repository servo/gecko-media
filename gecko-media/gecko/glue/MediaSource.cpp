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

mozilla::LogModule *GetMediaSourceLog()
{
  static mozilla::LazyLogModule sLogModule("MediaSource");
  return sLogModule;
}

#define MSE_DEBUG(arg, ...) MOZ_LOG(GetMediaSourceLog(), mozilla::LogLevel::Debug, ("MediaSource(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))

namespace mozilla
{

// Returns true if we should enable MSE webm regardless of preferences.
// 1. If MP4/H264 isn't supported:
//   * Windows XP
//   * Windows Vista and Server 2008 without the optional "Platform Update Supplement"
//   * N/KN editions (Europe and Korea) of Windows 7/8/8.1/10 without the
//     optional "Windows Media Feature Pack"
// 2. If H264 hardware acceleration is not available.
// 3. The CPU is considered to be fast enough
static bool
IsWebMForced(DecoderDoctorDiagnostics *aDiagnostics)
{
  bool mp4supported =
      DecoderTraits::IsMP4SupportedType(MediaContainerType(MEDIAMIMETYPE("video/mp4")),
                                        aDiagnostics);
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

namespace dom
{

MediaSource::MediaSource(GeckoMediaSourceImpl aImpl)
    : mImpl(aImpl)
{
}

MediaSource::~MediaSource()
{
  if (!NS_WARN_IF(mImpl.mContext || mImpl.mFree)) {
    return;
  }
  (*mImpl.mFree)(mImpl.mContext);
}

double
MediaSource::Duration()
{
  if (!NS_WARN_IF(!mImpl.mContext || !mImpl.mGetDuration)) {
    return 0;
  }
  return (*mImpl.mGetDuration)(mImpl.mContext);
}

MediaSourceReadyState
MediaSource::ReadyState()
{
  MOZ_ASSERT(NS_IsMainThread());
  if (NS_WARN_IF(!mImpl.mContext || !mImpl.mGetReadyState))
  {
    return MediaSourceReadyState::Unknown;
  }

  return (*mImpl.mGetReadyState)(mImpl.mContext);
}

/* static */
bool MediaSource::IsTypeSupported(const char *aType)
{
  auto containerType = MakeMediaContainerType(aType);
  if (containerType.isNothing())
  {
    return false;
  }

  if (DecoderTraits::CanHandleContainerType(containerType.value(), nullptr) == CANPLAY_NO)
  {
    return false;
  }

  // Now we know that this media type could be played.
  // MediaSource imposes extra restrictions, and some prefs.
  const MediaMIMEType &mimeType = containerType->Type();

  if (mimeType == MEDIAMIMETYPE("video/mp4") ||
      mimeType == MEDIAMIMETYPE("audio/mp4"))
  {
    if (!Preferences::GetBool("media.mediasource.mp4.enabled", false))
    {
      return false;
    }
    return true;
  }

  if (mimeType == MEDIAMIMETYPE("video/webm"))
  {
    if (!(Preferences::GetBool("media.mediasource.webm.enabled", false) ||
          containerType->ExtendedType().Codecs().Contains(
              NS_LITERAL_STRING("vp8")) ||
#ifdef MOZ_AV1
          // FIXME: Temporary comparison with the full codecs attribute.
          // See bug 1377015.
          AOMDecoder::IsSupportedCodec(containerType->ExtendedType().Codecs().AsString()) ||
#endif
          IsWebMForced(nullptr)))
    {
      return false;
    }
    return true;
  }

  if (mimeType == MEDIAMIMETYPE("audio/webm"))
  {
    if (!(Preferences::GetBool("media.mediasource.webm.enabled", false) ||
          Preferences::GetBool("media.mediasource.webm.audio.enabled", true)))
    {
      return false;
    }
    return true;
  }

  return false;
}

} // namespace dom
} // namespace mozilla
