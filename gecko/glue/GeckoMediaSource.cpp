/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaSource.h"
#include "MediaSource.h"

#include "nsTArray.h"
#include "UniquePtr.h"

using namespace mozilla;
using mozilla::dom::MediaSource;

struct GeckoMediaSource
{
  GeckoMediaSource(size_t aId, GeckoMediaSourceImpl aImpl)
      : mMediaSource(MakeUnique<MediaSource>(aImpl)), mId(aId)
  {
  }

  UniquePtr<MediaSource> mMediaSource;
  const size_t mId;
};

static nsTArray<GeckoMediaSource> sMediaSources;

static GeckoMediaSource *
GetMediaSource(size_t aId)
{
  for (GeckoMediaSource &mediaSource : sMediaSources)
  {
    if (mediaSource.mId == aId)
    {
      return &mediaSource;
    }
  }
  return nullptr;
}

void GeckoMedia_MediaSource_Create(size_t aId,
                                   GeckoMediaSourceImpl aImpl)
{
  GeckoMediaSource *mediaSource =
      sMediaSources.AppendElement(GeckoMediaSource(aId, aImpl));
  MOZ_ASSERT(GetMediaSource(aId) == mediaSource);
}

void GeckoMedia_MediaSource_Shutdown(size_t aId)
{
  GeckoMediaSource *mediaSource = GetMediaSource(aId);
  if (NS_WARN_IF(!mediaSource))
  {
    return;
  }

  for (size_t i = 0; i < sMediaSources.Length(); i++)
  {
    if (sMediaSources[i].mId == aId)
    {
      sMediaSources.RemoveElementAt(i);
      break;
    }
  }
}

bool GeckoMedia_MediaSource_IsTypeSupported(const char *aMimeType)
{
  return MediaSource::IsTypeSupported(aMimeType);
}
