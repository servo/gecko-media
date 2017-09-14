/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIWeakReference.idl
 */

#ifndef __gen_nsIWeakReference_h__
#define __gen_nsIWeakReference_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "mozilla/Attributes.h"
#include "mozilla/MemoryReporting.h"
// For MOZ_THREAD_SAFETY_OWNERSHIP_CHECKS_SUPPORTED.
#include "nsDebug.h"
#ifdef MOZ_THREAD_SAFETY_OWNERSHIP_CHECKS_SUPPORTED
#define MOZ_WEAKREF_DECL_OWNINGTHREAD nsAutoOwningThread _mWeakRefOwningThread;
#define MOZ_WEAKREF_ASSERT_OWNINGTHREAD \
  _mWeakRefOwningThread.AssertOwnership("nsWeakReference not thread-safe")
#define MOZ_WEAKREF_ASSERT_OWNINGTHREAD_DELEGATED(that) \
  (that)->_mWeakRefOwningThread.AssertOwnership("nsWeakReference not thread-safe")
#else
#define MOZ_WEAKREF_DECL_OWNINGTHREAD
#define MOZ_WEAKREF_ASSERT_OWNINGTHREAD do { } while (false)
#define MOZ_WEAKREF_ASSERT_OWNINGTHREAD_DELEGATED(that) do { } while (false)
#endif

/* starting interface:    nsIWeakReference */
#define NS_IWEAKREFERENCE_IID_STR "9188bc85-f92e-11d2-81ef-0060083a0bcf"

#define NS_IWEAKREFERENCE_IID \
  {0x9188bc85, 0xf92e, 0x11d2, \
    { 0x81, 0xef, 0x00, 0x60, 0x08, 0x3a, 0x0b, 0xcf }}

class nsIWeakReference : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IWEAKREFERENCE_IID)

  /* [binaryname(QueryReferentFromScript)] void QueryReferent (in nsIIDRef uuid, [iid_is (uuid), retval] out nsQIResult result); */
  NS_IMETHOD QueryReferentFromScript(const nsIID & uuid, void **result) = 0;

   virtual size_t SizeOfOnlyThis(mozilla::MallocSizeOf aMallocSizeOf) const = 0;
  /**
   * Returns true if the referring object is alive.  Otherwise, false.
   */
  bool IsAlive() const
  {
    return !!mObject;
  }
  nsresult QueryReferent(const nsIID& aIID, void** aInstancePtr);
protected:
  friend class nsSupportsWeakReference;
  nsIWeakReference(nsISupports* aObject)
    : mObject(aObject)
  {
  }
  nsIWeakReference() = delete;
  MOZ_WEAKREF_DECL_OWNINGTHREAD
  // The object we're holding a weak reference to.
  nsISupports* MOZ_NON_OWNING_REF mObject;
};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIWeakReference, NS_IWEAKREFERENCE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEAKREFERENCE \
  NS_IMETHOD QueryReferentFromScript(const nsIID & uuid, void **result) override; \

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIWEAKREFERENCE \
  nsresult QueryReferentFromScript(const nsIID & uuid, void **result); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEAKREFERENCE(_to) \
  NS_IMETHOD QueryReferentFromScript(const nsIID & uuid, void **result) override { return _to QueryReferentFromScript(uuid, result); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEAKREFERENCE(_to) \
  NS_IMETHOD QueryReferentFromScript(const nsIID & uuid, void **result) override { return !_to ? NS_ERROR_NULL_POINTER : _to->QueryReferentFromScript(uuid, result); } \


/* starting interface:    nsISupportsWeakReference */
#define NS_ISUPPORTSWEAKREFERENCE_IID_STR "9188bc86-f92e-11d2-81ef-0060083a0bcf"

#define NS_ISUPPORTSWEAKREFERENCE_IID \
  {0x9188bc86, 0xf92e, 0x11d2, \
    { 0x81, 0xef, 0x00, 0x60, 0x08, 0x3a, 0x0b, 0xcf }}

class NS_NO_VTABLE nsISupportsWeakReference : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSWEAKREFERENCE_IID)

  /* nsIWeakReference GetWeakReference (); */
  NS_IMETHOD GetWeakReference(nsIWeakReference * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsWeakReference, NS_ISUPPORTSWEAKREFERENCE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSWEAKREFERENCE \
  NS_IMETHOD GetWeakReference(nsIWeakReference * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSWEAKREFERENCE \
  nsresult GetWeakReference(nsIWeakReference * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSWEAKREFERENCE(_to) \
  NS_IMETHOD GetWeakReference(nsIWeakReference * *_retval) override { return _to GetWeakReference(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSWEAKREFERENCE(_to) \
  NS_IMETHOD GetWeakReference(nsIWeakReference * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWeakReference(_retval); } 

#ifdef MOZILLA_INTERNAL_API
#include "nsIWeakReferenceUtils.h"
#endif

#endif /* __gen_nsIWeakReference_h__ */
