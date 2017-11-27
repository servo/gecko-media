/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsILocalFileMac.idl
 */

#ifndef __gen_nsILocalFileMac_h__
#define __gen_nsILocalFileMac_h__


#ifndef __gen_nsIFile_h__
#include "nsIFile.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

/* starting interface:    nsILocalFileMac */
#define NS_ILOCALFILEMAC_IID_STR "623eca5b-c25d-4e27-be5a-789a66c4b2f7"

#define NS_ILOCALFILEMAC_IID \
  {0x623eca5b, 0xc25d, 0x4e27, \
    { 0xbe, 0x5a, 0x78, 0x9a, 0x66, 0xc4, 0xb2, 0xf7 }}

class NS_NO_VTABLE nsILocalFileMac : public nsIFile {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ILOCALFILEMAC_IID)

  /* [noscript] void initWithCFURL (in CFURLRef aCFURL); */
  NS_IMETHOD InitWithCFURL(CFURLRef aCFURL) = 0;

  /* [noscript] void initWithFSRef ([const] in FSRefPtr aFSRef); */
  NS_IMETHOD InitWithFSRef(const FSRef *aFSRef) = 0;

  /* [noscript] CFURLRef getCFURL (); */
  NS_IMETHOD GetCFURL(CFURLRef *_retval) = 0;

  /* [noscript] FSRef getFSRef (); */
  NS_IMETHOD GetFSRef(FSRef *_retval) = 0;

  /* [noscript] FSSpec getFSSpec (); */
  NS_IMETHOD GetFSSpec(FSSpec *_retval) = 0;

  /* readonly attribute int64_t fileSizeWithResFork; */
  NS_IMETHOD GetFileSizeWithResFork(int64_t *aFileSizeWithResFork) = 0;

  /* [noscript] attribute OSType fileType; */
  NS_IMETHOD GetFileType(OSType *aFileType) = 0;
  NS_IMETHOD SetFileType(OSType aFileType) = 0;

  /* [noscript] attribute OSType fileCreator; */
  NS_IMETHOD GetFileCreator(OSType *aFileCreator) = 0;
  NS_IMETHOD SetFileCreator(OSType aFileCreator) = 0;

  /* void launchWithDoc (in nsIFile aDocToLoad, in boolean aLaunchInBackground); */
  NS_IMETHOD LaunchWithDoc(nsIFile *aDocToLoad, bool aLaunchInBackground) = 0;

  /* void openDocWithApp (in nsIFile aAppToOpenWith, in boolean aLaunchInBackground); */
  NS_IMETHOD OpenDocWithApp(nsIFile *aAppToOpenWith, bool aLaunchInBackground) = 0;

  /* boolean isPackage (); */
  NS_IMETHOD IsPackage(bool *_retval) = 0;

  /* readonly attribute AString bundleDisplayName; */
  NS_IMETHOD GetBundleDisplayName(nsAString & aBundleDisplayName) = 0;

  /* readonly attribute AUTF8String bundleIdentifier; */
  NS_IMETHOD GetBundleIdentifier(nsACString & aBundleIdentifier) = 0;

  /* readonly attribute int64_t bundleContentsLastModifiedTime; */
  NS_IMETHOD GetBundleContentsLastModifiedTime(int64_t *aBundleContentsLastModifiedTime) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsILocalFileMac, NS_ILOCALFILEMAC_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSILOCALFILEMAC \
  NS_IMETHOD InitWithCFURL(CFURLRef aCFURL) override; \
  NS_IMETHOD InitWithFSRef(const FSRef *aFSRef) override; \
  NS_IMETHOD GetCFURL(CFURLRef *_retval) override; \
  NS_IMETHOD GetFSRef(FSRef *_retval) override; \
  NS_IMETHOD GetFSSpec(FSSpec *_retval) override; \
  NS_IMETHOD GetFileSizeWithResFork(int64_t *aFileSizeWithResFork) override; \
  NS_IMETHOD GetFileType(OSType *aFileType) override; \
  NS_IMETHOD SetFileType(OSType aFileType) override; \
  NS_IMETHOD GetFileCreator(OSType *aFileCreator) override; \
  NS_IMETHOD SetFileCreator(OSType aFileCreator) override; \
  NS_IMETHOD LaunchWithDoc(nsIFile *aDocToLoad, bool aLaunchInBackground) override; \
  NS_IMETHOD OpenDocWithApp(nsIFile *aAppToOpenWith, bool aLaunchInBackground) override; \
  NS_IMETHOD IsPackage(bool *_retval) override; \
  NS_IMETHOD GetBundleDisplayName(nsAString & aBundleDisplayName) override; \
  NS_IMETHOD GetBundleIdentifier(nsACString & aBundleIdentifier) override; \
  NS_IMETHOD GetBundleContentsLastModifiedTime(int64_t *aBundleContentsLastModifiedTime) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSILOCALFILEMAC \
  nsresult InitWithCFURL(CFURLRef aCFURL); \
  nsresult InitWithFSRef(const FSRef *aFSRef); \
  nsresult GetCFURL(CFURLRef *_retval); \
  nsresult GetFSRef(FSRef *_retval); \
  nsresult GetFSSpec(FSSpec *_retval); \
  nsresult GetFileSizeWithResFork(int64_t *aFileSizeWithResFork); \
  nsresult GetFileType(OSType *aFileType); \
  nsresult SetFileType(OSType aFileType); \
  nsresult GetFileCreator(OSType *aFileCreator); \
  nsresult SetFileCreator(OSType aFileCreator); \
  nsresult LaunchWithDoc(nsIFile *aDocToLoad, bool aLaunchInBackground); \
  nsresult OpenDocWithApp(nsIFile *aAppToOpenWith, bool aLaunchInBackground); \
  nsresult IsPackage(bool *_retval); \
  nsresult GetBundleDisplayName(nsAString & aBundleDisplayName); \
  nsresult GetBundleIdentifier(nsACString & aBundleIdentifier); \
  nsresult GetBundleContentsLastModifiedTime(int64_t *aBundleContentsLastModifiedTime); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSILOCALFILEMAC(_to) \
  NS_IMETHOD InitWithCFURL(CFURLRef aCFURL) override { return _to InitWithCFURL(aCFURL); } \
  NS_IMETHOD InitWithFSRef(const FSRef *aFSRef) override { return _to InitWithFSRef(aFSRef); } \
  NS_IMETHOD GetCFURL(CFURLRef *_retval) override { return _to GetCFURL(_retval); } \
  NS_IMETHOD GetFSRef(FSRef *_retval) override { return _to GetFSRef(_retval); } \
  NS_IMETHOD GetFSSpec(FSSpec *_retval) override { return _to GetFSSpec(_retval); } \
  NS_IMETHOD GetFileSizeWithResFork(int64_t *aFileSizeWithResFork) override { return _to GetFileSizeWithResFork(aFileSizeWithResFork); } \
  NS_IMETHOD GetFileType(OSType *aFileType) override { return _to GetFileType(aFileType); } \
  NS_IMETHOD SetFileType(OSType aFileType) override { return _to SetFileType(aFileType); } \
  NS_IMETHOD GetFileCreator(OSType *aFileCreator) override { return _to GetFileCreator(aFileCreator); } \
  NS_IMETHOD SetFileCreator(OSType aFileCreator) override { return _to SetFileCreator(aFileCreator); } \
  NS_IMETHOD LaunchWithDoc(nsIFile *aDocToLoad, bool aLaunchInBackground) override { return _to LaunchWithDoc(aDocToLoad, aLaunchInBackground); } \
  NS_IMETHOD OpenDocWithApp(nsIFile *aAppToOpenWith, bool aLaunchInBackground) override { return _to OpenDocWithApp(aAppToOpenWith, aLaunchInBackground); } \
  NS_IMETHOD IsPackage(bool *_retval) override { return _to IsPackage(_retval); } \
  NS_IMETHOD GetBundleDisplayName(nsAString & aBundleDisplayName) override { return _to GetBundleDisplayName(aBundleDisplayName); } \
  NS_IMETHOD GetBundleIdentifier(nsACString & aBundleIdentifier) override { return _to GetBundleIdentifier(aBundleIdentifier); } \
  NS_IMETHOD GetBundleContentsLastModifiedTime(int64_t *aBundleContentsLastModifiedTime) override { return _to GetBundleContentsLastModifiedTime(aBundleContentsLastModifiedTime); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSILOCALFILEMAC(_to) \
  NS_IMETHOD InitWithCFURL(CFURLRef aCFURL) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InitWithCFURL(aCFURL); } \
  NS_IMETHOD InitWithFSRef(const FSRef *aFSRef) override { return !_to ? NS_ERROR_NULL_POINTER : _to->InitWithFSRef(aFSRef); } \
  NS_IMETHOD GetCFURL(CFURLRef *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCFURL(_retval); } \
  NS_IMETHOD GetFSRef(FSRef *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFSRef(_retval); } \
  NS_IMETHOD GetFSSpec(FSSpec *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFSSpec(_retval); } \
  NS_IMETHOD GetFileSizeWithResFork(int64_t *aFileSizeWithResFork) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFileSizeWithResFork(aFileSizeWithResFork); } \
  NS_IMETHOD GetFileType(OSType *aFileType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFileType(aFileType); } \
  NS_IMETHOD SetFileType(OSType aFileType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFileType(aFileType); } \
  NS_IMETHOD GetFileCreator(OSType *aFileCreator) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFileCreator(aFileCreator); } \
  NS_IMETHOD SetFileCreator(OSType aFileCreator) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFileCreator(aFileCreator); } \
  NS_IMETHOD LaunchWithDoc(nsIFile *aDocToLoad, bool aLaunchInBackground) override { return !_to ? NS_ERROR_NULL_POINTER : _to->LaunchWithDoc(aDocToLoad, aLaunchInBackground); } \
  NS_IMETHOD OpenDocWithApp(nsIFile *aAppToOpenWith, bool aLaunchInBackground) override { return !_to ? NS_ERROR_NULL_POINTER : _to->OpenDocWithApp(aAppToOpenWith, aLaunchInBackground); } \
  NS_IMETHOD IsPackage(bool *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->IsPackage(_retval); } \
  NS_IMETHOD GetBundleDisplayName(nsAString & aBundleDisplayName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBundleDisplayName(aBundleDisplayName); } \
  NS_IMETHOD GetBundleIdentifier(nsACString & aBundleIdentifier) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBundleIdentifier(aBundleIdentifier); } \
  NS_IMETHOD GetBundleContentsLastModifiedTime(int64_t *aBundleContentsLastModifiedTime) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBundleContentsLastModifiedTime(aBundleContentsLastModifiedTime); } 

extern "C"
{
NS_EXPORT nsresult NS_NewLocalFileWithFSRef(const FSRef* aFSRef, bool aFollowSymlinks, nsILocalFileMac** result);
NS_EXPORT nsresult NS_NewLocalFileWithCFURL(const CFURLRef aURL, bool aFollowSymlinks, nsILocalFileMac** result);
}

#endif /* __gen_nsILocalFileMac_h__ */
