/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef AsyncShutdown_h_
#define AsyncShutdown_h_

#include "nsIAsyncShutdown.h"
#include "nsISupportsUtils.h"
#include "nsTArray.h"
#include "nsCOMPtr.h"
#include "mozilla/Monitor.h"

#define NS_ASYNCSHUTDOWNSERVICE_CID \
  {0xdb365c78, 0xc860, 0x4e64, \
    { 0x9a, 0x63, 0x25, 0xb7, 0x3f, 0x89, 0xa0, 0x16 }}

class AsyncShutdownService : public nsIAsyncShutdownService
                           , public nsIAsyncShutdownClient
{
public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIASYNCSHUTDOWNSERVICE
  NS_DECL_NSIASYNCSHUTDOWNCLIENT

  static MOZ_MUST_USE nsresult
  Create(nsISupports* aOuter,
         const nsIID& aIID,
         void** aInstancePtr);

  static already_AddRefed<AsyncShutdownService> Get();

  void BeginAsyncShutdown();

  void SpinEventLoopUntilShutdown();

private:
  AsyncShutdownService();
  ~AsyncShutdownService() {}
  mozilla::Monitor mMonitor;
  nsTArray<nsCOMPtr<nsIAsyncShutdownBlocker>> mShutdownBlockers;
  bool mShutdown = false;
};

#endif // AsyncShutdown_h_
