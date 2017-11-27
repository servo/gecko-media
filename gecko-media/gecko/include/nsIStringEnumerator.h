/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIStringEnumerator.idl
 */

#ifndef __gen_nsIStringEnumerator_h__
#define __gen_nsIStringEnumerator_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIStringEnumerator */
#define NS_ISTRINGENUMERATOR_IID_STR "50d3ef6c-9380-4f06-9fb2-95488f7d141c"

#define NS_ISTRINGENUMERATOR_IID \
  {0x50d3ef6c, 0x9380, 0x4f06, \
    { 0x9f, 0xb2, 0x95, 0x48, 0x8f, 0x7d, 0x14, 0x1c }}

class NS_NO_VTABLE nsIStringEnumerator : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISTRINGENUMERATOR_IID)

  /* boolean hasMore (); */
  NS_IMETHOD HasMore(bool *_retval) = 0;

  /* AString getNext (); */
  NS_IMETHOD GetNext(nsAString & _retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIStringEnumerator, NS_ISTRINGENUMERATOR_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISTRINGENUMERATOR \
  NS_IMETHOD HasMore(bool *_retval) override; \
  NS_IMETHOD GetNext(nsAString & _retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISTRINGENUMERATOR \
  nsresult HasMore(bool *_retval); \
  nsresult GetNext(nsAString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISTRINGENUMERATOR(_to) \
  NS_IMETHOD HasMore(bool *_retval) override { return _to HasMore(_retval); } \
  NS_IMETHOD GetNext(nsAString & _retval) override { return _to GetNext(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISTRINGENUMERATOR(_to) \
  NS_IMETHOD HasMore(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->HasMore(_retval); } \
  NS_IMETHOD GetNext(nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNext(_retval); } 


/* starting interface:    nsIUTF8StringEnumerator */
#define NS_IUTF8STRINGENUMERATOR_IID_STR "9bdf1010-3695-4907-95ed-83d0410ec307"

#define NS_IUTF8STRINGENUMERATOR_IID \
  {0x9bdf1010, 0x3695, 0x4907, \
    { 0x95, 0xed, 0x83, 0xd0, 0x41, 0x0e, 0xc3, 0x07 }}

class NS_NO_VTABLE nsIUTF8StringEnumerator : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IUTF8STRINGENUMERATOR_IID)

  /* boolean hasMore (); */
  NS_IMETHOD HasMore(bool *_retval) = 0;

  /* AUTF8String getNext (); */
  NS_IMETHOD GetNext(nsACString & _retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIUTF8StringEnumerator, NS_IUTF8STRINGENUMERATOR_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIUTF8STRINGENUMERATOR \
  NS_IMETHOD HasMore(bool *_retval) override; \
  NS_IMETHOD GetNext(nsACString & _retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIUTF8STRINGENUMERATOR \
  nsresult HasMore(bool *_retval); \
  nsresult GetNext(nsACString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIUTF8STRINGENUMERATOR(_to) \
  NS_IMETHOD HasMore(bool *_retval) override { return _to HasMore(_retval); } \
  NS_IMETHOD GetNext(nsACString & _retval) override { return _to GetNext(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIUTF8STRINGENUMERATOR(_to) \
  NS_IMETHOD HasMore(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->HasMore(_retval); } \
  NS_IMETHOD GetNext(nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNext(_retval); } 


#endif /* __gen_nsIStringEnumerator_h__ */
