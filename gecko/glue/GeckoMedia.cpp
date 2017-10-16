/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GeckoMedia.h"

#include "MediaPrefs.h"
#include "mozilla/Logging.h"
#include "mozilla/Preferences.h"
#include "mozilla/SharedThreadPool.h"
#include "nsThreadManager.h"
#include "nsThreadUtils.h"

void
GeckoMedia_Initialize()
{
  mozilla::LogModule::Init();
  NS_SetMainThread();
  nsThreadManager::get().Init();
  NS_InitMinimalXPCOM();
  mozilla::SharedThreadPool::InitStatics();
  mozilla::MediaPrefs::GetSingleton();
}

void
GeckoMedia_Shutdown()
{
  mozilla::Preferences::Shutdown();
  NS_ShutdownXPCOM(nullptr);
}

CanPlayTypeResult
GeckoMedia_CanPlayType(const char* aMimeType)
{
  return CanPlayTypeResult::No;
}
