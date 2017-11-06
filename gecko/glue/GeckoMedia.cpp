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
#include "nsIObserverService.h"
#include "mozilla/Services.h"

class IndirectThreadObserver final : public nsIThreadObserver
{
public:
  NS_DECL_THREADSAFE_ISUPPORTS

  IndirectThreadObserver(ThreadObserverObject aObject)
      : nsIThreadObserver(), mObject(aObject) {}

  NS_IMETHOD OnDispatchedEvent(void) override
  {
    (mObject.mVtable->mOnDispatchedEvent)(mObject.mData);
    return NS_OK;
  }
  NS_IMETHOD OnProcessNextEvent(nsIThreadInternal* thread,
                                bool mayWait) override
  {
    return NS_OK;
  }
  NS_IMETHOD AfterProcessNextEvent(nsIThreadInternal* thread,
                                   bool eventWasProcessed) override
  {
    return NS_OK;
  }

private:
  ThreadObserverObject mObject;

  ~IndirectThreadObserver()
  {
    (mObject.mVtable->mFree)(mObject.mData);
  }
};
NS_IMPL_ISUPPORTS(IndirectThreadObserver, nsIThreadObserver)

static bool
AddMainThreadObserver(ThreadObserverObject aObject)
{
  nsCOMPtr<nsIThreadInternal> thread = do_QueryInterface(NS_GetCurrentThread());
  return thread &&
         NS_SUCCEEDED(thread->SetObserver(new IndirectThreadObserver(aObject)));
}

bool
GeckoMedia_Initialize(ThreadObserverObject aObject)
{
  NS_SetMainThread();
  if (NS_FAILED(nsThreadManager::get().Init())) {
    return false;
  }
  mozilla::LogModule::Init();
  if (NS_FAILED(NS_InitMinimalXPCOM())) {
    return false;
  }

  mozilla::SharedThreadPool::InitStatics();
  mozilla::MediaPrefs::GetSingleton();

  // Add a thread observer so whenever runnables are dispatched
  // to the main thread we can send a message to the Rust message
  // loop to run the tasks on the Rust message loop thread. This
  // ensures we use the Rust message loop as the Gecko "main"
  // thread.
  if (!AddMainThreadObserver(aObject)) {
    return false;
  }

  // Run any events that were dispatched during initialization.
  if (NS_FAILED(NS_ProcessPendingEvents(nullptr))) {
    return false;
  }

  return true;
}

void
GeckoMedia_Shutdown()
{
  MOZ_ASSERT(NS_IsMainThread());

  mozilla::Preferences::Shutdown();

  // Broadcast a shutdown notification to all threads.
  nsCOMPtr<nsIObserverService> obsService = mozilla::services::GetObserverService();
  MOZ_ASSERT(obsService);
  obsService->NotifyObservers(nullptr, "xpcom-shutdown-threads", nullptr);

  NS_ShutdownXPCOM(nullptr);
}

CanPlayTypeResult
GeckoMedia_CanPlayType(const char* aMimeType)
{
  return CanPlayTypeResult::No;
}

void
GeckoMedia_ProcessEvents()
{
  MOZ_ASSERT(NS_IsMainThread());
  NS_ProcessPendingEvents(nullptr);
}

void
GeckoMedia_QueueRustRunnable(RustRunnable aRunnable) {
  RefPtr<mozilla::Runnable> task = NS_NewRunnableFunction(
      "RustRunnableDispatcher",
      [aRunnable]() { (aRunnable.mFunction)(aRunnable.mData); });
  auto rv = NS_DispatchToMainThread(task.forget());
  MOZ_ASSERT(NS_SUCCEEDED(rv));
}
