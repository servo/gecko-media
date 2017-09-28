/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIInterfaceInfoManager.idl
 */

#ifndef __gen_nsIInterfaceInfoManager_h__
#define __gen_nsIInterfaceInfoManager_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIInterfaceInfo; /* forward declaration */


/* starting interface:    nsIInterfaceInfoManager */
#define NS_IINTERFACEINFOMANAGER_IID_STR "1d53d8d9-1d92-428f-b5cc-198b55e897d7"

#define NS_IINTERFACEINFOMANAGER_IID \
  {0x1d53d8d9, 0x1d92, 0x428f, \
    { 0xb5, 0xcc, 0x19, 0x8b, 0x55, 0xe8, 0x97, 0xd7 }}

class NS_NO_VTABLE nsIInterfaceInfoManager : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IINTERFACEINFOMANAGER_IID)

  /* nsIInterfaceInfo getInfoForIID (in nsIIDPtr iid); */
  NS_IMETHOD GetInfoForIID(const nsIID *iid, nsIInterfaceInfo * *_retval) = 0;

  /* nsIInterfaceInfo getInfoForName (in string name); */
  NS_IMETHOD GetInfoForName(const char * name, nsIInterfaceInfo * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIInterfaceInfoManager, NS_IINTERFACEINFOMANAGER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIINTERFACEINFOMANAGER \
  NS_IMETHOD GetInfoForIID(const nsIID *iid, nsIInterfaceInfo * *_retval) override; \
  NS_IMETHOD GetInfoForName(const char * name, nsIInterfaceInfo * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIINTERFACEINFOMANAGER \
  nsresult GetInfoForIID(const nsIID *iid, nsIInterfaceInfo * *_retval); \
  nsresult GetInfoForName(const char * name, nsIInterfaceInfo * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIINTERFACEINFOMANAGER(_to) \
  NS_IMETHOD GetInfoForIID(const nsIID *iid, nsIInterfaceInfo * *_retval) override { return _to GetInfoForIID(iid, _retval); } \
  NS_IMETHOD GetInfoForName(const char * name, nsIInterfaceInfo * *_retval) override { return _to GetInfoForName(name, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIINTERFACEINFOMANAGER(_to) \
  NS_IMETHOD GetInfoForIID(const nsIID *iid, nsIInterfaceInfo * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInfoForIID(iid, _retval); } \
  NS_IMETHOD GetInfoForName(const char * name, nsIInterfaceInfo * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInfoForName(name, _retval); } 

#define NS_INTERFACEINFOMANAGER_SERVICE_CID            \
 { /* 13bef784-f8e0-4f96-85c1-09f9ef4f9a19 */          \
  0x13bef784, 0xf8e0, 0x4f96,                          \
  {0x85, 0xc1, 0x09, 0xf9, 0xef, 0x4f, 0x9a, 0x19} }
#define NS_INTERFACEINFOMANAGER_SERVICE_CONTRACTID  \
   "@mozilla.org/xpti/interfaceinfomanager-service;1"

#endif /* __gen_nsIInterfaceInfoManager_h__ */
