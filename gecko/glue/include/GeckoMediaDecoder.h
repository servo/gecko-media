/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GECKO_MEDIA_DECODER_H_
#define GECKO_MEDIA_DECODER_H_

#include "BufferMediaResource.h"
#include "MediaDecoder.h"
#include "mozilla/RefPtr.h"

namespace mozilla {

class MediaDecoderStateMachine;

class GeckoMediaDecoder : public MediaDecoder
{
public:
  explicit GeckoMediaDecoder(MediaDecoderInit& aInit);

  nsresult Load(BufferMediaResource* aResource);

  void Shutdown() override;

  already_AddRefed<nsIPrincipal> GetCurrentPrincipal() override;

  bool IsTransportSeekable() override { return true; }

  void AddSizeOfResources(ResourceSizes* aSizes) override;

protected:
  RefPtr<BufferMediaResource> mResource;

private:
  void PinForSeek() override {}
  void UnpinForSeek() override {}
  MediaDecoderStateMachine* CreateStateMachine();
  media::TimeInterval ClampIntervalToEnd(const media::TimeInterval& aInterval);
  bool CanPlayThroughImpl() override;
  bool IsLiveStream() override final { return !mEnded; }

  bool mEnded;
};

} // namespace mozilla

#endif /* GECKO_MEDIA_DECODER_H_ */