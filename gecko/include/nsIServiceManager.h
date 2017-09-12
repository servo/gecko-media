/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIServiceManager.idl
 */

#ifndef __gen_nsIServiceManager_h__
#define __gen_nsIServiceManager_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIServiceManager */
#define NS_ISERVICEMANAGER_IID_STR "8bb35ed9-e332-462d-9155-4a002ab5c958"

#define NS_ISERVICEMANAGER_IID \
  {0x8bb35ed9, 0xe332, 0x462d, \
    { 0x91, 0x55, 0x4a, 0x00, 0x2a, 0xb5, 0xc9, 0x58 }}

class NS_NO_VTABLE nsIServiceManager : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISERVICEMANAGER_IID)

  /* void getService (in nsCIDRef aClass, in nsIIDRef aIID, [iid_is (aIID), retval] out nsQIResult result); */
  NS_IMETHOD GetService(const nsCID & aClass, const nsIID & aIID, void **result) = 0;

  /* void getServiceByContractID (in string aContractID, in nsIIDRef aIID, [iid_is (aIID), retval] out nsQIResult result); */
  NS_IMETHOD GetServiceByContractID(const char * aContractID, const nsIID & aIID, void **result) = 0;

  /* boolean isServiceInstantiated (in nsCIDRef aClass, in nsIIDRef aIID); */
  NS_IMETHOD IsServiceInstantiated(const nsCID & aClass, const nsIID & aIID, bool *_retval) = 0;

  /* boolean isServiceInstantiatedByContractID (in string aContractID, in nsIIDRef aIID); */
  NS_IMETHOD IsServiceInstantiatedByContractID(const char * aContractID, const nsIID & aIID, bool *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIServiceManager, NS_ISERVICEMANAGER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISERVICEMANAGER \
  NS_IMETHOD GetService(const nsCID & aClass, const nsIID & aIID, void **result) override; \
  NS_IMETHOD GetServiceByContractID(const char * aContractID, const nsIID & aIID, void **result) override; \
  NS_IMETHOD IsServiceInstantiated(const nsCID & aClass, const nsIID & aIID, bool *_retval) override; \
  NS_IMETHOD IsServiceInstantiatedByContractID(const char * aContractID, const nsIID & aIID, bool *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISERVICEMANAGER \
  nsresult GetService(const nsCID & aClass, const nsIID & aIID, void **result); \
  nsresult GetServiceByContractID(const char * aContractID, const nsIID & aIID, void **result); \
  nsresult IsServiceInstantiated(const nsCID & aClass, const nsIID & aIID, bool *_retval); \
  nsresult IsServiceInstantiatedByContractID(const char * aContractID, const nsIID & aIID, bool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISERVICEMANAGER(_to) \
  NS_IMETHOD GetService(const nsCID & aClass, const nsIID & aIID, void **result) override { return _to GetService(aClass, aIID, result); } \
  NS_IMETHOD GetServiceByContractID(const char * aContractID, const nsIID & aIID, void **result) override { return _to GetServiceByContractID(aContractID, aIID, result); } \
  NS_IMETHOD IsServiceInstantiated(const nsCID & aClass, const nsIID & aIID, bool *_retval) override { return _to IsServiceInstantiated(aClass, aIID, _retval); } \
  NS_IMETHOD IsServiceInstantiatedByContractID(const char * aContractID, const nsIID & aIID, bool *_retval) override { return _to IsServiceInstantiatedByContractID(aContractID, aIID, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISERVICEMANAGER(_to) \
  NS_IMETHOD GetService(const nsCID & aClass, const nsIID & aIID, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetService(aClass, aIID, result); } \
  NS_IMETHOD GetServiceByContractID(const char * aContractID, const nsIID & aIID, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetServiceByContractID(aContractID, aIID, result); } \
  NS_IMETHOD IsServiceInstantiated(const nsCID & aClass, const nsIID & aIID, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsServiceInstantiated(aClass, aIID, _retval); } \
  NS_IMETHOD IsServiceInstantiatedByContractID(const char * aContractID, const nsIID & aIID, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsServiceInstantiatedByContractID(aContractID, aIID, _retval); } 

// Observing xpcom autoregistration.  Topics will be 'start' and 'stop'.
#define NS_XPCOM_AUTOREGISTRATION_OBSERVER_ID "xpcom-autoregistration"
#ifdef MOZILLA_INTERNAL_API
#include "nsXPCOM.h"
#include "nsComponentManagerUtils.h"
#include "nsServiceManagerUtils.h"
#endif

#endif /* __gen_nsIServiceManager_h__ */
