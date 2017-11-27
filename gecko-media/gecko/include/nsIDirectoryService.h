/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIDirectoryService.idl
 */

#ifndef __gen_nsIDirectoryService_h__
#define __gen_nsIDirectoryService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */

class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIDirectoryServiceProvider */
#define NS_IDIRECTORYSERVICEPROVIDER_IID_STR "bbf8cab0-d43a-11d3-8cc2-00609792278c"

#define NS_IDIRECTORYSERVICEPROVIDER_IID \
  {0xbbf8cab0, 0xd43a, 0x11d3, \
    { 0x8c, 0xc2, 0x00, 0x60, 0x97, 0x92, 0x27, 0x8c }}

class NS_NO_VTABLE nsIDirectoryServiceProvider : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDIRECTORYSERVICEPROVIDER_IID)

  /* nsIFile getFile (in string prop, out boolean persistent); */
  NS_IMETHOD GetFile(const char * prop, bool *persistent, nsIFile * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDirectoryServiceProvider, NS_IDIRECTORYSERVICEPROVIDER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDIRECTORYSERVICEPROVIDER \
  NS_IMETHOD GetFile(const char * prop, bool *persistent, nsIFile * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIDIRECTORYSERVICEPROVIDER \
  nsresult GetFile(const char * prop, bool *persistent, nsIFile * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDIRECTORYSERVICEPROVIDER(_to) \
  NS_IMETHOD GetFile(const char * prop, bool *persistent, nsIFile * *_retval) override { return _to GetFile(prop, persistent, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDIRECTORYSERVICEPROVIDER(_to) \
  NS_IMETHOD GetFile(const char * prop, bool *persistent, nsIFile * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFile(prop, persistent, _retval); } 


/* starting interface:    nsIDirectoryServiceProvider2 */
#define NS_IDIRECTORYSERVICEPROVIDER2_IID_STR "2f977d4b-5485-11d4-87e2-0010a4e75ef2"

#define NS_IDIRECTORYSERVICEPROVIDER2_IID \
  {0x2f977d4b, 0x5485, 0x11d4, \
    { 0x87, 0xe2, 0x00, 0x10, 0xa4, 0xe7, 0x5e, 0xf2 }}

class NS_NO_VTABLE nsIDirectoryServiceProvider2 : public nsIDirectoryServiceProvider {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDIRECTORYSERVICEPROVIDER2_IID)

  /* nsISimpleEnumerator getFiles (in string prop); */
  NS_IMETHOD GetFiles(const char * prop, nsISimpleEnumerator * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDirectoryServiceProvider2, NS_IDIRECTORYSERVICEPROVIDER2_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDIRECTORYSERVICEPROVIDER2 \
  NS_IMETHOD GetFiles(const char * prop, nsISimpleEnumerator * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIDIRECTORYSERVICEPROVIDER2 \
  nsresult GetFiles(const char * prop, nsISimpleEnumerator * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDIRECTORYSERVICEPROVIDER2(_to) \
  NS_IMETHOD GetFiles(const char * prop, nsISimpleEnumerator * *_retval) override { return _to GetFiles(prop, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDIRECTORYSERVICEPROVIDER2(_to) \
  NS_IMETHOD GetFiles(const char * prop, nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFiles(prop, _retval); } 


/* starting interface:    nsIDirectoryService */
#define NS_IDIRECTORYSERVICE_IID_STR "57a66a60-d43a-11d3-8cc2-00609792278c"

#define NS_IDIRECTORYSERVICE_IID \
  {0x57a66a60, 0xd43a, 0x11d3, \
    { 0x8c, 0xc2, 0x00, 0x60, 0x97, 0x92, 0x27, 0x8c }}

class NS_NO_VTABLE nsIDirectoryService : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDIRECTORYSERVICE_IID)

  /* void init (); */
  NS_IMETHOD Init(void) = 0;

  /* void registerProvider (in nsIDirectoryServiceProvider prov); */
  NS_IMETHOD RegisterProvider(nsIDirectoryServiceProvider *prov) = 0;

  /* void unregisterProvider (in nsIDirectoryServiceProvider prov); */
  NS_IMETHOD UnregisterProvider(nsIDirectoryServiceProvider *prov) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDirectoryService, NS_IDIRECTORYSERVICE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDIRECTORYSERVICE \
  NS_IMETHOD Init(void) override; \
  NS_IMETHOD RegisterProvider(nsIDirectoryServiceProvider *prov) override; \
  NS_IMETHOD UnregisterProvider(nsIDirectoryServiceProvider *prov) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIDIRECTORYSERVICE \
  nsresult Init(void); \
  nsresult RegisterProvider(nsIDirectoryServiceProvider *prov); \
  nsresult UnregisterProvider(nsIDirectoryServiceProvider *prov); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDIRECTORYSERVICE(_to) \
  NS_IMETHOD Init(void) override { return _to Init(); } \
  NS_IMETHOD RegisterProvider(nsIDirectoryServiceProvider *prov) override { return _to RegisterProvider(prov); } \
  NS_IMETHOD UnregisterProvider(nsIDirectoryServiceProvider *prov) override { return _to UnregisterProvider(prov); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDIRECTORYSERVICE(_to) \
  NS_IMETHOD Init(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(); } \
  NS_IMETHOD RegisterProvider(nsIDirectoryServiceProvider *prov) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterProvider(prov); } \
  NS_IMETHOD UnregisterProvider(nsIDirectoryServiceProvider *prov) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterProvider(prov); } 


#endif /* __gen_nsIDirectoryService_h__ */
