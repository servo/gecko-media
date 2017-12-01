/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMediaSourceBufferList_h_
#define GeckoMediaSourceBufferList_h_

#include <stddef.h>
#include <stdint.h>

struct GeckoMediaSourceBuffer;
struct GeckoMediaSourceBufferList;

struct GeckoMediaSourceBufferListImpl
{
  void* mContext;
  void (*mFree)(void*);
  bool (*mIndexedGetter)(void*, uint32_t, size_t*);
};

extern "C" {
void
GeckoMedia_SourceBufferList_Create(size_t aId,
                                   GeckoMediaSourceBufferListImpl aImpl);

void
GeckoMedia_SourceBufferList_Shutdown(size_t aId);
}

#endif // GeckoMediaSourceBufferList_h_