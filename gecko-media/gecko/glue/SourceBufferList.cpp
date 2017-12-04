/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SourceBufferList.h"

#include "GeckoMediaSourceBuffer.h"
#include "nsThreadManager.h"
#include "SourceBuffer.h"

namespace mozilla {
namespace dom {

SourceBufferList::SourceBufferList(GeckoMediaSourceBufferListImpl aImpl)
  : mImpl(aImpl)
{
}

SourceBufferList::~SourceBufferList()
{
  MOZ_ASSERT(NS_IsMainThread());

  if (NS_WARN_IF(!mImpl.mContext || !mImpl.mFree)) {
    return;
  }

  (*mImpl.mFree)(mImpl.mContext);
}

SourceBuffer*
SourceBufferList::IndexedGetter(uint32_t aIndex, bool& aFound)
{
  MOZ_ASSERT(NS_IsMainThread());

  if (NS_WARN_IF(!mImpl.mContext || !mImpl.mIndexedGetter)) {
    aFound = false;
    return nullptr;
  }

  size_t id;
  aFound = (*mImpl.mIndexedGetter)(mImpl.mContext, aIndex, &id);

  if (NS_WARN_IF(!aFound)) {
    return nullptr;
  }

  auto sourceBuffer = GetSourceBuffer(id);
  if (NS_WARN_IF(!sourceBuffer)) {
    aFound = false;
    return nullptr;
  }

  return sourceBuffer;
}

uint32_t
SourceBufferList::Length()
{
  MOZ_ASSERT(NS_IsMainThread());

  if (NS_WARN_IF(!mImpl.mContext || !mImpl.mLength)) {
    return 0;
  }

  return (*mImpl.mLength)(mImpl.mContext);
}

} // namespace dom
} // namespace mozilla
