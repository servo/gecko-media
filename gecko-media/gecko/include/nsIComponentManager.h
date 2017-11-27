/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIComponentManager.idl
 */

#ifndef __gen_nsIComponentManager_h__
#define __gen_nsIComponentManager_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */

class nsIFactory; /* forward declaration */

class nsIArray; /* forward declaration */


/* starting interface:    nsIComponentManager */
#define NS_ICOMPONENTMANAGER_IID_STR "d604ffc3-1ba3-4f6c-b65f-1ed4199364c3"

#define NS_ICOMPONENTMANAGER_IID \
  {0xd604ffc3, 0x1ba3, 0x4f6c, \
    { 0xb6, 0x5f, 0x1e, 0xd4, 0x19, 0x93, 0x64, 0xc3 }}

class NS_NO_VTABLE nsIComponentManager : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICOMPONENTMANAGER_IID)

  /* void getClassObject (in nsCIDRef aClass, in nsIIDRef aIID, [iid_is (aIID), retval] out nsQIResult result); */
  NS_IMETHOD GetClassObject(const nsCID & aClass, const nsIID & aIID, void **result) = 0;

  /* void getClassObjectByContractID (in string aContractID, in nsIIDRef aIID, [iid_is (aIID), retval] out nsQIResult result); */
  NS_IMETHOD GetClassObjectByContractID(const char * aContractID, const nsIID & aIID, void **result) = 0;

  /* void createInstance (in nsCIDRef aClass, in nsISupports aDelegate, in nsIIDRef aIID, [iid_is (aIID), retval] out nsQIResult result); */
  NS_IMETHOD CreateInstance(const nsCID & aClass, nsISupports *aDelegate, const nsIID & aIID, void **result) = 0;

  /* void createInstanceByContractID (in string aContractID, in nsISupports aDelegate, in nsIIDRef aIID, [iid_is (aIID), retval] out nsQIResult result); */
  NS_IMETHOD CreateInstanceByContractID(const char * aContractID, nsISupports *aDelegate, const nsIID & aIID, void **result) = 0;

  /* void addBootstrappedManifestLocation (in nsIFile aLocation); */
  NS_IMETHOD AddBootstrappedManifestLocation(nsIFile *aLocation) = 0;

  /* void removeBootstrappedManifestLocation (in nsIFile aLocation); */
  NS_IMETHOD RemoveBootstrappedManifestLocation(nsIFile *aLocation) = 0;

  /* nsIArray getManifestLocations (); */
  NS_IMETHOD GetManifestLocations(nsIArray * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIComponentManager, NS_ICOMPONENTMANAGER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICOMPONENTMANAGER \
  NS_IMETHOD GetClassObject(const nsCID & aClass, const nsIID & aIID, void **result) override; \
  NS_IMETHOD GetClassObjectByContractID(const char * aContractID, const nsIID & aIID, void **result) override; \
  NS_IMETHOD CreateInstance(const nsCID & aClass, nsISupports *aDelegate, const nsIID & aIID, void **result) override; \
  NS_IMETHOD CreateInstanceByContractID(const char * aContractID, nsISupports *aDelegate, const nsIID & aIID, void **result) override; \
  NS_IMETHOD AddBootstrappedManifestLocation(nsIFile *aLocation) override; \
  NS_IMETHOD RemoveBootstrappedManifestLocation(nsIFile *aLocation) override; \
  NS_IMETHOD GetManifestLocations(nsIArray * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSICOMPONENTMANAGER \
  nsresult GetClassObject(const nsCID & aClass, const nsIID & aIID, void **result); \
  nsresult GetClassObjectByContractID(const char * aContractID, const nsIID & aIID, void **result); \
  nsresult CreateInstance(const nsCID & aClass, nsISupports *aDelegate, const nsIID & aIID, void **result); \
  nsresult CreateInstanceByContractID(const char * aContractID, nsISupports *aDelegate, const nsIID & aIID, void **result); \
  nsresult AddBootstrappedManifestLocation(nsIFile *aLocation); \
  nsresult RemoveBootstrappedManifestLocation(nsIFile *aLocation); \
  nsresult GetManifestLocations(nsIArray * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICOMPONENTMANAGER(_to) \
  NS_IMETHOD GetClassObject(const nsCID & aClass, const nsIID & aIID, void **result) override { return _to GetClassObject(aClass, aIID, result); } \
  NS_IMETHOD GetClassObjectByContractID(const char * aContractID, const nsIID & aIID, void **result) override { return _to GetClassObjectByContractID(aContractID, aIID, result); } \
  NS_IMETHOD CreateInstance(const nsCID & aClass, nsISupports *aDelegate, const nsIID & aIID, void **result) override { return _to CreateInstance(aClass, aDelegate, aIID, result); } \
  NS_IMETHOD CreateInstanceByContractID(const char * aContractID, nsISupports *aDelegate, const nsIID & aIID, void **result) override { return _to CreateInstanceByContractID(aContractID, aDelegate, aIID, result); } \
  NS_IMETHOD AddBootstrappedManifestLocation(nsIFile *aLocation) override { return _to AddBootstrappedManifestLocation(aLocation); } \
  NS_IMETHOD RemoveBootstrappedManifestLocation(nsIFile *aLocation) override { return _to RemoveBootstrappedManifestLocation(aLocation); } \
  NS_IMETHOD GetManifestLocations(nsIArray * *_retval) override { return _to GetManifestLocations(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICOMPONENTMANAGER(_to) \
  NS_IMETHOD GetClassObject(const nsCID & aClass, const nsIID & aIID, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClassObject(aClass, aIID, result); } \
  NS_IMETHOD GetClassObjectByContractID(const char * aContractID, const nsIID & aIID, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClassObjectByContractID(aContractID, aIID, result); } \
  NS_IMETHOD CreateInstance(const nsCID & aClass, nsISupports *aDelegate, const nsIID & aIID, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateInstance(aClass, aDelegate, aIID, result); } \
  NS_IMETHOD CreateInstanceByContractID(const char * aContractID, nsISupports *aDelegate, const nsIID & aIID, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateInstanceByContractID(aContractID, aDelegate, aIID, result); } \
  NS_IMETHOD AddBootstrappedManifestLocation(nsIFile *aLocation) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AddBootstrappedManifestLocation(aLocation); } \
  NS_IMETHOD RemoveBootstrappedManifestLocation(nsIFile *aLocation) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveBootstrappedManifestLocation(aLocation); } \
  NS_IMETHOD GetManifestLocations(nsIArray * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetManifestLocations(_retval); } 

#ifdef MOZILLA_INTERNAL_API
#include "nsComponentManagerUtils.h"
#endif

#endif /* __gen_nsIComponentManager_h__ */
