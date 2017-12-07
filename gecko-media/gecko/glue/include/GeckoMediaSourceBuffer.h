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

// https://w3c.github.io/media-source/#idl-def-appendmode
enum class SourceBufferAppendMode
{
  Segments,
  Sequence,
};

// Current state as per Segment Parser Loop Algorithm
// http://w3c.github.io/media-source/index.html#sourcebuffer-segment-parser-loop
// ** This needs to go in capital letters cause it is used by non-glued code **
enum class AppendState
{
  WAITING_FOR_SEGMENT,
  PARSING_INIT_SEGMENT,
  PARSING_MEDIA_SEGMENT,
};
}
}

using namespace mozilla::dom;

struct GeckoMediaSourceBuffer;

struct GeckoMediaSourceBufferImpl
{
  void* mContext;
  void (*mFree)(void*);
  double (*mGetAppendWindowStart)(void*);
  void (*mSetAppendWindowStart)(void*, double);
  double (*mGetAppendWindowEnd)(void*);
  void (*mSetAppendWindowEnd)(void*, double);
  double (*mGetTimestampOffset)(void*);
  void (*mSetTimestampOffset)(void*, double);
  SourceBufferAppendMode (*mGetAppendMode)(void*);
  void (*mSetAppendMode)(void*, SourceBufferAppendMode);
  void (*mGetGroupStartTimestamp)(void*, double&);
  void (*mSetGroupStartTimestamp)(void*, double);
  bool (*mHaveGroupStartTimestamp)(void*);
  void (*mResetGroupStartTimestamp)(void*);
  void (*mRestartGroupStartTimestamp)(void*);
  double (*mGetGroupEndTimestamp)(void*);
  void (*mSetGroupEndTimestamp)(void*, double);
  AppendState (*mGetAppendState)(void*);
  void (*mSetAppendState)(void*, AppendState);
};

SourceBuffer*
GetSourceBuffer(const size_t aId);

extern "C" {
// TODO error handling (i.e. aId or aParentId may not be valid)
void
GeckoMedia_SourceBuffer_Create(size_t aId,
                               GeckoMediaSourceBufferImpl aImpl,
                               size_t aParentId,
                               const char* aMimeType,
                               bool aGenerateTimestamps);

void
GeckoMedia_SourceBuffer_Shutdown(size_t aId);

void
GeckoMedia_SourceBuffer_EvictData(size_t aId,
                                  size_t aParentId,
                                  int64_t aLength,
                                  bool* aBufferFull);
}

#endif // GeckoMediaSourceBuffer_h_