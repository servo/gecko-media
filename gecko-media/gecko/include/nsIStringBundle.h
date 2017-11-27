/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIStringBundle.idl
 */

#ifndef __gen_nsIStringBundle_h__
#define __gen_nsIStringBundle_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsISimpleEnumerator_h__
#include "nsISimpleEnumerator.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

// Define Contractid and CID
// {D85A17C1-AA7C-11d2-9B8C-00805F8A16D9}
#define NS_STRINGBUNDLESERVICE_CID \
{ 0xd85a17c1, 0xaa7c, 0x11d2, \
  { 0x9b, 0x8c, 0x0, 0x80, 0x5f, 0x8a, 0x16, 0xd9 } }
#define NS_STRINGBUNDLE_CONTRACTID "@mozilla.org/intl/stringbundle;1"

/* starting interface:    nsIStringBundle */
#define NS_ISTRINGBUNDLE_IID_STR "d85a17c2-aa7c-11d2-9b8c-00805f8a16d9"

#define NS_ISTRINGBUNDLE_IID \
  {0xd85a17c2, 0xaa7c, 0x11d2, \
    { 0x9b, 0x8c, 0x00, 0x80, 0x5f, 0x8a, 0x16, 0xd9 }}

class NS_NO_VTABLE nsIStringBundle : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISTRINGBUNDLE_IID)

  /* AString GetStringFromID (in long aID); */
  NS_IMETHOD GetStringFromID(int32_t aID, nsAString & _retval) = 0;

  /* [binaryname(GetStringFromAUTF8Name)] AString GetStringFromName (in AUTF8String aName); */
  NS_IMETHOD GetStringFromAUTF8Name(const nsACString & aName, nsAString & _retval) = 0;

  /* [binaryname(GetStringFromName),noscript] AString GetStringFromNameCpp (in string aName); */
  NS_IMETHOD GetStringFromName(const char * aName, nsAString & _retval) = 0;

  /* AString formatStringFromID (in long aID, [array, size_is (length)] in wstring params, in unsigned long length); */
  NS_IMETHOD FormatStringFromID(int32_t aID, const char16_t * *params, uint32_t length, nsAString & _retval) = 0;

  /* [binaryname(FormatStringFromAUTF8Name)] AString formatStringFromName (in AUTF8String aName, [array, size_is (length)] in wstring params, in unsigned long length); */
  NS_IMETHOD FormatStringFromAUTF8Name(const nsACString & aName, const char16_t * *params, uint32_t length, nsAString & _retval) = 0;

  /* [binaryname(FormatStringFromName),noscript] AString formatStringFromNameCpp (in string aName, [array, size_is (length)] in wstring params, in unsigned long length); */
  NS_IMETHOD FormatStringFromName(const char * aName, const char16_t * *params, uint32_t length, nsAString & _retval) = 0;

  /* nsISimpleEnumerator getSimpleEnumeration (); */
  NS_IMETHOD GetSimpleEnumeration(nsISimpleEnumerator * *_retval) = 0;

  /* void asyncPreload (); */
  NS_IMETHOD AsyncPreload(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIStringBundle, NS_ISTRINGBUNDLE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISTRINGBUNDLE \
  NS_IMETHOD GetStringFromID(int32_t aID, nsAString & _retval) override; \
  NS_IMETHOD GetStringFromAUTF8Name(const nsACString & aName, nsAString & _retval) override; \
  NS_IMETHOD GetStringFromName(const char * aName, nsAString & _retval) override; \
  NS_IMETHOD FormatStringFromID(int32_t aID, const char16_t * *params, uint32_t length, nsAString & _retval) override; \
  NS_IMETHOD FormatStringFromAUTF8Name(const nsACString & aName, const char16_t * *params, uint32_t length, nsAString & _retval) override; \
  NS_IMETHOD FormatStringFromName(const char * aName, const char16_t * *params, uint32_t length, nsAString & _retval) override; \
  NS_IMETHOD GetSimpleEnumeration(nsISimpleEnumerator * *_retval) override; \
  NS_IMETHOD AsyncPreload(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISTRINGBUNDLE \
  nsresult GetStringFromID(int32_t aID, nsAString & _retval); \
  nsresult GetStringFromAUTF8Name(const nsACString & aName, nsAString & _retval); \
  nsresult GetStringFromName(const char * aName, nsAString & _retval); \
  nsresult FormatStringFromID(int32_t aID, const char16_t * *params, uint32_t length, nsAString & _retval); \
  nsresult FormatStringFromAUTF8Name(const nsACString & aName, const char16_t * *params, uint32_t length, nsAString & _retval); \
  nsresult FormatStringFromName(const char * aName, const char16_t * *params, uint32_t length, nsAString & _retval); \
  nsresult GetSimpleEnumeration(nsISimpleEnumerator * *_retval); \
  nsresult AsyncPreload(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISTRINGBUNDLE(_to) \
  NS_IMETHOD GetStringFromID(int32_t aID, nsAString & _retval) override { return _to GetStringFromID(aID, _retval); } \
  NS_IMETHOD GetStringFromAUTF8Name(const nsACString & aName, nsAString & _retval) override { return _to GetStringFromAUTF8Name(aName, _retval); } \
  NS_IMETHOD GetStringFromName(const char * aName, nsAString & _retval) override { return _to GetStringFromName(aName, _retval); } \
  NS_IMETHOD FormatStringFromID(int32_t aID, const char16_t * *params, uint32_t length, nsAString & _retval) override { return _to FormatStringFromID(aID, params, length, _retval); } \
  NS_IMETHOD FormatStringFromAUTF8Name(const nsACString & aName, const char16_t * *params, uint32_t length, nsAString & _retval) override { return _to FormatStringFromAUTF8Name(aName, params, length, _retval); } \
  NS_IMETHOD FormatStringFromName(const char * aName, const char16_t * *params, uint32_t length, nsAString & _retval) override { return _to FormatStringFromName(aName, params, length, _retval); } \
  NS_IMETHOD GetSimpleEnumeration(nsISimpleEnumerator * *_retval) override { return _to GetSimpleEnumeration(_retval); } \
  NS_IMETHOD AsyncPreload(void) override { return _to AsyncPreload(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISTRINGBUNDLE(_to) \
  NS_IMETHOD GetStringFromID(int32_t aID, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringFromID(aID, _retval); } \
  NS_IMETHOD GetStringFromAUTF8Name(const nsACString & aName, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringFromAUTF8Name(aName, _retval); } \
  NS_IMETHOD GetStringFromName(const char * aName, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringFromName(aName, _retval); } \
  NS_IMETHOD FormatStringFromID(int32_t aID, const char16_t * *params, uint32_t length, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->FormatStringFromID(aID, params, length, _retval); } \
  NS_IMETHOD FormatStringFromAUTF8Name(const nsACString & aName, const char16_t * *params, uint32_t length, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->FormatStringFromAUTF8Name(aName, params, length, _retval); } \
  NS_IMETHOD FormatStringFromName(const char * aName, const char16_t * *params, uint32_t length, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->FormatStringFromName(aName, params, length, _retval); } \
  NS_IMETHOD GetSimpleEnumeration(nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSimpleEnumeration(_retval); } \
  NS_IMETHOD AsyncPreload(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AsyncPreload(); } 


/* starting interface:    nsIStringBundleService */
#define NS_ISTRINGBUNDLESERVICE_IID_STR "d85a17c0-aa7c-11d2-9b8c-00805f8a16d9"

#define NS_ISTRINGBUNDLESERVICE_IID \
  {0xd85a17c0, 0xaa7c, 0x11d2, \
    { 0x9b, 0x8c, 0x00, 0x80, 0x5f, 0x8a, 0x16, 0xd9 }}

class NS_NO_VTABLE nsIStringBundleService : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISTRINGBUNDLESERVICE_IID)

  /* nsIStringBundle createBundle (in string aURLSpec); */
  NS_IMETHOD CreateBundle(const char * aURLSpec, nsIStringBundle * *_retval) = 0;

  /* nsIStringBundle createExtensibleBundle (in string aRegistryKey); */
  NS_IMETHOD CreateExtensibleBundle(const char * aRegistryKey, nsIStringBundle * *_retval) = 0;

  /* AString formatStatusMessage (in nsresult aStatus, in wstring aStatusArg); */
  NS_IMETHOD FormatStatusMessage(nsresult aStatus, const char16_t * aStatusArg, nsAString & _retval) = 0;

  /* void flushBundles (); */
  NS_IMETHOD FlushBundles(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIStringBundleService, NS_ISTRINGBUNDLESERVICE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISTRINGBUNDLESERVICE \
  NS_IMETHOD CreateBundle(const char * aURLSpec, nsIStringBundle * *_retval) override; \
  NS_IMETHOD CreateExtensibleBundle(const char * aRegistryKey, nsIStringBundle * *_retval) override; \
  NS_IMETHOD FormatStatusMessage(nsresult aStatus, const char16_t * aStatusArg, nsAString & _retval) override; \
  NS_IMETHOD FlushBundles(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISTRINGBUNDLESERVICE \
  nsresult CreateBundle(const char * aURLSpec, nsIStringBundle * *_retval); \
  nsresult CreateExtensibleBundle(const char * aRegistryKey, nsIStringBundle * *_retval); \
  nsresult FormatStatusMessage(nsresult aStatus, const char16_t * aStatusArg, nsAString & _retval); \
  nsresult FlushBundles(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISTRINGBUNDLESERVICE(_to) \
  NS_IMETHOD CreateBundle(const char * aURLSpec, nsIStringBundle * *_retval) override { return _to CreateBundle(aURLSpec, _retval); } \
  NS_IMETHOD CreateExtensibleBundle(const char * aRegistryKey, nsIStringBundle * *_retval) override { return _to CreateExtensibleBundle(aRegistryKey, _retval); } \
  NS_IMETHOD FormatStatusMessage(nsresult aStatus, const char16_t * aStatusArg, nsAString & _retval) override { return _to FormatStatusMessage(aStatus, aStatusArg, _retval); } \
  NS_IMETHOD FlushBundles(void) override { return _to FlushBundles(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISTRINGBUNDLESERVICE(_to) \
  NS_IMETHOD CreateBundle(const char * aURLSpec, nsIStringBundle * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateBundle(aURLSpec, _retval); } \
  NS_IMETHOD CreateExtensibleBundle(const char * aRegistryKey, nsIStringBundle * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateExtensibleBundle(aRegistryKey, _retval); } \
  NS_IMETHOD FormatStatusMessage(nsresult aStatus, const char16_t * aStatusArg, nsAString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->FormatStatusMessage(aStatus, aStatusArg, _retval); } \
  NS_IMETHOD FlushBundles(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->FlushBundles(); } 


#endif /* __gen_nsIStringBundle_h__ */
