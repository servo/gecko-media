/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMediaSourceBufferList.h"

#include "GeckoMediaMacros.h"
#include "SourceBufferList.h"
#include "UniquePtr.h"

using namespace mozilla;
using mozilla::dom::SourceBufferList;

struct GeckoMediaSourceBufferList
{
  GeckoMediaSourceBufferList(size_t aId, GeckoMediaSourceBufferListImpl aImpl)
    : mSourceBufferList(MakeUnique<SourceBufferList>(aImpl))
    , mId(aId)
  {
  }

  UniquePtr<SourceBufferList> mSourceBufferList;
  const size_t mId;
};

IMPL_GECKO_MEDIA_REFLECTOR(GeckoMediaSourceBufferList,
                           SourceBufferList,
                           GeckoMediaSourceBufferListImpl)