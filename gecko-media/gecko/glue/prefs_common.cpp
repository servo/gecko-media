pref("media.dormant-on-pause-timeout-ms", 5000);
pref("media.omt_data_delivery.enabled", true);
pref("media.cache_size", 512000);
pref("media.cache_resume_threshold", 30);
pref("media.cache_readahead_limit", 60);
pref("media.memory_cache_max_size", 8192);
pref("media.memory_caches_combined_limit_kb", 524288);
pref("media.memory_caches_combined_limit_pc_sysmem", 5);
pref("media.throttle-factor", 2);
pref("media.throttle-regardless-of-download-rate", false);
pref("media.volume_scale", "1.0");
pref("media.play-stand-alone", true);
pref("media.hardware-video-decoding.enabled", true);
pref("media.hardware-video-decoding.force-enabled", false);
#ifdef MOZ_FMP4
pref("media.mp4.enabled", true);
#endif
pref("media.use-blank-decoder", false);
#ifdef MOZ_WMF
pref("media.wmf.enabled", true);
pref("media.wmf.dxva.enabled", true);
pref("media.wmf.dxva.d3d11.enabled", true);
pref("media.wmf.dxva.max-videos", 8);
pref("media.wmf.low-latency.enabled", false);
pref("media.wmf.skip-blacklist", false);
pref("media.wmf.vp9.enabled", true);
pref("media.wmf.amd.vp9.enabled", true);
pref("media.wmf.amd.highres.enabled", true);
pref("media.wmf.allow-unsupported-resolutions", false);
pref("media.wmf.use-nv12-format", true);
pref("media.wmf.disable-d3d11-for-dlls", "igd11dxva64.dll: 20.19.15.4463, 20.19.15.4454, 20.19.15.4444, 20.19.15.4416, 20.19.15.4404, 20.19.15.4390, 20.19.15.4380, 20.19.15.4377, 20.19.15.4364, 20.19.15.4360, 20.19.15.4352, 20.19.15.4331, 20.19.15.4326, 20.19.15.4300; igd10iumd32.dll: 20.19.15.4444, 20.19.15.4424, 20.19.15.4409, 20.19.15.4390, 20.19.15.4380, 20.19.15.4360, 10.18.10.4358, 20.19.15.4331, 20.19.15.4312, 20.19.15.4300, 10.18.15.4281, 10.18.15.4279, 10.18.10.4276, 10.18.15.4268, 10.18.15.4256, 10.18.10.4252, 10.18.15.4248, 10.18.14.4112, 10.18.10.3958, 10.18.10.3496, 10.18.10.3431, 10.18.10.3412, 10.18.10.3355, 9.18.10.3234, 9.18.10.3071, 9.18.10.3055, 9.18.10.3006; igd10umd32.dll: 9.17.10.4229, 9.17.10.3040, 9.17.10.2884, 9.17.10.2857, 8.15.10.2274, 8.15.10.2272, 8.15.10.2246, 8.15.10.1840, 8.15.10.1808; igd10umd64.dll: 9.17.10.4229, 9.17.10.2884, 9.17.10.2857, 10.18.10.3496; isonyvideoprocessor.dll: 4.1.2247.8090, 4.1.2153.6200; tosqep.dll: 1.2.15.526, 1.1.12.201, 1.0.11.318, 1.0.11.215, 1.0.10.1224; tosqep64.dll: 1.1.12.201, 1.0.11.215; nvwgf2um.dll: 22.21.13.8253, 22.21.13.8233, 22.21.13.8205, 22.21.13.8189, 22.21.13.8178, 22.21.13.8165, 21.21.13.7892, 21.21.13.7878, 21.21.13.7866, 21.21.13.7849, 21.21.13.7654, 21.21.13.7653, 21.21.13.7633, 21.21.13.7619, 21.21.13.7563, 21.21.13.7306, 21.21.13.7290, 21.21.13.7270, 21.21.13.7254, 21.21.13.6939, 21.21.13.6926, 21.21.13.6909, 21.21.13.4201, 21.21.13.4200, 10.18.13.6881, 10.18.13.6839, 10.18.13.6510, 10.18.13.6472, 10.18.13.6143, 10.18.13.5946, 10.18.13.5923, 10.18.13.5921, 10.18.13.5891, 10.18.13.5887, 10.18.13.5582, 10.18.13.5445, 10.18.13.5382, 10.18.13.5362, 9.18.13.4788, 9.18.13.4752, 9.18.13.4725, 9.18.13.4709, 9.18.13.4195, 9.18.13.4192, 9.18.13.4144, 9.18.13.4052, 9.18.13.3788, 9.18.13.3523, 9.18.13.3235, 9.18.13.3165, 9.18.13.2723, 9.18.13.2702, 9.18.13.1422, 9.18.13.1407, 9.18.13.1106, 9.18.13.546; atidxx32.dll: 21.19.151.3, 21.19.142.257, 21.19.137.514, 21.19.137.1, 21.19.134.1, 21.19.128.7, 21.19.128.4, 20.19.0.32837, 20.19.0.32832, 8.17.10.682, 8.17.10.671, 8.17.10.661, 8.17.10.648, 8.17.10.644, 8.17.10.625, 8.17.10.605, 8.17.10.581, 8.17.10.569, 8.17.10.560, 8.17.10.545, 8.17.10.539, 8.17.10.531, 8.17.10.525, 8.17.10.520, 8.17.10.519, 8.17.10.514, 8.17.10.511, 8.17.10.494, 8.17.10.489, 8.17.10.483, 8.17.10.453, 8.17.10.451, 8.17.10.441, 8.17.10.436, 8.17.10.432, 8.17.10.425, 8.17.10.418, 8.17.10.414, 8.17.10.401, 8.17.10.395, 8.17.10.385, 8.17.10.378, 8.17.10.362, 8.17.10.355, 8.17.10.342, 8.17.10.331, 8.17.10.318, 8.17.10.310, 8.17.10.286, 8.17.10.269, 8.17.10.261, 8.17.10.247, 8.17.10.240, 8.15.10.212; atidxx64.dll: 21.19.151.3, 21.19.142.257, 21.19.137.514, 21.19.137.1, 21.19.134.1, 21.19.128.7, 21.19.128.4, 20.19.0.32832, 8.17.10.682, 8.17.10.661, 8.17.10.644, 8.17.10.625; nvumdshim.dll: 10.18.13.6822");
pref("media.wmf.disable-d3d9-for-dlls", "igdumd64.dll: 8.15.10.2189, 8.15.10.2119, 8.15.10.2104, 8.15.10.2102, 8.771.1.0; atiumd64.dll: 7.14.10.833, 7.14.10.867, 7.14.10.885, 7.14.10.903, 7.14.10.911, 8.14.10.768, 9.14.10.1001, 9.14.10.1017, 9.14.10.1080, 9.14.10.1128, 9.14.10.1162, 9.14.10.1171, 9.14.10.1183, 9.14.10.1197, 9.14.10.945, 9.14.10.972, 9.14.10.984, 9.14.10.996");
pref("media.wmf.deblacklisting-for-telemetry-in-gpu-process", true);
pref("media.wmf.play-stand-alone", true);
pref("media.wmf.use-sync-texture", true);
#endif
#if defined(MOZ_FFMPEG)
#if defined(XP_MACOSX)
pref("media.ffmpeg.enabled", false);
#else
pref("media.ffmpeg.enabled", true);
#endif
pref("media.libavcodec.allow-obsolete", false);
#endif
#if defined(MOZ_FFVPX)
pref("media.ffvpx.enabled", true);
#endif
#if defined(MOZ_FFMPEG) || defined(MOZ_FFVPX)
pref("media.ffmpeg.low-latency.enabled", false);
#endif
pref("media.gmp.decoder.enabled", false);
pref("media.gmp.decoder.aac", 0);
pref("media.gmp.decoder.h264", 0);
pref("media.ogg.enabled", true);
pref("media.opus.enabled", true);
pref("media.wave.enabled", true);
pref("media.webm.enabled", true);
pref("media.eme.chromium-api.video-shmems", 6);
pref("media.gmp.storage.version.expected", 1);
#ifdef NIGHTLY_BUILD
pref("media.decoder-doctor.notifications-allowed", "MediaWMFNeeded,MediaWidevineNoWMF,MediaCannotInitializePulseAudio,MediaCannotPlayNoDecoders,MediaUnsupportedLibavcodec,MediaDecodeError");
#else
pref("media.decoder-doctor.notifications-allowed", "MediaWMFNeeded,MediaWidevineNoWMF,MediaCannotInitializePulseAudio,MediaCannotPlayNoDecoders,MediaUnsupportedLibavcodec");
#endif
pref("media.decoder-doctor.decode-errors-allowed", "");
pref("media.decoder-doctor.decode-warnings-allowed", "");
pref("media.decoder-doctor.verbose", false);
pref("media.decoder-doctor.wmf-disabled-is-failure", false);
pref("media.decoder-doctor.new-issue-endpoint", "https://webcompat.com/issues/new");
pref("media.suspend-bkgnd-video.enabled", true);
pref("media.suspend-bkgnd-video.delay-ms", 10000);
pref("media.resume-bkgnd-video-on-tabhover", true);;
pref("media.seamless-looping", true);
#ifdef MOZ_WEBRTC
pref("media.navigator.enabled", true);
pref("media.navigator.video.enabled", true);
pref("media.navigator.video.default_fps",30);
pref("media.navigator.video.use_remb", true);
pref("media.navigator.video.use_tmmbr", false);
pref("media.navigator.audio.use_fec", true);
pref("media.navigator.video.red_ulpfec_enabled", false);
pref("media.peerconnection.dtmf.enabled", true);
pref("media.webrtc.debug.trace_mask", 0);
pref("media.webrtc.debug.multi_log", false);
pref("media.webrtc.debug.log_file", "");
pref("media.webrtc.debug.aec_dump_max_size", 4194304); // 4MB
pref("media.navigator.video.default_width",0);  // adaptive default
pref("media.navigator.video.default_height",0); // adaptive default
pref("media.peerconnection.enabled", true);
pref("media.peerconnection.video.enabled", true);
pref("media.navigator.video.max_fs", 12288); // Enough for 2048x1536
pref("media.navigator.video.max_fr", 60);
pref("media.navigator.video.h264.level", 31); // 0x42E01f - level 3.1
pref("media.navigator.video.h264.max_br", 0);
pref("media.navigator.video.h264.max_mbps", 0);
pref("media.peerconnection.video.vp9_enabled", true);
pref("media.peerconnection.video.vp9_preferred", false);
pref("media.getusermedia.aec", 1);
pref("media.getusermedia.browser.enabled", false);
pref("media.getusermedia.channels", 0);
pref("media.peerconnection.video.min_bitrate", 0);
pref("media.peerconnection.video.start_bitrate", 0);
pref("media.peerconnection.video.max_bitrate", 0);
pref("media.peerconnection.video.min_bitrate_estimate", 0);
pref("media.peerconnection.video.denoising", false);
pref("media.navigator.audio.fake_frequency", 1000);
pref("media.navigator.permission.disabled", false);
pref("media.navigator.streams.fake", false);
pref("media.peerconnection.simulcast", true);
pref("media.peerconnection.default_iceservers", "[]");
pref("media.peerconnection.ice.loopback", false); // Set only for testing in offline environments.
pref("media.peerconnection.ice.tcp", true);
pref("media.peerconnection.ice.tcp_so_sock_count", 0); // Disable SO gathering
pref("media.peerconnection.ice.link_local", false); // Set only for testing IPV6 in networks that don't assign IPV6 addresses
pref("media.peerconnection.ice.force_interface", ""); // Limit to only a single interface
pref("media.peerconnection.ice.relay_only", false); // Limit candidates to TURN
pref("media.peerconnection.use_document_iceservers", true);
pref("media.peerconnection.identity.enabled", true);
pref("media.peerconnection.identity.timeout", 10000);
pref("media.peerconnection.ice.stun_client_maximum_transmits", 7);
pref("media.peerconnection.ice.trickle_grace_period", 5000);
pref("media.peerconnection.ice.no_host", false);
pref("media.peerconnection.ice.default_address_only", false);
pref("media.peerconnection.ice.proxy_only", false);
pref("media.peerconnection.turn.disable", false);
#if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
pref("media.getusermedia.aec_enabled", false);
pref("media.getusermedia.noise_enabled", false);
#else
pref("media.getusermedia.aec_enabled", true);
pref("media.getusermedia.noise_enabled", true);
#endif
pref("media.getusermedia.aec_extended_filter", true);
pref("media.getusermedia.noise", 1);
pref("media.getusermedia.agc_enabled", false);
pref("media.getusermedia.agc", 3); // kAgcAdaptiveDigital
#if defined(XP_MACOSX)
pref("media.peerconnection.capture_delay", 50);
pref("media.navigator.audio.full_duplex", true);
#elif defined(XP_WIN)
pref("media.peerconnection.capture_delay", 50);
pref("media.navigator.audio.full_duplex", true);
#elif defined(ANDROID)
pref("media.peerconnection.capture_delay", 100);
pref("media.navigator.audio.full_duplex", true);
pref("media.navigator.hardware.vp8_encode.acceleration_enabled", true);
pref("media.navigator.hardware.vp8_encode.acceleration_remote_enabled", true);
pref("media.navigator.hardware.vp8_decode.acceleration_enabled", false);
#elif defined(XP_LINUX) || defined(MOZ_SNDIO)
pref("media.peerconnection.capture_delay", 70);
pref("media.navigator.audio.full_duplex", true);
#else
pref("media.peerconnection.capture_delay", 50);
pref("media.navigator.audio.full_duplex", false);
#endif
pref("media.navigator.mediadatadecoder_enabled", false);
#endif
#if !defined(ANDROID)
pref("media.getusermedia.screensharing.enabled", true);
#endif
pref("media.getusermedia.audiocapture.enabled", false);
pref("media.webvtt.regions.enabled", true);
pref("media.webvtt.pseudo.enabled", true);
pref("media.track.enabled", false);
pref("media.mediasource.enabled", true);
pref("media.mediasource.mp4.enabled", true);
#if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)
pref("media.mediasource.webm.enabled", false);
#else
pref("media.mediasource.webm.enabled", true);
#endif
pref("media.mediasource.webm.audio.enabled", true);
pref("media.flac.enabled", true);
pref("media.ogg.flac.enabled", true);
pref("media.benchmark.vp9.threshold", 150);
pref("media.benchmark.frames", 300);
pref("media.benchmark.timeout", 1000);
#ifdef MOZ_WEBSPEECH
pref("media.webspeech.recognition.enable", false);
pref("media.webspeech.synth.enabled", false);
#endif
#ifdef MOZ_WEBM_ENCODER
pref("media.encoder.webm.enabled", true);
#endif
pref("media.recorder.audio_node.enabled", false);
pref("media.recorder.video.frame_drops", true);
pref("media.autoplay.enabled", true);
#ifdef NIGHTLY_BUILD
pref("media.autoplay.enabled.user-gestures-needed", false);
#endif
pref("media.video-queue.default-size", 10);
pref("media.video-queue.send-to-compositor-size", 9999);
pref("media.video_stats.enabled", true);
pref("media.decoder.recycle.enabled", false);
pref("media.decoder.skip-to-next-key-frame.enabled", true);
pref("media.cubeb.logging_level", "");
#ifdef NIGHTLY_BUILD
pref("media.cubeb.sandbox", true);
#endif
pref("media.playback.warnings-as-errors", false);
#if defined(XP_WIN)
pref("media.gpu-process-decoder", true);
#endif
pref("media.ondevicechange.enabled", true);
pref("media.ondevicechange.fakeDeviceChangeEvent.enabled", false);
pref("media.gmp-manager.url", "https://aus5.mozilla.org/update/3/GMP/%VERSION%/%BUILD_ID%/%BUILD_TARGET%/%LOCALE%/%CHANNEL%/%OS_VERSION%/%DISTRIBUTION%/%DISTRIBUTION_VERSION%/update.xml");
pref("media.gmp-manager.cert.requireBuiltIn", true);
pref("media.gmp-manager.cert.checkAttributes", true);
pref("media.gmp-manager.certs.1.issuerName", "CN=DigiCert SHA2 Secure Server CA,O=DigiCert Inc,C=US");
pref("media.gmp-manager.certs.1.commonName", "aus5.mozilla.org");
pref("media.gmp-manager.certs.2.issuerName", "CN=thawte SSL CA - G2,O=\"thawte, Inc.\",C=US");
pref("media.gmp-manager.certs.2.commonName", "aus5.mozilla.org");
#if defined(XP_LINUX) && defined(MOZ_GMP_SANDBOX)
pref("media.gmp.insecure.allow", false);
#endif
pref("media.useAudioChannelAPI", false);
pref("media.default_volume", "1.0");
pref("media.seekToNextFrame.enabled", true);
pref("media.block-autoplay-until-in-foreground", true);
