/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIErrorService.idl
 */

#ifndef __gen_nsIErrorService_h__
#define __gen_nsIErrorService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIErrorService */
#define NS_IERRORSERVICE_IID_STR "afe1f190-a3c2-11e3-a5e2-0800200c9a66"

#define NS_IERRORSERVICE_IID \
  {0xafe1f190, 0xa3c2, 0x11e3, \
    { 0xa5, 0xe2, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

class NS_NO_VTABLE nsIErrorService : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IERRORSERVICE_IID)

  /* void registerErrorStringBundle (in short errorModule, in string stringBundleURL); */
  NS_IMETHOD RegisterErrorStringBundle(int16_t errorModule, const char * stringBundleURL) = 0;

  /* void unregisterErrorStringBundle (in short errorModule); */
  NS_IMETHOD UnregisterErrorStringBundle(int16_t errorModule) = 0;

  /* string getErrorStringBundle (in short errorModule); */
  NS_IMETHOD GetErrorStringBundle(int16_t errorModule, char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIErrorService, NS_IERRORSERVICE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIERRORSERVICE \
  NS_IMETHOD RegisterErrorStringBundle(int16_t errorModule, const char * stringBundleURL) override; \
  NS_IMETHOD UnregisterErrorStringBundle(int16_t errorModule) override; \
  NS_IMETHOD GetErrorStringBundle(int16_t errorModule, char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIERRORSERVICE \
  nsresult RegisterErrorStringBundle(int16_t errorModule, const char * stringBundleURL); \
  nsresult UnregisterErrorStringBundle(int16_t errorModule); \
  nsresult GetErrorStringBundle(int16_t errorModule, char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIERRORSERVICE(_to) \
  NS_IMETHOD RegisterErrorStringBundle(int16_t errorModule, const char * stringBundleURL) override { return _to RegisterErrorStringBundle(errorModule, stringBundleURL); } \
  NS_IMETHOD UnregisterErrorStringBundle(int16_t errorModule) override { return _to UnregisterErrorStringBundle(errorModule); } \
  NS_IMETHOD GetErrorStringBundle(int16_t errorModule, char * *_retval) override { return _to GetErrorStringBundle(errorModule, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIERRORSERVICE(_to) \
  NS_IMETHOD RegisterErrorStringBundle(int16_t errorModule, const char * stringBundleURL) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterErrorStringBundle(errorModule, stringBundleURL); } \
  NS_IMETHOD UnregisterErrorStringBundle(int16_t errorModule) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterErrorStringBundle(errorModule); } \
  NS_IMETHOD GetErrorStringBundle(int16_t errorModule, char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetErrorStringBundle(errorModule, _retval); } 


// The global nsIErrorService:
#define NS_ERRORSERVICE_NAME   "Error Service" 
#define NS_ERRORSERVICE_CONTRACTID "@mozilla.org/xpcom/error-service;1"
#define NS_ERRORSERVICE_CID                          \
{ /* 744afd5e-5f8c-11d4-9877-00c04fa0cf4a */         \
    0x744afd5e,                                      \
    0x5f8c,                                          \
    0x11d4,                                          \
    {0x98, 0x77, 0x00, 0xc0, 0x4f, 0xa0, 0xcf, 0x4a} \
}

#endif /* __gen_nsIErrorService_h__ */
