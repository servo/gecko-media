
#undef MOZ_GECKO_PROFILER
#undef MOZ_MEMORY
#undef MOZ_GLUE_IN_PROGRAM

// This depends on the media/audioipc "prototype".
// TODO: import media/audioipc?
#undef MOZ_CUBEB_REMOTING

#undef MOZ_WIDGET_ANDROID
#undef MOZ_AV1

#undef MOZ_WEBRTC

#define TimeStamp GeckoMedia_TimeStamp
#define HashBytes GeckoMedia_HashBytes
#define BaseTimeDurationPlatformUtils GeckoMedia_BaseTimeDurationPlatformUtils
#define Unused GeckoMedia_Unused
#define detail GeckoMedia_detail