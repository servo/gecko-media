/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIPrincipal.idl
 */

#ifndef __gen_nsIPrincipal_h__
#define __gen_nsIPrincipal_h__

#ifndef __gen_nsISerializable_h__
#include "nsISerializable.h"
#endif

/* starting interface:    nsIPrincipal */
#define NS_IPRINCIPAL_IID_STR "f75f502d-79fd-48be-a079-e5a7b8f80c8b"

#define NS_IPRINCIPAL_IID \
  {0xf75f502d, 0x79fd, 0x48be, \
    { 0xa0, 0x79, 0xe5, 0xa7, 0xb8, 0xf8, 0x0c, 0x8b }}

class nsIPrincipal : public nsISerializable {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPRINCIPAL_IID)

  /* boolean equals (in nsIPrincipal other); */
  NS_IMETHOD Equals(nsIPrincipal *other, bool *_retval) = 0;
};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPrincipal, NS_IPRINCIPAL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPRINCIPAL \
  NS_IMETHOD Equals(nsIPrincipal *other, bool *_retval) override;

#endif /* __gen_nsIPrincipal_h__ */
