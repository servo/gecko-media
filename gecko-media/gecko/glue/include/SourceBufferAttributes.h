/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SourceBufferAttributes_h_
#define mozilla_SourceBufferAttributes_h_

#include "GeckoMediaMacros.h"
#include "GeckoMediaSourceBuffer.h"
#include "TimeUnits.h"
#include "mozilla/Maybe.h"

namespace mozilla {

class SourceBufferAttributes
{
public:
  typedef mozilla::dom::AppendState AppendState;

  SourceBufferAttributes(GeckoMediaSourceBufferImpl aImpl,
                         bool aGenerateTimestamps)
    : mImpl(aImpl)
    , mGenerateTimestamps(aGenerateTimestamps)
  {
  }

  SourceBufferAttributes(const SourceBufferAttributes& aOther) = default;

  ~SourceBufferAttributes()
  {
    if (NS_WARN_IF(!mImpl.mContext || !mImpl.mFree)) {
      return;
    }
    (*mImpl.mFree)(mImpl.mContext);
  }

  IMPL_GECKO_MEDIA_SIMPLE_GETTER(GetAppendWindowStart, double, 0)
  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetAppendWindowStart, double)
  IMPL_GECKO_MEDIA_SIMPLE_GETTER(GetAppendWindowEnd, double, 0)
  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetAppendWindowEnd, double)
  IMPL_GECKO_MEDIA_SIMPLE_GETTER(GetAppendMode,
                                 dom::SourceBufferAppendMode,
                                 dom::SourceBufferAppendMode::Segments)
  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetAppendMode, dom::SourceBufferAppendMode)
  IMPL_GECKO_MEDIA_SIMPLE_GETTER(HaveGroupStartTimestamp, bool, false)
  IMPL_GECKO_MEDIA_CALLBACK(ResetGroupStartTimestamp)
  IMPL_GECKO_MEDIA_CALLBACK(RestartGroupStartTimestamp)
  IMPL_GECKO_MEDIA_SIMPLE_GETTER(GetAppendState,
                                 AppendState,
                                 AppendState::WAITING_FOR_SEGMENT)
  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetAppendState, AppendState)
  IMPL_GECKO_MEDIA_SIMPLE_GETTER(GetUpdating, bool, false)
  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetUpdating, bool)
  IMPL_GECKO_MEDIA_SIMPLE_GETTER(GetActive, bool, false)
  IMPL_GECKO_MEDIA_SIMPLE_SETTER(SetActive, bool)

  double GetApparentTimestampOffset()
  {
    CALLBACK_GUARD(GetTimestampOffset, 0);
    return CALLBACK_CALL(GetTimestampOffset);
  }

  void SetApparentTimestampOffset(double aTimestampOffset)
  {
    CALLBACK_GUARD_VOID(SetTimestampOffset);
    (*mImpl.mSetTimestampOffset)(mImpl.mContext, aTimestampOffset);
  }

  media::TimeUnit GetTimestampOffset()
  {
    double timestampOffset = GetApparentTimestampOffset();
    return media::TimeUnit::FromSeconds(timestampOffset);
  }

  void SetTimestampOffset(const media::TimeUnit& aTimestampOffset)
  {
    CALLBACK_GUARD_VOID(SetTimestampOffset);
    (*mImpl.mSetTimestampOffset)(mImpl.mContext, aTimestampOffset.ToSeconds());
  }

  media::TimeUnit GetGroupStartTimestamp()
  {
    CALLBACK_GUARD(GetGroupStartTimestamp, media::TimeUnit::FromSeconds(0));
    double timestamp = 0;
    (*mImpl.mGetGroupStartTimestamp)(mImpl.mContext, timestamp);
    return media::TimeUnit::FromSeconds(timestamp);
  }

  void SetGroupStartTimestamp(const media::TimeUnit& aGroupStartTimestamp)
  {
    CALLBACK_GUARD_VOID(SetGroupStartTimestamp);
    (*mImpl.mSetGroupStartTimestamp)(mImpl.mContext,
                                     aGroupStartTimestamp.ToSeconds());
  }

  media::TimeUnit GetGroupEndTimestamp()
  {
    CALLBACK_GUARD(GetGroupEndTimestamp, media::TimeUnit::FromSeconds(0));
    double timestamp = CALLBACK_CALL(GetGroupEndTimestamp);
    return media::TimeUnit::FromSeconds(timestamp);
  }

  void SetGroupEndTimestamp(const media::TimeUnit& aGroupEndTimestamp)
  {
    CALLBACK_GUARD_VOID(SetGroupEndTimestamp);
    (*mImpl.mSetGroupEndTimestamp)(mImpl.mContext,
                                   aGroupEndTimestamp.ToSeconds());
  }

  // mGenerateTimestamp isn't mutable once the source buffer has been
  // constructed
  bool mGenerateTimestamps;

  SourceBufferAttributes& operator=(const SourceBufferAttributes& aOther) =
    default;

private:
  SourceBufferAttributes() = delete;

  GeckoMediaSourceBufferImpl mImpl;
};

} // end namespace mozilla

#endif /* mozilla_SourceBufferAttributes_h_ */
