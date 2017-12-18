/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIGIOService.idl
 */

#ifndef __gen_nsIGIOService_h__
#define __gen_nsIGIOService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIUTF8StringEnumerator; /* forward declaration */

class nsIURI; /* forward declaration */


/* starting interface:    nsIGIOMimeApp */
#define NS_IGIOMIMEAPP_IID_STR "ca6bad0c-8a48-48ac-82c7-27bb8f510fbe"

#define NS_IGIOMIMEAPP_IID \
  {0xca6bad0c, 0x8a48, 0x48ac, \
    { 0x82, 0xc7, 0x27, 0xbb, 0x8f, 0x51, 0x0f, 0xbe }}

class NS_NO_VTABLE nsIGIOMimeApp : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IGIOMIMEAPP_IID)

  enum {
    EXPECTS_URIS = 0,
    EXPECTS_PATHS = 1,
    EXPECTS_URIS_FOR_NON_FILES = 2
  };

  /* readonly attribute AUTF8String id; */
  NS_IMETHOD GetId(nsACString & aId) = 0;

  /* readonly attribute AUTF8String name; */
  NS_IMETHOD GetName(nsACString & aName) = 0;

  /* readonly attribute AUTF8String command; */
  NS_IMETHOD GetCommand(nsACString & aCommand) = 0;

  /* readonly attribute long expectsURIs; */
  NS_IMETHOD GetExpectsURIs(int32_t *aExpectsURIs) = 0;

  /* readonly attribute nsIUTF8StringEnumerator supportedURISchemes; */
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) = 0;

  /* void launch (in AUTF8String uri); */
  NS_IMETHOD Launch(const nsACString & uri) = 0;

  /* void setAsDefaultForMimeType (in AUTF8String mimeType); */
  NS_IMETHOD SetAsDefaultForMimeType(const nsACString & mimeType) = 0;

  /* void setAsDefaultForFileExtensions (in AUTF8String extensions); */
  NS_IMETHOD SetAsDefaultForFileExtensions(const nsACString & extensions) = 0;

  /* void setAsDefaultForURIScheme (in AUTF8String uriScheme); */
  NS_IMETHOD SetAsDefaultForURIScheme(const nsACString & uriScheme) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIGIOMimeApp, NS_IGIOMIMEAPP_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGIOMIMEAPP \
  NS_IMETHOD GetId(nsACString & aId) override; \
  NS_IMETHOD GetName(nsACString & aName) override; \
  NS_IMETHOD GetCommand(nsACString & aCommand) override; \
  NS_IMETHOD GetExpectsURIs(int32_t *aExpectsURIs) override; \
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) override; \
  NS_IMETHOD Launch(const nsACString & uri) override; \
  NS_IMETHOD SetAsDefaultForMimeType(const nsACString & mimeType) override; \
  NS_IMETHOD SetAsDefaultForFileExtensions(const nsACString & extensions) override; \
  NS_IMETHOD SetAsDefaultForURIScheme(const nsACString & uriScheme) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIGIOMIMEAPP \
  nsresult GetId(nsACString & aId); \
  nsresult GetName(nsACString & aName); \
  nsresult GetCommand(nsACString & aCommand); \
  nsresult GetExpectsURIs(int32_t *aExpectsURIs); \
  nsresult GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes); \
  nsresult Launch(const nsACString & uri); \
  nsresult SetAsDefaultForMimeType(const nsACString & mimeType); \
  nsresult SetAsDefaultForFileExtensions(const nsACString & extensions); \
  nsresult SetAsDefaultForURIScheme(const nsACString & uriScheme); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGIOMIMEAPP(_to) \
  NS_IMETHOD GetId(nsACString & aId) override { return _to GetId(aId); } \
  NS_IMETHOD GetName(nsACString & aName) override { return _to GetName(aName); } \
  NS_IMETHOD GetCommand(nsACString & aCommand) override { return _to GetCommand(aCommand); } \
  NS_IMETHOD GetExpectsURIs(int32_t *aExpectsURIs) override { return _to GetExpectsURIs(aExpectsURIs); } \
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) override { return _to GetSupportedURISchemes(aSupportedURISchemes); } \
  NS_IMETHOD Launch(const nsACString & uri) override { return _to Launch(uri); } \
  NS_IMETHOD SetAsDefaultForMimeType(const nsACString & mimeType) override { return _to SetAsDefaultForMimeType(mimeType); } \
  NS_IMETHOD SetAsDefaultForFileExtensions(const nsACString & extensions) override { return _to SetAsDefaultForFileExtensions(extensions); } \
  NS_IMETHOD SetAsDefaultForURIScheme(const nsACString & uriScheme) override { return _to SetAsDefaultForURIScheme(uriScheme); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGIOMIMEAPP(_to) \
  NS_IMETHOD GetId(nsACString & aId) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetId(aId); } \
  NS_IMETHOD GetName(nsACString & aName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetCommand(nsACString & aCommand) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCommand(aCommand); } \
  NS_IMETHOD GetExpectsURIs(int32_t *aExpectsURIs) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetExpectsURIs(aExpectsURIs); } \
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSupportedURISchemes(aSupportedURISchemes); } \
  NS_IMETHOD Launch(const nsACString & uri) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Launch(uri); } \
  NS_IMETHOD SetAsDefaultForMimeType(const nsACString & mimeType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAsDefaultForMimeType(mimeType); } \
  NS_IMETHOD SetAsDefaultForFileExtensions(const nsACString & extensions) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAsDefaultForFileExtensions(extensions); } \
  NS_IMETHOD SetAsDefaultForURIScheme(const nsACString & uriScheme) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAsDefaultForURIScheme(uriScheme); } 


/* starting interface:    nsIGIOService */
#define NS_IGIOSERVICE_IID_STR "eda22a30-84e1-4e16-9ca0-cd1553c2b34a"

#define NS_IGIOSERVICE_IID \
  {0xeda22a30, 0x84e1, 0x4e16, \
    { 0x9c, 0xa0, 0xcd, 0x15, 0x53, 0xc2, 0xb3, 0x4a }}

class NS_NO_VTABLE nsIGIOService : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IGIOSERVICE_IID)

  /* AUTF8String getMimeTypeFromExtension (in AUTF8String extension); */
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) = 0;

  /* nsIGIOMimeApp getAppForURIScheme (in AUTF8String aURIScheme); */
  NS_IMETHOD GetAppForURIScheme(const nsACString & aURIScheme, nsIGIOMimeApp * *_retval) = 0;

  /* nsIGIOMimeApp getAppForMimeType (in AUTF8String mimeType); */
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGIOMimeApp * *_retval) = 0;

  /* nsIGIOMimeApp createAppFromCommand (in AUTF8String cmd, in AUTF8String appName); */
  NS_IMETHOD CreateAppFromCommand(const nsACString & cmd, const nsACString & appName, nsIGIOMimeApp * *_retval) = 0;

  /* AUTF8String getDescriptionForMimeType (in AUTF8String mimeType); */
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) = 0;

  /* void showURI (in nsIURI uri); */
  NS_IMETHOD ShowURI(nsIURI *uri) = 0;

  /* [noscript] void showURIForInput (in ACString uri); */
  NS_IMETHOD ShowURIForInput(const nsACString & uri) = 0;

  /* [noscript] void orgFreedesktopFileManager1ShowItems (in ACString path); */
  NS_IMETHOD OrgFreedesktopFileManager1ShowItems(const nsACString & path) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIGIOService, NS_IGIOSERVICE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGIOSERVICE \
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) override; \
  NS_IMETHOD GetAppForURIScheme(const nsACString & aURIScheme, nsIGIOMimeApp * *_retval) override; \
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGIOMimeApp * *_retval) override; \
  NS_IMETHOD CreateAppFromCommand(const nsACString & cmd, const nsACString & appName, nsIGIOMimeApp * *_retval) override; \
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) override; \
  NS_IMETHOD ShowURI(nsIURI *uri) override; \
  NS_IMETHOD ShowURIForInput(const nsACString & uri) override; \
  NS_IMETHOD OrgFreedesktopFileManager1ShowItems(const nsACString & path) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIGIOSERVICE \
  nsresult GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval); \
  nsresult GetAppForURIScheme(const nsACString & aURIScheme, nsIGIOMimeApp * *_retval); \
  nsresult GetAppForMimeType(const nsACString & mimeType, nsIGIOMimeApp * *_retval); \
  nsresult CreateAppFromCommand(const nsACString & cmd, const nsACString & appName, nsIGIOMimeApp * *_retval); \
  nsresult GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval); \
  nsresult ShowURI(nsIURI *uri); \
  nsresult ShowURIForInput(const nsACString & uri); \
  nsresult OrgFreedesktopFileManager1ShowItems(const nsACString & path); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGIOSERVICE(_to) \
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) override { return _to GetMimeTypeFromExtension(extension, _retval); } \
  NS_IMETHOD GetAppForURIScheme(const nsACString & aURIScheme, nsIGIOMimeApp * *_retval) override { return _to GetAppForURIScheme(aURIScheme, _retval); } \
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGIOMimeApp * *_retval) override { return _to GetAppForMimeType(mimeType, _retval); } \
  NS_IMETHOD CreateAppFromCommand(const nsACString & cmd, const nsACString & appName, nsIGIOMimeApp * *_retval) override { return _to CreateAppFromCommand(cmd, appName, _retval); } \
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) override { return _to GetDescriptionForMimeType(mimeType, _retval); } \
  NS_IMETHOD ShowURI(nsIURI *uri) override { return _to ShowURI(uri); } \
  NS_IMETHOD ShowURIForInput(const nsACString & uri) override { return _to ShowURIForInput(uri); } \
  NS_IMETHOD OrgFreedesktopFileManager1ShowItems(const nsACString & path) override { return _to OrgFreedesktopFileManager1ShowItems(path); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGIOSERVICE(_to) \
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMimeTypeFromExtension(extension, _retval); } \
  NS_IMETHOD GetAppForURIScheme(const nsACString & aURIScheme, nsIGIOMimeApp * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAppForURIScheme(aURIScheme, _retval); } \
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGIOMimeApp * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAppForMimeType(mimeType, _retval); } \
  NS_IMETHOD CreateAppFromCommand(const nsACString & cmd, const nsACString & appName, nsIGIOMimeApp * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateAppFromCommand(cmd, appName, _retval); } \
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDescriptionForMimeType(mimeType, _retval); } \
  NS_IMETHOD ShowURI(nsIURI *uri) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ShowURI(uri); } \
  NS_IMETHOD ShowURIForInput(const nsACString & uri) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ShowURIForInput(uri); } \
  NS_IMETHOD OrgFreedesktopFileManager1ShowItems(const nsACString & path) override { return !_to ? NS_ERROR_NULL_POINTER : _to->OrgFreedesktopFileManager1ShowItems(path); } 

#define NS_GIOSERVICE_CONTRACTID "@mozilla.org/gio-service;1"

#endif /* __gen_nsIGIOService_h__ */
