/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GeckoMedia_h_
#define GeckoMedia_h_

#include "MediaDecoderFFI.h"

struct rust_msg_sender_t;

bool
GeckoMedia_Initialize(rust_msg_sender_t* aSender);

void
GeckoMedia_Shutdown();

enum CanPlayTypeResult
{
  No = 0,
  Maybe = 1,
  Probably = 2,
};

CanPlayTypeResult
GeckoMedia_CanPlayType(const char* aMimeType);

void
GeckoMedia_ProcessEvents();

struct RustRunnable
{
  void* data;
  void (*function)(void* data);
};

void
GeckoMedia_QueueRustRunnable(RustRunnable runnable);

#endif // GeckoMedia_h_
