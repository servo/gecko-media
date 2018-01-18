/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SourceBufferList_h_
#define mozilla_dom_SourceBufferList_h_

#include "GeckoMediaMacros.h"
#include "GeckoMediaSourceBufferList.h"
#include "nsISupportsImpl.h"

namespace mozilla {
namespace dom {

class SourceBuffer;

class SourceBufferList final
{
public:
  NS_INLINE_DECL_REFCOUNTING(SourceBufferList)

  SourceBufferList(GeckoMediaSourceBufferListImpl aImpl);

  SourceBuffer* IndexedGetter(uint32_t aIndex, bool& aFound);

  IMPL_GECKO_MEDIA_SIMPLE_GETTER(Length, uint32_t, 0);

private:
  friend class MediaSource;

  ~SourceBufferList();

  void Append(SourceBuffer* aSourceBuffer, bool aNotify);

  IMPL_GECKO_MEDIA_CALLBACK(Clear);

  GeckoMediaSourceBufferListImpl mImpl;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_SourceBufferList_h_