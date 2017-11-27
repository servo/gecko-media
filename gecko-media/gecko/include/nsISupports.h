/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsISupports.idl
 */

#ifndef __gen_nsISupports_h__
#define __gen_nsISupports_h__


#ifndef __gen_nsrootidl_h__
#include "nsrootidl.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
/* 
 * Start commenting out the C++ versions of the below in the output header
 */
#if 0

/* starting interface:    nsISupports */
#define NS_ISUPPORTS_IID_STR "00000000-0000-0000-c000-000000000046"

#define NS_ISUPPORTS_IID \
  {0x00000000, 0x0000, 0x0000, \
    { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 }}

class NS_NO_VTABLE nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTS_IID)

  /* void QueryInterface (in nsIIDRef uuid, [iid_is (uuid), retval] out nsQIResult result); */
  NS_IMETHOD QueryInterface(const nsIID & uuid, void **result) = 0;

  /* [noscript,notxpcom] nsrefcnt AddRef (); */
  NS_IMETHOD_(nsrefcnt) AddRef(void) = 0;

  /* [noscript,notxpcom] nsrefcnt Release (); */
  NS_IMETHOD_(nsrefcnt) Release(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupports, NS_ISUPPORTS_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTS \
  NS_IMETHOD QueryInterface(const nsIID & uuid, void **result) override; \
  NS_IMETHOD_(nsrefcnt) AddRef(void) override; \
  NS_IMETHOD_(nsrefcnt) Release(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTS \
  nsresult QueryInterface(const nsIID & uuid, void **result); \
  nsresult_(nsrefcnt) AddRef(void); \
  nsresult_(nsrefcnt) Release(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTS(_to) \
  NS_IMETHOD QueryInterface(const nsIID & uuid, void **result) override { return _to QueryInterface(uuid, result); } \
  NS_IMETHOD_(nsrefcnt) AddRef(void) override { return _to AddRef(); } \
  NS_IMETHOD_(nsrefcnt) Release(void) override { return _to Release(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTS(_to) \
  NS_IMETHOD QueryInterface(const nsIID & uuid, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->QueryInterface(uuid, result); } \
  NS_IMETHOD_(nsrefcnt) AddRef(void) override; \
  NS_IMETHOD_(nsrefcnt) Release(void) override; 

/* 
 * End commenting out the C++ versions of the above in the output header
 */
#endif
#include "nsISupportsBase.h"
#include "nsISupportsUtils.h"

#endif /* __gen_nsISupports_h__ */
