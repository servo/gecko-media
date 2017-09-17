/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIPrincipal.idl
 */

#ifndef __gen_nsIPrincipal_h__
#define __gen_nsIPrincipal_h__


#ifndef __gen_nsISerializable_h__
#include "nsISerializable.h"
#endif

#include "js/Value.h"

#include "mozilla/Assertions.h"
#include "mozilla/DebugOnly.h"

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
struct JSPrincipals;
#include "nsCOMPtr.h"
#include "nsTArray.h"
#include "mozilla/DebugOnly.h"
namespace mozilla {
class OriginAttributes;
}
/**
 * Some methods have a fast path for the case when we're comparing a principal
 * to itself. The situation may happen for example with about:blank documents.
 */
#define DECL_FAST_INLINE_HELPER(method_)                       \
  inline bool method_(nsIPrincipal* aOther)                    \
  {                                                            \
    mozilla::DebugOnly<bool> val = false;                      \
    MOZ_ASSERT_IF(this == aOther,                              \
                  NS_SUCCEEDED(method_(aOther, &val)) && val); \
                                                               \
    bool retVal = false;                                       \
    return                                                     \
      this == aOther ||                                        \
      (NS_SUCCEEDED(method_(aOther, &retVal)) && retVal);      \
  }
class nsIURI; /* forward declaration */

class nsIContentSecurityPolicy; /* forward declaration */

class nsIDOMDocument; /* forward declaration */


/* starting interface:    nsIPrincipal */
#define NS_IPRINCIPAL_IID_STR "f75f502d-79fd-48be-a079-e5a7b8f80c8b"

#define NS_IPRINCIPAL_IID \
  {0xf75f502d, 0x79fd, 0x48be, \
    { 0xa0, 0x79, 0xe5, 0xa7, 0xb8, 0xf8, 0x0c, 0x8b }}

class nsIPrincipal : public nsISerializable {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPRINCIPAL_IID)

  /* boolean equals (in nsIPrincipal other); */
  NS_IMETHOD Equals(nsIPrincipal *other, bool *_retval) = 0;

  /* boolean equalsConsideringDomain (in nsIPrincipal other); */
  NS_IMETHOD EqualsConsideringDomain(nsIPrincipal *other, bool *_retval) = 0;

       DECL_FAST_INLINE_HELPER(Equals)
      DECL_FAST_INLINE_HELPER(EqualsConsideringDomain)
      /* [noscript] readonly attribute unsigned long hashValue; */
  NS_IMETHOD GetHashValue(uint32_t *aHashValue) = 0;

  /* readonly attribute nsIURI URI; */
  NS_IMETHOD GetURI(nsIURI * *aURI) = 0;

  /* [noscript] attribute nsIURI domain; */
  NS_IMETHOD GetDomain(nsIURI * *aDomain) = 0;
  NS_IMETHOD SetDomain(nsIURI *aDomain) = 0;

  /* boolean subsumes (in nsIPrincipal other); */
  NS_IMETHOD Subsumes(nsIPrincipal *other, bool *_retval) = 0;

  /* boolean subsumesConsideringDomain (in nsIPrincipal other); */
  NS_IMETHOD SubsumesConsideringDomain(nsIPrincipal *other, bool *_retval) = 0;

  /* boolean subsumesConsideringDomainIgnoringFPD (in nsIPrincipal other); */
  NS_IMETHOD SubsumesConsideringDomainIgnoringFPD(nsIPrincipal *other, bool *_retval) = 0;

       DECL_FAST_INLINE_HELPER(Subsumes)
      DECL_FAST_INLINE_HELPER(SubsumesConsideringDomain)
      DECL_FAST_INLINE_HELPER(SubsumesConsideringDomainIgnoringFPD)
#undef DECL_FAST_INLINE_HELPER
      /* void checkMayLoad (in nsIURI uri, in boolean report, in boolean allowIfInheritsPrincipal); */
  NS_IMETHOD CheckMayLoad(nsIURI *uri, bool report, bool allowIfInheritsPrincipal) = 0;

  /* [noscript] attribute nsIContentSecurityPolicy csp; */
  NS_IMETHOD GetCsp(nsIContentSecurityPolicy * *aCsp) = 0;
  NS_IMETHOD SetCsp(nsIContentSecurityPolicy *aCsp) = 0;

  /* [noscript] nsIContentSecurityPolicy ensureCSP (in nsIDOMDocument aDocument); */
  NS_IMETHOD EnsureCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) = 0;

  /* [noscript] readonly attribute nsIContentSecurityPolicy preloadCsp; */
  NS_IMETHOD GetPreloadCsp(nsIContentSecurityPolicy * *aPreloadCsp) = 0;

  /* [noscript] nsIContentSecurityPolicy ensurePreloadCSP (in nsIDOMDocument aDocument); */
  NS_IMETHOD EnsurePreloadCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) = 0;

  /* readonly attribute AString cspJSON; */
  NS_IMETHOD GetCspJSON(nsAString & aCspJSON) = 0;

  /* [implicit_jscontext] readonly attribute jsval originAttributes; */
  NS_IMETHOD GetOriginAttributes(JSContext* cx, JS::MutableHandleValue aOriginAttributes) = 0;

  /* [binaryname(OriginAttributesRef),noscript,nostdcall,notxpcom] const_OriginAttributes OriginAttributesRef (); */
  virtual const mozilla::OriginAttributes & OriginAttributesRef(void) = 0;

  /* readonly attribute ACString origin; */
  NS_IMETHOD GetOrigin(nsACString & aOrigin) = 0;

  /* readonly attribute ACString originNoSuffix; */
  NS_IMETHOD GetOriginNoSuffix(nsACString & aOriginNoSuffix) = 0;

  /* readonly attribute AUTF8String originSuffix; */
  NS_IMETHOD GetOriginSuffix(nsACString & aOriginSuffix) = 0;

  /* readonly attribute ACString baseDomain; */
  NS_IMETHOD GetBaseDomain(nsACString & aBaseDomain) = 0;

  /* [infallible] readonly attribute unsigned long appId; */
  NS_IMETHOD GetAppId(uint32_t *aAppId) = 0;
  inline uint32_t GetAppId()
  {
    uint32_t result;
    mozilla::DebugOnly<nsresult> rv = GetAppId(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* readonly attribute AString addonId; */
  NS_IMETHOD GetAddonId(nsAString & aAddonId) = 0;

  /* readonly attribute nsISupports addonPolicy; */
  NS_IMETHOD GetAddonPolicy(nsISupports * *aAddonPolicy) = 0;

  /* [infallible] readonly attribute unsigned long userContextId; */
  NS_IMETHOD GetUserContextId(uint32_t *aUserContextId) = 0;
  inline uint32_t GetUserContextId()
  {
    uint32_t result;
    mozilla::DebugOnly<nsresult> rv = GetUserContextId(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute unsigned long privateBrowsingId; */
  NS_IMETHOD GetPrivateBrowsingId(uint32_t *aPrivateBrowsingId) = 0;
  inline uint32_t GetPrivateBrowsingId()
  {
    uint32_t result;
    mozilla::DebugOnly<nsresult> rv = GetPrivateBrowsingId(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isInIsolatedMozBrowserElement; */
  NS_IMETHOD GetIsInIsolatedMozBrowserElement(bool *aIsInIsolatedMozBrowserElement) = 0;
  inline bool GetIsInIsolatedMozBrowserElement()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsInIsolatedMozBrowserElement(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isNullPrincipal; */
  NS_IMETHOD GetIsNullPrincipal(bool *aIsNullPrincipal) = 0;
  inline bool GetIsNullPrincipal()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsNullPrincipal(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isCodebasePrincipal; */
  NS_IMETHOD GetIsCodebasePrincipal(bool *aIsCodebasePrincipal) = 0;
  inline bool GetIsCodebasePrincipal()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsCodebasePrincipal(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isExpandedPrincipal; */
  NS_IMETHOD GetIsExpandedPrincipal(bool *aIsExpandedPrincipal) = 0;
  inline bool GetIsExpandedPrincipal()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsExpandedPrincipal(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isSystemPrincipal; */
  NS_IMETHOD GetIsSystemPrincipal(bool *aIsSystemPrincipal) = 0;
  inline bool GetIsSystemPrincipal()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsSystemPrincipal(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPrincipal, NS_IPRINCIPAL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPRINCIPAL \
  NS_IMETHOD Equals(nsIPrincipal *other, bool *_retval) override; \
  NS_IMETHOD EqualsConsideringDomain(nsIPrincipal *other, bool *_retval) override; \
  NS_IMETHOD GetHashValue(uint32_t *aHashValue) override; \
  NS_IMETHOD GetURI(nsIURI * *aURI) override; \
  NS_IMETHOD GetDomain(nsIURI * *aDomain) override; \
  NS_IMETHOD SetDomain(nsIURI *aDomain) override; \
  NS_IMETHOD Subsumes(nsIPrincipal *other, bool *_retval) override; \
  NS_IMETHOD SubsumesConsideringDomain(nsIPrincipal *other, bool *_retval) override; \
  NS_IMETHOD SubsumesConsideringDomainIgnoringFPD(nsIPrincipal *other, bool *_retval) override; \
  NS_IMETHOD CheckMayLoad(nsIURI *uri, bool report, bool allowIfInheritsPrincipal) override; \
  NS_IMETHOD GetCsp(nsIContentSecurityPolicy * *aCsp) override; \
  NS_IMETHOD SetCsp(nsIContentSecurityPolicy *aCsp) override; \
  NS_IMETHOD EnsureCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) override; \
  NS_IMETHOD GetPreloadCsp(nsIContentSecurityPolicy * *aPreloadCsp) override; \
  NS_IMETHOD EnsurePreloadCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) override; \
  NS_IMETHOD GetCspJSON(nsAString & aCspJSON) override; \
  NS_IMETHOD GetOriginAttributes(JSContext* cx, JS::MutableHandleValue aOriginAttributes) override; \
  virtual const mozilla::OriginAttributes & OriginAttributesRef(void) override; \
  NS_IMETHOD GetOrigin(nsACString & aOrigin) override; \
  NS_IMETHOD GetOriginNoSuffix(nsACString & aOriginNoSuffix) override; \
  NS_IMETHOD GetOriginSuffix(nsACString & aOriginSuffix) override; \
  NS_IMETHOD GetBaseDomain(nsACString & aBaseDomain) override; \
  using nsIPrincipal::GetAppId; \
  NS_IMETHOD GetAppId(uint32_t *aAppId) override; \
  NS_IMETHOD GetAddonId(nsAString & aAddonId) override; \
  NS_IMETHOD GetAddonPolicy(nsISupports * *aAddonPolicy) override; \
  using nsIPrincipal::GetUserContextId; \
  NS_IMETHOD GetUserContextId(uint32_t *aUserContextId) override; \
  using nsIPrincipal::GetPrivateBrowsingId; \
  NS_IMETHOD GetPrivateBrowsingId(uint32_t *aPrivateBrowsingId) override; \
  using nsIPrincipal::GetIsInIsolatedMozBrowserElement; \
  NS_IMETHOD GetIsInIsolatedMozBrowserElement(bool *aIsInIsolatedMozBrowserElement) override; \
  using nsIPrincipal::GetIsNullPrincipal; \
  NS_IMETHOD GetIsNullPrincipal(bool *aIsNullPrincipal) override; \
  using nsIPrincipal::GetIsCodebasePrincipal; \
  NS_IMETHOD GetIsCodebasePrincipal(bool *aIsCodebasePrincipal) override; \
  using nsIPrincipal::GetIsExpandedPrincipal; \
  NS_IMETHOD GetIsExpandedPrincipal(bool *aIsExpandedPrincipal) override; \
  using nsIPrincipal::GetIsSystemPrincipal; \
  NS_IMETHOD GetIsSystemPrincipal(bool *aIsSystemPrincipal) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIPRINCIPAL \
  nsresult Equals(nsIPrincipal *other, bool *_retval); \
  nsresult EqualsConsideringDomain(nsIPrincipal *other, bool *_retval); \
  nsresult GetHashValue(uint32_t *aHashValue); \
  nsresult GetURI(nsIURI * *aURI); \
  nsresult GetDomain(nsIURI * *aDomain); \
  nsresult SetDomain(nsIURI *aDomain); \
  nsresult Subsumes(nsIPrincipal *other, bool *_retval); \
  nsresult SubsumesConsideringDomain(nsIPrincipal *other, bool *_retval); \
  nsresult SubsumesConsideringDomainIgnoringFPD(nsIPrincipal *other, bool *_retval); \
  nsresult CheckMayLoad(nsIURI *uri, bool report, bool allowIfInheritsPrincipal); \
  nsresult GetCsp(nsIContentSecurityPolicy * *aCsp); \
  nsresult SetCsp(nsIContentSecurityPolicy *aCsp); \
  nsresult EnsureCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval); \
  nsresult GetPreloadCsp(nsIContentSecurityPolicy * *aPreloadCsp); \
  nsresult EnsurePreloadCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval); \
  nsresult GetCspJSON(nsAString & aCspJSON); \
  nsresult GetOriginAttributes(JSContext* cx, JS::MutableHandleValue aOriginAttributes); \
  const mozilla::OriginAttributes & OriginAttributesRef(void); \
  nsresult GetOrigin(nsACString & aOrigin); \
  nsresult GetOriginNoSuffix(nsACString & aOriginNoSuffix); \
  nsresult GetOriginSuffix(nsACString & aOriginSuffix); \
  nsresult GetBaseDomain(nsACString & aBaseDomain); \
  using nsIPrincipal::GetAppId; \
  nsresult GetAppId(uint32_t *aAppId); \
  nsresult GetAddonId(nsAString & aAddonId); \
  nsresult GetAddonPolicy(nsISupports * *aAddonPolicy); \
  using nsIPrincipal::GetUserContextId; \
  nsresult GetUserContextId(uint32_t *aUserContextId); \
  using nsIPrincipal::GetPrivateBrowsingId; \
  nsresult GetPrivateBrowsingId(uint32_t *aPrivateBrowsingId); \
  using nsIPrincipal::GetIsInIsolatedMozBrowserElement; \
  nsresult GetIsInIsolatedMozBrowserElement(bool *aIsInIsolatedMozBrowserElement); \
  using nsIPrincipal::GetIsNullPrincipal; \
  nsresult GetIsNullPrincipal(bool *aIsNullPrincipal); \
  using nsIPrincipal::GetIsCodebasePrincipal; \
  nsresult GetIsCodebasePrincipal(bool *aIsCodebasePrincipal); \
  using nsIPrincipal::GetIsExpandedPrincipal; \
  nsresult GetIsExpandedPrincipal(bool *aIsExpandedPrincipal); \
  using nsIPrincipal::GetIsSystemPrincipal; \
  nsresult GetIsSystemPrincipal(bool *aIsSystemPrincipal); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPRINCIPAL(_to) \
  NS_IMETHOD Equals(nsIPrincipal *other, bool *_retval) override { return _to Equals(other, _retval); } \
  NS_IMETHOD EqualsConsideringDomain(nsIPrincipal *other, bool *_retval) override { return _to EqualsConsideringDomain(other, _retval); } \
  NS_IMETHOD GetHashValue(uint32_t *aHashValue) override { return _to GetHashValue(aHashValue); } \
  NS_IMETHOD GetURI(nsIURI * *aURI) override { return _to GetURI(aURI); } \
  NS_IMETHOD GetDomain(nsIURI * *aDomain) override { return _to GetDomain(aDomain); } \
  NS_IMETHOD SetDomain(nsIURI *aDomain) override { return _to SetDomain(aDomain); } \
  NS_IMETHOD Subsumes(nsIPrincipal *other, bool *_retval) override { return _to Subsumes(other, _retval); } \
  NS_IMETHOD SubsumesConsideringDomain(nsIPrincipal *other, bool *_retval) override { return _to SubsumesConsideringDomain(other, _retval); } \
  NS_IMETHOD SubsumesConsideringDomainIgnoringFPD(nsIPrincipal *other, bool *_retval) override { return _to SubsumesConsideringDomainIgnoringFPD(other, _retval); } \
  NS_IMETHOD CheckMayLoad(nsIURI *uri, bool report, bool allowIfInheritsPrincipal) override { return _to CheckMayLoad(uri, report, allowIfInheritsPrincipal); } \
  NS_IMETHOD GetCsp(nsIContentSecurityPolicy * *aCsp) override { return _to GetCsp(aCsp); } \
  NS_IMETHOD SetCsp(nsIContentSecurityPolicy *aCsp) override { return _to SetCsp(aCsp); } \
  NS_IMETHOD EnsureCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) override { return _to EnsureCSP(aDocument, _retval); } \
  NS_IMETHOD GetPreloadCsp(nsIContentSecurityPolicy * *aPreloadCsp) override { return _to GetPreloadCsp(aPreloadCsp); } \
  NS_IMETHOD EnsurePreloadCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) override { return _to EnsurePreloadCSP(aDocument, _retval); } \
  NS_IMETHOD GetCspJSON(nsAString & aCspJSON) override { return _to GetCspJSON(aCspJSON); } \
  NS_IMETHOD GetOriginAttributes(JSContext* cx, JS::MutableHandleValue aOriginAttributes) override { return _to GetOriginAttributes(cx, aOriginAttributes); } \
  virtual const mozilla::OriginAttributes & OriginAttributesRef(void) override { return _to OriginAttributesRef(); } \
  NS_IMETHOD GetOrigin(nsACString & aOrigin) override { return _to GetOrigin(aOrigin); } \
  NS_IMETHOD GetOriginNoSuffix(nsACString & aOriginNoSuffix) override { return _to GetOriginNoSuffix(aOriginNoSuffix); } \
  NS_IMETHOD GetOriginSuffix(nsACString & aOriginSuffix) override { return _to GetOriginSuffix(aOriginSuffix); } \
  NS_IMETHOD GetBaseDomain(nsACString & aBaseDomain) override { return _to GetBaseDomain(aBaseDomain); } \
  using nsIPrincipal::GetAppId; \
  NS_IMETHOD GetAppId(uint32_t *aAppId) override { return _to GetAppId(aAppId); } \
  NS_IMETHOD GetAddonId(nsAString & aAddonId) override { return _to GetAddonId(aAddonId); } \
  NS_IMETHOD GetAddonPolicy(nsISupports * *aAddonPolicy) override { return _to GetAddonPolicy(aAddonPolicy); } \
  using nsIPrincipal::GetUserContextId; \
  NS_IMETHOD GetUserContextId(uint32_t *aUserContextId) override { return _to GetUserContextId(aUserContextId); } \
  using nsIPrincipal::GetPrivateBrowsingId; \
  NS_IMETHOD GetPrivateBrowsingId(uint32_t *aPrivateBrowsingId) override { return _to GetPrivateBrowsingId(aPrivateBrowsingId); } \
  using nsIPrincipal::GetIsInIsolatedMozBrowserElement; \
  NS_IMETHOD GetIsInIsolatedMozBrowserElement(bool *aIsInIsolatedMozBrowserElement) override { return _to GetIsInIsolatedMozBrowserElement(aIsInIsolatedMozBrowserElement); } \
  using nsIPrincipal::GetIsNullPrincipal; \
  NS_IMETHOD GetIsNullPrincipal(bool *aIsNullPrincipal) override { return _to GetIsNullPrincipal(aIsNullPrincipal); } \
  using nsIPrincipal::GetIsCodebasePrincipal; \
  NS_IMETHOD GetIsCodebasePrincipal(bool *aIsCodebasePrincipal) override { return _to GetIsCodebasePrincipal(aIsCodebasePrincipal); } \
  using nsIPrincipal::GetIsExpandedPrincipal; \
  NS_IMETHOD GetIsExpandedPrincipal(bool *aIsExpandedPrincipal) override { return _to GetIsExpandedPrincipal(aIsExpandedPrincipal); } \
  using nsIPrincipal::GetIsSystemPrincipal; \
  NS_IMETHOD GetIsSystemPrincipal(bool *aIsSystemPrincipal) override { return _to GetIsSystemPrincipal(aIsSystemPrincipal); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPRINCIPAL(_to) \
  NS_IMETHOD Equals(nsIPrincipal *other, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Equals(other, _retval); } \
  NS_IMETHOD EqualsConsideringDomain(nsIPrincipal *other, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EqualsConsideringDomain(other, _retval); } \
  NS_IMETHOD GetHashValue(uint32_t *aHashValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHashValue(aHashValue); } \
  NS_IMETHOD GetURI(nsIURI * *aURI) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetURI(aURI); } \
  NS_IMETHOD GetDomain(nsIURI * *aDomain) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDomain(aDomain); } \
  NS_IMETHOD SetDomain(nsIURI *aDomain) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDomain(aDomain); } \
  NS_IMETHOD Subsumes(nsIPrincipal *other, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Subsumes(other, _retval); } \
  NS_IMETHOD SubsumesConsideringDomain(nsIPrincipal *other, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SubsumesConsideringDomain(other, _retval); } \
  NS_IMETHOD SubsumesConsideringDomainIgnoringFPD(nsIPrincipal *other, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SubsumesConsideringDomainIgnoringFPD(other, _retval); } \
  NS_IMETHOD CheckMayLoad(nsIURI *uri, bool report, bool allowIfInheritsPrincipal) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CheckMayLoad(uri, report, allowIfInheritsPrincipal); } \
  NS_IMETHOD GetCsp(nsIContentSecurityPolicy * *aCsp) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCsp(aCsp); } \
  NS_IMETHOD SetCsp(nsIContentSecurityPolicy *aCsp) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCsp(aCsp); } \
  NS_IMETHOD EnsureCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EnsureCSP(aDocument, _retval); } \
  NS_IMETHOD GetPreloadCsp(nsIContentSecurityPolicy * *aPreloadCsp) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPreloadCsp(aPreloadCsp); } \
  NS_IMETHOD EnsurePreloadCSP(nsIDOMDocument *aDocument, nsIContentSecurityPolicy * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EnsurePreloadCSP(aDocument, _retval); } \
  NS_IMETHOD GetCspJSON(nsAString & aCspJSON) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCspJSON(aCspJSON); } \
  NS_IMETHOD GetOriginAttributes(JSContext* cx, JS::MutableHandleValue aOriginAttributes) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOriginAttributes(cx, aOriginAttributes); } \
  virtual const mozilla::OriginAttributes & OriginAttributesRef(void) override; \
  NS_IMETHOD GetOrigin(nsACString & aOrigin) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOrigin(aOrigin); } \
  NS_IMETHOD GetOriginNoSuffix(nsACString & aOriginNoSuffix) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOriginNoSuffix(aOriginNoSuffix); } \
  NS_IMETHOD GetOriginSuffix(nsACString & aOriginSuffix) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOriginSuffix(aOriginSuffix); } \
  NS_IMETHOD GetBaseDomain(nsACString & aBaseDomain) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBaseDomain(aBaseDomain); } \
  NS_IMETHOD GetAppId(uint32_t *aAppId) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAppId(aAppId); } \
  NS_IMETHOD GetAddonId(nsAString & aAddonId) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAddonId(aAddonId); } \
  NS_IMETHOD GetAddonPolicy(nsISupports * *aAddonPolicy) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAddonPolicy(aAddonPolicy); } \
  NS_IMETHOD GetUserContextId(uint32_t *aUserContextId) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUserContextId(aUserContextId); } \
  NS_IMETHOD GetPrivateBrowsingId(uint32_t *aPrivateBrowsingId) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrivateBrowsingId(aPrivateBrowsingId); } \
  NS_IMETHOD GetIsInIsolatedMozBrowserElement(bool *aIsInIsolatedMozBrowserElement) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsInIsolatedMozBrowserElement(aIsInIsolatedMozBrowserElement); } \
  NS_IMETHOD GetIsNullPrincipal(bool *aIsNullPrincipal) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsNullPrincipal(aIsNullPrincipal); } \
  NS_IMETHOD GetIsCodebasePrincipal(bool *aIsCodebasePrincipal) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsCodebasePrincipal(aIsCodebasePrincipal); } \
  NS_IMETHOD GetIsExpandedPrincipal(bool *aIsExpandedPrincipal) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsExpandedPrincipal(aIsExpandedPrincipal); } \
  NS_IMETHOD GetIsSystemPrincipal(bool *aIsSystemPrincipal) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsSystemPrincipal(aIsSystemPrincipal); } 


/* starting interface:    nsIExpandedPrincipal */
#define NS_IEXPANDEDPRINCIPAL_IID_STR "f3e177df-6a5e-489f-80a7-2dd1481471d8"

#define NS_IEXPANDEDPRINCIPAL_IID \
  {0xf3e177df, 0x6a5e, 0x489f, \
    { 0x80, 0xa7, 0x2d, 0xd1, 0x48, 0x14, 0x71, 0xd8 }}

class NS_NO_VTABLE nsIExpandedPrincipal : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IEXPANDEDPRINCIPAL_IID)

  /* [noscript] readonly attribute PrincipalArray whiteList; */
  NS_IMETHOD GetWhiteList(nsTArray<nsCOMPtr<nsIPrincipal> > **aWhiteList) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIExpandedPrincipal, NS_IEXPANDEDPRINCIPAL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEXPANDEDPRINCIPAL \
  NS_IMETHOD GetWhiteList(nsTArray<nsCOMPtr<nsIPrincipal> > **aWhiteList) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIEXPANDEDPRINCIPAL \
  nsresult GetWhiteList(nsTArray<nsCOMPtr<nsIPrincipal> > **aWhiteList); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEXPANDEDPRINCIPAL(_to) \
  NS_IMETHOD GetWhiteList(nsTArray<nsCOMPtr<nsIPrincipal> > **aWhiteList) override { return _to GetWhiteList(aWhiteList); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEXPANDEDPRINCIPAL(_to) \
  NS_IMETHOD GetWhiteList(nsTArray<nsCOMPtr<nsIPrincipal> > **aWhiteList) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWhiteList(aWhiteList); } 


#endif /* __gen_nsIPrincipal_h__ */
