/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIStringBundleOverride.idl
 */

#ifndef __gen_nsIStringBundleOverride_h__
#define __gen_nsIStringBundleOverride_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIStringBundleOverride */
#define NS_ISTRINGBUNDLEOVERRIDE_IID_STR "965eb278-5678-456b-82a7-20a0c86a803c"

#define NS_ISTRINGBUNDLEOVERRIDE_IID \
  {0x965eb278, 0x5678, 0x456b, \
    { 0x82, 0xa7, 0x20, 0xa0, 0xc8, 0x6a, 0x80, 0x3c }}

class NS_NO_VTABLE nsIStringBundleOverride : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISTRINGBUNDLEOVERRIDE_IID)

  /* AString getStringFromName (in AUTF8String url, in ACString key); */
  NS_IMETHOD GetStringFromName(const nsACString & url, const nsACString & key, nsAString & _retval) = 0;

  /* nsISimpleEnumerator enumerateKeysInBundle (in AUTF8String url); */
  NS_IMETHOD EnumerateKeysInBundle(const nsACString & url, nsISimpleEnumerator * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIStringBundleOverride, NS_ISTRINGBUNDLEOVERRIDE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISTRINGBUNDLEOVERRIDE \
  NS_IMETHOD GetStringFromName(const nsACString & url, const nsACString & key, nsAString & _retval) override; \
  NS_IMETHOD EnumerateKeysInBundle(const nsACString & url, nsISimpleEnumerator * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISTRINGBUNDLEOVERRIDE \
  nsresult GetStringFromName(const nsACString & url, const nsACString & key, nsAString & _retval); \
  nsresult EnumerateKeysInBundle(const nsACString & url, nsISimpleEnumerator * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISTRINGBUNDLEOVERRIDE(_to) \
  NS_IMETHOD GetStringFromName(const nsACString & url, const nsACString & key, nsAString & _retval) override { return _to GetStringFromName(url, key, _retval); } \
  NS_IMETHOD EnumerateKeysInBundle(const nsACString & url, nsISimpleEnumerator * *_retval) override { return _to EnumerateKeysInBundle(url, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISTRINGBUNDLEOVERRIDE(_to) \
  NS_IMETHOD GetStringFromName(const nsACString & url, const nsACString & key, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringFromName(url, key, _retval); } \
  NS_IMETHOD EnumerateKeysInBundle(const nsACString & url, nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EnumerateKeysInBundle(url, _retval); } 


#endif /* __gen_nsIStringBundleOverride_h__ */
