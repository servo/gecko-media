/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsINetUtil.idl
 */

#ifndef __gen_nsINetUtil_h__
#define __gen_nsINetUtil_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIURI; /* forward declaration */

class nsIPrefBranch; /* forward declaration */


/* starting interface:    nsINetUtil */
#define NS_INETUTIL_IID_STR "fe2625ec-b884-4df1-b39c-9e830e47aa94"

#define NS_INETUTIL_IID \
  {0xfe2625ec, 0xb884, 0x4df1, \
    { 0xb3, 0x9c, 0x9e, 0x83, 0x0e, 0x47, 0xaa, 0x94 }}

class NS_NO_VTABLE nsINetUtil : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_INETUTIL_IID)

  /* AUTF8String parseRequestContentType (in AUTF8String aTypeHeader, out AUTF8String aCharset, out boolean aHadCharset); */
  NS_IMETHOD ParseRequestContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) = 0;

  /* AUTF8String parseResponseContentType (in AUTF8String aTypeHeader, out AUTF8String aCharset, out boolean aHadCharset); */
  NS_IMETHOD ParseResponseContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) = 0;

  /* boolean protocolHasFlags (in nsIURI aURI, in unsigned long aFlag); */
  NS_IMETHOD ProtocolHasFlags(nsIURI *aURI, uint32_t aFlag, bool *_retval) = 0;

  /* boolean URIChainHasFlags (in nsIURI aURI, in unsigned long aFlags); */
  NS_IMETHOD URIChainHasFlags(nsIURI *aURI, uint32_t aFlags, bool *_retval) = 0;

  /* nsIURI toImmutableURI (in nsIURI aURI); */
  NS_IMETHOD ToImmutableURI(nsIURI *aURI, nsIURI * *_retval) = 0;

  /* nsIURI newSimpleNestedURI (in nsIURI aURI); */
  NS_IMETHOD NewSimpleNestedURI(nsIURI *aURI, nsIURI * *_retval) = 0;

  enum {
    ESCAPE_ALL = 0U,
    ESCAPE_XALPHAS = 1U,
    ESCAPE_XPALPHAS = 2U,
    ESCAPE_URL_PATH = 4U
  };

  /* ACString escapeString (in ACString aString, in unsigned long aEscapeType); */
  NS_IMETHOD EscapeString(const nsACString & aString, uint32_t aEscapeType, nsACString & _retval) = 0;

  enum {
    ESCAPE_URL_SCHEME = 1U,
    ESCAPE_URL_USERNAME = 2U,
    ESCAPE_URL_PASSWORD = 4U,
    ESCAPE_URL_HOST = 8U,
    ESCAPE_URL_DIRECTORY = 16U,
    ESCAPE_URL_FILE_BASENAME = 32U,
    ESCAPE_URL_FILE_EXTENSION = 64U,
    ESCAPE_URL_PARAM = 128U,
    ESCAPE_URL_QUERY = 256U,
    ESCAPE_URL_REF = 512U,
    ESCAPE_URL_FILEPATH = 112U,
    ESCAPE_URL_MINIMAL = 1023U,
    ESCAPE_URL_FORCED = 1024U,
    ESCAPE_URL_ONLY_ASCII = 2048U,
    ESCAPE_URL_ONLY_NONASCII = 4096U,
    ESCAPE_URL_COLON = 16384U,
    ESCAPE_URL_SKIP_CONTROL = 32768U
  };

  /* ACString escapeURL (in ACString aStr, in unsigned long aFlags); */
  NS_IMETHOD EscapeURL(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) = 0;

  /* ACString unescapeString (in AUTF8String aStr, in unsigned long aFlags); */
  NS_IMETHOD UnescapeString(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) = 0;

  /* boolean extractCharsetFromContentType (in AUTF8String aTypeHeader, out AUTF8String aCharset, out long aCharsetStart, out long aCharsetEnd); */
  NS_IMETHOD ExtractCharsetFromContentType(const nsACString & aTypeHeader, nsACString & aCharset, int32_t *aCharsetStart, int32_t *aCharsetEnd, bool *_retval) = 0;

  /* unsigned long parseAttributePolicyString (in AString aPolicyString); */
  NS_IMETHOD ParseAttributePolicyString(const nsAString & aPolicyString, uint32_t *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsINetUtil, NS_INETUTIL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSINETUTIL \
  NS_IMETHOD ParseRequestContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) override; \
  NS_IMETHOD ParseResponseContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) override; \
  NS_IMETHOD ProtocolHasFlags(nsIURI *aURI, uint32_t aFlag, bool *_retval) override; \
  NS_IMETHOD URIChainHasFlags(nsIURI *aURI, uint32_t aFlags, bool *_retval) override; \
  NS_IMETHOD ToImmutableURI(nsIURI *aURI, nsIURI * *_retval) override; \
  NS_IMETHOD NewSimpleNestedURI(nsIURI *aURI, nsIURI * *_retval) override; \
  NS_IMETHOD EscapeString(const nsACString & aString, uint32_t aEscapeType, nsACString & _retval) override; \
  NS_IMETHOD EscapeURL(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) override; \
  NS_IMETHOD UnescapeString(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) override; \
  NS_IMETHOD ExtractCharsetFromContentType(const nsACString & aTypeHeader, nsACString & aCharset, int32_t *aCharsetStart, int32_t *aCharsetEnd, bool *_retval) override; \
  NS_IMETHOD ParseAttributePolicyString(const nsAString & aPolicyString, uint32_t *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSINETUTIL \
  nsresult ParseRequestContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval); \
  nsresult ParseResponseContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval); \
  nsresult ProtocolHasFlags(nsIURI *aURI, uint32_t aFlag, bool *_retval); \
  nsresult URIChainHasFlags(nsIURI *aURI, uint32_t aFlags, bool *_retval); \
  nsresult ToImmutableURI(nsIURI *aURI, nsIURI * *_retval); \
  nsresult NewSimpleNestedURI(nsIURI *aURI, nsIURI * *_retval); \
  nsresult EscapeString(const nsACString & aString, uint32_t aEscapeType, nsACString & _retval); \
  nsresult EscapeURL(const nsACString & aStr, uint32_t aFlags, nsACString & _retval); \
  nsresult UnescapeString(const nsACString & aStr, uint32_t aFlags, nsACString & _retval); \
  nsresult ExtractCharsetFromContentType(const nsACString & aTypeHeader, nsACString & aCharset, int32_t *aCharsetStart, int32_t *aCharsetEnd, bool *_retval); \
  nsresult ParseAttributePolicyString(const nsAString & aPolicyString, uint32_t *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSINETUTIL(_to) \
  NS_IMETHOD ParseRequestContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) override { return _to ParseRequestContentType(aTypeHeader, aCharset, aHadCharset, _retval); } \
  NS_IMETHOD ParseResponseContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) override { return _to ParseResponseContentType(aTypeHeader, aCharset, aHadCharset, _retval); } \
  NS_IMETHOD ProtocolHasFlags(nsIURI *aURI, uint32_t aFlag, bool *_retval) override { return _to ProtocolHasFlags(aURI, aFlag, _retval); } \
  NS_IMETHOD URIChainHasFlags(nsIURI *aURI, uint32_t aFlags, bool *_retval) override { return _to URIChainHasFlags(aURI, aFlags, _retval); } \
  NS_IMETHOD ToImmutableURI(nsIURI *aURI, nsIURI * *_retval) override { return _to ToImmutableURI(aURI, _retval); } \
  NS_IMETHOD NewSimpleNestedURI(nsIURI *aURI, nsIURI * *_retval) override { return _to NewSimpleNestedURI(aURI, _retval); } \
  NS_IMETHOD EscapeString(const nsACString & aString, uint32_t aEscapeType, nsACString & _retval) override { return _to EscapeString(aString, aEscapeType, _retval); } \
  NS_IMETHOD EscapeURL(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) override { return _to EscapeURL(aStr, aFlags, _retval); } \
  NS_IMETHOD UnescapeString(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) override { return _to UnescapeString(aStr, aFlags, _retval); } \
  NS_IMETHOD ExtractCharsetFromContentType(const nsACString & aTypeHeader, nsACString & aCharset, int32_t *aCharsetStart, int32_t *aCharsetEnd, bool *_retval) override { return _to ExtractCharsetFromContentType(aTypeHeader, aCharset, aCharsetStart, aCharsetEnd, _retval); } \
  NS_IMETHOD ParseAttributePolicyString(const nsAString & aPolicyString, uint32_t *_retval) override { return _to ParseAttributePolicyString(aPolicyString, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSINETUTIL(_to) \
  NS_IMETHOD ParseRequestContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ParseRequestContentType(aTypeHeader, aCharset, aHadCharset, _retval); } \
  NS_IMETHOD ParseResponseContentType(const nsACString & aTypeHeader, nsACString & aCharset, bool *aHadCharset, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ParseResponseContentType(aTypeHeader, aCharset, aHadCharset, _retval); } \
  NS_IMETHOD ProtocolHasFlags(nsIURI *aURI, uint32_t aFlag, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ProtocolHasFlags(aURI, aFlag, _retval); } \
  NS_IMETHOD URIChainHasFlags(nsIURI *aURI, uint32_t aFlags, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->URIChainHasFlags(aURI, aFlags, _retval); } \
  NS_IMETHOD ToImmutableURI(nsIURI *aURI, nsIURI * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToImmutableURI(aURI, _retval); } \
  NS_IMETHOD NewSimpleNestedURI(nsIURI *aURI, nsIURI * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->NewSimpleNestedURI(aURI, _retval); } \
  NS_IMETHOD EscapeString(const nsACString & aString, uint32_t aEscapeType, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EscapeString(aString, aEscapeType, _retval); } \
  NS_IMETHOD EscapeURL(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EscapeURL(aStr, aFlags, _retval); } \
  NS_IMETHOD UnescapeString(const nsACString & aStr, uint32_t aFlags, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnescapeString(aStr, aFlags, _retval); } \
  NS_IMETHOD ExtractCharsetFromContentType(const nsACString & aTypeHeader, nsACString & aCharset, int32_t *aCharsetStart, int32_t *aCharsetEnd, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ExtractCharsetFromContentType(aTypeHeader, aCharset, aCharsetStart, aCharsetEnd, _retval); } \
  NS_IMETHOD ParseAttributePolicyString(const nsAString & aPolicyString, uint32_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ParseAttributePolicyString(aPolicyString, _retval); } 


#endif /* __gen_nsINetUtil_h__ */
