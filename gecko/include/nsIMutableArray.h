/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIMutableArray.idl
 */

#ifndef __gen_nsIMutableArray_h__
#define __gen_nsIMutableArray_h__


#ifndef __gen_nsIArrayExtensions_h__
#include "nsIArrayExtensions.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIMutableArray */
#define NS_IMUTABLEARRAY_IID_STR "af059da0-c85b-40ec-af07-ae4bfdc192cc"

#define NS_IMUTABLEARRAY_IID \
  {0xaf059da0, 0xc85b, 0x40ec, \
    { 0xaf, 0x07, 0xae, 0x4b, 0xfd, 0xc1, 0x92, 0xcc }}

class NS_NO_VTABLE nsIMutableArray : public nsIArrayExtensions {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IMUTABLEARRAY_IID)

  /* void appendElement (in nsISupports element, [optional] in boolean weak); */
  NS_IMETHOD AppendElement(nsISupports *element, bool weak) = 0;

  /* void removeElementAt (in unsigned long index); */
  NS_IMETHOD RemoveElementAt(uint32_t index) = 0;

  /* void insertElementAt (in nsISupports element, in unsigned long index, in boolean weak); */
  NS_IMETHOD InsertElementAt(nsISupports *element, uint32_t index, bool weak) = 0;

  /* void replaceElementAt (in nsISupports element, in unsigned long index, in boolean weak); */
  NS_IMETHOD ReplaceElementAt(nsISupports *element, uint32_t index, bool weak) = 0;

  /* void clear (); */
  NS_IMETHOD Clear(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIMutableArray, NS_IMUTABLEARRAY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIMUTABLEARRAY \
  NS_IMETHOD AppendElement(nsISupports *element, bool weak) override; \
  NS_IMETHOD RemoveElementAt(uint32_t index) override; \
  NS_IMETHOD InsertElementAt(nsISupports *element, uint32_t index, bool weak) override; \
  NS_IMETHOD ReplaceElementAt(nsISupports *element, uint32_t index, bool weak) override; \
  NS_IMETHOD Clear(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIMUTABLEARRAY \
  nsresult AppendElement(nsISupports *element, bool weak); \
  nsresult RemoveElementAt(uint32_t index); \
  nsresult InsertElementAt(nsISupports *element, uint32_t index, bool weak); \
  nsresult ReplaceElementAt(nsISupports *element, uint32_t index, bool weak); \
  nsresult Clear(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIMUTABLEARRAY(_to) \
  NS_IMETHOD AppendElement(nsISupports *element, bool weak) override { return _to AppendElement(element, weak); } \
  NS_IMETHOD RemoveElementAt(uint32_t index) override { return _to RemoveElementAt(index); } \
  NS_IMETHOD InsertElementAt(nsISupports *element, uint32_t index, bool weak) override { return _to InsertElementAt(element, index, weak); } \
  NS_IMETHOD ReplaceElementAt(nsISupports *element, uint32_t index, bool weak) override { return _to ReplaceElementAt(element, index, weak); } \
  NS_IMETHOD Clear(void) override { return _to Clear(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIMUTABLEARRAY(_to) \
  NS_IMETHOD AppendElement(nsISupports *element, bool weak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AppendElement(element, weak); } \
  NS_IMETHOD RemoveElementAt(uint32_t index) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveElementAt(index); } \
  NS_IMETHOD InsertElementAt(nsISupports *element, uint32_t index, bool weak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InsertElementAt(element, index, weak); } \
  NS_IMETHOD ReplaceElementAt(nsISupports *element, uint32_t index, bool weak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ReplaceElementAt(element, index, weak); } \
  NS_IMETHOD Clear(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Clear(); } 


#endif /* __gen_nsIMutableArray_h__ */
