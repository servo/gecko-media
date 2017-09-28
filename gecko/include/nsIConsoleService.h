/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIConsoleService.idl
 */

#ifndef __gen_nsIConsoleService_h__
#define __gen_nsIConsoleService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIConsoleListener; /* forward declaration */

class nsIConsoleMessage; /* forward declaration */


/* starting interface:    nsIConsoleService */
#define NS_ICONSOLESERVICE_IID_STR "0eb81d20-c37e-42d4-82a8-ca9ae96bdf52"

#define NS_ICONSOLESERVICE_IID \
  {0x0eb81d20, 0xc37e, 0x42d4, \
    { 0x82, 0xa8, 0xca, 0x9a, 0xe9, 0x6b, 0xdf, 0x52 }}

class NS_NO_VTABLE nsIConsoleService : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ICONSOLESERVICE_IID)

  /* void logMessage (in nsIConsoleMessage message); */
  NS_IMETHOD LogMessage(nsIConsoleMessage *message) = 0;

  /* void logStringMessage (in wstring message); */
  NS_IMETHOD LogStringMessage(const char16_t * message) = 0;

  /* void getMessageArray ([optional] out uint32_t count, [array, size_is (count), retval] out nsIConsoleMessage messages); */
  NS_IMETHOD GetMessageArray(uint32_t *count, nsIConsoleMessage * **messages) = 0;

  /* void registerListener (in nsIConsoleListener listener); */
  NS_IMETHOD RegisterListener(nsIConsoleListener *listener) = 0;

  /* void unregisterListener (in nsIConsoleListener listener); */
  NS_IMETHOD UnregisterListener(nsIConsoleListener *listener) = 0;

  /* void reset (); */
  NS_IMETHOD Reset(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIConsoleService, NS_ICONSOLESERVICE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICONSOLESERVICE \
  NS_IMETHOD LogMessage(nsIConsoleMessage *message) override; \
  NS_IMETHOD LogStringMessage(const char16_t * message) override; \
  NS_IMETHOD GetMessageArray(uint32_t *count, nsIConsoleMessage * **messages) override; \
  NS_IMETHOD RegisterListener(nsIConsoleListener *listener) override; \
  NS_IMETHOD UnregisterListener(nsIConsoleListener *listener) override; \
  NS_IMETHOD Reset(void) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSICONSOLESERVICE \
  nsresult LogMessage(nsIConsoleMessage *message); \
  nsresult LogStringMessage(const char16_t * message); \
  nsresult GetMessageArray(uint32_t *count, nsIConsoleMessage * **messages); \
  nsresult RegisterListener(nsIConsoleListener *listener); \
  nsresult UnregisterListener(nsIConsoleListener *listener); \
  nsresult Reset(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICONSOLESERVICE(_to) \
  NS_IMETHOD LogMessage(nsIConsoleMessage *message) override { return _to LogMessage(message); } \
  NS_IMETHOD LogStringMessage(const char16_t * message) override { return _to LogStringMessage(message); } \
  NS_IMETHOD GetMessageArray(uint32_t *count, nsIConsoleMessage * **messages) override { return _to GetMessageArray(count, messages); } \
  NS_IMETHOD RegisterListener(nsIConsoleListener *listener) override { return _to RegisterListener(listener); } \
  NS_IMETHOD UnregisterListener(nsIConsoleListener *listener) override { return _to UnregisterListener(listener); } \
  NS_IMETHOD Reset(void) override { return _to Reset(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICONSOLESERVICE(_to) \
  NS_IMETHOD LogMessage(nsIConsoleMessage *message) override { return !_to ? NS_ERROR_NULL_POINTER : _to->LogMessage(message); } \
  NS_IMETHOD LogStringMessage(const char16_t * message) override { return !_to ? NS_ERROR_NULL_POINTER : _to->LogStringMessage(message); } \
  NS_IMETHOD GetMessageArray(uint32_t *count, nsIConsoleMessage * **messages) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMessageArray(count, messages); } \
  NS_IMETHOD RegisterListener(nsIConsoleListener *listener) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterListener(listener); } \
  NS_IMETHOD UnregisterListener(nsIConsoleListener *listener) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterListener(listener); } \
  NS_IMETHOD Reset(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Reset(); } 

#define NS_CONSOLESERVICE_CID \
{ 0x7e3ff85c, 0x1dd2, 0x11b2, { 0x8d, 0x4b, 0xeb, 0x45, 0x2c, 0xb0, 0xff, 0x40 }}
#define NS_CONSOLESERVICE_CONTRACTID "@mozilla.org/consoleservice;1"

#endif /* __gen_nsIConsoleService_h__ */
