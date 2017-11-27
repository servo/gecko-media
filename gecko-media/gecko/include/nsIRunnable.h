/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIRunnable.idl
 */

#ifndef __gen_nsIRunnable_h__
#define __gen_nsIRunnable_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIRunnable */
#define NS_IRUNNABLE_IID_STR "4a2abaf0-6886-11d3-9382-00104ba0fd40"

#define NS_IRUNNABLE_IID \
  {0x4a2abaf0, 0x6886, 0x11d3, \
    { 0x93, 0x82, 0x00, 0x10, 0x4b, 0xa0, 0xfd, 0x40 }}

class NS_NO_VTABLE nsIRunnable : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IRUNNABLE_IID)

  /* void run (); */
  NS_IMETHOD Run(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIRunnable, NS_IRUNNABLE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIRUNNABLE \
  NS_IMETHOD Run(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIRUNNABLE \
  nsresult Run(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIRUNNABLE(_to) \
  NS_IMETHOD Run(void) override { return _to Run(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIRUNNABLE(_to) \
  NS_IMETHOD Run(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Run(); } 


/* starting interface:    nsIRunnablePriority */
#define NS_IRUNNABLEPRIORITY_IID_STR "e75aa42a-80a9-11e6-afb5-e89d87348e2c"

#define NS_IRUNNABLEPRIORITY_IID \
  {0xe75aa42a, 0x80a9, 0x11e6, \
    { 0xaf, 0xb5, 0xe8, 0x9d, 0x87, 0x34, 0x8e, 0x2c }}

class NS_NO_VTABLE nsIRunnablePriority : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IRUNNABLEPRIORITY_IID)

  enum {
    PRIORITY_NORMAL = 0U,
    PRIORITY_INPUT = 1U,
    PRIORITY_HIGH = 2U
  };

  /* readonly attribute unsigned long priority; */
  NS_IMETHOD GetPriority(uint32_t *aPriority) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIRunnablePriority, NS_IRUNNABLEPRIORITY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIRUNNABLEPRIORITY \
  NS_IMETHOD GetPriority(uint32_t *aPriority) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIRUNNABLEPRIORITY \
  nsresult GetPriority(uint32_t *aPriority); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIRUNNABLEPRIORITY(_to) \
  NS_IMETHOD GetPriority(uint32_t *aPriority) override { return _to GetPriority(aPriority); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIRUNNABLEPRIORITY(_to) \
  NS_IMETHOD GetPriority(uint32_t *aPriority) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPriority(aPriority); } 


#endif /* __gen_nsIRunnable_h__ */
