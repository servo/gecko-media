/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIDirectoryEnumerator.idl
 */

#ifndef __gen_nsIDirectoryEnumerator_h__
#define __gen_nsIDirectoryEnumerator_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */


/* starting interface:    nsIDirectoryEnumerator */
#define NS_IDIRECTORYENUMERATOR_IID_STR "31f7f4ae-6916-4f2d-a81e-926a4e3022ee"

#define NS_IDIRECTORYENUMERATOR_IID \
  {0x31f7f4ae, 0x6916, 0x4f2d, \
    { 0xa8, 0x1e, 0x92, 0x6a, 0x4e, 0x30, 0x22, 0xee }}

class NS_NO_VTABLE nsIDirectoryEnumerator : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDIRECTORYENUMERATOR_IID)

  /* readonly attribute nsIFile nextFile; */
  NS_IMETHOD GetNextFile(nsIFile * *aNextFile) = 0;

  /* void close (); */
  NS_IMETHOD Close(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDirectoryEnumerator, NS_IDIRECTORYENUMERATOR_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDIRECTORYENUMERATOR \
  NS_IMETHOD GetNextFile(nsIFile * *aNextFile) override; \
  NS_IMETHOD Close(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIDIRECTORYENUMERATOR \
  nsresult GetNextFile(nsIFile * *aNextFile); \
  nsresult Close(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDIRECTORYENUMERATOR(_to) \
  NS_IMETHOD GetNextFile(nsIFile * *aNextFile) override { return _to GetNextFile(aNextFile); } \
  NS_IMETHOD Close(void) override { return _to Close(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDIRECTORYENUMERATOR(_to) \
  NS_IMETHOD GetNextFile(nsIFile * *aNextFile) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNextFile(aNextFile); } \
  NS_IMETHOD Close(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Close(); } 


#endif /* __gen_nsIDirectoryEnumerator_h__ */
