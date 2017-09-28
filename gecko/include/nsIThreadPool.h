/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIThreadPool.idl
 */

#ifndef __gen_nsIThreadPool_h__
#define __gen_nsIThreadPool_h__


#ifndef __gen_nsIEventTarget_h__
#include "nsIEventTarget.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIThreadPoolListener */
#define NS_ITHREADPOOLLISTENER_IID_STR "ef194cab-3f86-4b61-b132-e5e96a79e5d1"

#define NS_ITHREADPOOLLISTENER_IID \
  {0xef194cab, 0x3f86, 0x4b61, \
    { 0xb1, 0x32, 0xe5, 0xe9, 0x6a, 0x79, 0xe5, 0xd1 }}

class NS_NO_VTABLE nsIThreadPoolListener : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITHREADPOOLLISTENER_IID)

  /* void onThreadCreated (); */
  NS_IMETHOD OnThreadCreated(void) = 0;

  /* void onThreadShuttingDown (); */
  NS_IMETHOD OnThreadShuttingDown(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIThreadPoolListener, NS_ITHREADPOOLLISTENER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREADPOOLLISTENER \
  NS_IMETHOD OnThreadCreated(void) override; \
  NS_IMETHOD OnThreadShuttingDown(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITHREADPOOLLISTENER \
  nsresult OnThreadCreated(void); \
  nsresult OnThreadShuttingDown(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREADPOOLLISTENER(_to) \
  NS_IMETHOD OnThreadCreated(void) override { return _to OnThreadCreated(); } \
  NS_IMETHOD OnThreadShuttingDown(void) override { return _to OnThreadShuttingDown(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREADPOOLLISTENER(_to) \
  NS_IMETHOD OnThreadCreated(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->OnThreadCreated(); } \
  NS_IMETHOD OnThreadShuttingDown(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->OnThreadShuttingDown(); } 


/* starting interface:    nsIThreadPool */
#define NS_ITHREADPOOL_IID_STR "76ce99c9-8e43-489a-9789-f27cc4424965"

#define NS_ITHREADPOOL_IID \
  {0x76ce99c9, 0x8e43, 0x489a, \
    { 0x97, 0x89, 0xf2, 0x7c, 0xc4, 0x42, 0x49, 0x65 }}

class NS_NO_VTABLE nsIThreadPool : public nsIEventTarget {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITHREADPOOL_IID)

  /* void shutdown (); */
  NS_IMETHOD Shutdown(void) = 0;

  /* attribute unsigned long threadLimit; */
  NS_IMETHOD GetThreadLimit(uint32_t *aThreadLimit) = 0;
  NS_IMETHOD SetThreadLimit(uint32_t aThreadLimit) = 0;

  /* attribute unsigned long idleThreadLimit; */
  NS_IMETHOD GetIdleThreadLimit(uint32_t *aIdleThreadLimit) = 0;
  NS_IMETHOD SetIdleThreadLimit(uint32_t aIdleThreadLimit) = 0;

  /* attribute unsigned long idleThreadTimeout; */
  NS_IMETHOD GetIdleThreadTimeout(uint32_t *aIdleThreadTimeout) = 0;
  NS_IMETHOD SetIdleThreadTimeout(uint32_t aIdleThreadTimeout) = 0;

  /* attribute unsigned long threadStackSize; */
  NS_IMETHOD GetThreadStackSize(uint32_t *aThreadStackSize) = 0;
  NS_IMETHOD SetThreadStackSize(uint32_t aThreadStackSize) = 0;

  /* attribute nsIThreadPoolListener listener; */
  NS_IMETHOD GetListener(nsIThreadPoolListener * *aListener) = 0;
  NS_IMETHOD SetListener(nsIThreadPoolListener *aListener) = 0;

  /* void setName (in ACString aName); */
  NS_IMETHOD SetName(const nsACString & aName) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIThreadPool, NS_ITHREADPOOL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREADPOOL \
  NS_IMETHOD Shutdown(void) override; \
  NS_IMETHOD GetThreadLimit(uint32_t *aThreadLimit) override; \
  NS_IMETHOD SetThreadLimit(uint32_t aThreadLimit) override; \
  NS_IMETHOD GetIdleThreadLimit(uint32_t *aIdleThreadLimit) override; \
  NS_IMETHOD SetIdleThreadLimit(uint32_t aIdleThreadLimit) override; \
  NS_IMETHOD GetIdleThreadTimeout(uint32_t *aIdleThreadTimeout) override; \
  NS_IMETHOD SetIdleThreadTimeout(uint32_t aIdleThreadTimeout) override; \
  NS_IMETHOD GetThreadStackSize(uint32_t *aThreadStackSize) override; \
  NS_IMETHOD SetThreadStackSize(uint32_t aThreadStackSize) override; \
  NS_IMETHOD GetListener(nsIThreadPoolListener * *aListener) override; \
  NS_IMETHOD SetListener(nsIThreadPoolListener *aListener) override; \
  NS_IMETHOD SetName(const nsACString & aName) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITHREADPOOL \
  nsresult Shutdown(void); \
  nsresult GetThreadLimit(uint32_t *aThreadLimit); \
  nsresult SetThreadLimit(uint32_t aThreadLimit); \
  nsresult GetIdleThreadLimit(uint32_t *aIdleThreadLimit); \
  nsresult SetIdleThreadLimit(uint32_t aIdleThreadLimit); \
  nsresult GetIdleThreadTimeout(uint32_t *aIdleThreadTimeout); \
  nsresult SetIdleThreadTimeout(uint32_t aIdleThreadTimeout); \
  nsresult GetThreadStackSize(uint32_t *aThreadStackSize); \
  nsresult SetThreadStackSize(uint32_t aThreadStackSize); \
  nsresult GetListener(nsIThreadPoolListener * *aListener); \
  nsresult SetListener(nsIThreadPoolListener *aListener); \
  nsresult SetName(const nsACString & aName); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREADPOOL(_to) \
  NS_IMETHOD Shutdown(void) override { return _to Shutdown(); } \
  NS_IMETHOD GetThreadLimit(uint32_t *aThreadLimit) override { return _to GetThreadLimit(aThreadLimit); } \
  NS_IMETHOD SetThreadLimit(uint32_t aThreadLimit) override { return _to SetThreadLimit(aThreadLimit); } \
  NS_IMETHOD GetIdleThreadLimit(uint32_t *aIdleThreadLimit) override { return _to GetIdleThreadLimit(aIdleThreadLimit); } \
  NS_IMETHOD SetIdleThreadLimit(uint32_t aIdleThreadLimit) override { return _to SetIdleThreadLimit(aIdleThreadLimit); } \
  NS_IMETHOD GetIdleThreadTimeout(uint32_t *aIdleThreadTimeout) override { return _to GetIdleThreadTimeout(aIdleThreadTimeout); } \
  NS_IMETHOD SetIdleThreadTimeout(uint32_t aIdleThreadTimeout) override { return _to SetIdleThreadTimeout(aIdleThreadTimeout); } \
  NS_IMETHOD GetThreadStackSize(uint32_t *aThreadStackSize) override { return _to GetThreadStackSize(aThreadStackSize); } \
  NS_IMETHOD SetThreadStackSize(uint32_t aThreadStackSize) override { return _to SetThreadStackSize(aThreadStackSize); } \
  NS_IMETHOD GetListener(nsIThreadPoolListener * *aListener) override { return _to GetListener(aListener); } \
  NS_IMETHOD SetListener(nsIThreadPoolListener *aListener) override { return _to SetListener(aListener); } \
  NS_IMETHOD SetName(const nsACString & aName) override { return _to SetName(aName); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREADPOOL(_to) \
  NS_IMETHOD Shutdown(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Shutdown(); } \
  NS_IMETHOD GetThreadLimit(uint32_t *aThreadLimit) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetThreadLimit(aThreadLimit); } \
  NS_IMETHOD SetThreadLimit(uint32_t aThreadLimit) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetThreadLimit(aThreadLimit); } \
  NS_IMETHOD GetIdleThreadLimit(uint32_t *aIdleThreadLimit) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIdleThreadLimit(aIdleThreadLimit); } \
  NS_IMETHOD SetIdleThreadLimit(uint32_t aIdleThreadLimit) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIdleThreadLimit(aIdleThreadLimit); } \
  NS_IMETHOD GetIdleThreadTimeout(uint32_t *aIdleThreadTimeout) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIdleThreadTimeout(aIdleThreadTimeout); } \
  NS_IMETHOD SetIdleThreadTimeout(uint32_t aIdleThreadTimeout) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIdleThreadTimeout(aIdleThreadTimeout); } \
  NS_IMETHOD GetThreadStackSize(uint32_t *aThreadStackSize) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetThreadStackSize(aThreadStackSize); } \
  NS_IMETHOD SetThreadStackSize(uint32_t aThreadStackSize) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetThreadStackSize(aThreadStackSize); } \
  NS_IMETHOD GetListener(nsIThreadPoolListener * *aListener) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetListener(aListener); } \
  NS_IMETHOD SetListener(nsIThreadPoolListener *aListener) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetListener(aListener); } \
  NS_IMETHOD SetName(const nsACString & aName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetName(aName); } 


#endif /* __gen_nsIThreadPool_h__ */
