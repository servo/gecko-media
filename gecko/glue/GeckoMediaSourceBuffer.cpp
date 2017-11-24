/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaSourceBuffer.h"
#include "SourceBuffer.h"

#include "nsTArray.h"
#include "UniquePtr.h"

struct GeckoMediaSourceBuffer
{
  GeckoMediaSource(size_t aId, GeckoMediaSourceBufferImpl aImpl)
      : mSourceBuffer, mId(aId) {}

  UniquePtr<SourceBuffer> mSourceBuffer;
  const size_t mId;
};

static nsTArray<GeckoMediaSourceBuffer> sSourceBuffers;

static GeckoMediaSourceBuffer *GetSourceBuffer(size_t aId)
{
  for (GeckoMediaSourceBuffer &sourceBuffer : sSourceBuffers)
  {
    if (sourceBuffer.mId == aId)
    {
      return &sourceBuffer;
    }
  }
  return nullptr;
}

void GeckoMedia_SourceBuffer_Create(size_t aId,
                                    GeckoMediaSourceBufferImpl aImpl)
{
  GeckoMediaSourceBuffer *sourceBuffer =
      sSourceBuffers.AppendElement(GeckoMediaSourceBuffer(aId, aImpl));
  MOZ_ASSERT(GetSourceBuffer(aId) == sourceBuffer);
}

void GeckoMedia_SourceBuffer_Shutdown(size_t aId)
{
  GeckoMediaSourceBuffer *sourceBuffer = GetSourceBuffer(aId);
  if (NS_WARN_IF(!sourceBuffer))
  {
    return;
  }

  for (size_t i = 0; i < sSourceBuffers.Length(); i++)
  {
    if (sSourceBuffers[i].mId == aId)
    {
      sSourceBuffers.RemoveElementAt(i);
      break;
    }
  }
}