/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIRequest.idl
 */

#ifndef __gen_nsIRequest_h__
#define __gen_nsIRequest_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsILoadGroup; /* forward declaration */

typedef uint32_t  nsLoadFlags;


/* starting interface:    nsIRequest */
#define NS_IREQUEST_IID_STR "ef6bfbd2-fd46-48d8-96b7-9f8f0fd387fe"

#define NS_IREQUEST_IID \
  {0xef6bfbd2, 0xfd46, 0x48d8, \
    { 0x96, 0xb7, 0x9f, 0x8f, 0x0f, 0xd3, 0x87, 0xfe }}

class NS_NO_VTABLE nsIRequest : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IREQUEST_IID)

  /* readonly attribute AUTF8String name; */
  NS_IMETHOD GetName(nsACString & aName) = 0;

  /* boolean isPending (); */
  NS_IMETHOD IsPending(bool *_retval) = 0;

  /* readonly attribute nsresult status; */
  NS_IMETHOD GetStatus(nsresult *aStatus) = 0;

  /* void cancel (in nsresult aStatus); */
  NS_IMETHOD Cancel(nsresult aStatus) = 0;

  /* void suspend (); */
  NS_IMETHOD Suspend(void) = 0;

  /* void resume (); */
  NS_IMETHOD Resume(void) = 0;

  /* attribute nsILoadGroup loadGroup; */
  NS_IMETHOD GetLoadGroup(nsILoadGroup * *aLoadGroup) = 0;
  NS_IMETHOD SetLoadGroup(nsILoadGroup *aLoadGroup) = 0;

  /* attribute nsLoadFlags loadFlags; */
  NS_IMETHOD GetLoadFlags(nsLoadFlags *aLoadFlags) = 0;
  NS_IMETHOD SetLoadFlags(nsLoadFlags aLoadFlags) = 0;

  enum {
    LOAD_REQUESTMASK = 65535U,
    LOAD_NORMAL = 0U,
    LOAD_BACKGROUND = 1U,
    LOAD_HTML_OBJECT_DATA = 2U,
    LOAD_DOCUMENT_NEEDS_COOKIE = 4U,
    INHIBIT_CACHING = 128U,
    INHIBIT_PERSISTENT_CACHING = 256U,
    LOAD_BYPASS_CACHE = 512U,
    LOAD_FROM_CACHE = 1024U,
    VALIDATE_ALWAYS = 2048U,
    VALIDATE_NEVER = 4096U,
    VALIDATE_ONCE_PER_SESSION = 8192U,
    LOAD_ANONYMOUS = 16384U,
    LOAD_FRESH_CONNECTION = 32768U
  };

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIRequest, NS_IREQUEST_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIREQUEST \
  NS_IMETHOD GetName(nsACString & aName) override; \
  NS_IMETHOD IsPending(bool *_retval) override; \
  NS_IMETHOD GetStatus(nsresult *aStatus) override; \
  NS_IMETHOD Cancel(nsresult aStatus) override; \
  NS_IMETHOD Suspend(void) override; \
  NS_IMETHOD Resume(void) override; \
  NS_IMETHOD GetLoadGroup(nsILoadGroup * *aLoadGroup) override; \
  NS_IMETHOD SetLoadGroup(nsILoadGroup *aLoadGroup) override; \
  NS_IMETHOD GetLoadFlags(nsLoadFlags *aLoadFlags) override; \
  NS_IMETHOD SetLoadFlags(nsLoadFlags aLoadFlags) override; \

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIREQUEST \
  nsresult GetName(nsACString & aName); \
  nsresult IsPending(bool *_retval); \
  nsresult GetStatus(nsresult *aStatus); \
  nsresult Cancel(nsresult aStatus); \
  nsresult Suspend(void); \
  nsresult Resume(void); \
  nsresult GetLoadGroup(nsILoadGroup * *aLoadGroup); \
  nsresult SetLoadGroup(nsILoadGroup *aLoadGroup); \
  nsresult GetLoadFlags(nsLoadFlags *aLoadFlags); \
  nsresult SetLoadFlags(nsLoadFlags aLoadFlags); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIREQUEST(_to) \
  NS_IMETHOD GetName(nsACString & aName) override { return _to GetName(aName); } \
  NS_IMETHOD IsPending(bool *_retval) override { return _to IsPending(_retval); } \
  NS_IMETHOD GetStatus(nsresult *aStatus) override { return _to GetStatus(aStatus); } \
  NS_IMETHOD Cancel(nsresult aStatus) override { return _to Cancel(aStatus); } \
  NS_IMETHOD Suspend(void) override { return _to Suspend(); } \
  NS_IMETHOD Resume(void) override { return _to Resume(); } \
  NS_IMETHOD GetLoadGroup(nsILoadGroup * *aLoadGroup) override { return _to GetLoadGroup(aLoadGroup); } \
  NS_IMETHOD SetLoadGroup(nsILoadGroup *aLoadGroup) override { return _to SetLoadGroup(aLoadGroup); } \
  NS_IMETHOD GetLoadFlags(nsLoadFlags *aLoadFlags) override { return _to GetLoadFlags(aLoadFlags); } \
  NS_IMETHOD SetLoadFlags(nsLoadFlags aLoadFlags) override { return _to SetLoadFlags(aLoadFlags); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIREQUEST(_to) \
  NS_IMETHOD GetName(nsACString & aName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD IsPending(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsPending(_retval); } \
  NS_IMETHOD GetStatus(nsresult *aStatus) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStatus(aStatus); } \
  NS_IMETHOD Cancel(nsresult aStatus) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Cancel(aStatus); } \
  NS_IMETHOD Suspend(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Suspend(); } \
  NS_IMETHOD Resume(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Resume(); } \
  NS_IMETHOD GetLoadGroup(nsILoadGroup * *aLoadGroup) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLoadGroup(aLoadGroup); } \
  NS_IMETHOD SetLoadGroup(nsILoadGroup *aLoadGroup) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLoadGroup(aLoadGroup); } \
  NS_IMETHOD GetLoadFlags(nsLoadFlags *aLoadFlags) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLoadFlags(aLoadFlags); } \
  NS_IMETHOD SetLoadFlags(nsLoadFlags aLoadFlags) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLoadFlags(aLoadFlags); } \


#endif /* __gen_nsIRequest_h__ */
