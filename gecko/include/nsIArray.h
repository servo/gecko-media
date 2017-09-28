/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIArray.idl
 */

#ifndef __gen_nsIArray_h__
#define __gen_nsIArray_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIArray */
#define NS_IARRAY_IID_STR "114744d9-c369-456e-b55a-52fe52880d2d"

#define NS_IARRAY_IID \
  {0x114744d9, 0xc369, 0x456e, \
    { 0xb5, 0x5a, 0x52, 0xfe, 0x52, 0x88, 0x0d, 0x2d }}

class NS_NO_VTABLE nsIArray : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IARRAY_IID)

  /* readonly attribute unsigned long length; */
  NS_IMETHOD GetLength(uint32_t *aLength) = 0;

  /* void queryElementAt (in unsigned long index, in nsIIDRef uuid, [iid_is (uuid), retval] out nsQIResult result); */
  NS_IMETHOD QueryElementAt(uint32_t index, const nsIID & uuid, void **result) = 0;

  /* unsigned long indexOf (in unsigned long startIndex, in nsISupports element); */
  NS_IMETHOD IndexOf(uint32_t startIndex, nsISupports *element, uint32_t *_retval) = 0;

  /* nsISimpleEnumerator enumerate (); */
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIArray, NS_IARRAY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIARRAY \
  NS_IMETHOD GetLength(uint32_t *aLength) override; \
  NS_IMETHOD QueryElementAt(uint32_t index, const nsIID & uuid, void **result) override; \
  NS_IMETHOD IndexOf(uint32_t startIndex, nsISupports *element, uint32_t *_retval) override; \
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIARRAY \
  nsresult GetLength(uint32_t *aLength); \
  nsresult QueryElementAt(uint32_t index, const nsIID & uuid, void **result); \
  nsresult IndexOf(uint32_t startIndex, nsISupports *element, uint32_t *_retval); \
  nsresult Enumerate(nsISimpleEnumerator * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIARRAY(_to) \
  NS_IMETHOD GetLength(uint32_t *aLength) override { return _to GetLength(aLength); } \
  NS_IMETHOD QueryElementAt(uint32_t index, const nsIID & uuid, void **result) override { return _to QueryElementAt(index, uuid, result); } \
  NS_IMETHOD IndexOf(uint32_t startIndex, nsISupports *element, uint32_t *_retval) override { return _to IndexOf(startIndex, element, _retval); } \
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) override { return _to Enumerate(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIARRAY(_to) \
  NS_IMETHOD GetLength(uint32_t *aLength) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLength(aLength); } \
  NS_IMETHOD QueryElementAt(uint32_t index, const nsIID & uuid, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->QueryElementAt(index, uuid, result); } \
  NS_IMETHOD IndexOf(uint32_t startIndex, nsISupports *element, uint32_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IndexOf(startIndex, element, _retval); } \
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Enumerate(_retval); } 


#endif /* __gen_nsIArray_h__ */
