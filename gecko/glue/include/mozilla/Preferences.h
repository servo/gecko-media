/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_Preferences_h
#define mozilla_Preferences_h

#ifndef MOZILLA_INTERNAL_API
#error "This header is only usable from within libxul (MOZILLA_INTERNAL_API)."
#endif

#include "nsIPrefService.h"
#include "mozilla/Atomics.h"

#ifndef have_PrefChangedFunc_typedef
typedef void (*PrefChangedFunc)(const char*, void*);
#define have_PrefChangedFunc_typedef
#endif

namespace mozilla {

class Preferences final : public nsIPrefService
{
public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIPREFSERVICE


  /**
   * Finallizes global members.
   */
  static void Shutdown();

  /**
   * Returns shared pref service instance
   * NOTE: not addreffed.
   */
  static nsIPrefService* GetService();

  static nsresult AddBoolVarCache(bool* aVariable,
                                  const char* aPref,
                                  bool aDefault = false);
  static nsresult AddIntVarCache(int32_t* aVariable,
                                 const char* aPref,
                                 int32_t aDefault = 0);
  static nsresult AddUintVarCache(uint32_t* aVariable,
                                  const char* aPref,
                                  uint32_t aDefault = 0);
  template <MemoryOrdering Order>
  static nsresult AddAtomicUintVarCache(Atomic<uint32_t, Order>* aVariable,
                                        const char* aPref,
                                        uint32_t aDefault = 0);
  static nsresult AddFloatVarCache(float* aVariable,
                                   const char* aPref,
                                   float aDefault = 0.0f);

  /**
   * Gets int or bool type pref value with default value if failed to get
   * the pref.
   */
  static bool GetBool(const char* aPref, bool aDefault = false)
  {
    bool result = aDefault;
    GetBool(aPref, &result);
    return result;
  }

  static int32_t GetInt(const char* aPref, int32_t aDefault = 0)
  {
    int32_t result = aDefault;
    GetInt(aPref, &result);
    return result;
  }

  static uint32_t GetUint(const char* aPref, uint32_t aDefault = 0)
  {
    uint32_t result = aDefault;
    GetUint(aPref, &result);
    return result;
  }

  static float GetFloat(const char* aPref, float aDefault = 0)
  {
    float result = aDefault;
    GetFloat(aPref, &result);
    return result;
  }

  /**
   * Gets int, float, or bool type pref value with raw return value of
   * nsIPrefBranch.
   *
   * @param aPref       A pref name.
   * @param aResult     Must not be nullptr.  The value is never modified
   *                    when these methods fail.
   */
  static nsresult GetBool(const char* aPref, bool* aResult);
  static nsresult GetInt(const char* aPref, int32_t* aResult);
  static nsresult GetFloat(const char* aPref, float* aResult);
  static nsresult GetUint(const char* aPref, uint32_t* aResult)
  {
    int32_t result;
    nsresult rv = GetInt(aPref, &result);
    if (NS_SUCCEEDED(rv)) {
      *aResult = static_cast<uint32_t>(result);
    }
    return rv;
  }

  /**
   * Gets string type pref value with raw return value of nsIPrefBranch.
   *
   * @param aPref       A pref name.
   * @param aResult     The value is never modified when these methods fail.
   */
  static nsresult GetCString(const char* aPref, nsACString& aResult);
  static nsresult GetString(const char* aPref, nsAString& aResult);

  static bool HasUserValue(const char* aPref);

  /**
   * Registers/Unregisters the callback function for the aPref.
   */
  static nsresult RegisterCallback(PrefChangedFunc aCallback,
                                   const char* aPref,
                                   void* aClosure = nullptr)
  {
    return RegisterCallback(aCallback, aPref, aClosure, ExactMatch);
  }
  static nsresult UnregisterCallback(PrefChangedFunc aCallback,
                                     const char* aPref,
                                     void* aClosure = nullptr)
  {
    return UnregisterCallback(aCallback, aPref, aClosure, ExactMatch);
  }
  // Like RegisterCallback, but also calls the callback immediately for
  // initialization.
  static nsresult RegisterCallbackAndCall(PrefChangedFunc aCallback,
                                          const char* aPref,
                                          void* aClosure = nullptr)
  {
    return RegisterCallbackAndCall(aCallback, aPref, aClosure, ExactMatch);
  }

  /**
   * Like RegisterCallback, but registers a callback for a prefix of multiple
   * pref names, not a single pref name.
   */
  static nsresult RegisterPrefixCallback(PrefChangedFunc aCallback,
                                         const char* aPref,
                                         void* aClosure = nullptr)
  {
    return RegisterCallback(aCallback, aPref, aClosure, PrefixMatch);
  }

  /**
   * Like RegisterPrefixCallback, but also calls the callback immediately for
   * initialization.
   */
  static nsresult RegisterPrefixCallbackAndCall(PrefChangedFunc aCallback,
                                                const char* aPref,
                                                void* aClosure = nullptr)
  {
    return RegisterCallbackAndCall(aCallback, aPref, aClosure, PrefixMatch);
  }

  /**
   * Unregister a callback registered with RegisterPrefixCallback or
   * RegisterPrefixCallbackAndCall.
   */
  static nsresult UnregisterPrefixCallback(PrefChangedFunc aCallback,
                                           const char* aPref,
                                           void* aClosure = nullptr)
  {
    return UnregisterCallback(aCallback, aPref, aClosure, PrefixMatch);
  }

protected:
  virtual ~Preferences();
  Preferences();

public:
  enum MatchKind
  {
    PrefixMatch,
    ExactMatch,
  };

protected:
  static nsresult RegisterCallback(PrefChangedFunc aCallback,
                                   const char* aPref,
                                   void* aClosure,
                                   MatchKind aMatchKind);
  static nsresult UnregisterCallback(PrefChangedFunc aCallback,
                                     const char* aPref,
                                     void* aClosure,
                                     MatchKind aMatchKind);
  static nsresult RegisterCallbackAndCall(PrefChangedFunc aCallback,
                                          const char* aPref,
                                          void* aClosure,
                                          MatchKind aMatchKind);

private:
  static bool sShutdown;
  static bool InitStaticMembers();
};

} // namespace mozilla

#endif // mozilla_Preferences_h
