/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsITimer.idl
 */

#ifndef __gen_nsITimer_h__
#define __gen_nsITimer_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIObserver; /* forward declaration */

class nsIEventTarget; /* forward declaration */

#include "mozilla/MemoryReporting.h"
#include "mozilla/TimeStamp.h"
/**
 * The signature of the timer callback function passed to initWithFuncCallback.
 * This is the function that will get called when the timer expires if the
 * timer is initialized via initWithFuncCallback.
 *
 * @param aTimer the timer which has expired
 * @param aClosure opaque parameter passed to initWithFuncCallback
 */
class nsITimer;
typedef void (*nsTimerCallbackFunc) (nsITimer *aTimer, void *aClosure);
/**
 * The signature of the timer name callback function passed to
 * initWithNameableFuncCallback.
 * This is the function that will get called when timer profiling is enabled
 * via the "TimerFirings" log module.
 *
 * @param aTimer the timer which has expired
 * @param aAnonymize whether the name should avoid including privacy sensitive info
 * @param aClosure opaque parameter passed to initWithFuncCallback
 * @param aBuf a buffer in which to put the name
 * @param aLen the length of the buffer
 */
typedef void (*nsTimerNameCallbackFunc) (nsITimer *aTimer,
                                         bool aAnonymize,
                                         void *aClosure,
                                         char *aBuf, size_t aLen);
class nsITimer; /* forward declaration */


/* starting interface:    nsITimerCallback */
#define NS_ITIMERCALLBACK_IID_STR "a796816d-7d47-4348-9ab8-c7aeb3216a7d"

#define NS_ITIMERCALLBACK_IID \
  {0xa796816d, 0x7d47, 0x4348, \
    { 0x9a, 0xb8, 0xc7, 0xae, 0xb3, 0x21, 0x6a, 0x7d }}

class NS_NO_VTABLE nsITimerCallback : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITIMERCALLBACK_IID)

  /* void notify (in nsITimer timer); */
  NS_IMETHOD Notify(nsITimer *timer) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITimerCallback, NS_ITIMERCALLBACK_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITIMERCALLBACK \
  NS_IMETHOD Notify(nsITimer *timer) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITIMERCALLBACK \
  nsresult Notify(nsITimer *timer); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITIMERCALLBACK(_to) \
  NS_IMETHOD Notify(nsITimer *timer) override { return _to Notify(timer); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITIMERCALLBACK(_to) \
  NS_IMETHOD Notify(nsITimer *timer) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Notify(timer); } 

// Two timer deadlines must differ by less than half the PRIntervalTime domain.
#define DELAY_INTERVAL_LIMIT    PR_BIT(8 * sizeof(PRIntervalTime) - 1)

/* starting interface:    nsITimer */
#define NS_ITIMER_IID_STR "3de4b105-363c-482c-a409-baac83a01bfc"

#define NS_ITIMER_IID \
  {0x3de4b105, 0x363c, 0x482c, \
    { 0xa4, 0x09, 0xba, 0xac, 0x83, 0xa0, 0x1b, 0xfc }}

class nsITimer : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITIMER_IID)

  enum {
    TYPE_ONE_SHOT = 0,
    TYPE_REPEATING_SLACK = 1,
    TYPE_REPEATING_PRECISE = 2,
    TYPE_REPEATING_PRECISE_CAN_SKIP = 3,
    TYPE_REPEATING_SLACK_LOW_PRIORITY = 4,
    TYPE_ONE_SHOT_LOW_PRIORITY = 5
  };

  /* void init (in nsIObserver aObserver, in unsigned long aDelay, in unsigned long aType); */
  NS_IMETHOD Init(nsIObserver *aObserver, uint32_t aDelay, uint32_t aType) = 0;

  /* void initWithCallback (in nsITimerCallback aCallback, in unsigned long aDelay, in unsigned long aType); */
  NS_IMETHOD InitWithCallback(nsITimerCallback *aCallback, uint32_t aDelay, uint32_t aType) = 0;

  /* [noscript] void InitHighResolutionWithCallback (in nsITimerCallback aCallback, [const] in TimeDuration aDelay, in unsigned long aType); */
  NS_IMETHOD InitHighResolutionWithCallback(nsITimerCallback *aCallback, const mozilla::TimeDuration & aDelay, uint32_t aType) = 0;

  /* void cancel (); */
  NS_IMETHOD Cancel(void) = 0;

  /* [noscript] void initWithNamedFuncCallback (in nsTimerCallbackFunc aCallback, in voidPtr aClosure, in unsigned long aDelay, in unsigned long aType, in string aName); */
  NS_IMETHOD InitWithNamedFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, const char * aName) = 0;

  /* [noscript] void initWithNameableFuncCallback (in nsTimerCallbackFunc aCallback, in voidPtr aClosure, in unsigned long aDelay, in unsigned long aType, in nsTimerNameCallbackFunc aNameCallback); */
  NS_IMETHOD InitWithNameableFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, nsTimerNameCallbackFunc aNameCallback) = 0;

  /* attribute unsigned long delay; */
  NS_IMETHOD GetDelay(uint32_t *aDelay) = 0;
  NS_IMETHOD SetDelay(uint32_t aDelay) = 0;

  /* attribute unsigned long type; */
  NS_IMETHOD GetType(uint32_t *aType) = 0;
  NS_IMETHOD SetType(uint32_t aType) = 0;

  /* [noscript] readonly attribute voidPtr closure; */
  NS_IMETHOD GetClosure(void **aClosure) = 0;

  /* readonly attribute nsITimerCallback callback; */
  NS_IMETHOD GetCallback(nsITimerCallback * *aCallback) = 0;

  /* attribute nsIEventTarget target; */
  NS_IMETHOD GetTarget(nsIEventTarget * *aTarget) = 0;
  NS_IMETHOD SetTarget(nsIEventTarget *aTarget) = 0;

  /* [noscript] readonly attribute unsigned long allowedEarlyFiringMicroseconds; */
  NS_IMETHOD GetAllowedEarlyFiringMicroseconds(uint32_t *aAllowedEarlyFiringMicroseconds) = 0;

   virtual size_t SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const = 0;
};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITimer, NS_ITIMER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITIMER \
  NS_IMETHOD Init(nsIObserver *aObserver, uint32_t aDelay, uint32_t aType) override; \
  NS_IMETHOD InitWithCallback(nsITimerCallback *aCallback, uint32_t aDelay, uint32_t aType) override; \
  NS_IMETHOD InitHighResolutionWithCallback(nsITimerCallback *aCallback, const mozilla::TimeDuration & aDelay, uint32_t aType) override; \
  NS_IMETHOD Cancel(void) override; \
  NS_IMETHOD InitWithNamedFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, const char * aName) override; \
  NS_IMETHOD InitWithNameableFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, nsTimerNameCallbackFunc aNameCallback) override; \
  NS_IMETHOD GetDelay(uint32_t *aDelay) override; \
  NS_IMETHOD SetDelay(uint32_t aDelay) override; \
  NS_IMETHOD GetType(uint32_t *aType) override; \
  NS_IMETHOD SetType(uint32_t aType) override; \
  NS_IMETHOD GetClosure(void **aClosure) override; \
  NS_IMETHOD GetCallback(nsITimerCallback * *aCallback) override; \
  NS_IMETHOD GetTarget(nsIEventTarget * *aTarget) override; \
  NS_IMETHOD SetTarget(nsIEventTarget *aTarget) override; \
  NS_IMETHOD GetAllowedEarlyFiringMicroseconds(uint32_t *aAllowedEarlyFiringMicroseconds) override; \

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSITIMER \
  nsresult Init(nsIObserver *aObserver, uint32_t aDelay, uint32_t aType); \
  nsresult InitWithCallback(nsITimerCallback *aCallback, uint32_t aDelay, uint32_t aType); \
  nsresult InitHighResolutionWithCallback(nsITimerCallback *aCallback, const mozilla::TimeDuration & aDelay, uint32_t aType); \
  nsresult Cancel(void); \
  nsresult InitWithNamedFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, const char * aName); \
  nsresult InitWithNameableFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, nsTimerNameCallbackFunc aNameCallback); \
  nsresult GetDelay(uint32_t *aDelay); \
  nsresult SetDelay(uint32_t aDelay); \
  nsresult GetType(uint32_t *aType); \
  nsresult SetType(uint32_t aType); \
  nsresult GetClosure(void **aClosure); \
  nsresult GetCallback(nsITimerCallback * *aCallback); \
  nsresult GetTarget(nsIEventTarget * *aTarget); \
  nsresult SetTarget(nsIEventTarget *aTarget); \
  nsresult GetAllowedEarlyFiringMicroseconds(uint32_t *aAllowedEarlyFiringMicroseconds); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITIMER(_to) \
  NS_IMETHOD Init(nsIObserver *aObserver, uint32_t aDelay, uint32_t aType) override { return _to Init(aObserver, aDelay, aType); } \
  NS_IMETHOD InitWithCallback(nsITimerCallback *aCallback, uint32_t aDelay, uint32_t aType) override { return _to InitWithCallback(aCallback, aDelay, aType); } \
  NS_IMETHOD InitHighResolutionWithCallback(nsITimerCallback *aCallback, const mozilla::TimeDuration & aDelay, uint32_t aType) override { return _to InitHighResolutionWithCallback(aCallback, aDelay, aType); } \
  NS_IMETHOD Cancel(void) override { return _to Cancel(); } \
  NS_IMETHOD InitWithNamedFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, const char * aName) override { return _to InitWithNamedFuncCallback(aCallback, aClosure, aDelay, aType, aName); } \
  NS_IMETHOD InitWithNameableFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, nsTimerNameCallbackFunc aNameCallback) override { return _to InitWithNameableFuncCallback(aCallback, aClosure, aDelay, aType, aNameCallback); } \
  NS_IMETHOD GetDelay(uint32_t *aDelay) override { return _to GetDelay(aDelay); } \
  NS_IMETHOD SetDelay(uint32_t aDelay) override { return _to SetDelay(aDelay); } \
  NS_IMETHOD GetType(uint32_t *aType) override { return _to GetType(aType); } \
  NS_IMETHOD SetType(uint32_t aType) override { return _to SetType(aType); } \
  NS_IMETHOD GetClosure(void **aClosure) override { return _to GetClosure(aClosure); } \
  NS_IMETHOD GetCallback(nsITimerCallback * *aCallback) override { return _to GetCallback(aCallback); } \
  NS_IMETHOD GetTarget(nsIEventTarget * *aTarget) override { return _to GetTarget(aTarget); } \
  NS_IMETHOD SetTarget(nsIEventTarget *aTarget) override { return _to SetTarget(aTarget); } \
  NS_IMETHOD GetAllowedEarlyFiringMicroseconds(uint32_t *aAllowedEarlyFiringMicroseconds) override { return _to GetAllowedEarlyFiringMicroseconds(aAllowedEarlyFiringMicroseconds); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITIMER(_to) \
  NS_IMETHOD Init(nsIObserver *aObserver, uint32_t aDelay, uint32_t aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aObserver, aDelay, aType); } \
  NS_IMETHOD InitWithCallback(nsITimerCallback *aCallback, uint32_t aDelay, uint32_t aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InitWithCallback(aCallback, aDelay, aType); } \
  NS_IMETHOD InitHighResolutionWithCallback(nsITimerCallback *aCallback, const mozilla::TimeDuration & aDelay, uint32_t aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InitHighResolutionWithCallback(aCallback, aDelay, aType); } \
  NS_IMETHOD Cancel(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Cancel(); } \
  NS_IMETHOD InitWithNamedFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, const char * aName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InitWithNamedFuncCallback(aCallback, aClosure, aDelay, aType, aName); } \
  NS_IMETHOD InitWithNameableFuncCallback(nsTimerCallbackFunc aCallback, void *aClosure, uint32_t aDelay, uint32_t aType, nsTimerNameCallbackFunc aNameCallback) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InitWithNameableFuncCallback(aCallback, aClosure, aDelay, aType, aNameCallback); } \
  NS_IMETHOD GetDelay(uint32_t *aDelay) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDelay(aDelay); } \
  NS_IMETHOD SetDelay(uint32_t aDelay) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDelay(aDelay); } \
  NS_IMETHOD GetType(uint32_t *aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD SetType(uint32_t aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetType(aType); } \
  NS_IMETHOD GetClosure(void **aClosure) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClosure(aClosure); } \
  NS_IMETHOD GetCallback(nsITimerCallback * *aCallback) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCallback(aCallback); } \
  NS_IMETHOD GetTarget(nsIEventTarget * *aTarget) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTarget(aTarget); } \
  NS_IMETHOD SetTarget(nsIEventTarget *aTarget) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTarget(aTarget); } \
  NS_IMETHOD GetAllowedEarlyFiringMicroseconds(uint32_t *aAllowedEarlyFiringMicroseconds) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAllowedEarlyFiringMicroseconds(aAllowedEarlyFiringMicroseconds); } \

#include "nsCOMPtr.h"
already_AddRefed<nsITimer> NS_NewTimer();
already_AddRefed<nsITimer> NS_NewTimer(nsIEventTarget* aTarget);
nsresult
NS_NewTimerWithObserver(nsITimer** aTimer,
                        nsIObserver* aObserver,
                        uint32_t aDelay,
                        uint32_t aType,
                        nsIEventTarget* aTarget = nullptr);
mozilla::Result<nsCOMPtr<nsITimer>, nsresult>
NS_NewTimerWithObserver(nsIObserver* aObserver,
                        uint32_t aDelay,
                        uint32_t aType,
                        nsIEventTarget* aTarget = nullptr);
nsresult
NS_NewTimerWithCallback(nsITimer** aTimer,
                        nsITimerCallback* aCallback,
                        uint32_t aDelay,
                        uint32_t aType,
                        nsIEventTarget* aTarget = nullptr);
mozilla::Result<nsCOMPtr<nsITimer>, nsresult>
NS_NewTimerWithCallback(nsITimerCallback* aCallback,
                        uint32_t aDelay,
                        uint32_t aType,
                        nsIEventTarget* aTarget = nullptr);
nsresult
NS_NewTimerWithCallback(nsITimer** aTimer,
                        nsITimerCallback* aCallback,
                        const mozilla::TimeDuration& aDelay,
                        uint32_t aType,
                        nsIEventTarget* aTarget = nullptr);
mozilla::Result<nsCOMPtr<nsITimer>, nsresult>
NS_NewTimerWithCallback(nsITimerCallback* aCallback,
                        const mozilla::TimeDuration& aDelay,
                        uint32_t aType,
                        nsIEventTarget* aTarget = nullptr);
nsresult
NS_NewTimerWithFuncCallback(nsITimer** aTimer,
                            nsTimerCallbackFunc aCallback,
                            void* aClosure,
                            uint32_t aDelay,
                            uint32_t aType,
                            const char* aNameString,
                            nsIEventTarget* aTarget = nullptr);
mozilla::Result<nsCOMPtr<nsITimer>, nsresult>
NS_NewTimerWithFuncCallback(nsTimerCallbackFunc aCallback,
                            void* aClosure,
                            uint32_t aDelay,
                            uint32_t aType,
                            const char* aNameString,
                            nsIEventTarget* aTarget = nullptr);
nsresult
NS_NewTimerWithFuncCallback(nsITimer** aTimer,
                            nsTimerCallbackFunc aCallback,
                            void* aClosure,
                            uint32_t aDelay,
                            uint32_t aType,
                            nsTimerNameCallbackFunc aNameCallback,
                            nsIEventTarget* aTarget = nullptr);
mozilla::Result<nsCOMPtr<nsITimer>, nsresult>
NS_NewTimerWithFuncCallback(nsTimerCallbackFunc aCallback,
                            void* aClosure,
                            uint32_t aDelay,
                            uint32_t aType,
                            nsTimerNameCallbackFunc aNameCallback,
                            nsIEventTarget* aTarget = nullptr);
#define NS_TIMER_CONTRACTID "@mozilla.org/timer;1"
#define NS_TIMER_CALLBACK_TOPIC "timer-callback"

#endif /* __gen_nsITimer_h__ */
