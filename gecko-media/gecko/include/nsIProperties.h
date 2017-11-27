/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIProperties.idl
 */

#ifndef __gen_nsIProperties_h__
#define __gen_nsIProperties_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIProperties */
#define NS_IPROPERTIES_IID_STR "78650582-4e93-4b60-8e85-26ebd3eb14ca"

#define NS_IPROPERTIES_IID \
  {0x78650582, 0x4e93, 0x4b60, \
    { 0x8e, 0x85, 0x26, 0xeb, 0xd3, 0xeb, 0x14, 0xca }}

class NS_NO_VTABLE nsIProperties : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPROPERTIES_IID)

  /* void get (in string prop, in nsIIDRef iid, [iid_is (iid), retval] out nsQIResult result); */
  NS_IMETHOD Get(const char * prop, const nsIID & iid, void **result) = 0;

  /* void set (in string prop, in nsISupports value); */
  NS_IMETHOD Set(const char * prop, nsISupports *value) = 0;

  /* boolean has (in string prop); */
  NS_IMETHOD Has(const char * prop, bool *_retval) = 0;

  /* void undefine (in string prop); */
  NS_IMETHOD Undefine(const char * prop) = 0;

  /* void getKeys (out uint32_t count, [array, size_is (count), retval] out string keys); */
  NS_IMETHOD GetKeys(uint32_t *count, char * **keys) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIProperties, NS_IPROPERTIES_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPROPERTIES \
  NS_IMETHOD Get(const char * prop, const nsIID & iid, void **result) override; \
  NS_IMETHOD Set(const char * prop, nsISupports *value) override; \
  NS_IMETHOD Has(const char * prop, bool *_retval) override; \
  NS_IMETHOD Undefine(const char * prop) override; \
  NS_IMETHOD GetKeys(uint32_t *count, char * **keys) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIPROPERTIES \
  nsresult Get(const char * prop, const nsIID & iid, void **result); \
  nsresult Set(const char * prop, nsISupports *value); \
  nsresult Has(const char * prop, bool *_retval); \
  nsresult Undefine(const char * prop); \
  nsresult GetKeys(uint32_t *count, char * **keys); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPROPERTIES(_to) \
  NS_IMETHOD Get(const char * prop, const nsIID & iid, void **result) override { return _to Get(prop, iid, result); } \
  NS_IMETHOD Set(const char * prop, nsISupports *value) override { return _to Set(prop, value); } \
  NS_IMETHOD Has(const char * prop, bool *_retval) override { return _to Has(prop, _retval); } \
  NS_IMETHOD Undefine(const char * prop) override { return _to Undefine(prop); } \
  NS_IMETHOD GetKeys(uint32_t *count, char * **keys) override { return _to GetKeys(count, keys); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPROPERTIES(_to) \
  NS_IMETHOD Get(const char * prop, const nsIID & iid, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Get(prop, iid, result); } \
  NS_IMETHOD Set(const char * prop, nsISupports *value) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Set(prop, value); } \
  NS_IMETHOD Has(const char * prop, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Has(prop, _retval); } \
  NS_IMETHOD Undefine(const char * prop) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Undefine(prop); } \
  NS_IMETHOD GetKeys(uint32_t *count, char * **keys) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetKeys(count, keys); } 


#endif /* __gen_nsIProperties_h__ */
