static const BoolPref sDesktopBoolPrefs[] = {
#ifdef XP_LINUX
  { "media.eme.enabled", false },
#endif // #ifdef XP_LINUX
#ifdef XP_LINUX
  { "media.eme.enabled", true },
#endif // #ifdef XP_LINUX
#ifdef NIGHTLY_BUILD
  { "media.eme.vp9-in-mp4.enabled", true },
#endif // #ifdef NIGHTLY_BUILD
#ifdef NIGHTLY_BUILD
  { "media.eme.vp9-in-mp4.enabled", false },
#endif // #ifdef NIGHTLY_BUILD
  { "media.gmp-provider.enabled", true },
#ifdef MOZ_WIDEVINE_EME
  { "media.gmp-widevinecdm.enabled", true },
#endif // #ifdef MOZ_WIDEVINE_EME
#ifdef MOZ_WIDEVINE_EME
  { "media.gmp-widevinecdm.visible", true },
#endif // #ifdef MOZ_WIDEVINE_EME
  { "media.gmp.trial-create.enabled", true },
  { "media.webspeech.synth.enabled", true },
};

static const IntPref sDesktopIntPrefs[] = {
};

static const StringPref sDesktopStringPrefs[] = {
};

