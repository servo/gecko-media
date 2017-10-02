static const BoolPref sCommonBoolPrefs[] = {
#ifdef MOZ_APPLEMEDIA
  { "media.apple.mp3.enabled", true },
#endif // #ifdef MOZ_APPLEMEDIA
#ifdef MOZ_APPLEMEDIA
  { "media.apple.mp4.enabled", true },
#endif // #ifdef MOZ_APPLEMEDIA
  { "media.autoplay.enabled", true },
  { "media.block-autoplay-until-in-foreground", true },
  { "media.decoder-doctor.verbose", false },
  { "media.decoder-doctor.wmf-disabled-is-failure", false },
  { "media.decoder.recycle.enabled", false },
  { "media.decoder.skip-to-next-key-frame.enabled", true },
#ifdef MOZ_WEBM_ENCODER
  { "media.encoder.webm.enabled", true },
#endif // #ifdef MOZ_WEBM_ENCODER
#if defined(MOZ_FFMPEG)
#if defined(XP_MACOSX)
  { "media.ffmpeg.enabled", false },
#endif // #if defined(MOZ_FFMPEG)
#endif // #if defined(XP_MACOSX)
#if defined(MOZ_FFMPEG)
#if defined(XP_MACOSX)
#else
  { "media.ffmpeg.enabled", true },
#endif // #if defined(MOZ_FFMPEG)
#endif // #if defined(XP_MACOSX)
#if defined(MOZ_FFMPEG) || defined(MOZ_FFVPX)
  { "media.ffmpeg.low-latency.enabled", false },
#endif // #if defined(MOZ_FFMPEG) || defined(MOZ_FFVPX)
#if defined(MOZ_FFVPX)
  { "media.ffvpx.enabled", true },
#endif // #if defined(MOZ_FFVPX)
  { "media.flac.enabled", true },
#ifdef MOZ_WEBRTC
  { "media.getusermedia.aec_delay_agnostic", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
#if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
  { "media.getusermedia.aec_enabled", false },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
#ifdef MOZ_WEBRTC
#if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
#else
  { "media.getusermedia.aec_enabled", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
#ifdef MOZ_WEBRTC
  { "media.getusermedia.aec_extended_filter", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.getusermedia.agc_enabled", false },
#endif // #ifdef MOZ_WEBRTC
  { "media.getusermedia.audiocapture.enabled", false },
#ifdef MOZ_WEBRTC
  { "media.getusermedia.browser.enabled", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
#if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
  { "media.getusermedia.noise_enabled", false },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
#ifdef MOZ_WEBRTC
#if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
#else
  { "media.getusermedia.noise_enabled", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(MOZ_WEBRTC_HARDWARE_AEC_NS)
#if !defined(ANDROID)
  { "media.getusermedia.screensharing.enabled", true },
#endif // #if !defined(ANDROID)
  { "media.gmp-manager.cert.checkAttributes", true },
  { "media.gmp-manager.cert.requireBuiltIn", true },
  { "media.gmp.decoder.enabled", false },
#if defined(XP_LINUX) && defined(MOZ_GMP_SANDBOX)
  { "media.gmp.insecure.allow", false },
#endif // #if defined(XP_LINUX) && defined(MOZ_GMP_SANDBOX)
#if defined(XP_WIN)
  { "media.gpu-process-decoder", true },
#endif // #if defined(XP_WIN)
  { "media.hardware-video-decoding.enabled", true },
  { "media.hardware-video-decoding.force-enabled", false },
#if defined(MOZ_FFMPEG)
  { "media.libavcodec.allow-obsolete", false },
#endif // #if defined(MOZ_FFMPEG)
  { "media.mediasource.enabled", true },
  { "media.mediasource.mp4.enabled", true },
  { "media.mediasource.webm.audio.enabled", true },
#if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)
  { "media.mediasource.webm.enabled", false },
#endif // #if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)
#if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)
#else
  { "media.mediasource.webm.enabled", true },
#endif // #if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)
#ifdef MOZ_APPLEMEDIA
#ifdef MOZ_WIDGET_UIKIT
  { "media.mp3.enabled", true },
#endif // #ifdef MOZ_APPLEMEDIA
#endif // #ifdef MOZ_WIDGET_UIKIT
#ifdef MOZ_FMP4
  { "media.mp4.enabled", true },
#endif // #ifdef MOZ_FMP4
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.audio.full_duplex", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.audio.full_duplex", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.audio.full_duplex", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.audio.full_duplex", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
#else
  { "media.navigator.audio.full_duplex", false },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
  { "media.navigator.audio.use_fec", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.enabled", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.hardware.vp8_decode.acceleration_enabled", false },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.hardware.vp8_encode.acceleration_enabled", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.navigator.hardware.vp8_encode.acceleration_remote_enabled", true },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
  { "media.navigator.load_adapt", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.load_adapt.encoder_only", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.mediadatadecoder_enabled", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.permission.disabled", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.streams.fake", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.enabled", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.red_ulpfec_enabled", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.use_remb", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.use_tmmbr", false },
#endif // #ifdef MOZ_WEBRTC
  { "media.ogg.enabled", true },
  { "media.ogg.flac.enabled", true },
  { "media.ondevicechange.enabled", true },
  { "media.ondevicechange.fakeDeviceChangeEvent.enabled", false },
  { "media.opus.enabled", true },
#ifdef MOZ_WEBRTC
  { "media.peerconnection.dtmf.enabled", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.enabled", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.default_address_only", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.link_local", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.loopback", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.no_host", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.proxy_only", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.relay_only", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.tcp", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.identity.enabled", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.simulcast", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.turn.disable", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.use_document_iceservers", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.denoising", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.enabled", true },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.h264_enabled", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.vp9_enabled", true },
#endif // #ifdef MOZ_WEBRTC
  { "media.play-stand-alone", true },
  { "media.playback.warnings-as-errors", false },
#ifdef MOZ_RAW
  { "media.raw.enabled", true },
#endif // #ifdef MOZ_RAW
  { "media.resume-bkgnd-video-on-tabhover", true },
  { "media.seekToNextFrame.enabled", true },
  { "media.suspend-bkgnd-video.enabled", true },
  { "media.throttle-regardless-of-download-rate", false },
  { "media.track.enabled", false },
  { "media.use-blank-decoder", false },
  { "media.useAudioChannelAPI", false },
  { "media.video_stats.enabled", true },
  { "media.wave.enabled", true },
  { "media.webm.enabled", true },
#ifdef MOZ_WEBRTC
  { "media.webrtc.debug.multi_log", false },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBSPEECH
  { "media.webspeech.recognition.enable", false },
#endif // #ifdef MOZ_WEBSPEECH
#ifdef MOZ_WEBSPEECH
  { "media.webspeech.synth.enabled", false },
#endif // #ifdef MOZ_WEBSPEECH
  { "media.webvtt.pseudo.enabled", true },
  { "media.webvtt.regions.enabled", false },
#ifdef MOZ_WMF
  { "media.wmf.allow-unsupported-resolutions", false },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.amd.vp9.enabled", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.deblacklisting-for-telemetry-in-gpu-process", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.dxva.d3d11.enabled", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.dxva.enabled", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.enabled", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.low-latency.enabled", false },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.play-stand-alone", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.skip-blacklist", false },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.use-nv12-format", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.use-sync-texture", true },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.vp9.enabled", true },
#endif // #ifdef MOZ_WMF
};

static const IntPref sCommonIntPrefs[] = {
  { "media.benchmark.frames", 300 },
  { "media.benchmark.timeout", 1000 },
  { "media.benchmark.vp9.threshold", 150 },
  { "media.cache.resource-index", 8192 },
  { "media.cache_readahead_limit", 60 },
  { "media.cache_resume_threshold", 30 },
  { "media.cache_size", 512000 },
  { "media.dormant-on-pause-timeout-ms", 5000 },
  { "media.eme.chromium-api.video-shmems", 6 },
#ifdef MOZ_WEBRTC
  { "media.getusermedia.aec", 1 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.getusermedia.agc", 1 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.getusermedia.channels", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.getusermedia.noise", 1 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.getusermedia.playout_delay", 10 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.getusermedia.playout_delay", 40 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.getusermedia.playout_delay", 100 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.getusermedia.playout_delay", 50 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
#else
  { "media.getusermedia.playout_delay", 50 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
  { "media.gmp.decoder.aac", 0 },
  { "media.gmp.decoder.h264", 0 },
  { "media.gmp.storage.version.expected", 1 },
  { "media.memory_cache_max_size", 8192 },
  { "media.memory_caches_combined_limit_kb", 524288 },
  { "media.memory_caches_combined_limit_pc_sysmem", 5 },
#ifdef MOZ_WEBRTC
  { "media.navigator.audio.fake_frequency", 1000 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.load_adapt.avg_seconds", 3 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.load_adapt.measure_interval", 1000 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.default_fps", 30 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.default_height", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.default_minfps", 10 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.default_width", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.h264.level", 31 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.h264.max_br", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.h264.max_mbps", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.max_fr", 60 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.video.max_fs", 12288 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.peerconnection.capture_delay", 50 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.peerconnection.capture_delay", 50 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.peerconnection.capture_delay", 100 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
  { "media.peerconnection.capture_delay", 70 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
#if defined(XP_MACOSX)
#else
  { "media.peerconnection.capture_delay", 50 },
#endif // #ifdef MOZ_WEBRTC
#endif // #if defined(XP_MACOSX)
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.stun_client_maximum_transmits", 7 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.tcp_so_sock_count", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.trickle_grace_period", 5000 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.identity.timeout", 10000 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.max_bitrate", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.min_bitrate", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.min_bitrate_estimate", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.video.start_bitrate", 0 },
#endif // #ifdef MOZ_WEBRTC
  { "media.suspend-bkgnd-video.delay-ms", 10000 },
  { "media.throttle-factor", 2 },
  { "media.video-queue.default-size", 10 },
  { "media.video-queue.send-to-compositor-size", 9999 },
#ifdef MOZ_WEBRTC
  { "media.webrtc.debug.aec_dump_max_size", 4194304 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.webrtc.debug.trace_mask", 0 },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WMF
  { "media.wmf.decoder.thread-count", -1 },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.dxva.max-videos", 8 },
#endif // #ifdef MOZ_WMF
};

static const StringPref sCommonStringPrefs[] = {
  { "media.cubeb.logging_level", "" },
  { "media.decoder-doctor.decode-errors-allowed", "" },
  { "media.decoder-doctor.decode-warnings-allowed", "" },
  { "media.decoder-doctor.new-issue-endpoint", "https://webcompat.com/issues/new" },
#ifdef NIGHTLY_BUILD
  { "media.decoder-doctor.notifications-allowed", "MediaWMFNeeded,MediaWidevineNoWMF,MediaCannotInitializePulseAudio,MediaCannotPlayNoDecoders,MediaUnsupportedLibavcodec,MediaDecodeError" },
#endif // #ifdef NIGHTLY_BUILD
#ifdef NIGHTLY_BUILD
#else
  { "media.decoder-doctor.notifications-allowed", "MediaWMFNeeded,MediaWidevineNoWMF,MediaCannotInitializePulseAudio,MediaCannotPlayNoDecoders,MediaUnsupportedLibavcodec" },
#endif // #ifdef NIGHTLY_BUILD
  { "media.default_volume", "1.0" },
#ifdef RELEASE_OR_BETA
  { "media.getusermedia.screensharing.allowed_domains", "webex.com,*.webex.com,ciscospark.com,*.ciscospark.com,projectsquared.com,*.projectsquared.com,*.room.co,room.co,beta.talky.io,talky.io,*.clearslide.com,appear.in,*.appear.in,tokbox.com,*.tokbox.com,*.sso.francetelecom.fr,*.si.francetelecom.fr,*.sso.infra.ftgroup,*.multimedia-conference.orange-business.com,*.espacecollaboration.orange-business.com,free.gotomeeting.com,g2m.me,*.g2m.me,*.mypurecloud.com,*.mypurecloud.com.au,spreed.me,*.spreed.me,*.spreed.com,air.mozilla.org,*.circuit.com,*.yourcircuit.com,circuit.siemens.com,yourcircuit.siemens.com,circuitsandbox.net,*.unify.com,tandi.circuitsandbox.net,*.ericsson.net,*.cct.ericsson.net,*.opentok.com,*.conf.meetecho.com,meet.jit.si,*.meet.jit.si,web.stage.speakeasyapp.net,web.speakeasyapp.net,*.hipchat.me,*.beta-wspbx.com,*.wspbx.com,*.unifiedcloudit.com,*.smartboxuc.com,*.smartbox-uc.com,*.panterranetworks.com,pexipdemo.com,*.pexipdemo.com,pex.me,*.pex.me,*.rd.pexip.com,1click.io,*.1click.io,*.fuze.com,*.fuzemeeting.com,*.thinkingphones.com,gotomeeting.com,*.gotomeeting.com,gotowebinar.com,*.gotowebinar.com,gototraining.com,*.gototraining.com,citrix.com,*.citrix.com,expertcity.com,*.expertcity.com,citrixonline.com,*.citrixonline.com,g2m.me,*.g2m.me,gotomeet.me,*.gotomeet.me,gotomeet.at,*.gotomeet.at,miriadaxdes.miriadax.net,certificacion.miriadax.net,miriadax.net,*.wire.com,sylaps.com,*.sylaps.com,bluejeans.com,*.bluejeans.com,*.a.bluejeans.com,*.bbcollab.com" },
#endif // #ifdef RELEASE_OR_BETA
#ifdef RELEASE_OR_BETA
#else
  { "media.getusermedia.screensharing.allowed_domains", "mozilla.github.io,webex.com,*.webex.com,ciscospark.com,*.ciscospark.com,projectsquared.com,*.projectsquared.com,*.room.co,room.co,beta.talky.io,talky.io,*.clearslide.com,appear.in,*.appear.in,tokbox.com,*.tokbox.com,*.sso.francetelecom.fr,*.si.francetelecom.fr,*.sso.infra.ftgroup,*.multimedia-conference.orange-business.com,*.espacecollaboration.orange-business.com,free.gotomeeting.com,g2m.me,*.g2m.me,*.mypurecloud.com,*.mypurecloud.com.au,spreed.me,*.spreed.me,*.spreed.com,air.mozilla.org,*.circuit.com,*.yourcircuit.com,circuit.siemens.com,yourcircuit.siemens.com,circuitsandbox.net,*.unify.com,tandi.circuitsandbox.net,*.ericsson.net,*.cct.ericsson.net,*.opentok.com,*.conf.meetecho.com,meet.jit.si,*.meet.jit.si,web.stage.speakeasyapp.net,web.speakeasyapp.net,*.hipchat.me,*.beta-wspbx.com,*.wspbx.com,*.unifiedcloudit.com,*.smartboxuc.com,*.smartbox-uc.com,*.panterranetworks.com,pexipdemo.com,*.pexipdemo.com,pex.me,*.pex.me,*.rd.pexip.com,1click.io,*.1click.io,*.fuze.com,*.fuzemeeting.com,*.thinkingphones.com,gotomeeting.com,*.gotomeeting.com,gotowebinar.com,*.gotowebinar.com,gototraining.com,*.gototraining.com,citrix.com,*.citrix.com,expertcity.com,*.expertcity.com,citrixonline.com,*.citrixonline.com,g2m.me,*.g2m.me,gotomeet.me,*.gotomeet.me,gotomeet.at,*.gotomeet.at,miriadaxdes.miriadax.net,certificacion.miriadax.net,miriadax.net,*.wire.com,sylaps.com,*.sylaps.com,bluejeans.com,*.bluejeans.com,*.a.bluejeans.com,*.bbcollab.com" },
#endif // #ifdef RELEASE_OR_BETA
  { "media.gmp-manager.certs.1.commonName", "aus5.mozilla.org" },
  { "media.gmp-manager.certs.1.issuerName", "CN=DigiCert SHA2 Secure Server CA,O=DigiCert Inc,C=US" },
  { "media.gmp-manager.certs.2.commonName", "aus5.mozilla.org" },
  { "media.gmp-manager.certs.2.issuerName", "CN=thawte SSL CA - G2,O=\"thawte, Inc.\",C=US" },
  { "media.gmp-manager.url", "https://aus5.mozilla.org/update/3/GMP/%VERSION%/%BUILD_ID%/%BUILD_TARGET%/%LOCALE%/%CHANNEL%/%OS_VERSION%/%DISTRIBUTION%/%DISTRIBUTION_VERSION%/update.xml" },
#ifdef MOZ_WEBRTC
  { "media.navigator.load_adapt.high_load", "0.90" },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.navigator.load_adapt.low_load", "0.40" },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.default_iceservers", "[]" },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.peerconnection.ice.force_interface", "" },
#endif // #ifdef MOZ_WEBRTC
  { "media.volume_scale", "1.0" },
#ifdef MOZ_WEBRTC
  { "media.webrtc.debug.aec_log_dir", "" },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WEBRTC
  { "media.webrtc.debug.log_file", "" },
#endif // #ifdef MOZ_WEBRTC
#ifdef MOZ_WMF
  { "media.wmf.disable-d3d11-for-dlls", "igd11dxva64.dll: 20.19.15.4463, 20.19.15.4454, 20.19.15.4444, 20.19.15.4416, 20.19.15.4404, 20.19.15.4390, 20.19.15.4380, 20.19.15.4377, 20.19.15.4364, 20.19.15.4360, 20.19.15.4352, 20.19.15.4331, 20.19.15.4326, 20.19.15.4300; igd10iumd32.dll: 20.19.15.4444, 20.19.15.4424, 20.19.15.4409, 20.19.15.4390, 20.19.15.4380, 20.19.15.4360, 10.18.10.4358, 20.19.15.4331, 20.19.15.4312, 20.19.15.4300, 10.18.15.4281, 10.18.15.4279, 10.18.10.4276, 10.18.15.4268, 10.18.15.4256, 10.18.10.4252, 10.18.15.4248, 10.18.14.4112, 10.18.10.3958, 10.18.10.3496, 10.18.10.3431, 10.18.10.3412, 10.18.10.3355, 9.18.10.3234, 9.18.10.3071, 9.18.10.3055, 9.18.10.3006; igd10umd32.dll: 9.17.10.4229, 9.17.10.3040, 9.17.10.2884, 9.17.10.2857, 8.15.10.2274, 8.15.10.2272, 8.15.10.2246, 8.15.10.1840, 8.15.10.1808; igd10umd64.dll: 9.17.10.4229, 9.17.10.2884, 9.17.10.2857, 10.18.10.3496; isonyvideoprocessor.dll: 4.1.2247.8090, 4.1.2153.6200; tosqep.dll: 1.2.15.526, 1.1.12.201, 1.0.11.318, 1.0.11.215, 1.0.10.1224; tosqep64.dll: 1.1.12.201, 1.0.11.215; nvwgf2um.dll: 22.21.13.8253, 22.21.13.8233, 22.21.13.8205, 22.21.13.8189, 22.21.13.8178, 22.21.13.8165, 21.21.13.7892, 21.21.13.7878, 21.21.13.7866, 21.21.13.7849, 21.21.13.7654, 21.21.13.7653, 21.21.13.7633, 21.21.13.7619, 21.21.13.7563, 21.21.13.7306, 21.21.13.7290, 21.21.13.7270, 21.21.13.7254, 21.21.13.6939, 21.21.13.6926, 21.21.13.6909, 21.21.13.4201, 21.21.13.4200, 10.18.13.6881, 10.18.13.6839, 10.18.13.6510, 10.18.13.6472, 10.18.13.6143, 10.18.13.5946, 10.18.13.5923, 10.18.13.5921, 10.18.13.5891, 10.18.13.5887, 10.18.13.5582, 10.18.13.5445, 10.18.13.5382, 10.18.13.5362, 9.18.13.4788, 9.18.13.4752, 9.18.13.4725, 9.18.13.4709, 9.18.13.4195, 9.18.13.4192, 9.18.13.4144, 9.18.13.4052, 9.18.13.3788, 9.18.13.3523, 9.18.13.3235, 9.18.13.3165, 9.18.13.2723, 9.18.13.2702, 9.18.13.1422, 9.18.13.1407, 9.18.13.1106, 9.18.13.546; atidxx32.dll: 21.19.151.3, 21.19.142.257, 21.19.137.514, 21.19.137.1, 21.19.134.1, 21.19.128.7, 21.19.128.4, 20.19.0.32837, 20.19.0.32832, 8.17.10.682, 8.17.10.671, 8.17.10.661, 8.17.10.648, 8.17.10.644, 8.17.10.625, 8.17.10.605, 8.17.10.581, 8.17.10.569, 8.17.10.560, 8.17.10.545, 8.17.10.539, 8.17.10.531, 8.17.10.525, 8.17.10.520, 8.17.10.519, 8.17.10.514, 8.17.10.511, 8.17.10.494, 8.17.10.489, 8.17.10.483, 8.17.10.453, 8.17.10.451, 8.17.10.441, 8.17.10.436, 8.17.10.432, 8.17.10.425, 8.17.10.418, 8.17.10.414, 8.17.10.401, 8.17.10.395, 8.17.10.385, 8.17.10.378, 8.17.10.362, 8.17.10.355, 8.17.10.342, 8.17.10.331, 8.17.10.318, 8.17.10.310, 8.17.10.286, 8.17.10.269, 8.17.10.261, 8.17.10.247, 8.17.10.240, 8.15.10.212; atidxx64.dll: 21.19.151.3, 21.19.142.257, 21.19.137.514, 21.19.137.1, 21.19.134.1, 21.19.128.7, 21.19.128.4, 20.19.0.32832, 8.17.10.682, 8.17.10.661, 8.17.10.644, 8.17.10.625; nvumdshim.dll: 10.18.13.6822" },
#endif // #ifdef MOZ_WMF
#ifdef MOZ_WMF
  { "media.wmf.disable-d3d9-for-dlls", "igdumd64.dll: 8.15.10.2189, 8.15.10.2119, 8.15.10.2104, 8.15.10.2102, 8.771.1.0; atiumd64.dll: 7.14.10.833, 7.14.10.867, 7.14.10.885, 7.14.10.903, 7.14.10.911, 8.14.10.768, 9.14.10.1001, 9.14.10.1017, 9.14.10.1080, 9.14.10.1128, 9.14.10.1162, 9.14.10.1171, 9.14.10.1183, 9.14.10.1197, 9.14.10.945, 9.14.10.972, 9.14.10.984, 9.14.10.996" },
#endif // #ifdef MOZ_WMF
};

