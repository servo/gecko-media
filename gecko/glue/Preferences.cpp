/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Preferences.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/BinarySearch.h"
#include "nsString.h"
#include "mozilla/StaticPtr.h"
#include "gecko_media_prefs.h"

namespace mozilla {

static StaticRefPtr<Preferences> sPreferences;
bool Preferences::sShutdown = false;

// static
bool
Preferences::InitStaticMembers()
{
  if (!sShutdown && !sPreferences) {
    sPreferences = new Preferences();
  }
  return true;
}

// static
nsIPrefService* Preferences::GetService()
{
  NS_ENSURE_TRUE(InitStaticMembers(), nullptr);
  return sPreferences.get();
}

// static
void
Preferences::Shutdown()
{
  if (!sShutdown) {
    sShutdown = true;
    if (sPreferences) {
      sPreferences = nullptr;
    }
  }
}

Preferences::Preferences()
{
}

Preferences::~Preferences()
{
}

NS_IMPL_ADDREF(Preferences)
NS_IMPL_RELEASE(Preferences)

NS_INTERFACE_MAP_BEGIN(Preferences)
NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIPrefService)
NS_INTERFACE_MAP_ENTRY(nsIPrefService)
NS_INTERFACE_MAP_END

NS_IMETHODIMP
Preferences::ReadUserPrefsFromFile(nsIFile* aFile)
{
  MOZ_CRASH("Called unimplemented Preferences::ReadUserPrefsFromFile");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Preferences::ResetPrefs()
{
  MOZ_CRASH("Called unimplemented Preferences::ResetPrefs");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Preferences::ResetUserPrefs()
{
  MOZ_CRASH("Called unimplemented Preferences::ResetUserPrefs");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Preferences::SavePrefFile(nsIFile* aFile)
{
  MOZ_CRASH("Called unimplemented Preferences::SavePrefFile");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Preferences::GetBranch(const char* aPrefRoot, nsIPrefBranch** _retval)
{
  MOZ_CRASH("Called unimplemented Preferences::GetBranch");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Preferences::GetDefaultBranch(const char* aPrefRoot, nsIPrefBranch** _retval)
{
  MOZ_CRASH("Called unimplemented Preferences::GetDefaultBranch");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Preferences::GetDirty(bool* _retval)
{
  MOZ_CRASH("Called unimplemented Preferences::GetDirty");
  return NS_ERROR_NOT_IMPLEMENTED;
}

#include "prefs_common.cpp"
#if defined(MOZ_WIDGET_ANDROID)
#include "prefs_android.cpp"
#else
#include "prefs_desktop.cpp"
#endif

template<typename PrefArrayType, typename PrefValueType>
bool
FindPref(const PrefArrayType* aPlatformPrefs,
         size_t aPlatformPrefsLength,
         const PrefArrayType* aCommonPrefs,
         size_t aCommonPrefsLength,
         const char* aPref,
         PrefValueType* aResult)
{
  size_t match = 0;
  if (BinarySearchIf(aPlatformPrefs,
                     0,
                     aPlatformPrefsLength,
                     [&](const PrefArrayType& aOther) {
                       return strcmp(aPref, aOther.mName);
                     },
                     &match)) {
    *aResult = aPlatformPrefs[match].mValue;
    return true;
  }
  if (BinarySearchIf(aCommonPrefs,
                     0,
                     aCommonPrefsLength,
                     [&](const PrefArrayType& aOther) {
                       return strcmp(aPref, aOther.mName);
                     },
                     &match)) {
    *aResult = aCommonPrefs[match].mValue;
    return true;
  }
  return false;
}

#if defined(MOZ_WIDGET_ANDROID)
#define PLATFORM_BOOL_PREFS sAndroidBoolPrefs
#define PLATFORM_INT_PREFS sAndroidIntPrefs
#define PLATFORM_STRING_PREFS sAndroidStringPrefs
#else
#define PLATFORM_BOOL_PREFS sDesktopBoolPrefs
#define PLATFORM_INT_PREFS sDesktopIntPrefs
#define PLATFORM_STRING_PREFS sDesktopStringPrefs
#endif

// static
nsresult
Preferences::GetBool(const char* aPref, bool* aResult)
{
  NS_PRECONDITION(aResult, "aResult must not be NULL");
  NS_ENSURE_TRUE(InitStaticMembers(), NS_ERROR_NOT_AVAILABLE);
  if (FindPref(PLATFORM_BOOL_PREFS,
               sizeof(PLATFORM_BOOL_PREFS) / sizeof(BoolPref),
               sCommonBoolPrefs,
               sizeof(sCommonBoolPrefs) / sizeof(BoolPref),
               aPref,
               aResult)) {
    return NS_OK;
  }
  return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetInt(const char* aPref, int32_t* aResult)
{
  NS_PRECONDITION(aResult, "aResult must not be NULL");
  NS_ENSURE_TRUE(InitStaticMembers(), NS_ERROR_NOT_AVAILABLE);
  if (FindPref(PLATFORM_INT_PREFS,
               sizeof(PLATFORM_INT_PREFS) / sizeof(IntPref),
               sCommonIntPrefs,
               sizeof(sCommonIntPrefs) / sizeof(IntPref),
               aPref,
               aResult)) {
    return NS_OK;
  }
  return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetFloat(const char* aPref, float* aResult)
{
  NS_PRECONDITION(aResult, "aResult must not be NULL");
  NS_ENSURE_TRUE(InitStaticMembers(), NS_ERROR_NOT_AVAILABLE);
  nsAutoCString result;
  nsresult rv = GetCString(aPref, result);
  if (NS_SUCCEEDED(rv)) {
    *aResult = result.ToFloat(&rv);
  }
  return rv;
}

// static
nsresult
Preferences::GetCString(const char* aPref, nsACString& aResult)
{
  NS_ENSURE_TRUE(InitStaticMembers(), NS_ERROR_NOT_AVAILABLE);
  const char* result = nullptr;
  if (!FindPref(PLATFORM_STRING_PREFS,
                sizeof(PLATFORM_STRING_PREFS) / sizeof(StringPref),
                sCommonStringPrefs,
                sizeof(sCommonStringPrefs) / sizeof(StringPref),
                aPref,
                &result)) {
    return NS_ERROR_FAILURE;
  }
  aResult = nsDependentCString(result);
  return NS_OK;
}

// static
nsresult
Preferences::GetString(const char* aPref, nsAString& aResult)
{
  NS_ENSURE_TRUE(InitStaticMembers(), NS_ERROR_NOT_AVAILABLE);
  nsAutoCString result;
  nsresult rv = GetCString(aPref, result);
  if (NS_SUCCEEDED(rv)) {
    CopyUTF8toUTF16(result, aResult);
  }
  return rv;
}

// static
bool
Preferences::HasUserValue(const char* aPref)
{
  // Users can't set prefs.
  return false;
}

// static
nsresult
Preferences::RegisterCallback(PrefChangedFunc aCallback,
                              const char* aPref,
                              void* aClosure,
                              MatchKind aMatchKind)
{
  // We don't track the callback, as our pref's can't change
  // at runtime!
  return NS_OK;
}

// static
nsresult
Preferences::RegisterCallbackAndCall(PrefChangedFunc aCallback,
                                     const char* aPref,
                                     void* aClosure,
                                     MatchKind aMatchKind)
{
  MOZ_ASSERT(aCallback);
  (*aCallback)(aPref, aClosure);
  return NS_OK;
}

// static
nsresult
Preferences::UnregisterCallback(PrefChangedFunc aCallback,
                                const char* aPref,
                                void* aClosure,
                                MatchKind aMatchKind)
{
  // We don't track the callback, as our pref's can't change
  // at runtime!
  return NS_OK;
}

// static
nsresult
Preferences::AddBoolVarCache(bool* aVariable,
                             const char* aPref,
                             bool aDefault)
{
  NS_ASSERTION(aVariable, "aVariable must not be NULL");
  *aVariable = GetBool(aPref, aDefault);
  return NS_OK;
}

// static
nsresult
Preferences::AddIntVarCache(int32_t* aVariable,
                            const char* aPref,
                            int32_t aDefault)
{
  NS_ASSERTION(aVariable, "aVariable must not be NULL");
  *aVariable = GetInt(aPref, aDefault);
  return NS_OK;
}

// static
nsresult
Preferences::AddUintVarCache(uint32_t* aVariable,
                             const char* aPref,
                             uint32_t aDefault)
{
  NS_ASSERTION(aVariable, "aVariable must not be NULL");
  *aVariable = GetUint(aPref, aDefault);
  return NS_OK;
}

// static
nsresult
Preferences::AddFloatVarCache(float* aVariable,
                              const char* aPref,
                              float aDefault)
{
  NS_ASSERTION(aVariable, "aVariable must not be NULL");
  *aVariable = GetFloat(aPref, aDefault);
  return NS_OK;
}

template <MemoryOrdering Order>
// static
nsresult
Preferences::AddAtomicUintVarCache(Atomic<uint32_t, Order>* aCache,
                                   const char* aPref,
                                   uint32_t aDefault)
{
  NS_ASSERTION(aCache, "aCache must not be NULL");
  *aCache = Preferences::GetUint(aPref, aDefault);
  return NS_OK;
}

} // namespace mozilla
