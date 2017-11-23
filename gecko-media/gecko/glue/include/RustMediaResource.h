/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined(RustMediaResource_h_)
#define RustMediaResource_h_

#include "MediaResource.h"
#include "Player.h"

#include "mozilla/Atomics.h"

namespace mozilla {

class RustMediaResource : public MediaResource
{
public:
  RustMediaResource(NetworkResourceObject aNetworkResource);

  nsresult ReadAt(int64_t aOffset,
                  char* aBuffer,
                  uint32_t aCount,
                  uint32_t* aBytes) override;
  bool ShouldCacheReads() override;
  void Pin() override;
  void Unpin() override;
  int64_t GetLength() override;
  int64_t GetNextCachedData(int64_t aOffset) override;
  int64_t GetCachedDataEnd(int64_t aOffset) override;
  bool IsDataCachedToEndOfResource(int64_t aOffset) override;
  nsresult ReadFromCache(char* aBuffer,
                         int64_t aOffset,
                         uint32_t aCount) override;
  nsresult GetCachedRanges(MediaByteRangeSet& aRanges) override;

private:
  NetworkResourceObject mResource = { 0 };
};

} // namespace mozilla

#endif // MediaResoure_h_
