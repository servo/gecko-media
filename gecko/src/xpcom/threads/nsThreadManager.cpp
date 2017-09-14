/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsThreadManager.h"
#include "nsThread.h"
#include "nsThreadUtils.h"
#include "nsIClassInfoImpl.h"
#include "nsTArray.h"
#include "nsAutoPtr.h"
#include "nsXULAppAPI.h"
#include "LabeledEventQueue.h"
#include "MainThreadQueue.h"
#include "mozilla/AbstractThread.h"
#include "mozilla/EventQueue.h"
#include "mozilla/Preferences.h"
#include "mozilla/Scheduler.h"
#include "mozilla/SystemGroup.h"
#include "mozilla/ThreadEventQueue.h"
#include "mozilla/ThreadLocal.h"
#include "PrioritizedEventQueue.h"
#ifdef MOZ_CANARY
#include <fcntl.h>
#include <unistd.h>
#endif

#include "MainThreadIdlePeriod.h"
#include "InputEventStatistics.h"

using namespace mozilla;

static MOZ_THREAD_LOCAL(bool) sTLSIsMainThread;
static MOZ_THREAD_LOCAL(PRThread*) gTlsCurrentVirtualThread;

bool
NS_IsMainThreadTLSInitialized()
{
  return sTLSIsMainThread.initialized();
}

bool
NS_IsMainThread()
{
  return sTLSIsMainThread.get();
}

void
NS_SetMainThread()
{
  if (!sTLSIsMainThread.init()) {
    MOZ_CRASH();
  }
  sTLSIsMainThread.set(true);
  MOZ_ASSERT(NS_IsMainThread());
}

void
NS_SetMainThread(PRThread* aVirtualThread)
{
  MOZ_ASSERT(Scheduler::IsCooperativeThread());

  MOZ_ASSERT(!gTlsCurrentVirtualThread.get());
  gTlsCurrentVirtualThread.set(aVirtualThread);
  NS_SetMainThread();
}

void
NS_UnsetMainThread()
{
  MOZ_ASSERT(Scheduler::IsCooperativeThread());

  sTLSIsMainThread.set(false);
  MOZ_ASSERT(!NS_IsMainThread());
  gTlsCurrentVirtualThread.set(nullptr);
}

typedef nsTArray<NotNull<RefPtr<nsThread>>> nsThreadArray;

//-----------------------------------------------------------------------------

static void
ReleaseObject(void* aData)
{
  static_cast<nsISupports*>(aData)->Release();
}

// statically allocated instance
NS_IMETHODIMP_(MozExternalRefCountType)
nsThreadManager::AddRef()
{
  return 2;
}
NS_IMETHODIMP_(MozExternalRefCountType)
nsThreadManager::Release()
{
  return 1;
}
NS_IMPL_CLASSINFO(nsThreadManager, nullptr,
                  nsIClassInfo::THREADSAFE | nsIClassInfo::SINGLETON,
                  NS_THREADMANAGER_CID)
NS_IMPL_QUERY_INTERFACE_CI(nsThreadManager, nsIThreadManager)
NS_IMPL_CI_INTERFACE_GETTER(nsThreadManager, nsIThreadManager)

//-----------------------------------------------------------------------------

nsresult
nsThreadManager::Init()
{
  // Child processes need to initialize the thread manager before they
  // initialize XPCOM in order to set up the crash reporter. This leads to
  // situations where we get initialized twice.
  if (mInitialized) {
    return NS_OK;
  }

  if (!gTlsCurrentVirtualThread.init()) {
    return NS_ERROR_UNEXPECTED;
  }

  Scheduler::EventLoopActivation::Init();

  if (PR_NewThreadPrivateIndex(&mCurThreadIndex, ReleaseObject) == PR_FAILURE) {
    return NS_ERROR_FAILURE;
  }


#ifdef MOZ_CANARY
  const int flags = O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK;
  const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  char* env_var_flag = getenv("MOZ_KILL_CANARIES");
  sCanaryOutputFD =
    env_var_flag ? (env_var_flag[0] ? open(env_var_flag, flags, mode) :
                                      STDERR_FILENO) :
                   0;
#endif

  nsCOMPtr<nsIIdlePeriod> idlePeriod = new MainThreadIdlePeriod();

  bool startScheduler = false;
  if (XRE_IsContentProcess() && Scheduler::IsSchedulerEnabled()) {
    mMainThread = Scheduler::Init(idlePeriod);
    startScheduler = true;
  } else {
    if (XRE_IsContentProcess() && Scheduler::UseMultipleQueues()) {
      mMainThread = CreateMainThread<ThreadEventQueue<PrioritizedEventQueue<LabeledEventQueue>>, LabeledEventQueue>(idlePeriod);
    } else {
      mMainThread = CreateMainThread<ThreadEventQueue<PrioritizedEventQueue<EventQueue>>, EventQueue>(idlePeriod);
    }
  }

  nsresult rv = mMainThread->InitCurrentThread();
  if (NS_FAILED(rv)) {
    mMainThread = nullptr;
    return rv;
  }

  // We need to keep a pointer to the current thread, so we can satisfy
  // GetIsMainThread calls that occur post-Shutdown.
  mMainThread->GetPRThread(&mMainPRThread);

  // Init AbstractThread.
  AbstractThread::InitTLS();
  AbstractThread::InitMainThread();

  mInitialized = true;

  if (startScheduler) {
    Scheduler::Start();
  }
  return NS_OK;
}

void
nsThreadManager::Shutdown()
{
  MOZ_ASSERT(NS_IsMainThread(), "shutdown not called from main thread");

  // Prevent further access to the thread manager (no more new threads!)
  //
  // What happens if shutdown happens before NewThread completes?
  // We Shutdown() the new thread, and return error if we've started Shutdown
  // between when NewThread started, and when the thread finished initializing
  // and registering with ThreadManager.
  //
  mInitialized = false;

  // Empty the main thread event queue before we begin shutting down threads.
  NS_ProcessPendingEvents(mMainThread);

  // We gather the threads from the hashtable into a list, so that we avoid
  // holding the hashtable lock while calling nsIThread::Shutdown.
  nsThreadArray threads;
  {
    OffTheBooksMutexAutoLock lock(mLock);
    for (auto iter = mThreadsByPRThread.Iter(); !iter.Done(); iter.Next()) {
      RefPtr<nsThread>& thread = iter.Data();
      threads.AppendElement(WrapNotNull(thread));
      iter.Remove();
    }
  }

  // It's tempting to walk the list of threads here and tell them each to stop
  // accepting new events, but that could lead to badness if one of those
  // threads is stuck waiting for a response from another thread.  To do it
  // right, we'd need some way to interrupt the threads.
  //
  // Instead, we process events on the current thread while waiting for threads
  // to shutdown.  This means that we have to preserve a mostly functioning
  // world until such time as the threads exit.

  // Shutdown all threads that require it (join with threads that we created).
  for (uint32_t i = 0; i < threads.Length(); ++i) {
    NotNull<nsThread*> thread = threads[i];
    if (thread->ShutdownRequired()) {
      thread->Shutdown();
    }
  }

  // NB: It's possible that there are events in the queue that want to *start*
  // an asynchronous shutdown. But we have already shutdown the threads above,
  // so there's no need to worry about them. We only have to wait for all
  // in-flight asynchronous thread shutdowns to complete.
  mMainThread->WaitForAllAsynchronousShutdowns();

  // In case there are any more events somehow...
  NS_ProcessPendingEvents(mMainThread);

  // There are no more background threads at this point.

  // Clear the table of threads.
  {
    OffTheBooksMutexAutoLock lock(mLock);
    mThreadsByPRThread.Clear();
  }

  // Normally thread shutdown clears the observer for the thread, but since the
  // main thread is special we do it manually here after we're sure all events
  // have been processed.
  mMainThread->SetObserver(nullptr);
  mMainThread->ClearObservers();

  // Release main thread object.
  mMainThread = nullptr;

  // Remove the TLS entry for the main thread.
  PR_SetThreadPrivate(mCurThreadIndex, nullptr);
}

void
nsThreadManager::RegisterCurrentThread(nsThread& aThread)
{
  MOZ_ASSERT(aThread.GetPRThread() == PR_GetCurrentThread(), "bad aThread");

  OffTheBooksMutexAutoLock lock(mLock);

  ++mCurrentNumberOfThreads;
  if (mCurrentNumberOfThreads > mHighestNumberOfThreads) {
    mHighestNumberOfThreads = mCurrentNumberOfThreads;
  }

  mThreadsByPRThread.Put(aThread.GetPRThread(), &aThread);  // XXX check OOM?

  aThread.AddRef();  // for TLS entry
  PR_SetThreadPrivate(mCurThreadIndex, &aThread);
}

void
nsThreadManager::UnregisterCurrentThread(nsThread& aThread)
{
  MOZ_ASSERT(aThread.GetPRThread() == PR_GetCurrentThread(), "bad aThread");

  OffTheBooksMutexAutoLock lock(mLock);

  --mCurrentNumberOfThreads;
  mThreadsByPRThread.Remove(aThread.GetPRThread());

  PR_SetThreadPrivate(mCurThreadIndex, nullptr);
  // Ref-count balanced via ReleaseObject
}

nsThread*
nsThreadManager::CreateCurrentThread(SynchronizedEventQueue* aQueue,
                                     nsThread::MainThreadFlag aMainThread)
{
  // Make sure we don't have an nsThread yet.
  MOZ_ASSERT(!PR_GetThreadPrivate(mCurThreadIndex));

  if (!mInitialized) {
    return nullptr;
  }

  // OK, that's fine.  We'll dynamically create one :-)
  RefPtr<nsThread> thread = new nsThread(WrapNotNull(aQueue), aMainThread, 0);
  if (!thread || NS_FAILED(thread->InitCurrentThread())) {
    return nullptr;
  }

  return thread.get();  // reference held in TLS
}

nsThread*
nsThreadManager::GetCurrentThread()
{
  // read thread local storage
  void* data = PR_GetThreadPrivate(mCurThreadIndex);
  if (data) {
    return static_cast<nsThread*>(data);
  }

  if (!mInitialized) {
    return nullptr;
  }

  // OK, that's fine.  We'll dynamically create one :-)
  RefPtr<ThreadEventQueue<EventQueue>> queue =
    new ThreadEventQueue<EventQueue>(MakeUnique<EventQueue>());
  RefPtr<nsThread> thread = new nsThread(WrapNotNull(queue), nsThread::NOT_MAIN_THREAD, 0);
  if (!thread || NS_FAILED(thread->InitCurrentThread())) {
    return nullptr;
  }

  return thread.get();  // reference held in TLS
}

NS_IMETHODIMP
nsThreadManager::NewThread(uint32_t aCreationFlags,
                           uint32_t aStackSize,
                           nsIThread** aResult)
{
  return NewNamedThread(NS_LITERAL_CSTRING(""), aStackSize, aResult);
}

NS_IMETHODIMP
nsThreadManager::NewNamedThread(const nsACString& aName,
                                uint32_t aStackSize,
                                nsIThread** aResult)
{
  // Note: can be called from arbitrary threads

  // No new threads during Shutdown
  if (NS_WARN_IF(!mInitialized)) {
    return NS_ERROR_NOT_INITIALIZED;
  }

  RefPtr<ThreadEventQueue<EventQueue>> queue =
    new ThreadEventQueue<EventQueue>(MakeUnique<EventQueue>());
  RefPtr<nsThread> thr = new nsThread(WrapNotNull(queue), nsThread::NOT_MAIN_THREAD, aStackSize);
  nsresult rv = thr->Init(aName);  // Note: blocks until the new thread has been set up
  if (NS_FAILED(rv)) {
    return rv;
  }

  // At this point, we expect that the thread has been registered in mThreadByPRThread;
  // however, it is possible that it could have also been replaced by now, so
  // we cannot really assert that it was added.  Instead, kill it if we entered
  // Shutdown() during/before Init()

  if (NS_WARN_IF(!mInitialized)) {
    if (thr->ShutdownRequired()) {
      thr->Shutdown(); // ok if it happens multiple times
    }
    return NS_ERROR_NOT_INITIALIZED;
  }

  thr.forget(aResult);
  return NS_OK;
}

NS_IMETHODIMP
nsThreadManager::GetThreadFromPRThread(PRThread* aThread, nsIThread** aResult)
{
  // Keep this functioning during Shutdown
  if (NS_WARN_IF(!mMainThread)) {
    return NS_ERROR_NOT_INITIALIZED;
  }
  if (NS_WARN_IF(!aThread)) {
    return NS_ERROR_INVALID_ARG;
  }

  RefPtr<nsThread> temp;
  {
    OffTheBooksMutexAutoLock lock(mLock);
    mThreadsByPRThread.Get(aThread, getter_AddRefs(temp));
  }

  NS_IF_ADDREF(*aResult = temp);
  return NS_OK;
}

NS_IMETHODIMP
nsThreadManager::GetMainThread(nsIThread** aResult)
{
  // Keep this functioning during Shutdown
  if (NS_WARN_IF(!mMainThread)) {
    return NS_ERROR_NOT_INITIALIZED;
  }
  NS_ADDREF(*aResult = mMainThread);
  return NS_OK;
}

NS_IMETHODIMP
nsThreadManager::GetCurrentThread(nsIThread** aResult)
{
  // Keep this functioning during Shutdown
  if (!mMainThread) {
    return NS_ERROR_NOT_INITIALIZED;
  }
  *aResult = GetCurrentThread();
  if (!*aResult) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
  NS_ADDREF(*aResult);
  return NS_OK;
}

NS_IMETHODIMP
nsThreadManager::SpinEventLoopUntil(nsINestedEventLoopCondition* aCondition)
{
  nsCOMPtr<nsINestedEventLoopCondition> condition(aCondition);
  nsresult rv = NS_OK;

  if (!mozilla::SpinEventLoopUntil([&]() -> bool {
        bool isDone = false;
        rv = condition->IsDone(&isDone);
        // JS failure should be unusual, but we need to stop and propagate
        // the error back to the caller.
        if (NS_FAILED(rv)) {
          return true;
        }

        return isDone;
      })) {
    // We stopped early for some reason, which is unexpected.
    return NS_ERROR_UNEXPECTED;
  }

  // If we exited when the condition told us to, we need to return whether
  // the condition encountered failure when executing.
  return rv;
}

NS_IMETHODIMP
nsThreadManager::SpinEventLoopUntilEmpty()
{
  nsIThread* thread = NS_GetCurrentThread();

  while (NS_HasPendingEvents(thread)) {
    (void)NS_ProcessNextEvent(thread, false);
  }

  return NS_OK;
}

NS_IMETHODIMP
nsThreadManager::GetSystemGroupEventTarget(nsIEventTarget** aTarget)
{
  nsCOMPtr<nsIEventTarget> target = SystemGroup::EventTargetFor(TaskCategory::Other);
  target.forget(aTarget);
  return NS_OK;
}

uint32_t
nsThreadManager::GetHighestNumberOfThreads()
{
  OffTheBooksMutexAutoLock lock(mLock);
  return mHighestNumberOfThreads;
}

NS_IMETHODIMP
nsThreadManager::DispatchToMainThread(nsIRunnable *aEvent, uint32_t aPriority)
{
  // Note: C++ callers should instead use NS_DispatchToMainThread.
  MOZ_ASSERT(NS_IsMainThread());

  // Keep this functioning during Shutdown
  if (NS_WARN_IF(!mMainThread)) {
    return NS_ERROR_NOT_INITIALIZED;
  }
  if (aPriority != nsIRunnablePriority::PRIORITY_NORMAL) {
    nsCOMPtr<nsIRunnable> event(aEvent);
    return mMainThread->DispatchFromScript(
             new PrioritizableRunnable(event.forget(), aPriority), 0);
  }
  return mMainThread->DispatchFromScript(aEvent, 0);
}

void
nsThreadManager::EnableMainThreadEventPrioritization()
{
  MOZ_ASSERT(NS_IsMainThread());
  InputEventStatistics::Get().SetEnable(true);
  mMainThread->EnableInputEventPrioritization();
}

void
nsThreadManager::FlushInputEventPrioritization()
{
  MOZ_ASSERT(NS_IsMainThread());
  mMainThread->FlushInputEventPrioritization();
}

void
nsThreadManager::SuspendInputEventPrioritization()
{
  MOZ_ASSERT(NS_IsMainThread());
  mMainThread->SuspendInputEventPrioritization();
}

void
nsThreadManager::ResumeInputEventPrioritization()
{
  MOZ_ASSERT(NS_IsMainThread());
  mMainThread->ResumeInputEventPrioritization();
}

NS_IMETHODIMP
nsThreadManager::IdleDispatchToMainThread(nsIRunnable *aEvent, uint32_t aTimeout)
{
  // Note: C++ callers should instead use NS_IdleDispatchToThread or
  // NS_IdleDispatchToCurrentThread.
  MOZ_ASSERT(NS_IsMainThread());

  nsCOMPtr<nsIRunnable> event(aEvent);
  if (aTimeout) {
    return NS_IdleDispatchToThread(event.forget(), aTimeout, mMainThread);
  }

  return NS_IdleDispatchToThread(event.forget(), mMainThread);
}

namespace mozilla {

PRThread*
GetCurrentVirtualThread()
{
  // We call GetCurrentVirtualThread very early in startup, before the TLS is
  // initialized. Make sure we don't assert in that case.
  if (gTlsCurrentVirtualThread.initialized()) {
    if (gTlsCurrentVirtualThread.get()) {
      return gTlsCurrentVirtualThread.get();
    }
  }
  return PR_GetCurrentThread();
}

PRThread*
GetCurrentPhysicalThread()
{
  return PR_GetCurrentThread();
}

} // namespace mozilla
