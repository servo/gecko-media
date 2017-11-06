/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef RustFunctions_h_
#define RustFunctions_h_

#include "GeckoMedia.h"

void
InitializeRustFunctions(const RustFunctions* aFunctions);

void
CallGeckoProcessEvents(rust_msg_sender_t* aSender);

void
FreeProcessEventsSender(rust_msg_sender_t* aSender);

void
FreeRustVecU8(const uint8_t* aVec, size_t aLength);

#endif // RustFunctions_h_
