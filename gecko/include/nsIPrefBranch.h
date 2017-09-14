/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIPrefBranch.idl
 */

#ifndef __gen_nsIPrefBranch_h__
#define __gen_nsIPrefBranch_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIObserver; /* forward declaration */


/* starting interface:    nsIPrefBranch */
#define NS_IPREFBRANCH_IID_STR "55d25e49-793f-4727-a69f-de8b15f4b985"

#define NS_IPREFBRANCH_IID \
  {0x55d25e49, 0x793f, 0x4727, \
    { 0xa6, 0x9f, 0xde, 0x8b, 0x15, 0xf4, 0xb9, 0x85 }}

class NS_NO_VTABLE nsIPrefBranch : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPREFBRANCH_IID)

  enum {
    PREF_INVALID = 0,
    PREF_STRING = 32,
    PREF_INT = 64,
    PREF_BOOL = 128
  };

  /* readonly attribute string root; */
  NS_IMETHOD GetRoot(char * *aRoot) = 0;

  /* long getPrefType (in string aPrefName); */
  NS_IMETHOD GetPrefType(const char * aPrefName, int32_t *_retval) = 0;

  /* [binaryname(GetBoolPrefWithDefault),optional_argc] boolean getBoolPref (in string aPrefName, [optional] in boolean aDefaultValue); */
  NS_IMETHOD GetBoolPrefWithDefault(const char * aPrefName, bool aDefaultValue, uint8_t _argc, bool *_retval) = 0;

  /* [binaryname(GetBoolPref),noscript] boolean getBoolPrefXPCOM (in string aPrefName); */
  NS_IMETHOD GetBoolPref(const char * aPrefName, bool *_retval) = 0;

  /* void setBoolPref (in string aPrefName, in boolean aValue); */
  NS_IMETHOD SetBoolPref(const char * aPrefName, bool aValue) = 0;

  /* [binaryname(GetFloatPrefWithDefault),optional_argc] float getFloatPref (in string aPrefName, [optional] in float aDefaultValue); */
  NS_IMETHOD GetFloatPrefWithDefault(const char * aPrefName, float aDefaultValue, uint8_t _argc, float *_retval) = 0;

  /* [binaryname(GetFloatPref),noscript] float getFloatPrefXPCOM (in string aPrefName); */
  NS_IMETHOD GetFloatPref(const char * aPrefName, float *_retval) = 0;

  /* [binaryname(GetCharPrefWithDefault),optional_argc] string getCharPref (in string aPrefName, [optional] in string aDefaultValue); */
  NS_IMETHOD GetCharPrefWithDefault(const char * aPrefName, const char * aDefaultValue, uint8_t _argc, char * *_retval) = 0;

  /* [binaryname(GetCharPref),noscript] string getCharPrefXPCOM (in string aPrefName); */
  NS_IMETHOD GetCharPref(const char * aPrefName, char * *_retval) = 0;

  /* void setCharPref (in string aPrefName, in string aValue); */
  NS_IMETHOD SetCharPref(const char * aPrefName, const char * aValue) = 0;

  /* [optional_argc] AUTF8String getStringPref (in string aPrefName, [optional] in AUTF8String aDefaultValue); */
  NS_IMETHOD GetStringPref(const char * aPrefName, const nsACString & aDefaultValue, uint8_t _argc, nsACString & _retval) = 0;

  /* void setStringPref (in string aPrefName, in AUTF8String aValue); */
  NS_IMETHOD SetStringPref(const char * aPrefName, const nsACString & aValue) = 0;

  /* [binaryname(GetIntPrefWithDefault),optional_argc] long getIntPref (in string aPrefName, [optional] in long aDefaultValue); */
  NS_IMETHOD GetIntPrefWithDefault(const char * aPrefName, int32_t aDefaultValue, uint8_t _argc, int32_t *_retval) = 0;

  /* [binaryname(GetIntPref),noscript] long getIntPrefXPCOM (in string aPrefName); */
  NS_IMETHOD GetIntPref(const char * aPrefName, int32_t *_retval) = 0;

  /* void setIntPref (in string aPrefName, in long aValue); */
  NS_IMETHOD SetIntPref(const char * aPrefName, int32_t aValue) = 0;

  /* void getComplexValue (in string aPrefName, in nsIIDRef aType, [iid_is (aType), retval] out nsQIResult aValue); */
  NS_IMETHOD GetComplexValue(const char * aPrefName, const nsIID & aType, void **aValue) = 0;

  /* void setComplexValue (in string aPrefName, in nsIIDRef aType, in nsISupports aValue); */
  NS_IMETHOD SetComplexValue(const char * aPrefName, const nsIID & aType, nsISupports *aValue) = 0;

  /* void clearUserPref (in string aPrefName); */
  NS_IMETHOD ClearUserPref(const char * aPrefName) = 0;

  /* void lockPref (in string aPrefName); */
  NS_IMETHOD LockPref(const char * aPrefName) = 0;

  /* boolean prefHasUserValue (in string aPrefName); */
  NS_IMETHOD PrefHasUserValue(const char * aPrefName, bool *_retval) = 0;

  /* boolean prefIsLocked (in string aPrefName); */
  NS_IMETHOD PrefIsLocked(const char * aPrefName, bool *_retval) = 0;

  /* void unlockPref (in string aPrefName); */
  NS_IMETHOD UnlockPref(const char * aPrefName) = 0;

  /* void deleteBranch (in string aStartingAt); */
  NS_IMETHOD DeleteBranch(const char * aStartingAt) = 0;

  /* void getChildList (in string aStartingAt, [optional] out unsigned long aCount, [array, size_is (aCount), retval] out string aChildArray); */
  NS_IMETHOD GetChildList(const char * aStartingAt, uint32_t *aCount, char * **aChildArray) = 0;

  /* void resetBranch (in string aStartingAt); */
  NS_IMETHOD ResetBranch(const char * aStartingAt) = 0;

  /* void addObserver (in string aDomain, in nsIObserver aObserver, [optional] in boolean aHoldWeak); */
  NS_IMETHOD AddObserver(const char * aDomain, nsIObserver *aObserver, bool aHoldWeak) = 0;

  /* void removeObserver (in string aDomain, in nsIObserver aObserver); */
  NS_IMETHOD RemoveObserver(const char * aDomain, nsIObserver *aObserver) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPrefBranch, NS_IPREFBRANCH_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPREFBRANCH \
  NS_IMETHOD GetRoot(char * *aRoot) override; \
  NS_IMETHOD GetPrefType(const char * aPrefName, int32_t *_retval) override; \
  NS_IMETHOD GetBoolPrefWithDefault(const char * aPrefName, bool aDefaultValue, uint8_t _argc, bool *_retval) override; \
  NS_IMETHOD GetBoolPref(const char * aPrefName, bool *_retval) override; \
  NS_IMETHOD SetBoolPref(const char * aPrefName, bool aValue) override; \
  NS_IMETHOD GetFloatPrefWithDefault(const char * aPrefName, float aDefaultValue, uint8_t _argc, float *_retval) override; \
  NS_IMETHOD GetFloatPref(const char * aPrefName, float *_retval) override; \
  NS_IMETHOD GetCharPrefWithDefault(const char * aPrefName, const char * aDefaultValue, uint8_t _argc, char * *_retval) override; \
  NS_IMETHOD GetCharPref(const char * aPrefName, char * *_retval) override; \
  NS_IMETHOD SetCharPref(const char * aPrefName, const char * aValue) override; \
  NS_IMETHOD GetStringPref(const char * aPrefName, const nsACString & aDefaultValue, uint8_t _argc, nsACString & _retval) override; \
  NS_IMETHOD SetStringPref(const char * aPrefName, const nsACString & aValue) override; \
  NS_IMETHOD GetIntPrefWithDefault(const char * aPrefName, int32_t aDefaultValue, uint8_t _argc, int32_t *_retval) override; \
  NS_IMETHOD GetIntPref(const char * aPrefName, int32_t *_retval) override; \
  NS_IMETHOD SetIntPref(const char * aPrefName, int32_t aValue) override; \
  NS_IMETHOD GetComplexValue(const char * aPrefName, const nsIID & aType, void **aValue) override; \
  NS_IMETHOD SetComplexValue(const char * aPrefName, const nsIID & aType, nsISupports *aValue) override; \
  NS_IMETHOD ClearUserPref(const char * aPrefName) override; \
  NS_IMETHOD LockPref(const char * aPrefName) override; \
  NS_IMETHOD PrefHasUserValue(const char * aPrefName, bool *_retval) override; \
  NS_IMETHOD PrefIsLocked(const char * aPrefName, bool *_retval) override; \
  NS_IMETHOD UnlockPref(const char * aPrefName) override; \
  NS_IMETHOD DeleteBranch(const char * aStartingAt) override; \
  NS_IMETHOD GetChildList(const char * aStartingAt, uint32_t *aCount, char * **aChildArray) override; \
  NS_IMETHOD ResetBranch(const char * aStartingAt) override; \
  NS_IMETHOD AddObserver(const char * aDomain, nsIObserver *aObserver, bool aHoldWeak) override; \
  NS_IMETHOD RemoveObserver(const char * aDomain, nsIObserver *aObserver) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIPREFBRANCH \
  nsresult GetRoot(char * *aRoot); \
  nsresult GetPrefType(const char * aPrefName, int32_t *_retval); \
  nsresult GetBoolPrefWithDefault(const char * aPrefName, bool aDefaultValue, uint8_t _argc, bool *_retval); \
  nsresult GetBoolPref(const char * aPrefName, bool *_retval); \
  nsresult SetBoolPref(const char * aPrefName, bool aValue); \
  nsresult GetFloatPrefWithDefault(const char * aPrefName, float aDefaultValue, uint8_t _argc, float *_retval); \
  nsresult GetFloatPref(const char * aPrefName, float *_retval); \
  nsresult GetCharPrefWithDefault(const char * aPrefName, const char * aDefaultValue, uint8_t _argc, char * *_retval); \
  nsresult GetCharPref(const char * aPrefName, char * *_retval); \
  nsresult SetCharPref(const char * aPrefName, const char * aValue); \
  nsresult GetStringPref(const char * aPrefName, const nsACString & aDefaultValue, uint8_t _argc, nsACString & _retval); \
  nsresult SetStringPref(const char * aPrefName, const nsACString & aValue); \
  nsresult GetIntPrefWithDefault(const char * aPrefName, int32_t aDefaultValue, uint8_t _argc, int32_t *_retval); \
  nsresult GetIntPref(const char * aPrefName, int32_t *_retval); \
  nsresult SetIntPref(const char * aPrefName, int32_t aValue); \
  nsresult GetComplexValue(const char * aPrefName, const nsIID & aType, void **aValue); \
  nsresult SetComplexValue(const char * aPrefName, const nsIID & aType, nsISupports *aValue); \
  nsresult ClearUserPref(const char * aPrefName); \
  nsresult LockPref(const char * aPrefName); \
  nsresult PrefHasUserValue(const char * aPrefName, bool *_retval); \
  nsresult PrefIsLocked(const char * aPrefName, bool *_retval); \
  nsresult UnlockPref(const char * aPrefName); \
  nsresult DeleteBranch(const char * aStartingAt); \
  nsresult GetChildList(const char * aStartingAt, uint32_t *aCount, char * **aChildArray); \
  nsresult ResetBranch(const char * aStartingAt); \
  nsresult AddObserver(const char * aDomain, nsIObserver *aObserver, bool aHoldWeak); \
  nsresult RemoveObserver(const char * aDomain, nsIObserver *aObserver); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPREFBRANCH(_to) \
  NS_IMETHOD GetRoot(char * *aRoot) override { return _to GetRoot(aRoot); } \
  NS_IMETHOD GetPrefType(const char * aPrefName, int32_t *_retval) override { return _to GetPrefType(aPrefName, _retval); } \
  NS_IMETHOD GetBoolPrefWithDefault(const char * aPrefName, bool aDefaultValue, uint8_t _argc, bool *_retval) override { return _to GetBoolPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetBoolPref(const char * aPrefName, bool *_retval) override { return _to GetBoolPref(aPrefName, _retval); } \
  NS_IMETHOD SetBoolPref(const char * aPrefName, bool aValue) override { return _to SetBoolPref(aPrefName, aValue); } \
  NS_IMETHOD GetFloatPrefWithDefault(const char * aPrefName, float aDefaultValue, uint8_t _argc, float *_retval) override { return _to GetFloatPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetFloatPref(const char * aPrefName, float *_retval) override { return _to GetFloatPref(aPrefName, _retval); } \
  NS_IMETHOD GetCharPrefWithDefault(const char * aPrefName, const char * aDefaultValue, uint8_t _argc, char * *_retval) override { return _to GetCharPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetCharPref(const char * aPrefName, char * *_retval) override { return _to GetCharPref(aPrefName, _retval); } \
  NS_IMETHOD SetCharPref(const char * aPrefName, const char * aValue) override { return _to SetCharPref(aPrefName, aValue); } \
  NS_IMETHOD GetStringPref(const char * aPrefName, const nsACString & aDefaultValue, uint8_t _argc, nsACString & _retval) override { return _to GetStringPref(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD SetStringPref(const char * aPrefName, const nsACString & aValue) override { return _to SetStringPref(aPrefName, aValue); } \
  NS_IMETHOD GetIntPrefWithDefault(const char * aPrefName, int32_t aDefaultValue, uint8_t _argc, int32_t *_retval) override { return _to GetIntPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetIntPref(const char * aPrefName, int32_t *_retval) override { return _to GetIntPref(aPrefName, _retval); } \
  NS_IMETHOD SetIntPref(const char * aPrefName, int32_t aValue) override { return _to SetIntPref(aPrefName, aValue); } \
  NS_IMETHOD GetComplexValue(const char * aPrefName, const nsIID & aType, void **aValue) override { return _to GetComplexValue(aPrefName, aType, aValue); } \
  NS_IMETHOD SetComplexValue(const char * aPrefName, const nsIID & aType, nsISupports *aValue) override { return _to SetComplexValue(aPrefName, aType, aValue); } \
  NS_IMETHOD ClearUserPref(const char * aPrefName) override { return _to ClearUserPref(aPrefName); } \
  NS_IMETHOD LockPref(const char * aPrefName) override { return _to LockPref(aPrefName); } \
  NS_IMETHOD PrefHasUserValue(const char * aPrefName, bool *_retval) override { return _to PrefHasUserValue(aPrefName, _retval); } \
  NS_IMETHOD PrefIsLocked(const char * aPrefName, bool *_retval) override { return _to PrefIsLocked(aPrefName, _retval); } \
  NS_IMETHOD UnlockPref(const char * aPrefName) override { return _to UnlockPref(aPrefName); } \
  NS_IMETHOD DeleteBranch(const char * aStartingAt) override { return _to DeleteBranch(aStartingAt); } \
  NS_IMETHOD GetChildList(const char * aStartingAt, uint32_t *aCount, char * **aChildArray) override { return _to GetChildList(aStartingAt, aCount, aChildArray); } \
  NS_IMETHOD ResetBranch(const char * aStartingAt) override { return _to ResetBranch(aStartingAt); } \
  NS_IMETHOD AddObserver(const char * aDomain, nsIObserver *aObserver, bool aHoldWeak) override { return _to AddObserver(aDomain, aObserver, aHoldWeak); } \
  NS_IMETHOD RemoveObserver(const char * aDomain, nsIObserver *aObserver) override { return _to RemoveObserver(aDomain, aObserver); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPREFBRANCH(_to) \
  NS_IMETHOD GetRoot(char * *aRoot) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRoot(aRoot); } \
  NS_IMETHOD GetPrefType(const char * aPrefName, int32_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrefType(aPrefName, _retval); } \
  NS_IMETHOD GetBoolPrefWithDefault(const char * aPrefName, bool aDefaultValue, uint8_t _argc, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBoolPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetBoolPref(const char * aPrefName, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBoolPref(aPrefName, _retval); } \
  NS_IMETHOD SetBoolPref(const char * aPrefName, bool aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetBoolPref(aPrefName, aValue); } \
  NS_IMETHOD GetFloatPrefWithDefault(const char * aPrefName, float aDefaultValue, uint8_t _argc, float *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFloatPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetFloatPref(const char * aPrefName, float *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFloatPref(aPrefName, _retval); } \
  NS_IMETHOD GetCharPrefWithDefault(const char * aPrefName, const char * aDefaultValue, uint8_t _argc, char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCharPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetCharPref(const char * aPrefName, char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCharPref(aPrefName, _retval); } \
  NS_IMETHOD SetCharPref(const char * aPrefName, const char * aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCharPref(aPrefName, aValue); } \
  NS_IMETHOD GetStringPref(const char * aPrefName, const nsACString & aDefaultValue, uint8_t _argc, nsACString & _retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringPref(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD SetStringPref(const char * aPrefName, const nsACString & aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetStringPref(aPrefName, aValue); } \
  NS_IMETHOD GetIntPrefWithDefault(const char * aPrefName, int32_t aDefaultValue, uint8_t _argc, int32_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIntPrefWithDefault(aPrefName, aDefaultValue, _argc, _retval); } \
  NS_IMETHOD GetIntPref(const char * aPrefName, int32_t *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIntPref(aPrefName, _retval); } \
  NS_IMETHOD SetIntPref(const char * aPrefName, int32_t aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIntPref(aPrefName, aValue); } \
  NS_IMETHOD GetComplexValue(const char * aPrefName, const nsIID & aType, void **aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetComplexValue(aPrefName, aType, aValue); } \
  NS_IMETHOD SetComplexValue(const char * aPrefName, const nsIID & aType, nsISupports *aValue) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetComplexValue(aPrefName, aType, aValue); } \
  NS_IMETHOD ClearUserPref(const char * aPrefName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ClearUserPref(aPrefName); } \
  NS_IMETHOD LockPref(const char * aPrefName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->LockPref(aPrefName); } \
  NS_IMETHOD PrefHasUserValue(const char * aPrefName, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->PrefHasUserValue(aPrefName, _retval); } \
  NS_IMETHOD PrefIsLocked(const char * aPrefName, bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->PrefIsLocked(aPrefName, _retval); } \
  NS_IMETHOD UnlockPref(const char * aPrefName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnlockPref(aPrefName); } \
  NS_IMETHOD DeleteBranch(const char * aStartingAt) override { return !_to ? NS_ERROR_NULL_POINTER : _to->DeleteBranch(aStartingAt); } \
  NS_IMETHOD GetChildList(const char * aStartingAt, uint32_t *aCount, char * **aChildArray) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChildList(aStartingAt, aCount, aChildArray); } \
  NS_IMETHOD ResetBranch(const char * aStartingAt) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ResetBranch(aStartingAt); } \
  NS_IMETHOD AddObserver(const char * aDomain, nsIObserver *aObserver, bool aHoldWeak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AddObserver(aDomain, aObserver, aHoldWeak); } \
  NS_IMETHOD RemoveObserver(const char * aDomain, nsIObserver *aObserver) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveObserver(aDomain, aObserver); } 


#define NS_PREFBRANCH_CONTRACTID "@mozilla.org/preferencesbranch;1"
/**
 * Notification sent when a preference changes.
 */
#define NS_PREFBRANCH_PREFCHANGE_TOPIC_ID "nsPref:changed"

#endif /* __gen_nsIPrefBranch_h__ */
