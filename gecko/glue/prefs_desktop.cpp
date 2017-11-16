#ifdef XP_LINUX
pref("media.eme.enabled", false);
#else
pref("media.eme.enabled", true);
#endif
#ifdef NIGHTLY_BUILD
pref("media.eme.vp9-in-mp4.enabled", true);
#else
pref("media.eme.vp9-in-mp4.enabled", false);
#endif
pref("media.eme.hdcp-policy-check.enabled", false);
pref("media.gmp.trial-create.enabled", true);
#ifdef MOZ_WIDEVINE_EME
pref("media.gmp-widevinecdm.visible", true);
pref("media.gmp-widevinecdm.enabled", true);
#endif
pref("media.gmp-provider.enabled", true);
pref("media.webspeech.synth.enabled", true);
