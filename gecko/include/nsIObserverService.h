/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIObserverService.idl
 */

#ifndef __gen_nsIObserverService_h__
#define __gen_nsIObserverService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIObserver; /* forward declaration */

class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIObserverService */
#define NS_IOBSERVERSERVICE_IID_STR "d07f5192-e3d1-11d2-8acd-00105a1b8860"

#define NS_IOBSERVERSERVICE_IID \
  {0xd07f5192, 0xe3d1, 0x11d2, \
    { 0x8a, 0xcd, 0x00, 0x10, 0x5a, 0x1b, 0x88, 0x60 }}

class NS_NO_VTABLE nsIObserverService : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IOBSERVERSERVICE_IID)

  /* void addObserver (in nsIObserver anObserver, in string aTopic, [optional] in boolean ownsWeak); */
  NS_IMETHOD AddObserver(nsIObserver *anObserver, const char * aTopic, bool ownsWeak) = 0;

  /* void removeObserver (in nsIObserver anObserver, in string aTopic); */
  NS_IMETHOD RemoveObserver(nsIObserver *anObserver, const char * aTopic) = 0;

  /* void notifyObservers (in nsISupports aSubject, in string aTopic, [optional] in wstring someData); */
  NS_IMETHOD NotifyObservers(nsISupports *aSubject, const char * aTopic, const char16_t * someData) = 0;

  /* nsISimpleEnumerator enumerateObservers (in string aTopic); */
  NS_IMETHOD EnumerateObservers(const char * aTopic, nsISimpleEnumerator * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIObserverService, NS_IOBSERVERSERVICE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIOBSERVERSERVICE \
  NS_IMETHOD AddObserver(nsIObserver *anObserver, const char * aTopic, bool ownsWeak) override; \
  NS_IMETHOD RemoveObserver(nsIObserver *anObserver, const char * aTopic) override; \
  NS_IMETHOD NotifyObservers(nsISupports *aSubject, const char * aTopic, const char16_t * someData) override; \
  NS_IMETHOD EnumerateObservers(const char * aTopic, nsISimpleEnumerator * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIOBSERVERSERVICE \
  nsresult AddObserver(nsIObserver *anObserver, const char * aTopic, bool ownsWeak); \
  nsresult RemoveObserver(nsIObserver *anObserver, const char * aTopic); \
  nsresult NotifyObservers(nsISupports *aSubject, const char * aTopic, const char16_t * someData); \
  nsresult EnumerateObservers(const char * aTopic, nsISimpleEnumerator * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIOBSERVERSERVICE(_to) \
  NS_IMETHOD AddObserver(nsIObserver *anObserver, const char * aTopic, bool ownsWeak) override { return _to AddObserver(anObserver, aTopic, ownsWeak); } \
  NS_IMETHOD RemoveObserver(nsIObserver *anObserver, const char * aTopic) override { return _to RemoveObserver(anObserver, aTopic); } \
  NS_IMETHOD NotifyObservers(nsISupports *aSubject, const char * aTopic, const char16_t * someData) override { return _to NotifyObservers(aSubject, aTopic, someData); } \
  NS_IMETHOD EnumerateObservers(const char * aTopic, nsISimpleEnumerator * *_retval) override { return _to EnumerateObservers(aTopic, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIOBSERVERSERVICE(_to) \
  NS_IMETHOD AddObserver(nsIObserver *anObserver, const char * aTopic, bool ownsWeak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->AddObserver(anObserver, aTopic, ownsWeak); } \
  NS_IMETHOD RemoveObserver(nsIObserver *anObserver, const char * aTopic) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveObserver(anObserver, aTopic); } \
  NS_IMETHOD NotifyObservers(nsISupports *aSubject, const char * aTopic, const char16_t * someData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->NotifyObservers(aSubject, aTopic, someData); } \
  NS_IMETHOD EnumerateObservers(const char * aTopic, nsISimpleEnumerator * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EnumerateObservers(aTopic, _retval); } 


#endif /* __gen_nsIObserverService_h__ */
