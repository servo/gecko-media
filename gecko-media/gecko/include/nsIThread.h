/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIThread.idl
 */

#ifndef __gen_nsIThread_h__
#define __gen_nsIThread_h__


#ifndef __gen_nsISerialEventTarget_h__
#include "nsISerialEventTarget.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "mozilla/AlreadyAddRefed.h"

/* starting interface:    nsIThread */
#define NS_ITHREAD_IID_STR "5801d193-29d1-4964-a6b7-70eb697ddf2b"

#define NS_ITHREAD_IID \
  {0x5801d193, 0x29d1, 0x4964, \
    { 0xa6, 0xb7, 0x70, 0xeb, 0x69, 0x7d, 0xdf, 0x2b }}

class NS_NO_VTABLE nsIThread : public nsISerialEventTarget {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITHREAD_IID)

  /* [noscript] readonly attribute PRThread PRThread; */
  NS_IMETHOD GetPRThread(PRThread **aPRThread) = 0;

  /* [noscript] attribute boolean CanInvokeJS; */
  NS_IMETHOD GetCanInvokeJS(bool *aCanInvokeJS) = 0;
  NS_IMETHOD SetCanInvokeJS(bool aCanInvokeJS) = 0;

  /* void shutdown (); */
  NS_IMETHOD Shutdown(void) = 0;

  /* boolean hasPendingEvents (); */
  NS_IMETHOD HasPendingEvents(bool *_retval) = 0;

  /* boolean processNextEvent (in boolean mayWait); */
  NS_IMETHOD ProcessNextEvent(bool mayWait, bool *_retval) = 0;

  /* void asyncShutdown (); */
  NS_IMETHOD AsyncShutdown(void) = 0;

  /* [noscript] void idleDispatch (in alreadyAddRefed_nsIRunnable event); */
  NS_IMETHOD IdleDispatch(already_AddRefed<nsIRunnable> event) = 0;

  /* readonly attribute nsIEventTarget eventTarget; */
  NS_IMETHOD GetEventTarget(nsIEventTarget * *aEventTarget) = 0;

  /* [noscript,notxpcom] nsIEventTargetPtr EventTarget (); */
  NS_IMETHOD_(nsIEventTarget*) EventTarget(void) = 0;

  /* [noscript,notxpcom] nsISerialEventTargetPtr SerialEventTarget (); */
  NS_IMETHOD_(nsISerialEventTarget*) SerialEventTarget(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIThread, NS_ITHREAD_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREAD \
  NS_IMETHOD GetPRThread(PRThread **aPRThread) override; \
  NS_IMETHOD GetCanInvokeJS(bool *aCanInvokeJS) override; \
  NS_IMETHOD SetCanInvokeJS(bool aCanInvokeJS) override; \
  NS_IMETHOD Shutdown(void) override; \
  NS_IMETHOD HasPendingEvents(bool *_retval) override; \
  NS_IMETHOD ProcessNextEvent(bool mayWait, bool *_retval) override; \
  NS_IMETHOD AsyncShutdown(void) override; \
  NS_IMETHOD IdleDispatch(already_AddRefed<nsIRunnable> event) override; \
  NS_IMETHOD GetEventTarget(nsIEventTarget * *aEventTarget) override; \
  NS_IMETHOD_(nsIEventTarget*) EventTarget(void) override; \
  NS_IMETHOD_(nsISerialEventTarget*) SerialEventTarget(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITHREAD \
  nsresult GetPRThread(PRThread **aPRThread); \
  nsresult GetCanInvokeJS(bool *aCanInvokeJS); \
  nsresult SetCanInvokeJS(bool aCanInvokeJS); \
  nsresult Shutdown(void); \
  nsresult HasPendingEvents(bool *_retval); \
  nsresult ProcessNextEvent(bool mayWait, bool *_retval); \
  nsresult AsyncShutdown(void); \
  nsresult IdleDispatch(already_AddRefed<nsIRunnable> event); \
  nsresult GetEventTarget(nsIEventTarget * *aEventTarget); \
  nsresult_(nsIEventTarget*) EventTarget(void); \
  nsresult_(nsISerialEventTarget*) SerialEventTarget(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREAD(_to) \
  NS_IMETHOD GetPRThread(PRThread **aPRThread) override { return _to GetPRThread(aPRThread); } \
  NS_IMETHOD GetCanInvokeJS(bool *aCanInvokeJS) override { return _to GetCanInvokeJS(aCanInvokeJS); } \
  NS_IMETHOD SetCanInvokeJS(bool aCanInvokeJS) override { return _to SetCanInvokeJS(aCanInvokeJS); } \
  NS_IMETHOD Shutdown(void) override { return _to Shutdown(); } \
  NS_IMETHOD HasPendingEvents(bool *_retval) override { return _to HasPendingEvents(_retval); } \
  NS_IMETHOD ProcessNextEvent(bool mayWait, bool *_retval) override { return _to ProcessNextEvent(mayWait, _retval); } \
  NS_IMETHOD AsyncShutdown(void) override { return _to AsyncShutdown(); } \
  NS_IMETHOD IdleDispatch(already_AddRefed<nsIRunnable> event) override { return _to IdleDispatch(event); } \
  NS_IMETHOD GetEventTarget(nsIEventTarget * *aEventTarget) override { return _to GetEventTarget(aEventTarget); } \
  NS_IMETHOD_(nsIEventTarget*) EventTarget(void) override { return _to EventTarget(); } \
  NS_IMETHOD_(nsISerialEventTarget*) SerialEventTarget(void) override { return _to SerialEventTarget(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREAD(_to) \
  NS_IMETHOD GetPRThread(PRThread **aPRThread) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRThread(aPRThread); } \
  NS_IMETHOD GetCanInvokeJS(bool *aCanInvokeJS) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCanInvokeJS(aCanInvokeJS); } \
  NS_IMETHOD SetCanInvokeJS(bool aCanInvokeJS) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCanInvokeJS(aCanInvokeJS); } \
  NS_IMETHOD Shutdown(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Shutdown(); } \
  NS_IMETHOD HasPendingEvents(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->HasPendingEvents(_retval); } \
  NS_IMETHOD ProcessNextEvent(bool mayWait, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ProcessNextEvent(mayWait, _retval); } \
  NS_IMETHOD AsyncShutdown(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AsyncShutdown(); } \
  NS_IMETHOD IdleDispatch(already_AddRefed<nsIRunnable> event) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IdleDispatch(event); } \
  NS_IMETHOD GetEventTarget(nsIEventTarget * *aEventTarget) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEventTarget(aEventTarget); } \
  NS_IMETHOD_(nsIEventTarget*) EventTarget(void) override; \
  NS_IMETHOD_(nsISerialEventTarget*) SerialEventTarget(void) override; 


#endif /* __gen_nsIThread_h__ */
