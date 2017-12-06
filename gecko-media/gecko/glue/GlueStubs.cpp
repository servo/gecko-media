/* -*-  Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsIMemoryReporter.h"
#include "mozilla/Telemetry.h"
#include "DecoderDoctorDiagnostics.h"

namespace mozilla {

void
DecoderDoctorDiagnostics::StoreEvent(nsIDocument* aDocument,
                                     const DecoderDoctorEvent& aEvent,
                                     const char* aCallSite)
{
}

namespace Telemetry {

void AccumulateTimeDelta(HistogramID id, TimeStamp start, TimeStamp end) {
}

} // namespace Telemetry
} // namespace mozilla

extern "C" {

nsresult
RegisterWeakMemoryReporter(nsIMemoryReporter* aReporter)
{
  return NS_ERROR_FAILURE;
}

nsresult
RegisterWeakAsyncMemoryReporter(nsIMemoryReporter* aReporter)
{
  return NS_ERROR_FAILURE;
}

nsresult
UnregisterStrongMemoryReporter(nsIMemoryReporter* aReporter)
{
  return NS_ERROR_FAILURE;
}

nsresult
UnregisterWeakMemoryReporter(nsIMemoryReporter* aReporter)
{
  return NS_ERROR_FAILURE;
}

} // extern "C"


