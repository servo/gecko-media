static const BoolPref sAndroidBoolPrefs[] = {
  { "media.android-media-codec.enabled", true },
  { "media.android-media-codec.preferred", true },
  { "media.block-autoplay-until-in-foreground", false },
  { "media.decoder.recycle.enabled", true },
#ifdef NIGHTLY_BUILD
  { "media.eme.enabled", true },
#endif // #ifdef NIGHTLY_BUILD
  { "media.gmp-provider.enabled", true },
#ifdef NIGHTLY_BUILD
  { "media.hls.enabled", true },
#endif // #ifdef NIGHTLY_BUILD
  { "media.mediadrm-widevinecdm.visible", true },
  { "media.mediasource.enabled", true },
  { "media.navigator.permission.device", true },
  { "media.openUnsupportedTypeWithExternalApp", true },
  { "media.plugins.enabled", true },
  { "media.realtime_decoder.enabled", true },
  { "media.suspend-bkgnd-video.enabled", true },
  { "media.throttle-regardless-of-download-rate", true },
#ifdef NIGHTLY_BUILD
  { "media.videocontrols.lock-video-orientation", true },
#endif // #ifdef NIGHTLY_BUILD
};

static const IntPref sAndroidIntPrefs[] = {
  { "media.cache_readahead_limit", 30 },
  { "media.cache_resume_threshold", 10 },
  { "media.cache_size", 32768 },
  { "media.preload.auto", 2 },
  { "media.preload.default", 1 },
  { "media.stagefright.omxcodec.flags", 0 },
  { "media.video-queue.default-size", 3 },
  { "media.video-queue.send-to-compositor-size", 1 },
};

static const StringPref sAndroidStringPrefs[] = {
};

