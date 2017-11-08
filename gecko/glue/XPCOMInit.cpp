/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "nsComponentManager.h"
#include "nsCategoryManager.h"
#include "nsCategoryManagerUtils.h"
#include "nsObserverService.h"
#include "nsThreadManager.h"
#include "nsThreadPool.h"
#include "nsTimerImpl.h"
#include "nsXPCOMCIDInternal.h"
#include "AsyncShutdown.h"

#include "mozilla/ClearOnShutdown.h"
#include "mozilla/Module.h"
#include "mozilla/ModuleUtils.h"
#include "mozilla/SystemGroup.h"

bool gXPCOMThreadsShutDown = false;
bool gXPCOMShuttingDown = false;
nsComponentManagerImpl* nsComponentManagerImpl::gComponentManager = nullptr;

static NS_DEFINE_CID(kComponentManagerCID, NS_COMPONENTMANAGER_CID);

using namespace mozilla;

NS_GENERIC_FACTORY_CONSTRUCTOR(nsTimer)

static nsresult
nsThreadManagerGetSingleton(nsISupports* aOuter,
                            const nsIID& aIID,
                            void** aInstancePtr)
{
  NS_ASSERTION(aInstancePtr, "null outptr");
  if (NS_WARN_IF(aOuter)) {
    return NS_ERROR_NO_AGGREGATION;
  }

  return nsThreadManager::get().QueryInterface(aIID, aInstancePtr);
}

NS_GENERIC_FACTORY_CONSTRUCTOR(nsThreadPool)


#define COMPONENT(NAME, Ctor) static NS_DEFINE_CID(kNS_##NAME##_CID, NS_##NAME##_CID);
#define COMPONENT_M(NAME, Ctor, Selector) static NS_DEFINE_CID(kNS_##NAME##_CID, NS_##NAME##_CID);
#include "XPCOMModule.inc"
#undef COMPONENT
#undef COMPONENT_M

#define COMPONENT(NAME, Ctor) { &kNS_##NAME##_CID, false, nullptr, Ctor },
#define COMPONENT_M(NAME, Ctor, Selector) { &kNS_##NAME##_CID, false, nullptr, Ctor, Selector },
const mozilla::Module::CIDEntry kXPCOMCIDEntries[] = {
  { &kComponentManagerCID, true, nullptr, nsComponentManagerImpl::Create, Module::ALLOW_IN_GPU_PROCESS },
  // { &kINIParserFactoryCID, false, CreateINIParserFactory },
#include "XPCOMModule.inc"
  // { &kNS_CHROMEREGISTRY_CID, false, nullptr, nsChromeRegistryConstructor },
  // { &kNS_CHROMEPROTOCOLHANDLER_CID, false, nullptr, nsChromeProtocolHandlerConstructor },
  // { &kNS_SECURITY_CONSOLE_MESSAGE_CID, false, nullptr, nsSecurityConsoleMessageConstructor },
  { nullptr }
};
#undef COMPONENT
#undef COMPONENT_M

#define COMPONENT(NAME, Ctor) { NS_##NAME##_CONTRACTID, &kNS_##NAME##_CID },
#define COMPONENT_M(NAME, Ctor, Selector) { NS_##NAME##_CONTRACTID, &kNS_##NAME##_CID, Selector },
const mozilla::Module::ContractIDEntry kXPCOMContracts[] = {
#include "XPCOMModule.inc"
  // { NS_CHROMEREGISTRY_CONTRACTID, &kNS_CHROMEREGISTRY_CID },
  // { NS_NETWORK_PROTOCOL_CONTRACTID_PREFIX "chrome", &kNS_CHROMEPROTOCOLHANDLER_CID },
  // { NS_INIPARSERFACTORY_CONTRACTID, &kINIParserFactoryCID },
  // { NS_SECURITY_CONSOLE_MESSAGE_CONTRACTID, &kNS_SECURITY_CONSOLE_MESSAGE_CID },
  { nullptr }
};
#undef COMPONENT
#undef COMPONENT_M

const mozilla::Module kXPCOMModule = {
  mozilla::Module::kVersion, kXPCOMCIDEntries, kXPCOMContracts,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  Module::ALLOW_IN_GPU_PROCESS
};

XPCOM_API(nsresult)
NS_InitMinimalXPCOM()
{
  mozilla::TimeStamp::Startup();

  nsresult rv = NS_OK;

  // Create the Component/Service Manager
  nsComponentManagerImpl::gComponentManager = new nsComponentManagerImpl();
  NS_ADDREF(nsComponentManagerImpl::gComponentManager);

  rv = nsComponentManagerImpl::gComponentManager->Init();
  if (NS_FAILED(rv)) {
    NS_RELEASE(nsComponentManagerImpl::gComponentManager);
    return rv;
  }

  // Init the SystemGroup for dispatching main thread runnables.
  SystemGroup::InitStatic();

  // Set up the timer globals/timer thread.
  rv = nsTimerImpl::Startup();
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  return rv;
}

nsresult
NS_ShutdownXPCOM(nsIServiceManager* aServMgr)
{
  if (!NS_IsMainThread()) {
    MOZ_CRASH("Shutdown on wrong thread");
  }

  nsresult rv;
  nsCOMPtr<nsISimpleEnumerator> moduleLoaders;

  // Notify observers of xpcom shutting down
  {
    // Block it so that the COMPtr will get deleted before we hit
    // servicemanager shutdown

    nsCOMPtr<nsIThread> thread = do_GetCurrentThread();
    if (NS_WARN_IF(!thread)) {
      return NS_ERROR_UNEXPECTED;
    }

    RefPtr<nsObserverService> observerService;
    CallGetService("@mozilla.org/observer-service;1",
                   (nsObserverService**)getter_AddRefs(observerService));

    if (observerService) {
      mozilla::KillClearOnShutdown(ShutdownPhase::WillShutdown);
      observerService->NotifyObservers(nullptr,
                                       NS_XPCOM_WILL_SHUTDOWN_OBSERVER_ID,
                                       nullptr);

      nsCOMPtr<nsIServiceManager> mgr;
      rv = NS_GetServiceManager(getter_AddRefs(mgr));
      if (NS_SUCCEEDED(rv)) {
        mozilla::KillClearOnShutdown(ShutdownPhase::Shutdown);
        observerService->NotifyObservers(mgr, NS_XPCOM_SHUTDOWN_OBSERVER_ID,
                                         nullptr);
      }
    }

    gXPCOMThreadsShutDown = true;

    // Shutdown the timer thread and all timers that might still be alive before
    // shutting down the component manager
    nsTimerImpl::Shutdown();

    NS_ProcessPendingEvents(thread);

    // Shutdown all remaining threads.  This method does not return until
    // all threads created using the thread manager (with the exception of
    // the main thread) have exited.
    nsThreadManager::get().Shutdown();

    NS_ProcessPendingEvents(thread);

  }

  // Free ClearOnShutdown()'ed smart pointers.  This needs to happen *after*
  // we've finished notifying observers of XPCOM shutdown, because shutdown
  // observers themselves might call ClearOnShutdown().
  mozilla::KillClearOnShutdown(ShutdownPhase::ShutdownFinal);

  // XPCOM is officially in shutdown mode NOW
  // Set this only after the observers have been notified as this
  // will cause servicemanager to become inaccessible.
  //mozilla::services::Shutdown();

  // We may have AddRef'd for the caller of NS_InitXPCOM, so release it
  // here again:
  NS_IF_RELEASE(aServMgr);

  // Shutdown global servicemanager
  if (nsComponentManagerImpl::gComponentManager) {
    mozilla::KillClearOnShutdown(ShutdownPhase::ShutdownThreads);
    nsComponentManagerImpl::gComponentManager->FreeServices();
  }

  if (moduleLoaders) {
    bool more;
    nsCOMPtr<nsISupports> el;
    while (NS_SUCCEEDED(moduleLoaders->HasMoreElements(&more)) && more) {
      moduleLoaders->GetNext(getter_AddRefs(el));
    }

    moduleLoaders = nullptr;
  }

  // Shutdown xpcom. This will release all loaders and cause others holding
  // a refcount to the component manager to release it.
  if (nsComponentManagerImpl::gComponentManager) {
    rv = (nsComponentManagerImpl::gComponentManager)->Shutdown();
    NS_ASSERTION(NS_SUCCEEDED(rv), "Component Manager shutdown failed.");
  } else {
    NS_WARNING("Component Manager was never created ...");
  }

  // Finally, release the component manager last because it unloads the
  // libraries:
  if (nsComponentManagerImpl::gComponentManager) {
    nsrefcnt cnt;
    NS_RELEASE2(nsComponentManagerImpl::gComponentManager, cnt);
    NS_ASSERTION(cnt == 0, "Component Manager being held past XPCOM shutdown.");
  }
  nsComponentManagerImpl::gComponentManager = nullptr;
  nsCategoryManager::Destroy();

  return NS_OK;
}
