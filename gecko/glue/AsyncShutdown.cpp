/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AsyncShutdown.h"
#include "mozilla/Services.h"
#include "nsThreadUtils.h"

using namespace mozilla;

NS_IMPL_ISUPPORTS(AsyncShutdownService,
                  nsIAsyncShutdownService,
                  nsIAsyncShutdownClient)


AsyncShutdownService::AsyncShutdownService()
  : mMonitor("AsyncShutdownService")
{
}

nsresult
AsyncShutdownService::Create(nsISupports* aOuter,
                             const nsIID& aIID,
                             void** aInstancePtr)
{
  RefPtr<AsyncShutdownService> service = new AsyncShutdownService();
  return service->QueryInterface(aIID, aInstancePtr);
}

/* static */
already_AddRefed<AsyncShutdownService>
AsyncShutdownService::Get()
{
  nsCOMPtr<nsIAsyncShutdownService> service = services::GetAsyncShutdown();
  RefPtr<AsyncShutdownService> s(static_cast<AsyncShutdownService*>(service.get()));
  return s.forget();
}

nsresult
AsyncShutdownService::MakeBarrier(const nsAString& aName,
                                  nsIAsyncShutdownBarrier** _retval)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::GetProfileBeforeChange(
  nsIAsyncShutdownClient** aProfileBeforeChange)
{
  RefPtr<nsIAsyncShutdownClient> self(this);
  self.forget(aProfileBeforeChange);
  return NS_OK;
}

nsresult
AsyncShutdownService::GetProfileChangeTeardown(
  nsIAsyncShutdownClient** aProfileChangeTeardown)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::GetQuitApplicationGranted(
  nsIAsyncShutdownClient** aQuitApplicationGranted)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::GetSendTelemetry(nsIAsyncShutdownClient** aSendTelemetry)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::GetWebWorkersShutdown(
  nsIAsyncShutdownClient** aWebWorkersShutdown)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::GetXpcomWillShutdown(
  nsIAsyncShutdownClient** aXpcomWillShutdown)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::GetName(nsAString& aName)
{
  MOZ_CRASH("Unimplemented");
  return NS_OK;
}

nsresult
AsyncShutdownService::AddBlocker(nsIAsyncShutdownBlocker* aBlocker,
                                 const nsAString& aFileName,
                                 int32_t aLineNumber,
                                 const nsAString& aStack)
{
  MonitorAutoLock lock(mMonitor);
  if (mShutdown) {
    return NS_ERROR_ABORT;
  }
  mShutdownBlockers.AppendElement(aBlocker);
  return NS_OK;
}

nsresult
AsyncShutdownService::RemoveBlocker(nsIAsyncShutdownBlocker* aBlocker)
{
  MonitorAutoLock lock(mMonitor);
  mShutdownBlockers.RemoveElement(aBlocker);
  if (mShutdownBlockers.IsEmpty()) {
    lock.NotifyAll();
  }
  return NS_OK;
}

void
AsyncShutdownService::BeginAsyncShutdown()
{
  nsTArray<nsCOMPtr<nsIAsyncShutdownBlocker>> blockers;
  {
    MonitorAutoLock lock(mMonitor);
    // Prevent any more blockers being added.
    mShutdown = true;
    // Make a copy of the list of blockers, so that there's no problem if any
    // blockers remove themselves synchronously when we call BlockShutdown().
    blockers = mShutdownBlockers;
  }
  for (nsCOMPtr<nsIAsyncShutdownBlocker>& blocker : blockers) {
    blocker->BlockShutdown(this);
  }
}

void
AsyncShutdownService::SpinEventLoopUntilShutdown()
{
  SpinEventLoopUntil([this]() {
    MonitorAutoLock lock(mMonitor);
    return mShutdownBlockers.IsEmpty();
  });
}
