/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsISerialEventTarget.idl
 */

#ifndef __gen_nsISerialEventTarget_h__
#define __gen_nsISerialEventTarget_h__


#ifndef __gen_nsIEventTarget_h__
#include "nsIEventTarget.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsISerialEventTarget */
#define NS_ISERIALEVENTTARGET_IID_STR "9f982380-24b4-49f3-88f6-45e2952036c7"

#define NS_ISERIALEVENTTARGET_IID \
  {0x9f982380, 0x24b4, 0x49f3, \
    { 0x88, 0xf6, 0x45, 0xe2, 0x95, 0x20, 0x36, 0xc7 }}

class NS_NO_VTABLE nsISerialEventTarget : public nsIEventTarget {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISERIALEVENTTARGET_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISerialEventTarget, NS_ISERIALEVENTTARGET_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISERIALEVENTTARGET \
  /* no methods! */

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISERIALEVENTTARGET \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISERIALEVENTTARGET(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISERIALEVENTTARGET(_to) \
  /* no methods! */


#endif /* __gen_nsISerialEventTarget_h__ */
