/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIMemoryReporter.idl
 */

#ifndef __gen_nsIMemoryReporter_h__
#define __gen_nsIMemoryReporter_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#include "mozilla/Assertions.h"
#include "mozilla/DebugOnly.h"

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include <stdio.h>
class mozIDOMWindowProxy; /* forward declaration */

class nsIRunnable; /* forward declaration */

class nsISimpleEnumerator; /* forward declaration */


/* starting interface:    nsIMemoryReporterCallback */
#define NS_IMEMORYREPORTERCALLBACK_IID_STR "62ef0e1c-dbd6-11e3-aa75-3c970e9f4238"

#define NS_IMEMORYREPORTERCALLBACK_IID \
  {0x62ef0e1c, 0xdbd6, 0x11e3, \
    { 0xaa, 0x75, 0x3c, 0x97, 0x0e, 0x9f, 0x42, 0x38 }}

class NS_NO_VTABLE nsIMemoryReporterCallback : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IMEMORYREPORTERCALLBACK_IID)

  /* void callback (in ACString process, in AUTF8String path, in int32_t kind, in int32_t units, in int64_t amount, in AUTF8String description, in nsISupports data); */
  NS_IMETHOD Callback(const nsACString & process, const nsACString & path, int32_t kind, int32_t units, int64_t amount, const nsACString & description, nsISupports *data) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIMemoryReporterCallback, NS_IMEMORYREPORTERCALLBACK_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIMEMORYREPORTERCALLBACK \
  NS_IMETHOD Callback(const nsACString & process, const nsACString & path, int32_t kind, int32_t units, int64_t amount, const nsACString & description, nsISupports *data) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIMEMORYREPORTERCALLBACK \
  nsresult Callback(const nsACString & process, const nsACString & path, int32_t kind, int32_t units, int64_t amount, const nsACString & description, nsISupports *data); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIMEMORYREPORTERCALLBACK(_to) \
  NS_IMETHOD Callback(const nsACString & process, const nsACString & path, int32_t kind, int32_t units, int64_t amount, const nsACString & description, nsISupports *data) override { return _to Callback(process, path, kind, units, amount, description, data); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIMEMORYREPORTERCALLBACK(_to) \
  NS_IMETHOD Callback(const nsACString & process, const nsACString & path, int32_t kind, int32_t units, int64_t amount, const nsACString & description, nsISupports *data) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Callback(process, path, kind, units, amount, description, data); } 


/* starting interface:    nsIMemoryReporter */
#define NS_IMEMORYREPORTER_IID_STR "92a36db1-46bd-4fe6-988e-47db47236d8b"

#define NS_IMEMORYREPORTER_IID \
  {0x92a36db1, 0x46bd, 0x4fe6, \
    { 0x98, 0x8e, 0x47, 0xdb, 0x47, 0x23, 0x6d, 0x8b }}

class NS_NO_VTABLE nsIMemoryReporter : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IMEMORYREPORTER_IID)

  /* void collectReports (in nsIMemoryReporterCallback callback, in nsISupports data, in boolean anonymize); */
  NS_IMETHOD CollectReports(nsIMemoryReporterCallback *callback, nsISupports *data, bool anonymize) = 0;

  enum {
    KIND_NONHEAP = 0,
    KIND_HEAP = 1,
    KIND_OTHER = 2,
    UNITS_BYTES = 0,
    UNITS_COUNT = 1,
    UNITS_COUNT_CUMULATIVE = 2,
    UNITS_PERCENTAGE = 3
  };

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIMemoryReporter, NS_IMEMORYREPORTER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIMEMORYREPORTER \
  NS_IMETHOD CollectReports(nsIMemoryReporterCallback *callback, nsISupports *data, bool anonymize) override; \

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIMEMORYREPORTER \
  nsresult CollectReports(nsIMemoryReporterCallback *callback, nsISupports *data, bool anonymize); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIMEMORYREPORTER(_to) \
  NS_IMETHOD CollectReports(nsIMemoryReporterCallback *callback, nsISupports *data, bool anonymize) override { return _to CollectReports(callback, data, anonymize); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIMEMORYREPORTER(_to) \
  NS_IMETHOD CollectReports(nsIMemoryReporterCallback *callback, nsISupports *data, bool anonymize) override { return !_to ? NS_ERROR_NULL_POINTER : _to->CollectReports(callback, data, anonymize); } \


/* starting interface:    nsIFinishReportingCallback */
#define NS_IFINISHREPORTINGCALLBACK_IID_STR "548b3909-c04d-4ca6-8466-b8bee3837457"

#define NS_IFINISHREPORTINGCALLBACK_IID \
  {0x548b3909, 0xc04d, 0x4ca6, \
    { 0x84, 0x66, 0xb8, 0xbe, 0xe3, 0x83, 0x74, 0x57 }}

class NS_NO_VTABLE nsIFinishReportingCallback : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IFINISHREPORTINGCALLBACK_IID)

  /* void callback (in nsISupports data); */
  NS_IMETHOD Callback(nsISupports *data) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIFinishReportingCallback, NS_IFINISHREPORTINGCALLBACK_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIFINISHREPORTINGCALLBACK \
  NS_IMETHOD Callback(nsISupports *data) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIFINISHREPORTINGCALLBACK \
  nsresult Callback(nsISupports *data); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIFINISHREPORTINGCALLBACK(_to) \
  NS_IMETHOD Callback(nsISupports *data) override { return _to Callback(data); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIFINISHREPORTINGCALLBACK(_to) \
  NS_IMETHOD Callback(nsISupports *data) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Callback(data); } 


/* starting interface:    nsIHeapAllocatedCallback */
#define NS_IHEAPALLOCATEDCALLBACK_IID_STR "1a80cd0f-0d9e-4397-be69-68ad28fe5175"

#define NS_IHEAPALLOCATEDCALLBACK_IID \
  {0x1a80cd0f, 0x0d9e, 0x4397, \
    { 0xbe, 0x69, 0x68, 0xad, 0x28, 0xfe, 0x51, 0x75 }}

class NS_NO_VTABLE nsIHeapAllocatedCallback : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHEAPALLOCATEDCALLBACK_IID)

  /* void callback (in int64_t bytesAllocated); */
  NS_IMETHOD Callback(int64_t bytesAllocated) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHeapAllocatedCallback, NS_IHEAPALLOCATEDCALLBACK_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHEAPALLOCATEDCALLBACK \
  NS_IMETHOD Callback(int64_t bytesAllocated) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIHEAPALLOCATEDCALLBACK \
  nsresult Callback(int64_t bytesAllocated); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHEAPALLOCATEDCALLBACK(_to) \
  NS_IMETHOD Callback(int64_t bytesAllocated) override { return _to Callback(bytesAllocated); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHEAPALLOCATEDCALLBACK(_to) \
  NS_IMETHOD Callback(int64_t bytesAllocated) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Callback(bytesAllocated); } 


/* starting interface:    nsIMemoryReporterManager */
#define NS_IMEMORYREPORTERMANAGER_IID_STR "2998574d-8993-407a-b1a5-8ad7417653e1"

#define NS_IMEMORYREPORTERMANAGER_IID \
  {0x2998574d, 0x8993, 0x407a, \
    { 0xb1, 0xa5, 0x8a, 0xd7, 0x41, 0x76, 0x53, 0xe1 }}

class NS_NO_VTABLE nsIMemoryReporterManager : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IMEMORYREPORTERMANAGER_IID)

  /* [must_use] void init (); */
  MOZ_MUST_USE NS_IMETHOD Init(void) = 0;

  /* void registerStrongReporter (in nsIMemoryReporter reporter); */
  NS_IMETHOD RegisterStrongReporter(nsIMemoryReporter *reporter) = 0;

  /* void registerStrongAsyncReporter (in nsIMemoryReporter reporter); */
  NS_IMETHOD RegisterStrongAsyncReporter(nsIMemoryReporter *reporter) = 0;

  /* void registerWeakReporter (in nsIMemoryReporter reporter); */
  NS_IMETHOD RegisterWeakReporter(nsIMemoryReporter *reporter) = 0;

  /* void registerWeakAsyncReporter (in nsIMemoryReporter reporter); */
  NS_IMETHOD RegisterWeakAsyncReporter(nsIMemoryReporter *reporter) = 0;

  /* void unregisterStrongReporter (in nsIMemoryReporter reporter); */
  NS_IMETHOD UnregisterStrongReporter(nsIMemoryReporter *reporter) = 0;

  /* void unregisterWeakReporter (in nsIMemoryReporter reporter); */
  NS_IMETHOD UnregisterWeakReporter(nsIMemoryReporter *reporter) = 0;

  /* void blockRegistrationAndHideExistingReporters (); */
  NS_IMETHOD BlockRegistrationAndHideExistingReporters(void) = 0;

  /* void unblockRegistrationAndRestoreOriginalReporters (); */
  NS_IMETHOD UnblockRegistrationAndRestoreOriginalReporters(void) = 0;

  /* void registerStrongReporterEvenIfBlocked (in nsIMemoryReporter aReporter); */
  NS_IMETHOD RegisterStrongReporterEvenIfBlocked(nsIMemoryReporter *aReporter) = 0;

  /* void getReports (in nsIMemoryReporterCallback handleReport, in nsISupports handleReportData, in nsIFinishReportingCallback finishReporting, in nsISupports finishReportingData, in boolean anonymize); */
  NS_IMETHOD GetReports(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize) = 0;

  /* [noscript] void getReportsExtended (in nsIMemoryReporterCallback handleReport, in nsISupports handleReportData, in nsIFinishReportingCallback finishReporting, in nsISupports finishReportingData, in boolean anonymize, in boolean minimizeMemoryUsage, in AString DMDDumpIdent); */
  NS_IMETHOD GetReportsExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize, bool minimizeMemoryUsage, const nsAString & DMDDumpIdent) = 0;

  /* [noscript] void getReportsForThisProcessExtended (in nsIMemoryReporterCallback handleReport, in nsISupports handleReportData, in boolean anonymize, in FILE DMDFile, in nsIFinishReportingCallback finishReporting, in nsISupports finishReportingData); */
  NS_IMETHOD GetReportsForThisProcessExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, bool anonymize, FILE *DMDFile, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData) = 0;

  /* [noscript] void endReport (); */
  NS_IMETHOD EndReport(void) = 0;

  /* [must_use] readonly attribute int64_t vsize; */
  MOZ_MUST_USE NS_IMETHOD GetVsize(int64_t *aVsize) = 0;

  /* [must_use] readonly attribute int64_t vsizeMaxContiguous; */
  MOZ_MUST_USE NS_IMETHOD GetVsizeMaxContiguous(int64_t *aVsizeMaxContiguous) = 0;

  /* [must_use] readonly attribute int64_t resident; */
  MOZ_MUST_USE NS_IMETHOD GetResident(int64_t *aResident) = 0;

  /* [must_use] readonly attribute int64_t residentFast; */
  MOZ_MUST_USE NS_IMETHOD GetResidentFast(int64_t *aResidentFast) = 0;

  /* [must_use] readonly attribute int64_t residentPeak; */
  MOZ_MUST_USE NS_IMETHOD GetResidentPeak(int64_t *aResidentPeak) = 0;

  /* [must_use] readonly attribute int64_t residentUnique; */
  MOZ_MUST_USE NS_IMETHOD GetResidentUnique(int64_t *aResidentUnique) = 0;

  /* [must_use] readonly attribute int64_t heapAllocated; */
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocated(int64_t *aHeapAllocated) = 0;

  /* [must_use] readonly attribute int64_t heapOverheadFraction; */
  MOZ_MUST_USE NS_IMETHOD GetHeapOverheadFraction(int64_t *aHeapOverheadFraction) = 0;

  /* [must_use] readonly attribute int64_t JSMainRuntimeGCHeap; */
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeGCHeap(int64_t *aJSMainRuntimeGCHeap) = 0;

  /* [must_use] readonly attribute int64_t JSMainRuntimeTemporaryPeak; */
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeTemporaryPeak(int64_t *aJSMainRuntimeTemporaryPeak) = 0;

  /* [must_use] readonly attribute int64_t JSMainRuntimeCompartmentsSystem; */
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsSystem(int64_t *aJSMainRuntimeCompartmentsSystem) = 0;

  /* [must_use] readonly attribute int64_t JSMainRuntimeCompartmentsUser; */
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsUser(int64_t *aJSMainRuntimeCompartmentsUser) = 0;

  /* [must_use] readonly attribute int64_t imagesContentUsedUncompressed; */
  MOZ_MUST_USE NS_IMETHOD GetImagesContentUsedUncompressed(int64_t *aImagesContentUsedUncompressed) = 0;

  /* [must_use] readonly attribute int64_t storageSQLite; */
  MOZ_MUST_USE NS_IMETHOD GetStorageSQLite(int64_t *aStorageSQLite) = 0;

  /* [must_use] readonly attribute int64_t lowMemoryEventsVirtual; */
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsVirtual(int64_t *aLowMemoryEventsVirtual) = 0;

  /* [must_use] readonly attribute int64_t lowMemoryEventsPhysical; */
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsPhysical(int64_t *aLowMemoryEventsPhysical) = 0;

  /* [must_use] readonly attribute int64_t ghostWindows; */
  MOZ_MUST_USE NS_IMETHOD GetGhostWindows(int64_t *aGhostWindows) = 0;

  /* [must_use] readonly attribute int64_t pageFaultsHard; */
  MOZ_MUST_USE NS_IMETHOD GetPageFaultsHard(int64_t *aPageFaultsHard) = 0;

  /* [infallible] readonly attribute boolean hasMozMallocUsableSize; */
  NS_IMETHOD GetHasMozMallocUsableSize(bool *aHasMozMallocUsableSize) = 0;
  inline bool GetHasMozMallocUsableSize()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetHasMozMallocUsableSize(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isDMDEnabled; */
  NS_IMETHOD GetIsDMDEnabled(bool *aIsDMDEnabled) = 0;
  inline bool GetIsDMDEnabled()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsDMDEnabled(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [infallible] readonly attribute boolean isDMDRunning; */
  NS_IMETHOD GetIsDMDRunning(bool *aIsDMDRunning) = 0;
  inline bool GetIsDMDRunning()
  {
    bool result;
    mozilla::DebugOnly<nsresult> rv = GetIsDMDRunning(&result);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    return result;
  }

  /* [must_use] void getHeapAllocatedAsync (in nsIHeapAllocatedCallback callback); */
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocatedAsync(nsIHeapAllocatedCallback *callback) = 0;

  /* [must_use] void minimizeMemoryUsage (in nsIRunnable callback); */
  MOZ_MUST_USE NS_IMETHOD MinimizeMemoryUsage(nsIRunnable *callback) = 0;

  /* [must_use] void sizeOfTab (in mozIDOMWindowProxy window, out int64_t jsObjectsSize, out int64_t jsStringsSize, out int64_t jsOtherSize, out int64_t domSize, out int64_t styleSize, out int64_t otherSize, out int64_t totalSize, out double jsMilliseconds, out double nonJSMilliseconds); */
  MOZ_MUST_USE NS_IMETHOD SizeOfTab(mozIDOMWindowProxy *window, int64_t *jsObjectsSize, int64_t *jsStringsSize, int64_t *jsOtherSize, int64_t *domSize, int64_t *styleSize, int64_t *otherSize, int64_t *totalSize, double *jsMilliseconds, double *nonJSMilliseconds) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIMemoryReporterManager, NS_IMEMORYREPORTERMANAGER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIMEMORYREPORTERMANAGER \
  MOZ_MUST_USE NS_IMETHOD Init(void) override; \
  NS_IMETHOD RegisterStrongReporter(nsIMemoryReporter *reporter) override; \
  NS_IMETHOD RegisterStrongAsyncReporter(nsIMemoryReporter *reporter) override; \
  NS_IMETHOD RegisterWeakReporter(nsIMemoryReporter *reporter) override; \
  NS_IMETHOD RegisterWeakAsyncReporter(nsIMemoryReporter *reporter) override; \
  NS_IMETHOD UnregisterStrongReporter(nsIMemoryReporter *reporter) override; \
  NS_IMETHOD UnregisterWeakReporter(nsIMemoryReporter *reporter) override; \
  NS_IMETHOD BlockRegistrationAndHideExistingReporters(void) override; \
  NS_IMETHOD UnblockRegistrationAndRestoreOriginalReporters(void) override; \
  NS_IMETHOD RegisterStrongReporterEvenIfBlocked(nsIMemoryReporter *aReporter) override; \
  NS_IMETHOD GetReports(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize) override; \
  NS_IMETHOD GetReportsExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize, bool minimizeMemoryUsage, const nsAString & DMDDumpIdent) override; \
  NS_IMETHOD GetReportsForThisProcessExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, bool anonymize, FILE *DMDFile, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData) override; \
  NS_IMETHOD EndReport(void) override; \
  MOZ_MUST_USE NS_IMETHOD GetVsize(int64_t *aVsize) override; \
  MOZ_MUST_USE NS_IMETHOD GetVsizeMaxContiguous(int64_t *aVsizeMaxContiguous) override; \
  MOZ_MUST_USE NS_IMETHOD GetResident(int64_t *aResident) override; \
  MOZ_MUST_USE NS_IMETHOD GetResidentFast(int64_t *aResidentFast) override; \
  MOZ_MUST_USE NS_IMETHOD GetResidentPeak(int64_t *aResidentPeak) override; \
  MOZ_MUST_USE NS_IMETHOD GetResidentUnique(int64_t *aResidentUnique) override; \
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocated(int64_t *aHeapAllocated) override; \
  MOZ_MUST_USE NS_IMETHOD GetHeapOverheadFraction(int64_t *aHeapOverheadFraction) override; \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeGCHeap(int64_t *aJSMainRuntimeGCHeap) override; \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeTemporaryPeak(int64_t *aJSMainRuntimeTemporaryPeak) override; \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsSystem(int64_t *aJSMainRuntimeCompartmentsSystem) override; \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsUser(int64_t *aJSMainRuntimeCompartmentsUser) override; \
  MOZ_MUST_USE NS_IMETHOD GetImagesContentUsedUncompressed(int64_t *aImagesContentUsedUncompressed) override; \
  MOZ_MUST_USE NS_IMETHOD GetStorageSQLite(int64_t *aStorageSQLite) override; \
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsVirtual(int64_t *aLowMemoryEventsVirtual) override; \
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsPhysical(int64_t *aLowMemoryEventsPhysical) override; \
  MOZ_MUST_USE NS_IMETHOD GetGhostWindows(int64_t *aGhostWindows) override; \
  MOZ_MUST_USE NS_IMETHOD GetPageFaultsHard(int64_t *aPageFaultsHard) override; \
  using nsIMemoryReporterManager::GetHasMozMallocUsableSize; \
  NS_IMETHOD GetHasMozMallocUsableSize(bool *aHasMozMallocUsableSize) override; \
  using nsIMemoryReporterManager::GetIsDMDEnabled; \
  NS_IMETHOD GetIsDMDEnabled(bool *aIsDMDEnabled) override; \
  using nsIMemoryReporterManager::GetIsDMDRunning; \
  NS_IMETHOD GetIsDMDRunning(bool *aIsDMDRunning) override; \
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocatedAsync(nsIHeapAllocatedCallback *callback) override; \
  MOZ_MUST_USE NS_IMETHOD MinimizeMemoryUsage(nsIRunnable *callback) override; \
  MOZ_MUST_USE NS_IMETHOD SizeOfTab(mozIDOMWindowProxy *window, int64_t *jsObjectsSize, int64_t *jsStringsSize, int64_t *jsOtherSize, int64_t *domSize, int64_t *styleSize, int64_t *otherSize, int64_t *totalSize, double *jsMilliseconds, double *nonJSMilliseconds) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIMEMORYREPORTERMANAGER \
  MOZ_MUST_USE nsresult Init(void); \
  nsresult RegisterStrongReporter(nsIMemoryReporter *reporter); \
  nsresult RegisterStrongAsyncReporter(nsIMemoryReporter *reporter); \
  nsresult RegisterWeakReporter(nsIMemoryReporter *reporter); \
  nsresult RegisterWeakAsyncReporter(nsIMemoryReporter *reporter); \
  nsresult UnregisterStrongReporter(nsIMemoryReporter *reporter); \
  nsresult UnregisterWeakReporter(nsIMemoryReporter *reporter); \
  nsresult BlockRegistrationAndHideExistingReporters(void); \
  nsresult UnblockRegistrationAndRestoreOriginalReporters(void); \
  nsresult RegisterStrongReporterEvenIfBlocked(nsIMemoryReporter *aReporter); \
  nsresult GetReports(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize); \
  nsresult GetReportsExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize, bool minimizeMemoryUsage, const nsAString & DMDDumpIdent); \
  nsresult GetReportsForThisProcessExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, bool anonymize, FILE *DMDFile, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData); \
  nsresult EndReport(void); \
  MOZ_MUST_USE nsresult GetVsize(int64_t *aVsize); \
  MOZ_MUST_USE nsresult GetVsizeMaxContiguous(int64_t *aVsizeMaxContiguous); \
  MOZ_MUST_USE nsresult GetResident(int64_t *aResident); \
  MOZ_MUST_USE nsresult GetResidentFast(int64_t *aResidentFast); \
  MOZ_MUST_USE nsresult GetResidentPeak(int64_t *aResidentPeak); \
  MOZ_MUST_USE nsresult GetResidentUnique(int64_t *aResidentUnique); \
  MOZ_MUST_USE nsresult GetHeapAllocated(int64_t *aHeapAllocated); \
  MOZ_MUST_USE nsresult GetHeapOverheadFraction(int64_t *aHeapOverheadFraction); \
  MOZ_MUST_USE nsresult GetJSMainRuntimeGCHeap(int64_t *aJSMainRuntimeGCHeap); \
  MOZ_MUST_USE nsresult GetJSMainRuntimeTemporaryPeak(int64_t *aJSMainRuntimeTemporaryPeak); \
  MOZ_MUST_USE nsresult GetJSMainRuntimeCompartmentsSystem(int64_t *aJSMainRuntimeCompartmentsSystem); \
  MOZ_MUST_USE nsresult GetJSMainRuntimeCompartmentsUser(int64_t *aJSMainRuntimeCompartmentsUser); \
  MOZ_MUST_USE nsresult GetImagesContentUsedUncompressed(int64_t *aImagesContentUsedUncompressed); \
  MOZ_MUST_USE nsresult GetStorageSQLite(int64_t *aStorageSQLite); \
  MOZ_MUST_USE nsresult GetLowMemoryEventsVirtual(int64_t *aLowMemoryEventsVirtual); \
  MOZ_MUST_USE nsresult GetLowMemoryEventsPhysical(int64_t *aLowMemoryEventsPhysical); \
  MOZ_MUST_USE nsresult GetGhostWindows(int64_t *aGhostWindows); \
  MOZ_MUST_USE nsresult GetPageFaultsHard(int64_t *aPageFaultsHard); \
  using nsIMemoryReporterManager::GetHasMozMallocUsableSize; \
  nsresult GetHasMozMallocUsableSize(bool *aHasMozMallocUsableSize); \
  using nsIMemoryReporterManager::GetIsDMDEnabled; \
  nsresult GetIsDMDEnabled(bool *aIsDMDEnabled); \
  using nsIMemoryReporterManager::GetIsDMDRunning; \
  nsresult GetIsDMDRunning(bool *aIsDMDRunning); \
  MOZ_MUST_USE nsresult GetHeapAllocatedAsync(nsIHeapAllocatedCallback *callback); \
  MOZ_MUST_USE nsresult MinimizeMemoryUsage(nsIRunnable *callback); \
  MOZ_MUST_USE nsresult SizeOfTab(mozIDOMWindowProxy *window, int64_t *jsObjectsSize, int64_t *jsStringsSize, int64_t *jsOtherSize, int64_t *domSize, int64_t *styleSize, int64_t *otherSize, int64_t *totalSize, double *jsMilliseconds, double *nonJSMilliseconds); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIMEMORYREPORTERMANAGER(_to) \
  MOZ_MUST_USE NS_IMETHOD Init(void) override { return _to Init(); } \
  NS_IMETHOD RegisterStrongReporter(nsIMemoryReporter *reporter) override { return _to RegisterStrongReporter(reporter); } \
  NS_IMETHOD RegisterStrongAsyncReporter(nsIMemoryReporter *reporter) override { return _to RegisterStrongAsyncReporter(reporter); } \
  NS_IMETHOD RegisterWeakReporter(nsIMemoryReporter *reporter) override { return _to RegisterWeakReporter(reporter); } \
  NS_IMETHOD RegisterWeakAsyncReporter(nsIMemoryReporter *reporter) override { return _to RegisterWeakAsyncReporter(reporter); } \
  NS_IMETHOD UnregisterStrongReporter(nsIMemoryReporter *reporter) override { return _to UnregisterStrongReporter(reporter); } \
  NS_IMETHOD UnregisterWeakReporter(nsIMemoryReporter *reporter) override { return _to UnregisterWeakReporter(reporter); } \
  NS_IMETHOD BlockRegistrationAndHideExistingReporters(void) override { return _to BlockRegistrationAndHideExistingReporters(); } \
  NS_IMETHOD UnblockRegistrationAndRestoreOriginalReporters(void) override { return _to UnblockRegistrationAndRestoreOriginalReporters(); } \
  NS_IMETHOD RegisterStrongReporterEvenIfBlocked(nsIMemoryReporter *aReporter) override { return _to RegisterStrongReporterEvenIfBlocked(aReporter); } \
  NS_IMETHOD GetReports(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize) override { return _to GetReports(handleReport, handleReportData, finishReporting, finishReportingData, anonymize); } \
  NS_IMETHOD GetReportsExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize, bool minimizeMemoryUsage, const nsAString & DMDDumpIdent) override { return _to GetReportsExtended(handleReport, handleReportData, finishReporting, finishReportingData, anonymize, minimizeMemoryUsage, DMDDumpIdent); } \
  NS_IMETHOD GetReportsForThisProcessExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, bool anonymize, FILE *DMDFile, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData) override { return _to GetReportsForThisProcessExtended(handleReport, handleReportData, anonymize, DMDFile, finishReporting, finishReportingData); } \
  NS_IMETHOD EndReport(void) override { return _to EndReport(); } \
  MOZ_MUST_USE NS_IMETHOD GetVsize(int64_t *aVsize) override { return _to GetVsize(aVsize); } \
  MOZ_MUST_USE NS_IMETHOD GetVsizeMaxContiguous(int64_t *aVsizeMaxContiguous) override { return _to GetVsizeMaxContiguous(aVsizeMaxContiguous); } \
  MOZ_MUST_USE NS_IMETHOD GetResident(int64_t *aResident) override { return _to GetResident(aResident); } \
  MOZ_MUST_USE NS_IMETHOD GetResidentFast(int64_t *aResidentFast) override { return _to GetResidentFast(aResidentFast); } \
  MOZ_MUST_USE NS_IMETHOD GetResidentPeak(int64_t *aResidentPeak) override { return _to GetResidentPeak(aResidentPeak); } \
  MOZ_MUST_USE NS_IMETHOD GetResidentUnique(int64_t *aResidentUnique) override { return _to GetResidentUnique(aResidentUnique); } \
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocated(int64_t *aHeapAllocated) override { return _to GetHeapAllocated(aHeapAllocated); } \
  MOZ_MUST_USE NS_IMETHOD GetHeapOverheadFraction(int64_t *aHeapOverheadFraction) override { return _to GetHeapOverheadFraction(aHeapOverheadFraction); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeGCHeap(int64_t *aJSMainRuntimeGCHeap) override { return _to GetJSMainRuntimeGCHeap(aJSMainRuntimeGCHeap); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeTemporaryPeak(int64_t *aJSMainRuntimeTemporaryPeak) override { return _to GetJSMainRuntimeTemporaryPeak(aJSMainRuntimeTemporaryPeak); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsSystem(int64_t *aJSMainRuntimeCompartmentsSystem) override { return _to GetJSMainRuntimeCompartmentsSystem(aJSMainRuntimeCompartmentsSystem); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsUser(int64_t *aJSMainRuntimeCompartmentsUser) override { return _to GetJSMainRuntimeCompartmentsUser(aJSMainRuntimeCompartmentsUser); } \
  MOZ_MUST_USE NS_IMETHOD GetImagesContentUsedUncompressed(int64_t *aImagesContentUsedUncompressed) override { return _to GetImagesContentUsedUncompressed(aImagesContentUsedUncompressed); } \
  MOZ_MUST_USE NS_IMETHOD GetStorageSQLite(int64_t *aStorageSQLite) override { return _to GetStorageSQLite(aStorageSQLite); } \
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsVirtual(int64_t *aLowMemoryEventsVirtual) override { return _to GetLowMemoryEventsVirtual(aLowMemoryEventsVirtual); } \
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsPhysical(int64_t *aLowMemoryEventsPhysical) override { return _to GetLowMemoryEventsPhysical(aLowMemoryEventsPhysical); } \
  MOZ_MUST_USE NS_IMETHOD GetGhostWindows(int64_t *aGhostWindows) override { return _to GetGhostWindows(aGhostWindows); } \
  MOZ_MUST_USE NS_IMETHOD GetPageFaultsHard(int64_t *aPageFaultsHard) override { return _to GetPageFaultsHard(aPageFaultsHard); } \
  using nsIMemoryReporterManager::GetHasMozMallocUsableSize; \
  NS_IMETHOD GetHasMozMallocUsableSize(bool *aHasMozMallocUsableSize) override { return _to GetHasMozMallocUsableSize(aHasMozMallocUsableSize); } \
  using nsIMemoryReporterManager::GetIsDMDEnabled; \
  NS_IMETHOD GetIsDMDEnabled(bool *aIsDMDEnabled) override { return _to GetIsDMDEnabled(aIsDMDEnabled); } \
  using nsIMemoryReporterManager::GetIsDMDRunning; \
  NS_IMETHOD GetIsDMDRunning(bool *aIsDMDRunning) override { return _to GetIsDMDRunning(aIsDMDRunning); } \
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocatedAsync(nsIHeapAllocatedCallback *callback) override { return _to GetHeapAllocatedAsync(callback); } \
  MOZ_MUST_USE NS_IMETHOD MinimizeMemoryUsage(nsIRunnable *callback) override { return _to MinimizeMemoryUsage(callback); } \
  MOZ_MUST_USE NS_IMETHOD SizeOfTab(mozIDOMWindowProxy *window, int64_t *jsObjectsSize, int64_t *jsStringsSize, int64_t *jsOtherSize, int64_t *domSize, int64_t *styleSize, int64_t *otherSize, int64_t *totalSize, double *jsMilliseconds, double *nonJSMilliseconds) override { return _to SizeOfTab(window, jsObjectsSize, jsStringsSize, jsOtherSize, domSize, styleSize, otherSize, totalSize, jsMilliseconds, nonJSMilliseconds); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIMEMORYREPORTERMANAGER(_to) \
  MOZ_MUST_USE NS_IMETHOD Init(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(); } \
  NS_IMETHOD RegisterStrongReporter(nsIMemoryReporter *reporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterStrongReporter(reporter); } \
  NS_IMETHOD RegisterStrongAsyncReporter(nsIMemoryReporter *reporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterStrongAsyncReporter(reporter); } \
  NS_IMETHOD RegisterWeakReporter(nsIMemoryReporter *reporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterWeakReporter(reporter); } \
  NS_IMETHOD RegisterWeakAsyncReporter(nsIMemoryReporter *reporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterWeakAsyncReporter(reporter); } \
  NS_IMETHOD UnregisterStrongReporter(nsIMemoryReporter *reporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterStrongReporter(reporter); } \
  NS_IMETHOD UnregisterWeakReporter(nsIMemoryReporter *reporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnregisterWeakReporter(reporter); } \
  NS_IMETHOD BlockRegistrationAndHideExistingReporters(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->BlockRegistrationAndHideExistingReporters(); } \
  NS_IMETHOD UnblockRegistrationAndRestoreOriginalReporters(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->UnblockRegistrationAndRestoreOriginalReporters(); } \
  NS_IMETHOD RegisterStrongReporterEvenIfBlocked(nsIMemoryReporter *aReporter) override { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterStrongReporterEvenIfBlocked(aReporter); } \
  NS_IMETHOD GetReports(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReports(handleReport, handleReportData, finishReporting, finishReportingData, anonymize); } \
  NS_IMETHOD GetReportsExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData, bool anonymize, bool minimizeMemoryUsage, const nsAString & DMDDumpIdent) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReportsExtended(handleReport, handleReportData, finishReporting, finishReportingData, anonymize, minimizeMemoryUsage, DMDDumpIdent); } \
  NS_IMETHOD GetReportsForThisProcessExtended(nsIMemoryReporterCallback *handleReport, nsISupports *handleReportData, bool anonymize, FILE *DMDFile, nsIFinishReportingCallback *finishReporting, nsISupports *finishReportingData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReportsForThisProcessExtended(handleReport, handleReportData, anonymize, DMDFile, finishReporting, finishReportingData); } \
  NS_IMETHOD EndReport(void) override { return !_to ? NS_ERROR_NULL_POINTER : _to->EndReport(); } \
  MOZ_MUST_USE NS_IMETHOD GetVsize(int64_t *aVsize) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVsize(aVsize); } \
  MOZ_MUST_USE NS_IMETHOD GetVsizeMaxContiguous(int64_t *aVsizeMaxContiguous) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVsizeMaxContiguous(aVsizeMaxContiguous); } \
  MOZ_MUST_USE NS_IMETHOD GetResident(int64_t *aResident) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResident(aResident); } \
  MOZ_MUST_USE NS_IMETHOD GetResidentFast(int64_t *aResidentFast) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResidentFast(aResidentFast); } \
  MOZ_MUST_USE NS_IMETHOD GetResidentPeak(int64_t *aResidentPeak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResidentPeak(aResidentPeak); } \
  MOZ_MUST_USE NS_IMETHOD GetResidentUnique(int64_t *aResidentUnique) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResidentUnique(aResidentUnique); } \
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocated(int64_t *aHeapAllocated) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeapAllocated(aHeapAllocated); } \
  MOZ_MUST_USE NS_IMETHOD GetHeapOverheadFraction(int64_t *aHeapOverheadFraction) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeapOverheadFraction(aHeapOverheadFraction); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeGCHeap(int64_t *aJSMainRuntimeGCHeap) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetJSMainRuntimeGCHeap(aJSMainRuntimeGCHeap); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeTemporaryPeak(int64_t *aJSMainRuntimeTemporaryPeak) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetJSMainRuntimeTemporaryPeak(aJSMainRuntimeTemporaryPeak); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsSystem(int64_t *aJSMainRuntimeCompartmentsSystem) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetJSMainRuntimeCompartmentsSystem(aJSMainRuntimeCompartmentsSystem); } \
  MOZ_MUST_USE NS_IMETHOD GetJSMainRuntimeCompartmentsUser(int64_t *aJSMainRuntimeCompartmentsUser) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetJSMainRuntimeCompartmentsUser(aJSMainRuntimeCompartmentsUser); } \
  MOZ_MUST_USE NS_IMETHOD GetImagesContentUsedUncompressed(int64_t *aImagesContentUsedUncompressed) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetImagesContentUsedUncompressed(aImagesContentUsedUncompressed); } \
  MOZ_MUST_USE NS_IMETHOD GetStorageSQLite(int64_t *aStorageSQLite) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStorageSQLite(aStorageSQLite); } \
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsVirtual(int64_t *aLowMemoryEventsVirtual) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLowMemoryEventsVirtual(aLowMemoryEventsVirtual); } \
  MOZ_MUST_USE NS_IMETHOD GetLowMemoryEventsPhysical(int64_t *aLowMemoryEventsPhysical) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLowMemoryEventsPhysical(aLowMemoryEventsPhysical); } \
  MOZ_MUST_USE NS_IMETHOD GetGhostWindows(int64_t *aGhostWindows) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetGhostWindows(aGhostWindows); } \
  MOZ_MUST_USE NS_IMETHOD GetPageFaultsHard(int64_t *aPageFaultsHard) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPageFaultsHard(aPageFaultsHard); } \
  NS_IMETHOD GetHasMozMallocUsableSize(bool *aHasMozMallocUsableSize) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHasMozMallocUsableSize(aHasMozMallocUsableSize); } \
  NS_IMETHOD GetIsDMDEnabled(bool *aIsDMDEnabled) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsDMDEnabled(aIsDMDEnabled); } \
  NS_IMETHOD GetIsDMDRunning(bool *aIsDMDRunning) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsDMDRunning(aIsDMDRunning); } \
  MOZ_MUST_USE NS_IMETHOD GetHeapAllocatedAsync(nsIHeapAllocatedCallback *callback) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeapAllocatedAsync(callback); } \
  MOZ_MUST_USE NS_IMETHOD MinimizeMemoryUsage(nsIRunnable *callback) override { return !_to ? NS_ERROR_NULL_POINTER : _to->MinimizeMemoryUsage(callback); } \
  MOZ_MUST_USE NS_IMETHOD SizeOfTab(mozIDOMWindowProxy *window, int64_t *jsObjectsSize, int64_t *jsStringsSize, int64_t *jsOtherSize, int64_t *domSize, int64_t *styleSize, int64_t *otherSize, int64_t *totalSize, double *jsMilliseconds, double *nonJSMilliseconds) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SizeOfTab(window, jsObjectsSize, jsStringsSize, jsOtherSize, domSize, styleSize, otherSize, totalSize, jsMilliseconds, nonJSMilliseconds); } 


/* #include "js/TypeDecls.h" */
#include "nsString.h"
#include "nsTArray.h"
class nsPIDOMWindowOuter;
// nsIHandleReportCallback is a better name, but keep nsIMemoryReporterCallback
// around for backwards compatibility.
typedef nsIMemoryReporterCallback nsIHandleReportCallback;
namespace mozilla {
// All the following registration/unregistration functions don't use
// MOZ_MUST_USE because ignoring failures is common and reasonable.
// Register a memory reporter.  The manager service will hold a strong
// reference to this reporter.
XPCOM_API(nsresult) RegisterStrongMemoryReporter(nsIMemoryReporter* aReporter);
XPCOM_API(nsresult) RegisterStrongAsyncMemoryReporter(nsIMemoryReporter* aReporter);
// Register a memory reporter.  The manager service will hold a weak reference
// to this reporter.
XPCOM_API(nsresult) RegisterWeakMemoryReporter(nsIMemoryReporter* aReporter);
XPCOM_API(nsresult) RegisterWeakAsyncMemoryReporter(nsIMemoryReporter* aReporter);
// Unregister a strong memory reporter.
XPCOM_API(nsresult) UnregisterStrongMemoryReporter(nsIMemoryReporter* aReporter);
// Unregister a weak memory reporter.
XPCOM_API(nsresult) UnregisterWeakMemoryReporter(nsIMemoryReporter* aReporter);
// The memory reporter manager provides access to several distinguished
// amounts via attributes.  Some of these amounts are provided by Gecko
// components that cannot be accessed directly from XPCOM code.  So we provide
// the following functions for those components to be registered with the
// manager.
typedef int64_t (*InfallibleAmountFn)();
#define DECL_REGISTER_DISTINGUISHED_AMOUNT(kind, name) \
    nsresult Register##name##DistinguishedAmount(kind##AmountFn aAmountFn);
#define DECL_UNREGISTER_DISTINGUISHED_AMOUNT(name) \
    nsresult Unregister##name##DistinguishedAmount();
/* DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, JSMainRuntimeGCHeap) */
/* DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, JSMainRuntimeTemporaryPeak) */
/* DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, JSMainRuntimeCompartmentsSystem) */
/* DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, JSMainRuntimeCompartmentsUser) */
DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, ImagesContentUsedUncompressed)
DECL_UNREGISTER_DISTINGUISHED_AMOUNT(ImagesContentUsedUncompressed)
DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, StorageSQLite)
DECL_UNREGISTER_DISTINGUISHED_AMOUNT(StorageSQLite)
DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, LowMemoryEventsVirtual)
DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, LowMemoryEventsPhysical)
DECL_REGISTER_DISTINGUISHED_AMOUNT(Infallible, GhostWindows)
#undef DECL_REGISTER_DISTINGUISHED_AMOUNT
#undef DECL_UNREGISTER_DISTINGUISHED_AMOUNT
// Likewise for per-tab measurement.
/* typedef MOZ_MUST_USE nsresult (*JSSizeOfTabFn)(JSObject* aObj, */
/*                                                size_t* aJsObjectsSize, */
/*                                                size_t* aJsStringSize, */
/*                                                size_t* aJsPrivateSize, */
/*                                                size_t* aJsOtherSize); */
/* typedef MOZ_MUST_USE nsresult (*NonJSSizeOfTabFn)(nsPIDOMWindowOuter* aWindow, */
/*                                                   size_t* aDomSize, */
/*                                                   size_t* aStyleSize, */
/*                                                   size_t* aOtherSize); */
/* nsresult RegisterJSSizeOfTab(JSSizeOfTabFn aSizeOfTabFn); */
/* nsresult RegisterNonJSSizeOfTab(NonJSSizeOfTabFn aSizeOfTabFn); */
}
#if defined(MOZ_DMD)
#if !defined(MOZ_MEMORY)
#error "MOZ_DMD requires MOZ_MEMORY"
#endif
#include "DMD.h"
#define MOZ_REPORT(ptr)          mozilla::dmd::Report(ptr)
#define MOZ_REPORT_ON_ALLOC(ptr) mozilla::dmd::ReportOnAlloc(ptr)
#else
#define MOZ_REPORT(ptr)
#define MOZ_REPORT_ON_ALLOC(ptr)
#endif  // defined(MOZ_DMD)
// Functions generated via this macro should be used by all traversal-based
// memory reporters.  Such functions return |moz_malloc_size_of(ptr)|;  this
// will always be zero on some obscure platforms.
//
// You might be wondering why we have a macro that creates multiple functions
// that differ only in their name, instead of a single MallocSizeOf function.
// It's mostly to help with DMD integration, though it sometimes also helps
// with debugging and temporary ad hoc profiling.  The function name chosen
// doesn't matter greatly, but it's best to make it similar to the path used by
// the relevant memory reporter(s).
#define MOZ_DEFINE_MALLOC_SIZE_OF(fn)                                         \
  static size_t fn(const void* aPtr)                                          \
  {                                                                           \
      MOZ_REPORT(aPtr);                                                       \
      return moz_malloc_size_of(aPtr);                                        \
  }
// This is an alternative to MOZ_DEFINE_MALLOC_SIZE_OF that defines a
// MallocSizeOf function that can handle interior pointers.
#ifdef MOZ_MEMORY
#include "mozmemory.h"
#define MOZ_DEFINE_MALLOC_ENCLOSING_SIZE_OF(fn)                               \
  static size_t fn(const void* aPtr)                                          \
  {                                                                           \
      jemalloc_ptr_info_t info;                                               \
      jemalloc_ptr_info(aPtr, &info);                                         \
      MOZ_REPORT(info.addr);                                                  \
      return jemalloc_ptr_is_live(&info) ? info.size : 0;                     \
  }
#else
#define MOZ_DEFINE_MALLOC_ENCLOSING_SIZE_OF(fn)                               \
  static size_t fn(const void* aPtr)                                          \
  {                                                                           \
      return 0;                                                               \
  }
#endif
// Functions generated by the next two macros should be used by wrapping
// allocators that report heap blocks as soon as they are allocated and
// unreport them as soon as they are freed.  Such allocators are used in cases
// where we have third-party code that we cannot modify.  The two functions
// must always be used in tandem.
#define MOZ_DEFINE_MALLOC_SIZE_OF_ON_ALLOC(fn)                                \
  static size_t fn(const void* aPtr)                                          \
  {                                                                           \
      MOZ_REPORT_ON_ALLOC(aPtr);                                              \
      return moz_malloc_size_of(aPtr);                                        \
  }
#define MOZ_DEFINE_MALLOC_SIZE_OF_ON_FREE(fn)                                 \
  static size_t fn(const void* aPtr)                                          \
  {                                                                           \
      return moz_malloc_size_of(aPtr);                                        \
  }
// This macro assumes the presence of appropriate |aHandleReport| and |aData|
// variables. The (void) is there because we should always ignore the return
// value of the callback, because callback failures aren't fatal.
#define MOZ_COLLECT_REPORT(path, kind, units, amount, description)            \
  (void)aHandleReport->Callback(EmptyCString(), NS_LITERAL_CSTRING(path),     \
                                kind, units, amount,                          \
                                NS_LITERAL_CSTRING(description), aData)

#endif /* __gen_nsIMemoryReporter_h__ */
