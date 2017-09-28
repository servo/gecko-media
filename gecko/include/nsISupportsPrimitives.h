/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsISupportsPrimitives.idl
 */

#ifndef __gen_nsISupportsPrimitives_h__
#define __gen_nsISupportsPrimitives_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsISupportsPrimitive */
#define NS_ISUPPORTSPRIMITIVE_IID_STR "d0d4b136-1dd1-11b2-9371-f0727ef827c0"

#define NS_ISUPPORTSPRIMITIVE_IID \
  {0xd0d4b136, 0x1dd1, 0x11b2, \
    { 0x93, 0x71, 0xf0, 0x72, 0x7e, 0xf8, 0x27, 0xc0 }}

class NS_NO_VTABLE nsISupportsPrimitive : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRIMITIVE_IID)

  enum {
    TYPE_ID = 1U,
    TYPE_CSTRING = 2U,
    TYPE_STRING = 3U,
    TYPE_PRBOOL = 4U,
    TYPE_PRUINT8 = 5U,
    TYPE_PRUINT16 = 6U,
    TYPE_PRUINT32 = 7U,
    TYPE_PRUINT64 = 8U,
    TYPE_PRTIME = 9U,
    TYPE_CHAR = 10U,
    TYPE_PRINT16 = 11U,
    TYPE_PRINT32 = 12U,
    TYPE_PRINT64 = 13U,
    TYPE_FLOAT = 14U,
    TYPE_DOUBLE = 15U,
    TYPE_INTERFACE_POINTER = 17U
  };

  /* readonly attribute unsigned short type; */
  NS_IMETHOD GetType(uint16_t *aType) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPrimitive, NS_ISUPPORTSPRIMITIVE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRIMITIVE \
  NS_IMETHOD GetType(uint16_t *aType) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRIMITIVE \
  nsresult GetType(uint16_t *aType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRIMITIVE(_to) \
  NS_IMETHOD GetType(uint16_t *aType) override { return _to GetType(aType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRIMITIVE(_to) \
  NS_IMETHOD GetType(uint16_t *aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } 


/* starting interface:    nsISupportsID */
#define NS_ISUPPORTSID_IID_STR "d18290a0-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSID_IID \
  {0xd18290a0, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsID : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSID_IID)

  /* attribute nsIDPtr data; */
  NS_IMETHOD GetData(nsID **aData) = 0;
  NS_IMETHOD SetData(const nsID *aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsID, NS_ISUPPORTSID_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSID \
  NS_IMETHOD GetData(nsID **aData) override; \
  NS_IMETHOD SetData(const nsID *aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSID \
  nsresult GetData(nsID **aData); \
  nsresult SetData(const nsID *aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSID(_to) \
  NS_IMETHOD GetData(nsID **aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(const nsID *aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSID(_to) \
  NS_IMETHOD GetData(nsID **aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(const nsID *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsCString */
#define NS_ISUPPORTSCSTRING_IID_STR "d65ff270-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSCSTRING_IID \
  {0xd65ff270, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsCString : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSCSTRING_IID)

  /* attribute ACString data; */
  NS_IMETHOD GetData(nsACString & aData) = 0;
  NS_IMETHOD SetData(const nsACString & aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsCString, NS_ISUPPORTSCSTRING_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSCSTRING \
  NS_IMETHOD GetData(nsACString & aData) override; \
  NS_IMETHOD SetData(const nsACString & aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSCSTRING \
  nsresult GetData(nsACString & aData); \
  nsresult SetData(const nsACString & aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSCSTRING(_to) \
  NS_IMETHOD GetData(nsACString & aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(const nsACString & aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSCSTRING(_to) \
  NS_IMETHOD GetData(nsACString & aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(const nsACString & aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsString */
#define NS_ISUPPORTSSTRING_IID_STR "d79dc970-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSSTRING_IID \
  {0xd79dc970, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsString : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSSTRING_IID)

  /* attribute AString data; */
  NS_IMETHOD GetData(nsAString & aData) = 0;
  NS_IMETHOD SetData(const nsAString & aData) = 0;

  /* wstring toString (); */
  NS_IMETHOD ToString(char16_t * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsString, NS_ISUPPORTSSTRING_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSSTRING \
  NS_IMETHOD GetData(nsAString & aData) override; \
  NS_IMETHOD SetData(const nsAString & aData) override; \
  NS_IMETHOD ToString(char16_t * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSSTRING \
  nsresult GetData(nsAString & aData); \
  nsresult SetData(const nsAString & aData); \
  nsresult ToString(char16_t * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSSTRING(_to) \
  NS_IMETHOD GetData(nsAString & aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(const nsAString & aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char16_t * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSSTRING(_to) \
  NS_IMETHOD GetData(nsAString & aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(const nsAString & aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char16_t * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRBool */
#define NS_ISUPPORTSPRBOOL_IID_STR "ddc3b490-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRBOOL_IID \
  {0xddc3b490, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRBool : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRBOOL_IID)

  /* attribute boolean data; */
  NS_IMETHOD GetData(bool *aData) = 0;
  NS_IMETHOD SetData(bool aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRBool, NS_ISUPPORTSPRBOOL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRBOOL \
  NS_IMETHOD GetData(bool *aData) override; \
  NS_IMETHOD SetData(bool aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRBOOL \
  nsresult GetData(bool *aData); \
  nsresult SetData(bool aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRBOOL(_to) \
  NS_IMETHOD GetData(bool *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(bool aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRBOOL(_to) \
  NS_IMETHOD GetData(bool *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(bool aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRUint8 */
#define NS_ISUPPORTSPRUINT8_IID_STR "dec2e4e0-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRUINT8_IID \
  {0xdec2e4e0, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRUint8 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRUINT8_IID)

  /* attribute uint8_t data; */
  NS_IMETHOD GetData(uint8_t *aData) = 0;
  NS_IMETHOD SetData(uint8_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRUint8, NS_ISUPPORTSPRUINT8_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRUINT8 \
  NS_IMETHOD GetData(uint8_t *aData) override; \
  NS_IMETHOD SetData(uint8_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRUINT8 \
  nsresult GetData(uint8_t *aData); \
  nsresult SetData(uint8_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRUINT8(_to) \
  NS_IMETHOD GetData(uint8_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(uint8_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRUINT8(_to) \
  NS_IMETHOD GetData(uint8_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(uint8_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRUint16 */
#define NS_ISUPPORTSPRUINT16_IID_STR "dfacb090-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRUINT16_IID \
  {0xdfacb090, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRUint16 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRUINT16_IID)

  /* attribute uint16_t data; */
  NS_IMETHOD GetData(uint16_t *aData) = 0;
  NS_IMETHOD SetData(uint16_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRUint16, NS_ISUPPORTSPRUINT16_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRUINT16 \
  NS_IMETHOD GetData(uint16_t *aData) override; \
  NS_IMETHOD SetData(uint16_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRUINT16 \
  nsresult GetData(uint16_t *aData); \
  nsresult SetData(uint16_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRUINT16(_to) \
  NS_IMETHOD GetData(uint16_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(uint16_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRUINT16(_to) \
  NS_IMETHOD GetData(uint16_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(uint16_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRUint32 */
#define NS_ISUPPORTSPRUINT32_IID_STR "e01dc470-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRUINT32_IID \
  {0xe01dc470, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRUint32 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRUINT32_IID)

  /* attribute uint32_t data; */
  NS_IMETHOD GetData(uint32_t *aData) = 0;
  NS_IMETHOD SetData(uint32_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRUint32, NS_ISUPPORTSPRUINT32_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRUINT32 \
  NS_IMETHOD GetData(uint32_t *aData) override; \
  NS_IMETHOD SetData(uint32_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRUINT32 \
  nsresult GetData(uint32_t *aData); \
  nsresult SetData(uint32_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRUINT32(_to) \
  NS_IMETHOD GetData(uint32_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(uint32_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRUINT32(_to) \
  NS_IMETHOD GetData(uint32_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(uint32_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRUint64 */
#define NS_ISUPPORTSPRUINT64_IID_STR "e13567c0-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRUINT64_IID \
  {0xe13567c0, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRUint64 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRUINT64_IID)

  /* attribute uint64_t data; */
  NS_IMETHOD GetData(uint64_t *aData) = 0;
  NS_IMETHOD SetData(uint64_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRUint64, NS_ISUPPORTSPRUINT64_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRUINT64 \
  NS_IMETHOD GetData(uint64_t *aData) override; \
  NS_IMETHOD SetData(uint64_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRUINT64 \
  nsresult GetData(uint64_t *aData); \
  nsresult SetData(uint64_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRUINT64(_to) \
  NS_IMETHOD GetData(uint64_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(uint64_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRUINT64(_to) \
  NS_IMETHOD GetData(uint64_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(uint64_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRTime */
#define NS_ISUPPORTSPRTIME_IID_STR "e2563630-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRTIME_IID \
  {0xe2563630, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRTime : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRTIME_IID)

  /* attribute PRTime data; */
  NS_IMETHOD GetData(PRTime *aData) = 0;
  NS_IMETHOD SetData(PRTime aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRTime, NS_ISUPPORTSPRTIME_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRTIME \
  NS_IMETHOD GetData(PRTime *aData) override; \
  NS_IMETHOD SetData(PRTime aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRTIME \
  nsresult GetData(PRTime *aData); \
  nsresult SetData(PRTime aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRTIME(_to) \
  NS_IMETHOD GetData(PRTime *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(PRTime aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRTIME(_to) \
  NS_IMETHOD GetData(PRTime *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(PRTime aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsChar */
#define NS_ISUPPORTSCHAR_IID_STR "e2b05e40-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSCHAR_IID \
  {0xe2b05e40, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsChar : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSCHAR_IID)

  /* attribute char data; */
  NS_IMETHOD GetData(char *aData) = 0;
  NS_IMETHOD SetData(char aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsChar, NS_ISUPPORTSCHAR_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSCHAR \
  NS_IMETHOD GetData(char *aData) override; \
  NS_IMETHOD SetData(char aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSCHAR \
  nsresult GetData(char *aData); \
  nsresult SetData(char aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSCHAR(_to) \
  NS_IMETHOD GetData(char *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(char aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSCHAR(_to) \
  NS_IMETHOD GetData(char *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(char aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRInt16 */
#define NS_ISUPPORTSPRINT16_IID_STR "e30d94b0-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRINT16_IID \
  {0xe30d94b0, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRInt16 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRINT16_IID)

  /* attribute int16_t data; */
  NS_IMETHOD GetData(int16_t *aData) = 0;
  NS_IMETHOD SetData(int16_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRInt16, NS_ISUPPORTSPRINT16_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRINT16 \
  NS_IMETHOD GetData(int16_t *aData) override; \
  NS_IMETHOD SetData(int16_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRINT16 \
  nsresult GetData(int16_t *aData); \
  nsresult SetData(int16_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRINT16(_to) \
  NS_IMETHOD GetData(int16_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(int16_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRINT16(_to) \
  NS_IMETHOD GetData(int16_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(int16_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRInt32 */
#define NS_ISUPPORTSPRINT32_IID_STR "e36c5250-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRINT32_IID \
  {0xe36c5250, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRInt32 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRINT32_IID)

  /* attribute int32_t data; */
  NS_IMETHOD GetData(int32_t *aData) = 0;
  NS_IMETHOD SetData(int32_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRInt32, NS_ISUPPORTSPRINT32_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRINT32 \
  NS_IMETHOD GetData(int32_t *aData) override; \
  NS_IMETHOD SetData(int32_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRINT32 \
  nsresult GetData(int32_t *aData); \
  nsresult SetData(int32_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRINT32(_to) \
  NS_IMETHOD GetData(int32_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(int32_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRINT32(_to) \
  NS_IMETHOD GetData(int32_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(int32_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsPRInt64 */
#define NS_ISUPPORTSPRINT64_IID_STR "e3cb0ff0-4a1c-11d3-9890-006008962422"

#define NS_ISUPPORTSPRINT64_IID \
  {0xe3cb0ff0, 0x4a1c, 0x11d3, \
    { 0x98, 0x90, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsISupportsPRInt64 : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSPRINT64_IID)

  /* attribute int64_t data; */
  NS_IMETHOD GetData(int64_t *aData) = 0;
  NS_IMETHOD SetData(int64_t aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsPRInt64, NS_ISUPPORTSPRINT64_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSPRINT64 \
  NS_IMETHOD GetData(int64_t *aData) override; \
  NS_IMETHOD SetData(int64_t aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSPRINT64 \
  nsresult GetData(int64_t *aData); \
  nsresult SetData(int64_t aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSPRINT64(_to) \
  NS_IMETHOD GetData(int64_t *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(int64_t aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSPRINT64(_to) \
  NS_IMETHOD GetData(int64_t *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(int64_t aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsFloat */
#define NS_ISUPPORTSFLOAT_IID_STR "abeaa390-4ac0-11d3-baea-00805f8a5dd7"

#define NS_ISUPPORTSFLOAT_IID \
  {0xabeaa390, 0x4ac0, 0x11d3, \
    { 0xba, 0xea, 0x00, 0x80, 0x5f, 0x8a, 0x5d, 0xd7 }}

class NS_NO_VTABLE nsISupportsFloat : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSFLOAT_IID)

  /* attribute float data; */
  NS_IMETHOD GetData(float *aData) = 0;
  NS_IMETHOD SetData(float aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsFloat, NS_ISUPPORTSFLOAT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSFLOAT \
  NS_IMETHOD GetData(float *aData) override; \
  NS_IMETHOD SetData(float aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSFLOAT \
  nsresult GetData(float *aData); \
  nsresult SetData(float aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSFLOAT(_to) \
  NS_IMETHOD GetData(float *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(float aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSFLOAT(_to) \
  NS_IMETHOD GetData(float *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(float aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsDouble */
#define NS_ISUPPORTSDOUBLE_IID_STR "b32523a0-4ac0-11d3-baea-00805f8a5dd7"

#define NS_ISUPPORTSDOUBLE_IID \
  {0xb32523a0, 0x4ac0, 0x11d3, \
    { 0xba, 0xea, 0x00, 0x80, 0x5f, 0x8a, 0x5d, 0xd7 }}

class NS_NO_VTABLE nsISupportsDouble : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSDOUBLE_IID)

  /* attribute double data; */
  NS_IMETHOD GetData(double *aData) = 0;
  NS_IMETHOD SetData(double aData) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsDouble, NS_ISUPPORTSDOUBLE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSDOUBLE \
  NS_IMETHOD GetData(double *aData) override; \
  NS_IMETHOD SetData(double aData) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSDOUBLE \
  nsresult GetData(double *aData); \
  nsresult SetData(double aData); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSDOUBLE(_to) \
  NS_IMETHOD GetData(double *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(double aData) override { return _to SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSDOUBLE(_to) \
  NS_IMETHOD GetData(double *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(double aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


/* starting interface:    nsISupportsInterfacePointer */
#define NS_ISUPPORTSINTERFACEPOINTER_IID_STR "995ea724-1dd1-11b2-9211-c21bdd3e7ed0"

#define NS_ISUPPORTSINTERFACEPOINTER_IID \
  {0x995ea724, 0x1dd1, 0x11b2, \
    { 0x92, 0x11, 0xc2, 0x1b, 0xdd, 0x3e, 0x7e, 0xd0 }}

class NS_NO_VTABLE nsISupportsInterfacePointer : public nsISupportsPrimitive {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISUPPORTSINTERFACEPOINTER_IID)

  /* attribute nsISupports data; */
  NS_IMETHOD GetData(nsISupports * *aData) = 0;
  NS_IMETHOD SetData(nsISupports *aData) = 0;

  /* attribute nsIDPtr dataIID; */
  NS_IMETHOD GetDataIID(nsID **aDataIID) = 0;
  NS_IMETHOD SetDataIID(const nsID *aDataIID) = 0;

  /* string toString (); */
  NS_IMETHOD ToString(char * *_retval) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISupportsInterfacePointer, NS_ISUPPORTSINTERFACEPOINTER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISUPPORTSINTERFACEPOINTER \
  NS_IMETHOD GetData(nsISupports * *aData) override; \
  NS_IMETHOD SetData(nsISupports *aData) override; \
  NS_IMETHOD GetDataIID(nsID **aDataIID) override; \
  NS_IMETHOD SetDataIID(const nsID *aDataIID) override; \
  NS_IMETHOD ToString(char * *_retval) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSISUPPORTSINTERFACEPOINTER \
  nsresult GetData(nsISupports * *aData); \
  nsresult SetData(nsISupports *aData); \
  nsresult GetDataIID(nsID **aDataIID); \
  nsresult SetDataIID(const nsID *aDataIID); \
  nsresult ToString(char * *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISUPPORTSINTERFACEPOINTER(_to) \
  NS_IMETHOD GetData(nsISupports * *aData) override { return _to GetData(aData); } \
  NS_IMETHOD SetData(nsISupports *aData) override { return _to SetData(aData); } \
  NS_IMETHOD GetDataIID(nsID **aDataIID) override { return _to GetDataIID(aDataIID); } \
  NS_IMETHOD SetDataIID(const nsID *aDataIID) override { return _to SetDataIID(aDataIID); } \
  NS_IMETHOD ToString(char * *_retval) override { return _to ToString(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISUPPORTSINTERFACEPOINTER(_to) \
  NS_IMETHOD GetData(nsISupports * *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetData(aData); } \
  NS_IMETHOD SetData(nsISupports *aData) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetData(aData); } \
  NS_IMETHOD GetDataIID(nsID **aDataIID) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDataIID(aDataIID); } \
  NS_IMETHOD SetDataIID(const nsID *aDataIID) override { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDataIID(aDataIID); } \
  NS_IMETHOD ToString(char * *_retval) override { return !_to ? NS_ERROR_NULL_POINTER : _to->ToString(_retval); } 


#endif /* __gen_nsISupportsPrimitives_h__ */
