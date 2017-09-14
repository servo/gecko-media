/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIClassInfo.idl
 */

#ifndef __gen_nsIClassInfo_h__
#define __gen_nsIClassInfo_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIXPCScriptable; /* forward declaration */


/* starting interface:    nsIClassInfo */
#define NS_ICLASSINFO_IID_STR "a60569d7-d401-4677-ba63-2aa5971af25d"

#define NS_ICLASSINFO_IID \
  {0xa60569d7, 0xd401, 0x4677, \
    { 0xba, 0x63, 0x2a, 0xa5, 0x97, 0x1a, 0xf2, 0x5d }}

class NS_NO_VTABLE nsIClassInfo : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICLASSINFO_IID)

  /* void getInterfaces (out uint32_t count, [array, size_is (count), retval] out nsIIDPtr array); */
  NS_IMETHOD GetInterfaces(uint32_t *count, nsIID ***array) = 0;

  /* nsIXPCScriptable getScriptableHelper (); */
  NS_IMETHOD GetScriptableHelper(nsIXPCScriptable * *_retval) = 0;

  /* readonly attribute string contractID; */
  NS_IMETHOD GetContractID(char * *aContractID) = 0;

  /* readonly attribute string classDescription; */
  NS_IMETHOD GetClassDescription(char * *aClassDescription) = 0;

  /* readonly attribute nsCIDPtr classID; */
  NS_IMETHOD GetClassID(nsCID **aClassID) = 0;

  enum {
    SINGLETON = 1U,
    THREADSAFE = 2U,
    MAIN_THREAD_ONLY = 4U,
    DOM_OBJECT = 8U,
    PLUGIN_OBJECT = 16U,
    SINGLETON_CLASSINFO = 32U,
    CONTENT_NODE = 64U,
    RESERVED = 2147483648U
  };

  /* readonly attribute uint32_t flags; */
  NS_IMETHOD GetFlags(uint32_t *aFlags) = 0;

  /* [noscript] readonly attribute nsCID classIDNoAlloc; */
  NS_IMETHOD GetClassIDNoAlloc(nsCID *aClassIDNoAlloc) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIClassInfo, NS_ICLASSINFO_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICLASSINFO \
  NS_IMETHOD GetInterfaces(uint32_t *count, nsIID ***array) override; \
  NS_IMETHOD GetScriptableHelper(nsIXPCScriptable * *_retval) override; \
  NS_IMETHOD GetContractID(char * *aContractID) override; \
  NS_IMETHOD GetClassDescription(char * *aClassDescription) override; \
  NS_IMETHOD GetClassID(nsCID **aClassID) override; \
  NS_IMETHOD GetFlags(uint32_t *aFlags) override; \
  NS_IMETHOD GetClassIDNoAlloc(nsCID *aClassIDNoAlloc) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSICLASSINFO \
  nsresult GetInterfaces(uint32_t *count, nsIID ***array); \
  nsresult GetScriptableHelper(nsIXPCScriptable * *_retval); \
  nsresult GetContractID(char * *aContractID); \
  nsresult GetClassDescription(char * *aClassDescription); \
  nsresult GetClassID(nsCID **aClassID); \
  nsresult GetFlags(uint32_t *aFlags); \
  nsresult GetClassIDNoAlloc(nsCID *aClassIDNoAlloc); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICLASSINFO(_to) \
  NS_IMETHOD GetInterfaces(uint32_t *count, nsIID ***array) override { return _to GetInterfaces(count, array); } \
  NS_IMETHOD GetScriptableHelper(nsIXPCScriptable * *_retval) override { return _to GetScriptableHelper(_retval); } \
  NS_IMETHOD GetContractID(char * *aContractID) override { return _to GetContractID(aContractID); } \
  NS_IMETHOD GetClassDescription(char * *aClassDescription) override { return _to GetClassDescription(aClassDescription); } \
  NS_IMETHOD GetClassID(nsCID **aClassID) override { return _to GetClassID(aClassID); } \
  NS_IMETHOD GetFlags(uint32_t *aFlags) override { return _to GetFlags(aFlags); } \
  NS_IMETHOD GetClassIDNoAlloc(nsCID *aClassIDNoAlloc) override { return _to GetClassIDNoAlloc(aClassIDNoAlloc); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICLASSINFO(_to) \
  NS_IMETHOD GetInterfaces(uint32_t *count, nsIID ***array) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInterfaces(count, array); } \
  NS_IMETHOD GetScriptableHelper(nsIXPCScriptable * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetScriptableHelper(_retval); } \
  NS_IMETHOD GetContractID(char * *aContractID) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetContractID(aContractID); } \
  NS_IMETHOD GetClassDescription(char * *aClassDescription) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClassDescription(aClassDescription); } \
  NS_IMETHOD GetClassID(nsCID **aClassID) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClassID(aClassID); } \
  NS_IMETHOD GetFlags(uint32_t *aFlags) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFlags(aFlags); } \
  NS_IMETHOD GetClassIDNoAlloc(nsCID *aClassIDNoAlloc) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClassIDNoAlloc(aClassIDNoAlloc); } 


#endif /* __gen_nsIClassInfo_h__ */
