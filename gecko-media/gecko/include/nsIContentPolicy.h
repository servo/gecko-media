/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIContentPolicy.idl
 */

#ifndef __gen_nsIContentPolicy_h__
#define __gen_nsIContentPolicy_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIURI; /* forward declaration */

class nsIDOMNode; /* forward declaration */

class nsIPrincipal; /* forward declaration */

typedef uint32_t  nsContentPolicyType;


/* starting interface:    nsIContentPolicy */
#define NS_ICONTENTPOLICY_IID_STR "caad4f1f-d047-46ac-ae9d-dc598e4fb91b"

#define NS_ICONTENTPOLICY_IID \
  {0xcaad4f1f, 0xd047, 0x46ac, \
    { 0xae, 0x9d, 0xdc, 0x59, 0x8e, 0x4f, 0xb9, 0x1b }}

class NS_NO_VTABLE nsIContentPolicy : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICONTENTPOLICY_IID)

  enum {
    TYPE_INVALID = 0U,
    TYPE_OTHER = 1U,
    TYPE_SCRIPT = 2U,
    TYPE_IMAGE = 3U,
    TYPE_STYLESHEET = 4U,
    TYPE_OBJECT = 5U,
    TYPE_DOCUMENT = 6U,
    TYPE_SUBDOCUMENT = 7U,
    TYPE_REFRESH = 8U,
    TYPE_XBL = 9U,
    TYPE_PING = 10U,
    TYPE_XMLHTTPREQUEST = 11U,
    TYPE_DATAREQUEST = 11U,
    TYPE_OBJECT_SUBREQUEST = 12U,
    TYPE_DTD = 13U,
    TYPE_FONT = 14U,
    TYPE_MEDIA = 15U,
    TYPE_WEBSOCKET = 16U,
    TYPE_CSP_REPORT = 17U,
    TYPE_XSLT = 18U,
    TYPE_BEACON = 19U,
    TYPE_FETCH = 20U,
    TYPE_IMAGESET = 21U,
    TYPE_WEB_MANIFEST = 22U,
    TYPE_SAVEAS_DOWNLOAD = 43U,
    TYPE_INTERNAL_SCRIPT = 23U,
    TYPE_INTERNAL_WORKER = 24U,
    TYPE_INTERNAL_SHARED_WORKER = 25U,
    TYPE_INTERNAL_EMBED = 26U,
    TYPE_INTERNAL_OBJECT = 27U,
    TYPE_INTERNAL_FRAME = 28U,
    TYPE_INTERNAL_IFRAME = 29U,
    TYPE_INTERNAL_AUDIO = 30U,
    TYPE_INTERNAL_VIDEO = 31U,
    TYPE_INTERNAL_TRACK = 32U,
    TYPE_INTERNAL_XMLHTTPREQUEST = 33U,
    TYPE_INTERNAL_EVENTSOURCE = 34U,
    TYPE_INTERNAL_SERVICE_WORKER = 35U,
    TYPE_INTERNAL_SCRIPT_PRELOAD = 36U,
    TYPE_INTERNAL_IMAGE = 37U,
    TYPE_INTERNAL_IMAGE_PRELOAD = 38U,
    TYPE_INTERNAL_STYLESHEET = 39U,
    TYPE_INTERNAL_STYLESHEET_PRELOAD = 40U,
    TYPE_INTERNAL_IMAGE_FAVICON = 41U,
    TYPE_INTERNAL_WORKER_IMPORT_SCRIPTS = 42U,
    REJECT_REQUEST = -1,
    REJECT_TYPE = -2,
    REJECT_SERVER = -3,
    REJECT_OTHER = -4,
    ACCEPT = 1
  };

  /* short shouldLoad (in nsContentPolicyType aContentType, in nsIURI aContentLocation, in nsIURI aRequestOrigin, in nsISupports aContext, in ACString aMimeTypeGuess, in nsISupports aExtra, [optional] in nsIPrincipal aRequestPrincipal); */
  NS_IMETHOD ShouldLoad(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeTypeGuess, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) = 0;

  /* short shouldProcess (in nsContentPolicyType aContentType, in nsIURI aContentLocation, in nsIURI aRequestOrigin, in nsISupports aContext, in ACString aMimeType, in nsISupports aExtra, [optional] in nsIPrincipal aRequestPrincipal); */
  NS_IMETHOD ShouldProcess(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeType, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIContentPolicy, NS_ICONTENTPOLICY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICONTENTPOLICY \
  NS_IMETHOD ShouldLoad(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeTypeGuess, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) override; \
  NS_IMETHOD ShouldProcess(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeType, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSICONTENTPOLICY \
  nsresult ShouldLoad(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeTypeGuess, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval); \
  nsresult ShouldProcess(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeType, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICONTENTPOLICY(_to) \
  NS_IMETHOD ShouldLoad(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeTypeGuess, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) override { return _to ShouldLoad(aContentType, aContentLocation, aRequestOrigin, aContext, aMimeTypeGuess, aExtra, aRequestPrincipal, _retval); } \
  NS_IMETHOD ShouldProcess(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeType, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) override { return _to ShouldProcess(aContentType, aContentLocation, aRequestOrigin, aContext, aMimeType, aExtra, aRequestPrincipal, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICONTENTPOLICY(_to) \
  NS_IMETHOD ShouldLoad(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeTypeGuess, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ShouldLoad(aContentType, aContentLocation, aRequestOrigin, aContext, aMimeTypeGuess, aExtra, aRequestPrincipal, _retval); } \
  NS_IMETHOD ShouldProcess(nsContentPolicyType aContentType, nsIURI *aContentLocation, nsIURI *aRequestOrigin, nsISupports *aContext, const nsACString & aMimeType, nsISupports *aExtra, nsIPrincipal *aRequestPrincipal, int16_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ShouldProcess(aContentType, aContentLocation, aRequestOrigin, aContext, aMimeType, aExtra, aRequestPrincipal, _retval); } 


#endif /* __gen_nsIContentPolicy_h__ */
