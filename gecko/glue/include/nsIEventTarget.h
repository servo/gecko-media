/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIEventTarget.idl
 */

#ifndef __gen_nsIEventTarget_h__
#define __gen_nsIEventTarget_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIRunnable_h__
#include "nsIRunnable.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "nsCOMPtr.h"
#include "mozilla/AlreadyAddRefed.h"

/* starting interface:    nsIEventTarget */
#define NS_IEVENTTARGET_IID_STR "a03b8b63-af8b-4164-b0e5-c41e8b2b7cfa"

#define NS_IEVENTTARGET_IID \
  {0xa03b8b63, 0xaf8b, 0x4164, \
    { 0xb0, 0xe5, 0xc4, 0x1e, 0x8b, 0x2b, 0x7c, 0xfa }}

class nsIEventTarget : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IEVENTTARGET_IID)

     nsresult Dispatch(nsIRunnable* aEvent, uint32_t aFlags) {
      return Dispatch(nsCOMPtr<nsIRunnable>(aEvent).forget(), aFlags);
    }
  enum {
    DISPATCH_NORMAL = 0U,
    DISPATCH_SYNC = 1U,
    DISPATCH_AT_END = 2U
  };

 public:
  // Infallible. Defined in nsThreadUtils.cpp. Delegates to
  // IsOnCurrentThreadInfallible when mVirtualThread is null.
  bool IsOnCurrentThread();
protected:
  // PRThread* mVirtualThread;
  // nsIEventTarget() : mVirtualThread(nullptr) {}
    /* [noscript,notxpcom] boolean isOnCurrentThreadInfallible (); */
  NS_IMETHOD_(bool) IsOnCurrentThreadInfallible(void) = 0;

 public:
    /* boolean isOnCurrentThread (); */
  NS_IMETHOD IsOnCurrentThread(bool *_retval) = 0;

  /* [binaryname(Dispatch),noscript] void dispatchFromC (in alreadyAddRefed_nsIRunnable event, [default (DISPATCH_NORMAL)] in unsigned long flags); */
  NS_IMETHOD Dispatch(already_AddRefed<nsIRunnable> event, uint32_t flags = DISPATCH_NORMAL) = 0;

  /* [binaryname(DispatchFromScript)] void dispatch (in nsIRunnable event, in unsigned long flags); */
  NS_IMETHOD DispatchFromScript(nsIRunnable *event, uint32_t flags) = 0;

  /* [noscript] void delayedDispatch (in alreadyAddRefed_nsIRunnable event, in unsigned long delay); */
  NS_IMETHOD DelayedDispatch(already_AddRefed<nsIRunnable> event, uint32_t delay) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIEventTarget, NS_IEVENTTARGET_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEVENTTARGET \
  NS_IMETHOD_(bool) IsOnCurrentThreadInfallible(void) override; \
  NS_IMETHOD IsOnCurrentThread(bool *_retval) override; \
  NS_IMETHOD Dispatch(already_AddRefed<nsIRunnable> event, uint32_t flags = DISPATCH_NORMAL) override; \
  NS_IMETHOD DispatchFromScript(nsIRunnable *event, uint32_t flags) override; \
  NS_IMETHOD DelayedDispatch(already_AddRefed<nsIRunnable> event, uint32_t delay) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIEVENTTARGET \
  nsresult_(bool) IsOnCurrentThreadInfallible(void); \
  nsresult IsOnCurrentThread(bool *_retval); \
  nsresult Dispatch(already_AddRefed<nsIRunnable> event, uint32_t flags = DISPATCH_NORMAL); \
  nsresult DispatchFromScript(nsIRunnable *event, uint32_t flags); \
  nsresult DelayedDispatch(already_AddRefed<nsIRunnable> event, uint32_t delay); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEVENTTARGET(_to) \
  NS_IMETHOD_(bool) IsOnCurrentThreadInfallible(void) override { return _to IsOnCurrentThreadInfallible(); } \
  NS_IMETHOD IsOnCurrentThread(bool *_retval) override { return _to IsOnCurrentThread(_retval); } \
  NS_IMETHOD Dispatch(already_AddRefed<nsIRunnable> event, uint32_t flags = DISPATCH_NORMAL) override { return _to Dispatch(event, flags); } \
  NS_IMETHOD DispatchFromScript(nsIRunnable *event, uint32_t flags) override { return _to DispatchFromScript(event, flags); } \
  NS_IMETHOD DelayedDispatch(already_AddRefed<nsIRunnable> event, uint32_t delay) override { return _to DelayedDispatch(event, delay); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEVENTTARGET(_to) \
  NS_IMETHOD_(bool) IsOnCurrentThreadInfallible(void) override; \
  NS_IMETHOD IsOnCurrentThread(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsOnCurrentThread(_retval); } \
  NS_IMETHOD Dispatch(already_AddRefed<nsIRunnable> event, uint32_t flags = DISPATCH_NORMAL) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Dispatch(event, flags); } \
  NS_IMETHOD DispatchFromScript(nsIRunnable *event, uint32_t flags) override { return !_to ? NS_ERROR_NULL_POINTER : _to->DispatchFromScript(event, flags); } \
  NS_IMETHOD DelayedDispatch(already_AddRefed<nsIRunnable> event, uint32_t delay) override { return !_to ? NS_ERROR_NULL_POINTER : _to->DelayedDispatch(event, delay); } 

// convenient aliases:
#define NS_DISPATCH_NORMAL nsIEventTarget::DISPATCH_NORMAL
#define NS_DISPATCH_SYNC   nsIEventTarget::DISPATCH_SYNC
#define NS_DISPATCH_AT_END nsIEventTarget::DISPATCH_AT_END
// Convenient NS_DECL variant that includes some C++-only methods.
#define NS_DECL_NSIEVENTTARGET_FULL                                   \
    NS_DECL_NSIEVENTTARGET                                            \
    /* Avoid hiding these methods */                                  \
    using nsIEventTarget::Dispatch;                                   \
    using nsIEventTarget::IsOnCurrentThread;

#endif /* __gen_nsIEventTarget_h__ */
