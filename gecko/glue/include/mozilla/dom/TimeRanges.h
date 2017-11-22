/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_TimeRanges_h_
#define mozilla_dom_TimeRanges_h_

#include "nsTArray.h"
#include "mozilla/ErrorResult.h"
#include "SourceBuffer.h"
#include "TimeUnits.h"

namespace mozilla {

namespace dom {
class TimeRanges;
} // namespace dom

namespace dom {

// Implements media TimeRanges:
// http://www.whatwg.org/specs/web-apps/current-work/multipage/video.html#timeranges
class TimeRanges final
{
public:
  NS_INLINE_DECL_REFCOUNTING(TimeRanges)

  TimeRanges();
  explicit TimeRanges(SourceBuffer* aParent);
  explicit TimeRanges(const media::TimeIntervals& aTimeIntervals);
  TimeRanges(SourceBuffer* aParent, const media::TimeIntervals& aTimeIntervals);

  media::TimeIntervals ToTimeIntervals() const;

  void Add(double aStart, double aEnd);

  // Returns the start time of the first range, or -1 if no ranges exist.
  double GetStartTime();

  // Returns the end time of the last range, or -1 if no ranges exist.
  double GetEndTime();

  // See http://www.whatwg.org/html/#normalized-timeranges-object
  void Normalize(double aTolerance = 0.0);

  // Mutate this TimeRange to be the union of this and aOtherRanges.
  void Union(const TimeRanges* aOtherRanges, double aTolerance);

  // Mutate this TimeRange to be the intersection of this and aOtherRanges.
  void Intersection(const TimeRanges* aOtherRanges);

  SourceBuffer* GetParentObject() const;

  uint32_t Length() const
  {
    return mRanges.Length();
  }
  double Start(uint32_t aIndex, ErrorResult& aRv) const;
  double End(uint32_t aIndex, ErrorResult& aRv) const;

  nsresult GetLength(uint32_t* aLength);
  nsresult Start(uint32_t aIndex, double* aTime);
  nsresult End(uint32_t aIndex, double* aTime);

  // Shift all values by aOffset seconds.
  void Shift(double aOffset);

private:
  ~TimeRanges();

  // Comparator which orders TimeRanges by start time. Used by Normalize().
  struct TimeRange
  {
    TimeRange(double aStart, double aEnd)
      : mStart(aStart),
        mEnd(aEnd) {}
    double mStart;
    double mEnd;
  };

  struct CompareTimeRanges
  {
    bool Equals(const TimeRange& aTr1, const TimeRange& aTr2) const {
      return aTr1.mStart == aTr2.mStart && aTr1.mEnd == aTr2.mEnd;
    }

    bool LessThan(const TimeRange& aTr1, const TimeRange& aTr2) const {
      return aTr1.mStart < aTr2.mStart;
    }
  };

  AutoTArray<TimeRange, 4> mRanges;

  RefPtr<SourceBuffer> mParent;

public:
  typedef nsTArray<TimeRange>::index_type index_type;
  static const index_type NoIndex = index_type(-1);

  index_type Find(double aTime, double aTolerance = 0);

  bool Contains(double aStart, double aEnd) {
    index_type target = Find(aStart);
    if (target == NoIndex) {
      return false;
    }

    return mRanges[target].mEnd >= aEnd;
  }
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_TimeRanges_h_
