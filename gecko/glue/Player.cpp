/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Player.h"

#include "BufferMediaResource.h"
#include "GeckoMediaDecoder.h"
#include "GeckoMediaDecoderOwner.h"
#include "nsTArray.h"
#include "UniquePtr.h"
#include "mozilla/RefPtr.h"

using namespace mozilla;

class RustVecU8BufferMediaResource : public BufferMediaResource
{
public:
  RustVecU8BufferMediaResource(RustVecU8Object aVec)
    : BufferMediaResource(aVec.mData, aVec.mLength)
    , mRustVecU8(aVec)
  {
  }

private:
  ~RustVecU8BufferMediaResource()
  {
    (*mRustVecU8.mFree)(mRustVecU8.mData, mRustVecU8.mLength);
  }
  RustVecU8Object mRustVecU8;
};

struct Player
{
  Player(size_t aId, PlayerCallbackObject aCallback)
    : mDecoderOwner(new GeckoMediaDecoderOwner(aCallback))
    , mId(aId)
  {
  }
  RefPtr<GeckoMediaDecoder> mDecoder;
  RefPtr<GeckoMediaDecoderOwner> mDecoderOwner;
  const size_t mId;
};

static nsTArray<Player> sPlayers;

static Player*
GetPlayer(size_t aId)
{
  for (Player& player : sPlayers) {
    if (player.mId == aId) {
      return &player;
    }
  }
  return nullptr;
}

void
GeckoMedia_Player_CreateBlobPlayer(size_t aId,
                                   RustVecU8Object aMediaData,
                                   const char* aMimeType,
                                   PlayerCallbackObject aCallback)
{
  mozilla::Maybe<MediaContainerType> mime = MakeMediaContainerType(aMimeType);
  if (!mime) {
    return;
  }

  Player* player = sPlayers.AppendElement(Player(aId, aCallback));
  MOZ_ASSERT(GetPlayer(aId) == player);

  MediaDecoderInit decoderInit(player->mDecoderOwner.get(),
                               1.0,   // volume
                               true,  // mPreservesPitch
                               1.0,   // mPlaybackRate
                               false, // mMinimizePreroll
                               false, // mHasSuspendTaint
                               false, // mLooping
                               mime.value());
  player->mDecoder = new GeckoMediaDecoder(decoderInit);
  player->mDecoderOwner->SetDecoder(player->mDecoder);
  RefPtr<BufferMediaResource> resource =
    new RustVecU8BufferMediaResource(aMediaData);
  player->mDecoder->Load(resource);
}

void
GeckoMedia_Player_Play(size_t aId)
{
  Player* player = GetPlayer(aId);
  if (!player) {
    return;
  }
  player->mDecoder->Play();
}

void
GeckoMedia_Player_Pause(size_t aId)
{
  Player* player = GetPlayer(aId);
  if (!player) {
    return;
  }
  player->mDecoder->Pause();
}

void
GeckoMedia_Player_Seek(size_t aId, double aTimeOffsetSeconds)
{
  Player* player = GetPlayer(aId);
  if (!player) {
    return;
  }
  MOZ_ASSERT(player->mDecoder);
  player->mDecoder->Seek(aTimeOffsetSeconds, SeekTarget::Accurate);
}

void
GeckoMedia_Player_Shutdown(size_t aId)
{
  Player* player = GetPlayer(aId);
  if (!player) {
    return;
  }
  player->mDecoderOwner->Shutdown();
  player->mDecoder->Shutdown();
  for (size_t i = 0; i < sPlayers.Length(); i++) {
    if (sPlayers[i].mId == aId) {
      sPlayers.RemoveElementAt(i);
      break;
    }
  }
}

void
GeckoMedia_Player_SetVolume(size_t aId, double volume)
{
  Player* player = GetPlayer(aId);
  if (!player) {
    return;
  }
  player->mDecoder->SetVolume(volume);
}