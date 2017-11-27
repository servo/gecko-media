/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsINamed.idl
 */

#ifndef __gen_nsINamed_h__
#define __gen_nsINamed_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsINamed */
#define NS_INAMED_IID_STR "0c5fe7de-7e83-4d0d-a8a6-4a6518b9a7b3"

#define NS_INAMED_IID \
  {0x0c5fe7de, 0x7e83, 0x4d0d, \
    { 0xa8, 0xa6, 0x4a, 0x65, 0x18, 0xb9, 0xa7, 0xb3 }}

class NS_NO_VTABLE nsINamed : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_INAMED_IID)

  /* readonly attribute AUTF8String name; */
  NS_IMETHOD GetName(nsACString & aName) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsINamed, NS_INAMED_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSINAMED \
  NS_IMETHOD GetName(nsACString & aName) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSINAMED \
  nsresult GetName(nsACString & aName); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSINAMED(_to) \
  NS_IMETHOD GetName(nsACString & aName) override { return _to GetName(aName); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSINAMED(_to) \
  NS_IMETHOD GetName(nsACString & aName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } 


#endif /* __gen_nsINamed_h__ */
