pref("media.preload.default", 1); // default to preload none
pref("media.preload.auto", 2);    // preload metadata if preload=auto
pref("media.cache_size", 32768);    // 32MB media cache
pref("media.cache_resume_threshold", 10);
pref("media.cache_readahead_limit", 30);
pref("media.throttle-regardless-of-download-rate", true);
pref("media.video-queue.default-size", 3);
pref("media.video-queue.send-to-compositor-size", 1);
pref("media.decoder.recycle.enabled", true);
pref("media.android-media-codec.enabled", true);
pref("media.android-media-codec.preferred", true);
pref("media.mediasource.enabled", true);
pref("media.mediadrm-widevinecdm.visible", true);
#ifdef NIGHTLY_BUILD
pref("media.eme.enabled", true);
#endif
#ifdef NIGHTLY_BUILD
pref("media.hls.enabled", true);
#endif
pref("media.suspend-bkgnd-video.enabled", true);
pref("media.realtime_decoder.enabled", true);
pref("media.plugins.enabled", true);
pref("media.stagefright.omxcodec.flags", 0);
pref("media.gmp-provider.enabled", true);
pref("media.block-autoplay-until-in-foreground", false);
pref("media.openUnsupportedTypeWithExternalApp", true);
pref("media.navigator.permission.device", true);
#ifdef NIGHTLY_BUILD
pref("media.videocontrols.lock-video-orientation", true);
#endif
