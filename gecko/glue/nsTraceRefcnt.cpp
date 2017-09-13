/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsTraceRefcnt.h"

extern "C" {

EXPORT_XPCOM_API(void)
NS_LogCtor(void* aPtr, const char* aType, uint32_t aInstanceSize)
{
}


EXPORT_XPCOM_API(void)
NS_LogDtor(void* aPtr, const char* aType, uint32_t aInstanceSize)
{
}

EXPORT_XPCOM_API(void)
NS_LogAddRef(void* aPtr, nsrefcnt aRefcnt,
             const char* aClass, uint32_t aClassSize)
{
}

EXPORT_XPCOM_API(void)
NS_LogRelease(void* aPtr, nsrefcnt aRefcnt, const char* aClass)
{

}

}
