/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMediaSourceBuffer_h_
#define GeckoMediaSourceBuffer_h_

#include <stddef.h>
#include <stdint.h>

namespace mozilla {
namespace dom {
class SourceBuffer;
}
}

struct GeckoMediaSourceBuffer;

struct GeckoMediaSourceBufferImpl
{
  void* mContext;
  void (*mFree)(void*);
};

mozilla::dom::SourceBuffer*
GetSourceBuffer(const size_t aId);

extern "C" {
// TODO error handling (i.e. aId or aParentId may not be valid)
void
GeckoMedia_SourceBuffer_Create(size_t aId,
                               GeckoMediaSourceBufferImpl aImpl,
                               size_t aParentId,
                               const char* aMimeType);

void
GeckoMedia_SourceBuffer_Shutdown(size_t aId);

void
GeckoMedia_SourceBuffer_EvictData(size_t aId,
                                  size_t aParentId,
                                  int64_t aLength,
                                  bool* aBufferFull);
}

#endif // GeckoMediaSourceBuffer_h_