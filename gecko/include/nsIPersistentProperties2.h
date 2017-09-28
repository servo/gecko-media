/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIPersistentProperties2.idl
 */

#ifndef __gen_nsIPersistentProperties2_h__
#define __gen_nsIPersistentProperties2_h__


#ifndef __gen_nsIProperties_h__
#include "nsIProperties.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIInputStream; /* forward declaration */

class nsIOutputStream; /* forward declaration */

class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIPropertyElement */
#define NS_IPROPERTYELEMENT_IID_STR "283ee646-1aef-11d4-98b3-00c04fa0ce9a"

#define NS_IPROPERTYELEMENT_IID \
  {0x283ee646, 0x1aef, 0x11d4, \
    { 0x98, 0xb3, 0x00, 0xc0, 0x4f, 0xa0, 0xce, 0x9a }}

class NS_NO_VTABLE nsIPropertyElement : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPROPERTYELEMENT_IID)

  /* attribute AUTF8String key; */
  NS_IMETHOD GetKey(nsACString & aKey) = 0;
  NS_IMETHOD SetKey(const nsACString & aKey) = 0;

  /* attribute AString value; */
  NS_IMETHOD GetValue(nsAString & aValue) = 0;
  NS_IMETHOD SetValue(const nsAString & aValue) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPropertyElement, NS_IPROPERTYELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPROPERTYELEMENT \
  NS_IMETHOD GetKey(nsACString & aKey) override; \
  NS_IMETHOD SetKey(const nsACString & aKey) override; \
  NS_IMETHOD GetValue(nsAString & aValue) override; \
  NS_IMETHOD SetValue(const nsAString & aValue) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIPROPERTYELEMENT \
  nsresult GetKey(nsACString & aKey); \
  nsresult SetKey(const nsACString & aKey); \
  nsresult GetValue(nsAString & aValue); \
  nsresult SetValue(const nsAString & aValue); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPROPERTYELEMENT(_to) \
  NS_IMETHOD GetKey(nsACString & aKey) override { return _to GetKey(aKey); } \
  NS_IMETHOD SetKey(const nsACString & aKey) override { return _to SetKey(aKey); } \
  NS_IMETHOD GetValue(nsAString & aValue) override { return _to GetValue(aValue); } \
  NS_IMETHOD SetValue(const nsAString & aValue) override { return _to SetValue(aValue); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPROPERTYELEMENT(_to) \
  NS_IMETHOD GetKey(nsACString & aKey) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetKey(aKey); } \
  NS_IMETHOD SetKey(const nsACString & aKey) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetKey(aKey); } \
  NS_IMETHOD GetValue(nsAString & aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValue(aValue); } \
  NS_IMETHOD SetValue(const nsAString & aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetValue(aValue); } 


/* starting interface:    nsIPersistentProperties */
#define NS_IPERSISTENTPROPERTIES_IID_STR "706867af-0400-4faa-beb1-0dae87308784"

#define NS_IPERSISTENTPROPERTIES_IID \
  {0x706867af, 0x0400, 0x4faa, \
    { 0xbe, 0xb1, 0x0d, 0xae, 0x87, 0x30, 0x87, 0x84 }}

class NS_NO_VTABLE nsIPersistentProperties : public nsIProperties {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPERSISTENTPROPERTIES_IID)

  /* void load (in nsIInputStream input); */
  NS_IMETHOD Load(nsIInputStream *input) = 0;

  /* void save (in nsIOutputStream output, in AUTF8String header); */
  NS_IMETHOD Save(nsIOutputStream *output, const nsACString & header) = 0;

  /* nsISimpleEnumerator enumerate (); */
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) = 0;

  /* AString getStringProperty (in AUTF8String key); */
  NS_IMETHOD GetStringProperty(const nsACString & key, nsAString & _retval) = 0;

  /* AString setStringProperty (in AUTF8String key, in AString value); */
  NS_IMETHOD SetStringProperty(const nsACString & key, const nsAString & value, nsAString & _retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPersistentProperties, NS_IPERSISTENTPROPERTIES_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPERSISTENTPROPERTIES \
  NS_IMETHOD Load(nsIInputStream *input) override; \
  NS_IMETHOD Save(nsIOutputStream *output, const nsACString & header) override; \
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) override; \
  NS_IMETHOD GetStringProperty(const nsACString & key, nsAString & _retval) override; \
  NS_IMETHOD SetStringProperty(const nsACString & key, const nsAString & value, nsAString & _retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIPERSISTENTPROPERTIES \
  nsresult Load(nsIInputStream *input); \
  nsresult Save(nsIOutputStream *output, const nsACString & header); \
  nsresult Enumerate(nsISimpleEnumerator * *_retval); \
  nsresult GetStringProperty(const nsACString & key, nsAString & _retval); \
  nsresult SetStringProperty(const nsACString & key, const nsAString & value, nsAString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPERSISTENTPROPERTIES(_to) \
  NS_IMETHOD Load(nsIInputStream *input) override { return _to Load(input); } \
  NS_IMETHOD Save(nsIOutputStream *output, const nsACString & header) override { return _to Save(output, header); } \
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) override { return _to Enumerate(_retval); } \
  NS_IMETHOD GetStringProperty(const nsACString & key, nsAString & _retval) override { return _to GetStringProperty(key, _retval); } \
  NS_IMETHOD SetStringProperty(const nsACString & key, const nsAString & value, nsAString & _retval) override { return _to SetStringProperty(key, value, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPERSISTENTPROPERTIES(_to) \
  NS_IMETHOD Load(nsIInputStream *input) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Load(input); } \
  NS_IMETHOD Save(nsIOutputStream *output, const nsACString & header) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Save(output, header); } \
  NS_IMETHOD Enumerate(nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Enumerate(_retval); } \
  NS_IMETHOD GetStringProperty(const nsACString & key, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringProperty(key, _retval); } \
  NS_IMETHOD SetStringProperty(const nsACString & key, const nsAString & value, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetStringProperty(key, value, _retval); } 


#define NS_IPERSISTENTPROPERTIES_CID \
{ 0x2245e573, 0x9464, 0x11d2, \
  { 0x9b, 0x8b, 0x0, 0x80, 0x5f, 0x8a, 0x16, 0xd9 } }
#define NS_PERSISTENTPROPERTIES_CONTRACTID "@mozilla.org/persistent-properties;1"

#endif /* __gen_nsIPersistentProperties2_h__ */
