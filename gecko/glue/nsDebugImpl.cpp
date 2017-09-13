/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Assertions.h"

void
NS_ABORT_OOM(size_t aSize)
{
#if 0 // defined(MOZ_CRASHREPORTER)
  CrashReporter::AnnotateOOMAllocationSize(aSize);
#endif
  MOZ_CRASH("OOM");
}

extern "C" {

void
NS_DebugBreak(uint32_t aSeverity, const char* aStr, const char* aExpr,
              const char* aFile, int32_t aLine)
{
}

}
