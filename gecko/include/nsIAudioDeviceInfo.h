/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM ../../../dist/idl/nsIAudioDeviceInfo.idl
 */

#ifndef __gen_nsIAudioDeviceInfo_h__
#define __gen_nsIAudioDeviceInfo_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIAudioDeviceInfo */
#define NS_IAUDIODEVICEINFO_IID_STR "feb979a8-f8cc-4522-9dff-6c055ca50762"

#define NS_IAUDIODEVICEINFO_IID \
  {0xfeb979a8, 0xf8cc, 0x4522, \
    { 0x9d, 0xff, 0x6c, 0x05, 0x5c, 0xa5, 0x07, 0x62 }}

class NS_NO_VTABLE nsIAudioDeviceInfo : public nsISupports {
 public:

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IAUDIODEVICEINFO_IID)

  /* readonly attribute DOMString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute DOMString groupId; */
  NS_IMETHOD GetGroupId(nsAString & aGroupId) = 0;

  /* readonly attribute DOMString vendor; */
  NS_IMETHOD GetVendor(nsAString & aVendor) = 0;

  enum {
    TYPE_UNKNOWN = 0U,
    TYPE_INPUT = 1U,
    TYPE_OUTPUT = 2U
  };

  /* readonly attribute unsigned short type; */
  NS_IMETHOD GetType(uint16_t *aType) = 0;

  enum {
    STATE_DISABLED = 0U,
    STATE_UNPLUGGED = 1U,
    STATE_ENABLED = 2U
  };

  /* readonly attribute unsigned short state; */
  NS_IMETHOD GetState(uint16_t *aState) = 0;

  enum {
    PREF_NONE = 0U,
    PREF_MULTIMEDIA = 1U,
    PREF_VOICE = 2U,
    PREF_NOTIFICATION = 4U,
    PREF_ALL = 15U
  };

  /* readonly attribute unsigned short preferred; */
  NS_IMETHOD GetPreferred(uint16_t *aPreferred) = 0;

  enum {
    FMT_S16LE = 16U,
    FMT_S16BE = 32U,
    FMT_F32LE = 4096U,
    FMT_F32BE = 8192U
  };

  /* readonly attribute unsigned short supportedFormat; */
  NS_IMETHOD GetSupportedFormat(uint16_t *aSupportedFormat) = 0;

  /* readonly attribute unsigned short defaultFormat; */
  NS_IMETHOD GetDefaultFormat(uint16_t *aDefaultFormat) = 0;

  /* readonly attribute unsigned long maxChannels; */
  NS_IMETHOD GetMaxChannels(uint32_t *aMaxChannels) = 0;

  /* readonly attribute unsigned long defaultRate; */
  NS_IMETHOD GetDefaultRate(uint32_t *aDefaultRate) = 0;

  /* readonly attribute unsigned long maxRate; */
  NS_IMETHOD GetMaxRate(uint32_t *aMaxRate) = 0;

  /* readonly attribute unsigned long minRate; */
  NS_IMETHOD GetMinRate(uint32_t *aMinRate) = 0;

  /* readonly attribute unsigned long maxLatency; */
  NS_IMETHOD GetMaxLatency(uint32_t *aMaxLatency) = 0;

  /* readonly attribute unsigned long minLatency; */
  NS_IMETHOD GetMinLatency(uint32_t *aMinLatency) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIAudioDeviceInfo, NS_IAUDIODEVICEINFO_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUDIODEVICEINFO \
  NS_IMETHOD GetName(nsAString & aName) override; \
  NS_IMETHOD GetGroupId(nsAString & aGroupId) override; \
  NS_IMETHOD GetVendor(nsAString & aVendor) override; \
  NS_IMETHOD GetType(uint16_t *aType) override; \
  NS_IMETHOD GetState(uint16_t *aState) override; \
  NS_IMETHOD GetPreferred(uint16_t *aPreferred) override; \
  NS_IMETHOD GetSupportedFormat(uint16_t *aSupportedFormat) override; \
  NS_IMETHOD GetDefaultFormat(uint16_t *aDefaultFormat) override; \
  NS_IMETHOD GetMaxChannels(uint32_t *aMaxChannels) override; \
  NS_IMETHOD GetDefaultRate(uint32_t *aDefaultRate) override; \
  NS_IMETHOD GetMaxRate(uint32_t *aMaxRate) override; \
  NS_IMETHOD GetMinRate(uint32_t *aMinRate) override; \
  NS_IMETHOD GetMaxLatency(uint32_t *aMaxLatency) override; \
  NS_IMETHOD GetMinLatency(uint32_t *aMinLatency) override; 

/* Use this macro when declaring the members of this interface when the
   class doesn't implement the interface. This is useful for forwarding. */
#define NS_DECL_NON_VIRTUAL_NSIAUDIODEVICEINFO \
  nsresult GetName(nsAString & aName); \
  nsresult GetGroupId(nsAString & aGroupId); \
  nsresult GetVendor(nsAString & aVendor); \
  nsresult GetType(uint16_t *aType); \
  nsresult GetState(uint16_t *aState); \
  nsresult GetPreferred(uint16_t *aPreferred); \
  nsresult GetSupportedFormat(uint16_t *aSupportedFormat); \
  nsresult GetDefaultFormat(uint16_t *aDefaultFormat); \
  nsresult GetMaxChannels(uint32_t *aMaxChannels); \
  nsresult GetDefaultRate(uint32_t *aDefaultRate); \
  nsresult GetMaxRate(uint32_t *aMaxRate); \
  nsresult GetMinRate(uint32_t *aMinRate); \
  nsresult GetMaxLatency(uint32_t *aMaxLatency); \
  nsresult GetMinLatency(uint32_t *aMinLatency); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUDIODEVICEINFO(_to) \
  NS_IMETHOD GetName(nsAString & aName) override { return _to GetName(aName); } \
  NS_IMETHOD GetGroupId(nsAString & aGroupId) override { return _to GetGroupId(aGroupId); } \
  NS_IMETHOD GetVendor(nsAString & aVendor) override { return _to GetVendor(aVendor); } \
  NS_IMETHOD GetType(uint16_t *aType) override { return _to GetType(aType); } \
  NS_IMETHOD GetState(uint16_t *aState) override { return _to GetState(aState); } \
  NS_IMETHOD GetPreferred(uint16_t *aPreferred) override { return _to GetPreferred(aPreferred); } \
  NS_IMETHOD GetSupportedFormat(uint16_t *aSupportedFormat) override { return _to GetSupportedFormat(aSupportedFormat); } \
  NS_IMETHOD GetDefaultFormat(uint16_t *aDefaultFormat) override { return _to GetDefaultFormat(aDefaultFormat); } \
  NS_IMETHOD GetMaxChannels(uint32_t *aMaxChannels) override { return _to GetMaxChannels(aMaxChannels); } \
  NS_IMETHOD GetDefaultRate(uint32_t *aDefaultRate) override { return _to GetDefaultRate(aDefaultRate); } \
  NS_IMETHOD GetMaxRate(uint32_t *aMaxRate) override { return _to GetMaxRate(aMaxRate); } \
  NS_IMETHOD GetMinRate(uint32_t *aMinRate) override { return _to GetMinRate(aMinRate); } \
  NS_IMETHOD GetMaxLatency(uint32_t *aMaxLatency) override { return _to GetMaxLatency(aMaxLatency); } \
  NS_IMETHOD GetMinLatency(uint32_t *aMinLatency) override { return _to GetMinLatency(aMinLatency); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUDIODEVICEINFO(_to) \
  NS_IMETHOD GetName(nsAString & aName) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetGroupId(nsAString & aGroupId) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetGroupId(aGroupId); } \
  NS_IMETHOD GetVendor(nsAString & aVendor) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVendor(aVendor); } \
  NS_IMETHOD GetType(uint16_t *aType) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD GetState(uint16_t *aState) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetState(aState); } \
  NS_IMETHOD GetPreferred(uint16_t *aPreferred) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPreferred(aPreferred); } \
  NS_IMETHOD GetSupportedFormat(uint16_t *aSupportedFormat) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSupportedFormat(aSupportedFormat); } \
  NS_IMETHOD GetDefaultFormat(uint16_t *aDefaultFormat) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultFormat(aDefaultFormat); } \
  NS_IMETHOD GetMaxChannels(uint32_t *aMaxChannels) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMaxChannels(aMaxChannels); } \
  NS_IMETHOD GetDefaultRate(uint32_t *aDefaultRate) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultRate(aDefaultRate); } \
  NS_IMETHOD GetMaxRate(uint32_t *aMaxRate) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMaxRate(aMaxRate); } \
  NS_IMETHOD GetMinRate(uint32_t *aMinRate) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMinRate(aMinRate); } \
  NS_IMETHOD GetMaxLatency(uint32_t *aMaxLatency) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMaxLatency(aMaxLatency); } \
  NS_IMETHOD GetMinLatency(uint32_t *aMinLatency) override { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMinLatency(aMinLatency); } 


#endif /* __gen_nsIAudioDeviceInfo_h__ */
