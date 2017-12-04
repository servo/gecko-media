/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaSource.h"

#include "GeckoMediaMacros.h"
#include "MediaSource.h"
#include "UniquePtr.h"

using namespace mozilla;
using mozilla::dom::MediaSource;

struct GeckoMediaSource
{
  GeckoMediaSource(size_t aId, GeckoMediaSourceImpl aImpl)
    : mMediaSource(new MediaSource(aImpl))
    , mId(aId)
  {
  }

  RefPtr<MediaSource> mMediaSource;
  const size_t mId;
};

IMPL_GECKO_MEDIA_REFLECTOR(GeckoMediaSource, MediaSource, GeckoMediaSourceImpl)

bool
GeckoMedia_MediaSource_IsTypeSupported(const char* aMimeType)
{
  return MediaSource::IsTypeSupported(aMimeType);
}

void
GeckoMedia_MediaSource_DurationChange(size_t aId, const double aDuration)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(GeckoMediaSource)

  reflector->mMediaSource->DurationChange(aDuration);
}

void
GeckoMedia_MediaSource_DecoderEnded(size_t aId, const bool aEnded)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(GeckoMediaSource)

  reflector->mMediaSource->DecoderEnded(aEnded);
}

void
GeckoMedia_MediaSource_EndOfStreamError(size_t aId,
                                        GeckoMediaEndOfStreamError aError)
{
  IMPL_GECKO_MEDIA_REFLECTOR_GET(GeckoMediaSource)

  reflector->mMediaSource->EndOfStreamError(aError);
}

MediaSource*
GetMediaSource(const size_t aId)
{
  GeckoMediaSource* reflector = GetReflector(aId);
  if (NS_WARN_IF(!reflector)) {
    return nullptr;
  }
  return reflector->mMediaSource;
}