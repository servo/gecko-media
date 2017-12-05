/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SourceBuffer_h_
#define mozilla_dom_SourceBuffer_h_

#include "GeckoMediaSourceBuffer.h"
#include "nsISupportsImpl.h"
#include "TimeUnits.h"
#include "TrackBuffersManager.h"

namespace mozilla {
namespace dom {

class SourceBuffer final
{
public:
  NS_INLINE_DECL_REFCOUNTING(SourceBuffer)

  SourceBuffer(GeckoMediaSourceBufferImpl aImpl,
               size_t aParentId,
               const char* aMimeType);

  media::TimeIntervals GetTimeIntervals();

  void EvictData(size_t aParentId, size_t aLength, bool* aBufferFull);

private:
  ~SourceBuffer();

  GeckoMediaSourceBufferImpl mImpl;

  RefPtr<TrackBuffersManager> mTrackBuffersManager;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_SourceBuffer_h_