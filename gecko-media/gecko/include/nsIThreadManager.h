/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIThreadManager.idl
 */

#ifndef __gen_nsIThreadManager_h__
#define __gen_nsIThreadManager_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIEventTarget; /* forward declaration */

class nsIRunnable; /* forward declaration */

class nsIThread; /* forward declaration */


/* starting interface:    nsINestedEventLoopCondition */
#define NS_INESTEDEVENTLOOPCONDITION_IID_STR "039a227d-0cb7-44a5-a8f9-dbb7071979f2"

#define NS_INESTEDEVENTLOOPCONDITION_IID \
  {0x039a227d, 0x0cb7, 0x44a5, \
    { 0xa8, 0xf9, 0xdb, 0xb7, 0x07, 0x19, 0x79, 0xf2 }}

class NS_NO_VTABLE nsINestedEventLoopCondition : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_INESTEDEVENTLOOPCONDITION_IID)

  /* bool isDone (); */
  NS_IMETHOD IsDone(bool *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsINestedEventLoopCondition, NS_INESTEDEVENTLOOPCONDITION_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSINESTEDEVENTLOOPCONDITION \
  NS_IMETHOD IsDone(bool *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSINESTEDEVENTLOOPCONDITION \
  nsresult IsDone(bool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSINESTEDEVENTLOOPCONDITION(_to) \
  NS_IMETHOD IsDone(bool *_retval) override { return _to IsDone(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSINESTEDEVENTLOOPCONDITION(_to) \
  NS_IMETHOD IsDone(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsDone(_retval); } 


/* starting interface:    nsIThreadManager */
#define NS_ITHREADMANAGER_IID_STR "1be89eca-e2f7-453b-8d38-c11ba247f6f3"

#define NS_ITHREADMANAGER_IID \
  {0x1be89eca, 0xe2f7, 0x453b, \
    { 0x8d, 0x38, 0xc1, 0x1b, 0xa2, 0x47, 0xf6, 0xf3 }}

class NS_NO_VTABLE nsIThreadManager : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITHREADMANAGER_IID)

  enum {
    DEFAULT_STACK_SIZE = 0U
  };

  /* nsIThread newThread (in unsigned long creationFlags, [optional] in unsigned long stackSize); */
  NS_IMETHOD NewThread(uint32_t creationFlags, uint32_t stackSize, nsIThread * *_retval) = 0;

  /* [noscript] nsIThread newNamedThread (in ACString name, [optional] in unsigned long stackSize); */
  NS_IMETHOD NewNamedThread(const nsACString & name, uint32_t stackSize, nsIThread * *_retval) = 0;

  /* [noscript] nsIThread getThreadFromPRThread (in PRThread prthread); */
  NS_IMETHOD GetThreadFromPRThread(PRThread *prthread, nsIThread * *_retval) = 0;

  /* readonly attribute nsIThread mainThread; */
  NS_IMETHOD GetMainThread(nsIThread * *aMainThread) = 0;

  /* readonly attribute nsIThread currentThread; */
  NS_IMETHOD GetCurrentThread(nsIThread * *aCurrentThread) = 0;

  /* void dispatchToMainThread (in nsIRunnable event, [optional] in uint32_t priority); */
  NS_IMETHOD DispatchToMainThread(nsIRunnable *event, uint32_t priority) = 0;

  /* void idleDispatchToMainThread (in nsIRunnable event, [optional] in uint32_t timeout); */
  NS_IMETHOD IdleDispatchToMainThread(nsIRunnable *event, uint32_t timeout) = 0;

  /* void spinEventLoopUntil (in nsINestedEventLoopCondition condition); */
  NS_IMETHOD SpinEventLoopUntil(nsINestedEventLoopCondition *condition) = 0;

  /* void spinEventLoopUntilEmpty (); */
  NS_IMETHOD SpinEventLoopUntilEmpty(void) = 0;

  /* readonly attribute nsIEventTarget systemGroupEventTarget; */
  NS_IMETHOD GetSystemGroupEventTarget(nsIEventTarget * *aSystemGroupEventTarget) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIThreadManager, NS_ITHREADMANAGER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREADMANAGER \
  NS_IMETHOD NewThread(uint32_t creationFlags, uint32_t stackSize, nsIThread * *_retval) override; \
  NS_IMETHOD NewNamedThread(const nsACString & name, uint32_t stackSize, nsIThread * *_retval) override; \
  NS_IMETHOD GetThreadFromPRThread(PRThread *prthread, nsIThread * *_retval) override; \
  NS_IMETHOD GetMainThread(nsIThread * *aMainThread) override; \
  NS_IMETHOD GetCurrentThread(nsIThread * *aCurrentThread) override; \
  NS_IMETHOD DispatchToMainThread(nsIRunnable *event, uint32_t priority) override; \
  NS_IMETHOD IdleDispatchToMainThread(nsIRunnable *event, uint32_t timeout) override; \
  NS_IMETHOD SpinEventLoopUntil(nsINestedEventLoopCondition *condition) override; \
  NS_IMETHOD SpinEventLoopUntilEmpty(void) override; \
  NS_IMETHOD GetSystemGroupEventTarget(nsIEventTarget * *aSystemGroupEventTarget) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITHREADMANAGER \
  nsresult NewThread(uint32_t creationFlags, uint32_t stackSize, nsIThread * *_retval); \
  nsresult NewNamedThread(const nsACString & name, uint32_t stackSize, nsIThread * *_retval); \
  nsresult GetThreadFromPRThread(PRThread *prthread, nsIThread * *_retval); \
  nsresult GetMainThread(nsIThread * *aMainThread); \
  nsresult GetCurrentThread(nsIThread * *aCurrentThread); \
  nsresult DispatchToMainThread(nsIRunnable *event, uint32_t priority); \
  nsresult IdleDispatchToMainThread(nsIRunnable *event, uint32_t timeout); \
  nsresult SpinEventLoopUntil(nsINestedEventLoopCondition *condition); \
  nsresult SpinEventLoopUntilEmpty(void); \
  nsresult GetSystemGroupEventTarget(nsIEventTarget * *aSystemGroupEventTarget); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREADMANAGER(_to) \
  NS_IMETHOD NewThread(uint32_t creationFlags, uint32_t stackSize, nsIThread * *_retval) override { return _to NewThread(creationFlags, stackSize, _retval); } \
  NS_IMETHOD NewNamedThread(const nsACString & name, uint32_t stackSize, nsIThread * *_retval) override { return _to NewNamedThread(name, stackSize, _retval); } \
  NS_IMETHOD GetThreadFromPRThread(PRThread *prthread, nsIThread * *_retval) override { return _to GetThreadFromPRThread(prthread, _retval); } \
  NS_IMETHOD GetMainThread(nsIThread * *aMainThread) override { return _to GetMainThread(aMainThread); } \
  NS_IMETHOD GetCurrentThread(nsIThread * *aCurrentThread) override { return _to GetCurrentThread(aCurrentThread); } \
  NS_IMETHOD DispatchToMainThread(nsIRunnable *event, uint32_t priority) override { return _to DispatchToMainThread(event, priority); } \
  NS_IMETHOD IdleDispatchToMainThread(nsIRunnable *event, uint32_t timeout) override { return _to IdleDispatchToMainThread(event, timeout); } \
  NS_IMETHOD SpinEventLoopUntil(nsINestedEventLoopCondition *condition) override { return _to SpinEventLoopUntil(condition); } \
  NS_IMETHOD SpinEventLoopUntilEmpty(void) override { return _to SpinEventLoopUntilEmpty(); } \
  NS_IMETHOD GetSystemGroupEventTarget(nsIEventTarget * *aSystemGroupEventTarget) override { return _to GetSystemGroupEventTarget(aSystemGroupEventTarget); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREADMANAGER(_to) \
  NS_IMETHOD NewThread(uint32_t creationFlags, uint32_t stackSize, nsIThread * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->NewThread(creationFlags, stackSize, _retval); } \
  NS_IMETHOD NewNamedThread(const nsACString & name, uint32_t stackSize, nsIThread * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->NewNamedThread(name, stackSize, _retval); } \
  NS_IMETHOD GetThreadFromPRThread(PRThread *prthread, nsIThread * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetThreadFromPRThread(prthread, _retval); } \
  NS_IMETHOD GetMainThread(nsIThread * *aMainThread) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMainThread(aMainThread); } \
  NS_IMETHOD GetCurrentThread(nsIThread * *aCurrentThread) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCurrentThread(aCurrentThread); } \
  NS_IMETHOD DispatchToMainThread(nsIRunnable *event, uint32_t priority) override { return !_to ? NS_ERROR_NULL_POINTER : _to->DispatchToMainThread(event, priority); } \
  NS_IMETHOD IdleDispatchToMainThread(nsIRunnable *event, uint32_t timeout) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IdleDispatchToMainThread(event, timeout); } \
  NS_IMETHOD SpinEventLoopUntil(nsINestedEventLoopCondition *condition) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SpinEventLoopUntil(condition); } \
  NS_IMETHOD SpinEventLoopUntilEmpty(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SpinEventLoopUntilEmpty(); } \
  NS_IMETHOD GetSystemGroupEventTarget(nsIEventTarget * *aSystemGroupEventTarget) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSystemGroupEventTarget(aSystemGroupEventTarget); } 


#endif /* __gen_nsIThreadManager_h__ */
