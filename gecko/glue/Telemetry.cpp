/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/TelemetryHistogramEnums.h"

namespace mozilla {
namespace Telemetry {

// The external API for controlling recording state
// void
// SetHistogramRecordingEnabled(HistogramID aID, bool aEnabled)
// {
//   // TelemetryHistogram::SetHistogramRecordingEnabled(aID, aEnabled);
// }

void
Accumulate(HistogramID aHistogram, uint32_t aSample)
{
  // TelemetryHistogram::Accumulate(aHistogram, aSample);
}

// void
// Accumulate(HistogramID aID, const nsCString& aKey, uint32_t aSample)
// {
//   // TelemetryHistogram::Accumulate(aID, aKey, aSample);
// }

// void
// Accumulate(const char* name, uint32_t sample)
// {
//   // TelemetryHistogram::Accumulate(name, sample);
// }

// void
// Accumulate(const char *name, const nsCString& key, uint32_t sample)
// {
//   // TelemetryHistogram::Accumulate(name, key, sample);
// }

// void
// AccumulateCategorical(HistogramID id, const nsCString& label)
// {
//   // TelemetryHistogram::AccumulateCategorical(id, label);
// }

// void
// AccumulateTimeDelta(HistogramID aHistogram, TimeStamp start, TimeStamp end)
// {
//   Accumulate(aHistogram,
//              static_cast<uint32_t>((end - start).ToMilliseconds()));
// }


} // namespace Telemetry
} // namespace mozilla
