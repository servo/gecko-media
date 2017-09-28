/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsILoadGroup.idl
 */

#ifndef __gen_nsILoadGroup_h__
#define __gen_nsILoadGroup_h__


#ifndef __gen_nsIRequest_h__
#include "nsIRequest.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISimpleEnumerator; /* forward declaration */

class nsIRequestObserver; /* forward declaration */

class nsIInterfaceRequestor; /* forward declaration */

class nsIRequestContext; /* forward declaration */


/* starting interface:    nsILoadGroup */
#define NS_ILOADGROUP_IID_STR "f0c87725-7a35-463c-9ceb-2c07f23406cc"

#define NS_ILOADGROUP_IID \
  {0xf0c87725, 0x7a35, 0x463c, \
    { 0x9c, 0xeb, 0x2c, 0x07, 0xf2, 0x34, 0x06, 0xcc }}

class NS_NO_VTABLE nsILoadGroup : public nsIRequest {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ILOADGROUP_IID)

  /* attribute nsIRequestObserver groupObserver; */
  NS_IMETHOD GetGroupObserver(nsIRequestObserver * *aGroupObserver) = 0;
  NS_IMETHOD SetGroupObserver(nsIRequestObserver *aGroupObserver) = 0;

  /* attribute nsIRequest defaultLoadRequest; */
  NS_IMETHOD GetDefaultLoadRequest(nsIRequest * *aDefaultLoadRequest) = 0;
  NS_IMETHOD SetDefaultLoadRequest(nsIRequest *aDefaultLoadRequest) = 0;

  /* void addRequest (in nsIRequest aRequest, in nsISupports aContext); */
  NS_IMETHOD AddRequest(nsIRequest *aRequest, nsISupports *aContext) = 0;

  /* void removeRequest (in nsIRequest aRequest, in nsISupports aContext, in nsresult aStatus); */
  NS_IMETHOD RemoveRequest(nsIRequest *aRequest, nsISupports *aContext, nsresult aStatus) = 0;

  /* readonly attribute nsISimpleEnumerator requests; */
  NS_IMETHOD GetRequests(nsISimpleEnumerator * *aRequests) = 0;

  /* readonly attribute unsigned long activeCount; */
  NS_IMETHOD GetActiveCount(uint32_t *aActiveCount) = 0;

  /* attribute nsIInterfaceRequestor notificationCallbacks; */
  NS_IMETHOD GetNotificationCallbacks(nsIInterfaceRequestor * *aNotificationCallbacks) = 0;
  NS_IMETHOD SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks) = 0;

  /* [noscript] readonly attribute unsigned long long requestContextID; */
  NS_IMETHOD GetRequestContextID(uint64_t *aRequestContextID) = 0;

  /* attribute nsLoadFlags defaultLoadFlags; */
  NS_IMETHOD GetDefaultLoadFlags(nsLoadFlags *aDefaultLoadFlags) = 0;
  NS_IMETHOD SetDefaultLoadFlags(nsLoadFlags aDefaultLoadFlags) = 0;

  /* attribute ACString userAgentOverrideCache; */
  NS_IMETHOD GetUserAgentOverrideCache(nsACString & aUserAgentOverrideCache) = 0;
  NS_IMETHOD SetUserAgentOverrideCache(const nsACString & aUserAgentOverrideCache) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsILoadGroup, NS_ILOADGROUP_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSILOADGROUP \
  NS_IMETHOD GetGroupObserver(nsIRequestObserver * *aGroupObserver) override; \
  NS_IMETHOD SetGroupObserver(nsIRequestObserver *aGroupObserver) override; \
  NS_IMETHOD GetDefaultLoadRequest(nsIRequest * *aDefaultLoadRequest) override; \
  NS_IMETHOD SetDefaultLoadRequest(nsIRequest *aDefaultLoadRequest) override; \
  NS_IMETHOD AddRequest(nsIRequest *aRequest, nsISupports *aContext) override; \
  NS_IMETHOD RemoveRequest(nsIRequest *aRequest, nsISupports *aContext, nsresult aStatus) override; \
  NS_IMETHOD GetRequests(nsISimpleEnumerator * *aRequests) override; \
  NS_IMETHOD GetActiveCount(uint32_t *aActiveCount) override; \
  NS_IMETHOD GetNotificationCallbacks(nsIInterfaceRequestor * *aNotificationCallbacks) override; \
  NS_IMETHOD SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks) override; \
  NS_IMETHOD GetRequestContextID(uint64_t *aRequestContextID) override; \
  NS_IMETHOD GetDefaultLoadFlags(nsLoadFlags *aDefaultLoadFlags) override; \
  NS_IMETHOD SetDefaultLoadFlags(nsLoadFlags aDefaultLoadFlags) override; \
  NS_IMETHOD GetUserAgentOverrideCache(nsACString & aUserAgentOverrideCache) override; \
  NS_IMETHOD SetUserAgentOverrideCache(const nsACString & aUserAgentOverrideCache) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSILOADGROUP \
  nsresult GetGroupObserver(nsIRequestObserver * *aGroupObserver); \
  nsresult SetGroupObserver(nsIRequestObserver *aGroupObserver); \
  nsresult GetDefaultLoadRequest(nsIRequest * *aDefaultLoadRequest); \
  nsresult SetDefaultLoadRequest(nsIRequest *aDefaultLoadRequest); \
  nsresult AddRequest(nsIRequest *aRequest, nsISupports *aContext); \
  nsresult RemoveRequest(nsIRequest *aRequest, nsISupports *aContext, nsresult aStatus); \
  nsresult GetRequests(nsISimpleEnumerator * *aRequests); \
  nsresult GetActiveCount(uint32_t *aActiveCount); \
  nsresult GetNotificationCallbacks(nsIInterfaceRequestor * *aNotificationCallbacks); \
  nsresult SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks); \
  nsresult GetRequestContextID(uint64_t *aRequestContextID); \
  nsresult GetDefaultLoadFlags(nsLoadFlags *aDefaultLoadFlags); \
  nsresult SetDefaultLoadFlags(nsLoadFlags aDefaultLoadFlags); \
  nsresult GetUserAgentOverrideCache(nsACString & aUserAgentOverrideCache); \
  nsresult SetUserAgentOverrideCache(const nsACString & aUserAgentOverrideCache); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSILOADGROUP(_to) \
  NS_IMETHOD GetGroupObserver(nsIRequestObserver * *aGroupObserver) override { return _to GetGroupObserver(aGroupObserver); } \
  NS_IMETHOD SetGroupObserver(nsIRequestObserver *aGroupObserver) override { return _to SetGroupObserver(aGroupObserver); } \
  NS_IMETHOD GetDefaultLoadRequest(nsIRequest * *aDefaultLoadRequest) override { return _to GetDefaultLoadRequest(aDefaultLoadRequest); } \
  NS_IMETHOD SetDefaultLoadRequest(nsIRequest *aDefaultLoadRequest) override { return _to SetDefaultLoadRequest(aDefaultLoadRequest); } \
  NS_IMETHOD AddRequest(nsIRequest *aRequest, nsISupports *aContext) override { return _to AddRequest(aRequest, aContext); } \
  NS_IMETHOD RemoveRequest(nsIRequest *aRequest, nsISupports *aContext, nsresult aStatus) override { return _to RemoveRequest(aRequest, aContext, aStatus); } \
  NS_IMETHOD GetRequests(nsISimpleEnumerator * *aRequests) override { return _to GetRequests(aRequests); } \
  NS_IMETHOD GetActiveCount(uint32_t *aActiveCount) override { return _to GetActiveCount(aActiveCount); } \
  NS_IMETHOD GetNotificationCallbacks(nsIInterfaceRequestor * *aNotificationCallbacks) override { return _to GetNotificationCallbacks(aNotificationCallbacks); } \
  NS_IMETHOD SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks) override { return _to SetNotificationCallbacks(aNotificationCallbacks); } \
  NS_IMETHOD GetRequestContextID(uint64_t *aRequestContextID) override { return _to GetRequestContextID(aRequestContextID); } \
  NS_IMETHOD GetDefaultLoadFlags(nsLoadFlags *aDefaultLoadFlags) override { return _to GetDefaultLoadFlags(aDefaultLoadFlags); } \
  NS_IMETHOD SetDefaultLoadFlags(nsLoadFlags aDefaultLoadFlags) override { return _to SetDefaultLoadFlags(aDefaultLoadFlags); } \
  NS_IMETHOD GetUserAgentOverrideCache(nsACString & aUserAgentOverrideCache) override { return _to GetUserAgentOverrideCache(aUserAgentOverrideCache); } \
  NS_IMETHOD SetUserAgentOverrideCache(const nsACString & aUserAgentOverrideCache) override { return _to SetUserAgentOverrideCache(aUserAgentOverrideCache); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSILOADGROUP(_to) \
  NS_IMETHOD GetGroupObserver(nsIRequestObserver * *aGroupObserver) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetGroupObserver(aGroupObserver); } \
  NS_IMETHOD SetGroupObserver(nsIRequestObserver *aGroupObserver) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetGroupObserver(aGroupObserver); } \
  NS_IMETHOD GetDefaultLoadRequest(nsIRequest * *aDefaultLoadRequest) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultLoadRequest(aDefaultLoadRequest); } \
  NS_IMETHOD SetDefaultLoadRequest(nsIRequest *aDefaultLoadRequest) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDefaultLoadRequest(aDefaultLoadRequest); } \
  NS_IMETHOD AddRequest(nsIRequest *aRequest, nsISupports *aContext) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AddRequest(aRequest, aContext); } \
  NS_IMETHOD RemoveRequest(nsIRequest *aRequest, nsISupports *aContext, nsresult aStatus) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveRequest(aRequest, aContext, aStatus); } \
  NS_IMETHOD GetRequests(nsISimpleEnumerator * *aRequests) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRequests(aRequests); } \
  NS_IMETHOD GetActiveCount(uint32_t *aActiveCount) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetActiveCount(aActiveCount); } \
  NS_IMETHOD GetNotificationCallbacks(nsIInterfaceRequestor * *aNotificationCallbacks) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNotificationCallbacks(aNotificationCallbacks); } \
  NS_IMETHOD SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetNotificationCallbacks(aNotificationCallbacks); } \
  NS_IMETHOD GetRequestContextID(uint64_t *aRequestContextID) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRequestContextID(aRequestContextID); } \
  NS_IMETHOD GetDefaultLoadFlags(nsLoadFlags *aDefaultLoadFlags) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultLoadFlags(aDefaultLoadFlags); } \
  NS_IMETHOD SetDefaultLoadFlags(nsLoadFlags aDefaultLoadFlags) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDefaultLoadFlags(aDefaultLoadFlags); } \
  NS_IMETHOD GetUserAgentOverrideCache(nsACString & aUserAgentOverrideCache) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUserAgentOverrideCache(aUserAgentOverrideCache); } \
  NS_IMETHOD SetUserAgentOverrideCache(const nsACString & aUserAgentOverrideCache) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetUserAgentOverrideCache(aUserAgentOverrideCache); } 


#endif /* __gen_nsILoadGroup_h__ */
