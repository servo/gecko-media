/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIConsoleMessage.idl
 */

#ifndef __gen_nsIConsoleMessage_h__
#define __gen_nsIConsoleMessage_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIConsoleMessage */
#define NS_ICONSOLEMESSAGE_IID_STR "3aba9617-10e2-4839-83ae-2e6fc4df428b"

#define NS_ICONSOLEMESSAGE_IID \
  {0x3aba9617, 0x10e2, 0x4839, \
    { 0x83, 0xae, 0x2e, 0x6f, 0xc4, 0xdf, 0x42, 0x8b }}

class NS_NO_VTABLE nsIConsoleMessage : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICONSOLEMESSAGE_IID)

  enum {
    debug = 0U,
    info = 1U,
    warn = 2U,
    error = 3U
  };

  /* readonly attribute uint32_t logLevel; */
  NS_IMETHOD GetLogLevel(uint32_t *aLogLevel) = 0;

  /* readonly attribute long long timeStamp; */
  NS_IMETHOD GetTimeStamp(int64_t *aTimeStamp) = 0;

  /* [binaryname(MessageMoz)] readonly attribute wstring message; */
  NS_IMETHOD GetMessageMoz(char16_t * *aMessage) = 0;

  /* AUTF8String toString (); */
  NS_IMETHOD ToString(nsACString & _retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIConsoleMessage, NS_ICONSOLEMESSAGE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICONSOLEMESSAGE \
  NS_IMETHOD GetLogLevel(uint32_t *aLogLevel) override; \
  NS_IMETHOD GetTimeStamp(int64_t *aTimeStamp) override; \
  NS_IMETHOD GetMessageMoz(char16_t * *aMessage) override; \
  NS_IMETHOD ToString(nsACString & _retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSICONSOLEMESSAGE \
  nsresult GetLogLevel(uint32_t *aLogLevel); \
  nsresult GetTimeStamp(int64_t *aTimeStamp); \
  nsresult GetMessageMoz(char16_t * *aMessage); \
  nsresult ToString(nsACString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICONSOLEMESSAGE(_to) \
  NS_IMETHOD GetLogLevel(uint32_t *aLogLevel) override { return _to GetLogLevel(aLogLevel); } \
  NS_IMETHOD GetTimeStamp(int64_t *aTimeStamp) override { return _to GetTimeStamp(aTimeStamp); } \
  NS_IMETHOD GetMessageMoz(char16_t * *aMessage) override { return _to GetMessageMoz(aMessage); } \
  NS_IMETHOD ToString(nsACString & _retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICONSOLEMESSAGE(_to) \
  NS_IMETHOD GetLogLevel(uint32_t *aLogLevel) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLogLevel(aLogLevel); } \
  NS_IMETHOD GetTimeStamp(int64_t *aTimeStamp) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTimeStamp(aTimeStamp); } \
  NS_IMETHOD GetMessageMoz(char16_t * *aMessage) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMessageMoz(aMessage); } \
  NS_IMETHOD ToString(nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 

#define NS_CONSOLEMESSAGE_CID \
{ 0x024efc9e, 0x54dc, 0x4844, { 0x80, 0x4b, 0x41, 0xd3, 0xf3, 0x69, 0x90, 0x73 }}

#endif /* __gen_nsIConsoleMessage_h__ */
