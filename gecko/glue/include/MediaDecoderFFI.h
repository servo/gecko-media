/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MediaDecoderFFI_h_
#define MediaDecoderFFI_h_

#include <stddef.h>
#include <stdint.h>

void
MediaDecoder_Load(size_t aId,
                  const uint8_t* aData,
                  size_t aDataLength,
                  const char* aMimeType);

void
MediaDecoder_Unload(size_t aId);

#endif // MediaDecoderFFI_h_
