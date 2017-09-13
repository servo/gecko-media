/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsISupportsPriority.idl
 */

#ifndef __gen_nsISupportsPriority_h__
#define __gen_nsISupportsPriority_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsISupportsPriority */
#define NS_ISUPPORTSPRIORITY_IID_STR "aa578b44-abd5-4c19-8b14-36d4de6fdc36"

#define NS_ISUPPORTSPRIORITY_IID \
  {0xaa578b44, 0xabd5, 0x4c19, \
    { 0x8b, 0x14, 0x36, 0xd4, 0xde, 0x6f, 0xdc, 0x36 }}

class NS_NO_VTABLE nsISupportsPriority : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRIORITY_IID)

  enum {
    PRIORITY_HIGHEST = -20,
    PRIORITY_HIGH = -10,
    PRIORITY_NORMAL = 0,
    PRIORITY_LOW = 10,
    PRIORITY_LOWEST = 20
  };

  /* attribute long priority; */
  NS_IMETHOD GetPriority(int32_t *aPriority) = 0;
  NS_IMETHOD SetPriority(int32_t aPriority) = 0;

  /* void adjustPriority (in long delta); */
  NS_IMETHOD AdjustPriority(int32_t delta) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPriority, NS_ISUPPORTSPRIORITY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRIORITY \
  NS_IMETHOD GetPriority(int32_t *aPriority) override; \
  NS_IMETHOD SetPriority(int32_t aPriority) override; \
  NS_IMETHOD AdjustPriority(int32_t delta) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRIORITY \
  nsresult GetPriority(int32_t *aPriority); \
  nsresult SetPriority(int32_t aPriority); \
  nsresult AdjustPriority(int32_t delta); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRIORITY(_to) \
  NS_IMETHOD GetPriority(int32_t *aPriority) override { return _to GetPriority(aPriority); } \
  NS_IMETHOD SetPriority(int32_t aPriority) override { return _to SetPriority(aPriority); } \
  NS_IMETHOD AdjustPriority(int32_t delta) override { return _to AdjustPriority(delta); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRIORITY(_to) \
  NS_IMETHOD GetPriority(int32_t *aPriority) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPriority(aPriority); } \
  NS_IMETHOD SetPriority(int32_t aPriority) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPriority(aPriority); } \
  NS_IMETHOD AdjustPriority(int32_t delta) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AdjustPriority(delta); } 


#endif /* __gen_nsISupportsPriority_h__ */
