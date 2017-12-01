/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaSourceBuffer.h"

#include "GeckoMediaMacros.h"
#include "SourceBuffer.h"
#include "RefPtr.h"

using mozilla::dom::SourceBuffer;

struct GeckoMediaSourceBuffer
{
  GeckoMediaSourceBuffer(size_t aId, GeckoMediaSourceBufferImpl aImpl)
    : mSourceBuffer(new SourceBuffer(aImpl))
    , mId(aId)
  {
  }

  RefPtr<SourceBuffer> mSourceBuffer;
  const size_t mId;
};

IMPL_GECKO_MEDIA_REFLECTOR(GeckoMediaSourceBuffer,
                           SourceBuffer,
                           GeckoMediaSourceBufferImpl)

SourceBuffer*
GetSourceBuffer(const size_t aId)
{
  GeckoMediaSourceBuffer* reflector = GetReflector(aId);
  if (NS_WARN_IF(!reflector)) {
    return nullptr;
  }
  return reflector->mSourceBuffer;
}
