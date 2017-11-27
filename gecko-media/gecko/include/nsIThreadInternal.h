/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIThreadInternal.idl
 */

#ifndef __gen_nsIThreadInternal_h__
#define __gen_nsIThreadInternal_h__


#ifndef __gen_nsIThread_h__
#include "nsIThread.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIRunnable; /* forward declaration */

class nsIThreadObserver; /* forward declaration */


/* starting interface:    nsIThreadInternal */
#define NS_ITHREADINTERNAL_IID_STR "a3a72e5f-71d9-4add-8f30-59a78fb6d5eb"

#define NS_ITHREADINTERNAL_IID \
  {0xa3a72e5f, 0x71d9, 0x4add, \
    { 0x8f, 0x30, 0x59, 0xa7, 0x8f, 0xb6, 0xd5, 0xeb }}

class NS_NO_VTABLE nsIThreadInternal : public nsIThread {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITHREADINTERNAL_IID)

  /* attribute nsIThreadObserver observer; */
  NS_IMETHOD GetObserver(nsIThreadObserver * *aObserver) = 0;
  NS_IMETHOD SetObserver(nsIThreadObserver *aObserver) = 0;

  /* void addObserver (in nsIThreadObserver observer); */
  NS_IMETHOD AddObserver(nsIThreadObserver *observer) = 0;

  /* void removeObserver (in nsIThreadObserver observer); */
  NS_IMETHOD RemoveObserver(nsIThreadObserver *observer) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIThreadInternal, NS_ITHREADINTERNAL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREADINTERNAL \
  NS_IMETHOD GetObserver(nsIThreadObserver * *aObserver) override; \
  NS_IMETHOD SetObserver(nsIThreadObserver *aObserver) override; \
  NS_IMETHOD AddObserver(nsIThreadObserver *observer) override; \
  NS_IMETHOD RemoveObserver(nsIThreadObserver *observer) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITHREADINTERNAL \
  nsresult GetObserver(nsIThreadObserver * *aObserver); \
  nsresult SetObserver(nsIThreadObserver *aObserver); \
  nsresult AddObserver(nsIThreadObserver *observer); \
  nsresult RemoveObserver(nsIThreadObserver *observer); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREADINTERNAL(_to) \
  NS_IMETHOD GetObserver(nsIThreadObserver * *aObserver) override { return _to GetObserver(aObserver); } \
  NS_IMETHOD SetObserver(nsIThreadObserver *aObserver) override { return _to SetObserver(aObserver); } \
  NS_IMETHOD AddObserver(nsIThreadObserver *observer) override { return _to AddObserver(observer); } \
  NS_IMETHOD RemoveObserver(nsIThreadObserver *observer) override { return _to RemoveObserver(observer); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREADINTERNAL(_to) \
  NS_IMETHOD GetObserver(nsIThreadObserver * *aObserver) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetObserver(aObserver); } \
  NS_IMETHOD SetObserver(nsIThreadObserver *aObserver) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetObserver(aObserver); } \
  NS_IMETHOD AddObserver(nsIThreadObserver *observer) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AddObserver(observer); } \
  NS_IMETHOD RemoveObserver(nsIThreadObserver *observer) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveObserver(observer); } 


/* starting interface:    nsIThreadObserver */
#define NS_ITHREADOBSERVER_IID_STR "cc8da053-1776-44c2-9199-b5a629d0a19d"

#define NS_ITHREADOBSERVER_IID \
  {0xcc8da053, 0x1776, 0x44c2, \
    { 0x91, 0x99, 0xb5, 0xa6, 0x29, 0xd0, 0xa1, 0x9d }}

class NS_NO_VTABLE nsIThreadObserver : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITHREADOBSERVER_IID)

  /* void onDispatchedEvent (); */
  NS_IMETHOD OnDispatchedEvent(void) = 0;

  /* void onProcessNextEvent (in nsIThreadInternal thread, in boolean mayWait); */
  NS_IMETHOD OnProcessNextEvent(nsIThreadInternal *thread, bool mayWait) = 0;

  /* void afterProcessNextEvent (in nsIThreadInternal thread, in bool eventWasProcessed); */
  NS_IMETHOD AfterProcessNextEvent(nsIThreadInternal *thread, bool eventWasProcessed) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIThreadObserver, NS_ITHREADOBSERVER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREADOBSERVER \
  NS_IMETHOD OnDispatchedEvent(void) override; \
  NS_IMETHOD OnProcessNextEvent(nsIThreadInternal *thread, bool mayWait) override; \
  NS_IMETHOD AfterProcessNextEvent(nsIThreadInternal *thread, bool eventWasProcessed) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITHREADOBSERVER \
  nsresult OnDispatchedEvent(void); \
  nsresult OnProcessNextEvent(nsIThreadInternal *thread, bool mayWait); \
  nsresult AfterProcessNextEvent(nsIThreadInternal *thread, bool eventWasProcessed); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREADOBSERVER(_to) \
  NS_IMETHOD OnDispatchedEvent(void) override { return _to OnDispatchedEvent(); } \
  NS_IMETHOD OnProcessNextEvent(nsIThreadInternal *thread, bool mayWait) override { return _to OnProcessNextEvent(thread, mayWait); } \
  NS_IMETHOD AfterProcessNextEvent(nsIThreadInternal *thread, bool eventWasProcessed) override { return _to AfterProcessNextEvent(thread, eventWasProcessed); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREADOBSERVER(_to) \
  NS_IMETHOD OnDispatchedEvent(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->OnDispatchedEvent(); } \
  NS_IMETHOD OnProcessNextEvent(nsIThreadInternal *thread, bool mayWait) override { return !_to ? NS_ERROR_NULL_POINTER : _to->OnProcessNextEvent(thread, mayWait); } \
  NS_IMETHOD AfterProcessNextEvent(nsIThreadInternal *thread, bool eventWasProcessed) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AfterProcessNextEvent(thread, eventWasProcessed); } 


#endif /* __gen_nsIThreadInternal_h__ */
