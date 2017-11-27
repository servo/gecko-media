/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIComponentRegistrar.idl
 */

#ifndef __gen_nsIComponentRegistrar_h__
#define __gen_nsIComponentRegistrar_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */

class nsIFactory; /* forward declaration */

class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIComponentRegistrar */
#define NS_ICOMPONENTREGISTRAR_IID_STR "2417cbfe-65ad-48a6-b4b6-eb84db174392"

#define NS_ICOMPONENTREGISTRAR_IID \
  {0x2417cbfe, 0x65ad, 0x48a6, \
    { 0xb4, 0xb6, 0xeb, 0x84, 0xdb, 0x17, 0x43, 0x92 }}

class NS_NO_VTABLE nsIComponentRegistrar : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICOMPONENTREGISTRAR_IID)

  /* void autoRegister (in nsIFile aSpec); */
  NS_IMETHOD AutoRegister(nsIFile *aSpec) = 0;

  /* void autoUnregister (in nsIFile aSpec); */
  NS_IMETHOD AutoUnregister(nsIFile *aSpec) = 0;

  /* void registerFactory (in nsCIDRef aClass, in string aClassName, in string aContractID, in nsIFactory aFactory); */
  NS_IMETHOD RegisterFactory(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFactory *aFactory) = 0;

  /* void unregisterFactory (in nsCIDRef aClass, in nsIFactory aFactory); */
  NS_IMETHOD UnregisterFactory(const nsCID & aClass, nsIFactory *aFactory) = 0;

  /* void registerFactoryLocation (in nsCIDRef aClass, in string aClassName, in string aContractID, in nsIFile aFile, in string aLoaderStr, in string aType); */
  NS_IMETHOD RegisterFactoryLocation(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFile *aFile, const char * aLoaderStr, const char * aType) = 0;

  /* void unregisterFactoryLocation (in nsCIDRef aClass, in nsIFile aFile); */
  NS_IMETHOD UnregisterFactoryLocation(const nsCID & aClass, nsIFile *aFile) = 0;

  /* boolean isCIDRegistered (in nsCIDRef aClass); */
  NS_IMETHOD IsCIDRegistered(const nsCID & aClass, bool *_retval) = 0;

  /* boolean isContractIDRegistered (in string aContractID); */
  NS_IMETHOD IsContractIDRegistered(const char * aContractID, bool *_retval) = 0;

  /* nsISimpleEnumerator enumerateCIDs (); */
  NS_IMETHOD EnumerateCIDs(nsISimpleEnumerator * *_retval) = 0;

  /* nsISimpleEnumerator enumerateContractIDs (); */
  NS_IMETHOD EnumerateContractIDs(nsISimpleEnumerator * *_retval) = 0;

  /* string CIDToContractID (in nsCIDRef aClass); */
  NS_IMETHOD CIDToContractID(const nsCID & aClass, char * *_retval) = 0;

  /* nsCIDPtr contractIDToCID (in string aContractID); */
  NS_IMETHOD ContractIDToCID(const char * aContractID, nsCID **_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIComponentRegistrar, NS_ICOMPONENTREGISTRAR_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICOMPONENTREGISTRAR \
  NS_IMETHOD AutoRegister(nsIFile *aSpec) override; \
  NS_IMETHOD AutoUnregister(nsIFile *aSpec) override; \
  NS_IMETHOD RegisterFactory(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFactory *aFactory) override; \
  NS_IMETHOD UnregisterFactory(const nsCID & aClass, nsIFactory *aFactory) override; \
  NS_IMETHOD RegisterFactoryLocation(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFile *aFile, const char * aLoaderStr, const char * aType) override; \
  NS_IMETHOD UnregisterFactoryLocation(const nsCID & aClass, nsIFile *aFile) override; \
  NS_IMETHOD IsCIDRegistered(const nsCID & aClass, bool *_retval) override; \
  NS_IMETHOD IsContractIDRegistered(const char * aContractID, bool *_retval) override; \
  NS_IMETHOD EnumerateCIDs(nsISimpleEnumerator * *_retval) override; \
  NS_IMETHOD EnumerateContractIDs(nsISimpleEnumerator * *_retval) override; \
  NS_IMETHOD CIDToContractID(const nsCID & aClass, char * *_retval) override; \
  NS_IMETHOD ContractIDToCID(const char * aContractID, nsCID **_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSICOMPONENTREGISTRAR \
  nsresult AutoRegister(nsIFile *aSpec); \
  nsresult AutoUnregister(nsIFile *aSpec); \
  nsresult RegisterFactory(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFactory *aFactory); \
  nsresult UnregisterFactory(const nsCID & aClass, nsIFactory *aFactory); \
  nsresult RegisterFactoryLocation(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFile *aFile, const char * aLoaderStr, const char * aType); \
  nsresult UnregisterFactoryLocation(const nsCID & aClass, nsIFile *aFile); \
  nsresult IsCIDRegistered(const nsCID & aClass, bool *_retval); \
  nsresult IsContractIDRegistered(const char * aContractID, bool *_retval); \
  nsresult EnumerateCIDs(nsISimpleEnumerator * *_retval); \
  nsresult EnumerateContractIDs(nsISimpleEnumerator * *_retval); \
  nsresult CIDToContractID(const nsCID & aClass, char * *_retval); \
  nsresult ContractIDToCID(const char * aContractID, nsCID **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICOMPONENTREGISTRAR(_to) \
  NS_IMETHOD AutoRegister(nsIFile *aSpec) override { return _to AutoRegister(aSpec); } \
  NS_IMETHOD AutoUnregister(nsIFile *aSpec) override { return _to AutoUnregister(aSpec); } \
  NS_IMETHOD RegisterFactory(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFactory *aFactory) override { return _to RegisterFactory(aClass, aClassName, aContractID, aFactory); } \
  NS_IMETHOD UnregisterFactory(const nsCID & aClass, nsIFactory *aFactory) override { return _to UnregisterFactory(aClass, aFactory); } \
  NS_IMETHOD RegisterFactoryLocation(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFile *aFile, const char * aLoaderStr, const char * aType) override { return _to RegisterFactoryLocation(aClass, aClassName, aContractID, aFile, aLoaderStr, aType); } \
  NS_IMETHOD UnregisterFactoryLocation(const nsCID & aClass, nsIFile *aFile) override { return _to UnregisterFactoryLocation(aClass, aFile); } \
  NS_IMETHOD IsCIDRegistered(const nsCID & aClass, bool *_retval) override { return _to IsCIDRegistered(aClass, _retval); } \
  NS_IMETHOD IsContractIDRegistered(const char * aContractID, bool *_retval) override { return _to IsContractIDRegistered(aContractID, _retval); } \
  NS_IMETHOD EnumerateCIDs(nsISimpleEnumerator * *_retval) override { return _to EnumerateCIDs(_retval); } \
  NS_IMETHOD EnumerateContractIDs(nsISimpleEnumerator * *_retval) override { return _to EnumerateContractIDs(_retval); } \
  NS_IMETHOD CIDToContractID(const nsCID & aClass, char * *_retval) override { return _to CIDToContractID(aClass, _retval); } \
  NS_IMETHOD ContractIDToCID(const char * aContractID, nsCID **_retval) override { return _to ContractIDToCID(aContractID, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICOMPONENTREGISTRAR(_to) \
  NS_IMETHOD AutoRegister(nsIFile *aSpec) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AutoRegister(aSpec); } \
  NS_IMETHOD AutoUnregister(nsIFile *aSpec) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AutoUnregister(aSpec); } \
  NS_IMETHOD RegisterFactory(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFactory *aFactory) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterFactory(aClass, aClassName, aContractID, aFactory); } \
  NS_IMETHOD UnregisterFactory(const nsCID & aClass, nsIFactory *aFactory) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterFactory(aClass, aFactory); } \
  NS_IMETHOD RegisterFactoryLocation(const nsCID & aClass, const char * aClassName, const char * aContractID, nsIFile *aFile, const char * aLoaderStr, const char * aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterFactoryLocation(aClass, aClassName, aContractID, aFile, aLoaderStr, aType); } \
  NS_IMETHOD UnregisterFactoryLocation(const nsCID & aClass, nsIFile *aFile) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterFactoryLocation(aClass, aFile); } \
  NS_IMETHOD IsCIDRegistered(const nsCID & aClass, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsCIDRegistered(aClass, _retval); } \
  NS_IMETHOD IsContractIDRegistered(const char * aContractID, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsContractIDRegistered(aContractID, _retval); } \
  NS_IMETHOD EnumerateCIDs(nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EnumerateCIDs(_retval); } \
  NS_IMETHOD EnumerateContractIDs(nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EnumerateContractIDs(_retval); } \
  NS_IMETHOD CIDToContractID(const nsCID & aClass, char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CIDToContractID(aClass, _retval); } \
  NS_IMETHOD ContractIDToCID(const char * aContractID, nsCID **_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ContractIDToCID(aContractID, _retval); } 


#endif /* __gen_nsIComponentRegistrar_h__ */
