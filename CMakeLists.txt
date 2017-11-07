cmake_minimum_required (VERSION 3.1)
project (gecko_media)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(LINUX 1)
else()
  set(LINUX 0)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  set(MACOS 1)
else()
  set(MACOS 0)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(WINDOWS 1)
else()
  set(WINDOWS 0)
endif()

if($ENV{TARGET} MATCHES "Android")
  set(ANDROID 1)
else()
  set(ANDROID 0)
endif()

if ($ENV{TARGET} MATCHES "x86")
  set(X86 1) # Note: *Doesn't* imply NOT X86_64
else()
  set(X86 0)
endif()

if($ENV{TARGET} MATCHES "x86_64")
  set(X86_64 1)
else()
  set(X86_64 0)
endif()

if ($ENV{TARGET} MATCHES "arm")
  set(ARM 1)
else()
  set(ARM 0)
endif()

if($ENV{CARGO_FEATURE_AUDIO_SAMPLE_TYPE_F32})
  set(FEATURE_AUDIO_SAMPLE_TYPE_F32 1)
else()
  set(FEATURE_AUDIO_SAMPLE_TYPE_F32 0)
endif()

if($ENV{CARGO_FEATURE_AUDIO_SAMPLE_TYPE_S16})
  set(FEATURE_AUDIO_SAMPLE_TYPE_S16 1)
else()
  set(FEATURE_AUDIO_SAMPLE_TYPE_S16 0)
endif()

if($ENV{PROFILE} STREQUAL "debug")
  add_definitions(-DDEBUG=1)
endif()

find_program(SCCACHE_PROGRAM sccache)
if(SCCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${SCCACHE_PROGRAM}")
endif()

add_definitions(-DGECKO_MEDIA_CRATE=1)
add_definitions(-DMOZILLA_INTERNAL_API=1)
add_definitions(-D_PR_PTHREADS=1)
add_definitions(-DGECKO_MEDIA_CRATE=1)
add_definitions(-DRUST_BINDGEN=1)

include_directories(
  gecko/include
  gecko/glue/include
)

if(X86_64)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

if(LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --include ${CMAKE_SOURCE_DIR}/gecko/glue/include/mozilla-config-x86_64-linux.h")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --include ${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --include ${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
endif()

if(MACOS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/glue/include/mozilla-config-x86_64-apple-darwin.h")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inline-new-delete")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/glue/include/mozilla-config-x86_64-apple-darwin.h")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-inline-new-delete")

  add_definitions(-DDARWIN=1)

endif()




############
# libogg
############

add_library(ogg OBJECT
  gecko/src/media/libogg/src/ogg_alloc.c
  gecko/src/media/libogg/src/ogg_bitwise.c
  gecko/src/media/libogg/src/ogg_framing.c
  )
target_include_directories(ogg PRIVATE gecko/include/mozilla/media/libogg/include)




############
# Gecko core
############

add_library(gecko_core OBJECT
  gecko/glue/AsyncShutdown.cpp
  gecko/glue/AbstractThread.cpp
  gecko/glue/AgnosticDecoderModule.cpp
  gecko/glue/Benchmark.cpp
  gecko/glue/CooperativeThreadPool.cpp
  gecko/glue/CubebUtils.cpp
  gecko/glue/DecoderTraits.cpp
  gecko/glue/FFmpegRuntimeLinker.cpp
  gecko/glue/GeckoMedia.cpp
  gecko/glue/GeckoMediaDecoder.cpp
  gecko/glue/ImageContainer.cpp
  gecko/glue/InputEventStatistics.cpp
  gecko/glue/nsDebugImpl.cpp
  gecko/glue/LabeledEventQueue.cpp
  gecko/glue/Logging.cpp
  gecko/glue/MainThreadIdlePeriod.cpp
  gecko/glue/MediaData.cpp
  gecko/glue/MediaDecoder.cpp
  gecko/glue/MediaDecoderStateMachine.cpp
  gecko/glue/MediaFormatReader.cpp
  gecko/glue/MediaStreamGraph.cpp
  gecko/glue/PDMFactory.cpp
  gecko/glue/nsAppRunner.cpp
  gecko/glue/nsContentTypeParser.cpp
  gecko/glue/nsCategoryManager.cpp
  gecko/glue/nsComponentManager.cpp
  gecko/glue/nsComponentManagerUtils.cpp
  gecko/glue/nsObserverList.cpp
  gecko/glue/nsObserverService.cpp
  gecko/glue/nsThread.cpp
  gecko/glue/nsTraceRefcnt.cpp
  gecko/glue/nsCRTGlue.cpp
  gecko/glue/Preferences.cpp
  gecko/glue/Scheduler.cpp
  gecko/glue/SchedulerGroup.cpp
  gecko/glue/Services.cpp
  gecko/glue/Telemetry.cpp
  gecko/glue/ThreadEventQueue.cpp
  gecko/glue/ThreadAnnotation.cpp
  gecko/glue/VideoSegment.cpp
  gecko/glue/WebAudioUtils.cpp
  gecko/glue/XPCOMInit.cpp
  gecko/src/dom/media/AudioConverter.cpp
  gecko/src/dom/media/AudioSegment.cpp
  gecko/src/dom/media/AudioStream.cpp
  gecko/src/dom/media/MediaContainerType.cpp
  gecko/src/dom/media/MediaInfo.cpp
  gecko/src/dom/media/MediaMIMETypes.cpp
  gecko/src/dom/media/MediaShutdownManager.cpp
  gecko/src/dom/media/MediaPrefs.cpp
  gecko/src/dom/media/MediaResource.cpp
  gecko/src/dom/media/MediaStreamListener.cpp
  gecko/src/dom/media/MediaTimer.cpp
  gecko/src/dom/media/QueueObject.cpp
  gecko/src/dom/media/ReaderProxy.cpp
  gecko/src/dom/media/SeekJob.cpp
  gecko/src/dom/media/StreamTracks.cpp
  gecko/src/dom/media/fmp4/MP4Decoder.cpp
  gecko/src/dom/media/fmp4/MP4Demuxer.cpp
  gecko/src/dom/media/mediasink/AudioSink.cpp
  gecko/src/dom/media/mediasink/AudioSinkWrapper.cpp
  gecko/src/dom/media/mediasink/DecodedStream.cpp
  gecko/src/dom/media/mediasink/OutputStreamManager.cpp
  gecko/src/dom/media/mp3/MP3Decoder.cpp
  gecko/src/dom/media/mp3/MP3Demuxer.cpp
  gecko/src/dom/media/mp3/MP3FrameParser.cpp
  gecko/src/dom/media/ogg/OpusParser.cpp
  gecko/src/dom/media/ogg/OpusParser.cpp
  gecko/src/dom/media/platforms/agnostic/BlankDecoderModule.cpp
  gecko/src/dom/media/platforms/agnostic/DummyMediaDataDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/NullDecoderModule.cpp
  gecko/src/dom/media/platforms/agnostic/OpusDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/VorbisDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/WAVDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/FFmpegLibWrapper.cpp
  gecko/src/dom/media/platforms/wrappers/H264Converter.cpp
  gecko/src/dom/media/VideoUtils.cpp
  gecko/src/dom/media/wave/WaveDecoder.cpp
  gecko/src/dom/media/wave/WaveDemuxer.cpp
  gecko/src/memory/fallible/fallible.cpp
  gecko/src/memory/mozalloc/mozalloc.cpp
  gecko/src/memory/mozalloc/mozalloc_abort.cpp
  gecko/src/memory/mozalloc/mozalloc_oom.cpp
  gecko/src/mfbt/Assertions.cpp
  gecko/src/mfbt/ChaosMode.cpp
  gecko/src/mfbt/HashFunctions.cpp
  gecko/src/mfbt/Unused.cpp
  gecko/src/mozglue/misc/ConditionVariable_posix.cpp
  gecko/src/mozglue/misc/Printf.cpp
  gecko/src/mozglue/misc/TimeStamp.cpp
  gecko/src/toolkit/library/StaticXULComponentsStart.cpp
  gecko/src/toolkit/library/StaticXULComponentsEnd/StaticXULComponentsEnd.cpp
  gecko/src/xpcom/base/ClearOnShutdown.cpp
  gecko/src/xpcom/base/ErrorNames.cpp
  gecko/src/xpcom/base/nsCOMPtr.cpp
  gecko/src/xpcom/base/nsClassInfoImpl.cpp
  gecko/src/xpcom/base/nsID.cpp
  gecko/src/xpcom/base/nsISupportsImpl.cpp
  gecko/src/xpcom/base/nsMemory.cpp
  gecko/src/xpcom/base/nsWeakReference.cpp
  gecko/src/xpcom/components/GenericFactory.cpp
  gecko/src/xpcom/ds/PLDHashTable.cpp
  gecko/src/xpcom/ds/nsCOMArray.cpp
  gecko/src/xpcom/ds/nsDeque.cpp
  gecko/src/xpcom/ds/nsEnumeratorUtils.cpp
  gecko/src/xpcom/ds/nsQuickSort.cpp
  gecko/src/xpcom/ds/nsSupportsPrimitives.cpp
  gecko/src/xpcom/ds/nsTArray.cpp
  gecko/src/xpcom/ds/nsTObserverArray.cpp
  gecko/src/xpcom/io/nsDirectoryService.cpp
  gecko/src/xpcom/string/nsReadableUtilsSSE2.cpp
  gecko/src/xpcom/string/nsUTF8UtilsSSE2.cpp
  gecko/src/xpcom/string/unified.cpp
  gecko/src/xpcom/threads/BlockingResourceBase.cpp
  gecko/src/xpcom/threads/EventQueue.cpp
  gecko/src/xpcom/threads/nsILabelableRunnable.cpp
  gecko/src/xpcom/threads/nsProxyRelease.cpp
  gecko/src/xpcom/threads/nsThreadManager.cpp
  gecko/src/xpcom/threads/nsThreadPool.cpp
  gecko/src/xpcom/threads/nsThreadUtils.cpp
  gecko/src/xpcom/threads/nsTimerImpl.cpp
  gecko/src/xpcom/threads/PrioritizedEventQueue.cpp
  gecko/src/xpcom/threads/SharedThreadPool.cpp
  gecko/src/xpcom/threads/RecursiveMutex.cpp
  gecko/src/xpcom/threads/SynchronizedEventQueue.cpp
  gecko/src/xpcom/threads/SystemGroup.cpp
  gecko/src/xpcom/threads/TaskQueue.cpp
  gecko/src/xpcom/threads/ThreadEventTarget.cpp
  gecko/src/xpcom/threads/TimerThread.cpp
  gecko/src/nsprpub/pr/src/io/priometh.c
  gecko/src/nsprpub/pr/src/io/prfdcach.c
  gecko/src/nsprpub/pr/src/io/prlayer.c
  gecko/src/nsprpub/pr/src/io/prlog.c
  gecko/src/nsprpub/pr/src/io/prmapopt.c
  gecko/src/nsprpub/pr/src/io/prmmap.c
  gecko/src/nsprpub/pr/src/io/prmwait.c
  gecko/src/nsprpub/pr/src/io/prprf.c
  gecko/src/nsprpub/pr/src/linking/prlink.c
  gecko/src/nsprpub/pr/src/malloc/prmem.c
  gecko/src/nsprpub/pr/src/md/prosdep.c
  gecko/src/nsprpub/pr/src/md/unix/unix.c
  gecko/src/nsprpub/pr/src/md/unix/unix_errors.c
  gecko/src/nsprpub/pr/src/memory/prseg.c
  gecko/src/nsprpub/pr/src/misc/pratom.c
  gecko/src/nsprpub/pr/src/misc/prdtoa.c
  gecko/src/nsprpub/pr/src/misc/prenv.c
  gecko/src/nsprpub/pr/src/misc/prerr.c
  gecko/src/nsprpub/pr/src/misc/prerror.c
  gecko/src/nsprpub/pr/src/misc/prerrortable.c
  gecko/src/nsprpub/pr/src/misc/prinit.c
  gecko/src/nsprpub/pr/src/misc/prinrval.c
  gecko/src/nsprpub/pr/src/misc/prnetdb.c
  gecko/src/nsprpub/pr/src/misc/prsystem.c
  gecko/src/nsprpub/pr/src/misc/prtime.c
  gecko/src/nsprpub/pr/src/pthreads/ptio.c
  gecko/src/nsprpub/pr/src/pthreads/ptmisc.c
  gecko/src/nsprpub/pr/src/pthreads/ptsynch.c
  gecko/src/nsprpub/pr/src/pthreads/ptthread.c
  gecko/src/nsprpub/pr/src/threads/prcmon.c
  gecko/src/nsprpub/pr/src/threads/prrwlock.c
  gecko/src/nsprpub/pr/src/threads/prtpd.c
)
if(LINUX)
  target_sources(gecko_core PRIVATE
    gecko/src/mozglue/misc/Mutex_posix.cpp
    gecko/src/mozglue/misc/TimeStamp_posix.cpp
    gecko/src/nsprpub/pr/src/md/unix/linux.c
  )
endif()
if(MACOS)
  target_sources(gecko_core PRIVATE
    gecko/glue/AppleDecoderModule.cpp
    gecko/glue/AppleVTDecoder.cpp
    gecko/glue/MacIOSurface.cpp
    gecko/src/dom/media/platforms/apple/AppleATDecoder.cpp
    gecko/src/dom/media/platforms/apple/AppleCMLinker.cpp
    gecko/src/dom/media/platforms/apple/AppleVTLinker.cpp
    gecko/src/mozglue/misc/Mutex_posix.cpp
    gecko/src/mozglue/misc/TimeStamp_darwin.cpp
    gecko/src/dom/media/systemservices/OSXRunLoopSingleton.cpp
    gecko/src/nsprpub/pr/src/md/unix/darwin.c
  )
endif()
if(WINDOWS)
  target_sources(gecko_core PRIVATE
    gecko/src/mozglue/misc/TimeStamp_windows.cpp
  )
endif()
if(ANDROID)
  target_sources(gecko_core PRIVATE
    gecko/src/mozglue/misc/Mutex_posix.cpp
  )
endif()

target_include_directories(gecko_core PRIVATE
    gecko/glue/include/stl_wrappers
    gecko/include/system_wrappers
    gecko/include/nspr
    gecko/include/nspr/private
    gecko/include/mediasink
    gecko/include/mozilla/
    gecko/include/mozilla/gfx
    gecko/include/mozilla/double-conversion
    gecko/include/mozilla/media/libvorbis/include
    gecko/include/mozilla/media/libogg/include
    gecko/include/mozilla/media/libstagefright/
    gecko/include/mozilla/media/libstagefright/binding/include
    gecko/include/mozilla/media/libstagefright/frameworks/av/include
    gecko/include/mozilla/media/libstagefright/frameworks/av/include/media/stagefright/foundation
    gecko/include/mozilla/media/libstagefright/frameworks/av/media/libstagefright/
    gecko/include/mozilla/media/libstagefright/stubs/empty
    gecko/include/mozilla/media/libstagefright/stubs/include
    gecko/include/mozilla/media/libstagefright/stubs/include/media/stagefright/foundation
    gecko/include/mozilla/media/libstagefright/system/core/include
)

target_compile_definitions(gecko_core PRIVATE
  MOZILLA_INTERNAL_API=1
  _PR_PTHREADS=1
  GECKO_MEDIA_CRATE=1
  RUST_BINDGEN=1
)

############
# ffvpx
############

if (LINUX OR MACOS)
add_library(ffvpx OBJECT
  gecko/src/dom/media/platforms/ffmpeg/FFmpegAudioDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/FFmpegDataDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/FFmpegDecoderModule.cpp
  gecko/glue/FFmpegVideoDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/ffvpx/FFVPXRuntimeLinker.cpp
)
target_include_directories(ffvpx PRIVATE
    gecko/include/ffmpeg57/include
    gecko/include/nspr
    gecko/include/nspr/private
    gecko/include
    gecko/glue/include
    gecko/include/mozilla/
)
target_compile_definitions(ffvpx PRIVATE
  FFVPX_VERSION=46465650
  USING_MOZFFVPX=1
)
endif()

############
# ffmpeg
############
if (LINUX OR MACOS)
add_library(ffmpeg57 OBJECT
  gecko/src/dom/media/platforms/ffmpeg/FFmpegAudioDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/FFmpegDataDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/FFmpegDecoderModule.cpp
  gecko/glue/FFmpegVideoDecoder.cpp
)
target_include_directories(ffmpeg57 PRIVATE
    gecko/include/ffmpeg57/include
    gecko/include/nspr
    gecko/include/nspr/private
    gecko/include
    gecko/glue/include
    gecko/include/mozilla/
)
endif()


############
# libopus
############

add_library(opus OBJECT
  gecko/src/media/libopus/celt/bands.c
  gecko/src/media/libopus/celt/celt.c
  gecko/src/media/libopus/celt/celt_decoder.c
  gecko/src/media/libopus/celt/celt_encoder.c
  gecko/src/media/libopus/celt/celt_lpc.c
  gecko/src/media/libopus/celt/cwrs.c
  gecko/src/media/libopus/celt/entcode.c
  gecko/src/media/libopus/celt/entdec.c
  gecko/src/media/libopus/celt/entenc.c
  gecko/src/media/libopus/celt/kiss_fft.c
  gecko/src/media/libopus/celt/laplace.c
  gecko/src/media/libopus/celt/mathops.c
  gecko/src/media/libopus/celt/mdct.c
  gecko/src/media/libopus/celt/modes.c
  gecko/src/media/libopus/celt/pitch.c
  gecko/src/media/libopus/celt/quant_bands.c
  gecko/src/media/libopus/celt/rate.c
  gecko/src/media/libopus/celt/vq.c
  gecko/src/media/libopus/silk/A2NLSF.c
  gecko/src/media/libopus/silk/ana_filt_bank_1.c
  gecko/src/media/libopus/silk/biquad_alt.c
  gecko/src/media/libopus/silk/bwexpander.c
  gecko/src/media/libopus/silk/bwexpander_32.c
  gecko/src/media/libopus/silk/check_control_input.c
  gecko/src/media/libopus/silk/CNG.c
  gecko/src/media/libopus/silk/code_signs.c
  gecko/src/media/libopus/silk/control_audio_bandwidth.c
  gecko/src/media/libopus/silk/control_codec.c
  gecko/src/media/libopus/silk/control_SNR.c
  gecko/src/media/libopus/silk/debug.c
  gecko/src/media/libopus/silk/dec_API.c
  gecko/src/media/libopus/silk/decode_core.c
  gecko/src/media/libopus/silk/decode_frame.c
  gecko/src/media/libopus/silk/decode_indices.c
  gecko/src/media/libopus/silk/decode_parameters.c
  gecko/src/media/libopus/silk/decode_pitch.c
  gecko/src/media/libopus/silk/decode_pulses.c
  gecko/src/media/libopus/silk/decoder_set_fs.c
  gecko/src/media/libopus/silk/enc_API.c
  gecko/src/media/libopus/silk/encode_indices.c
  gecko/src/media/libopus/silk/encode_pulses.c
  gecko/src/media/libopus/silk/gain_quant.c
  gecko/src/media/libopus/silk/HP_variable_cutoff.c
  gecko/src/media/libopus/silk/init_decoder.c
  gecko/src/media/libopus/silk/init_encoder.c
  gecko/src/media/libopus/silk/inner_prod_aligned.c
  gecko/src/media/libopus/silk/interpolate.c
  gecko/src/media/libopus/silk/lin2log.c
  gecko/src/media/libopus/silk/log2lin.c
  gecko/src/media/libopus/silk/LP_variable_cutoff.c
  gecko/src/media/libopus/silk/LPC_analysis_filter.c
  gecko/src/media/libopus/silk/LPC_fit.c
  gecko/src/media/libopus/silk/LPC_inv_pred_gain.c
  gecko/src/media/libopus/silk/NLSF2A.c
  gecko/src/media/libopus/silk/NLSF_decode.c
  gecko/src/media/libopus/silk/NLSF_del_dec_quant.c
  gecko/src/media/libopus/silk/NLSF_encode.c
  gecko/src/media/libopus/silk/NLSF_stabilize.c
  gecko/src/media/libopus/silk/NLSF_unpack.c
  gecko/src/media/libopus/silk/NLSF_VQ.c
  gecko/src/media/libopus/silk/NLSF_VQ_weights_laroia.c
  gecko/src/media/libopus/silk/NSQ.c
  gecko/src/media/libopus/silk/NSQ_del_dec.c
  gecko/src/media/libopus/silk/pitch_est_tables.c
  gecko/src/media/libopus/silk/PLC.c
  gecko/src/media/libopus/silk/process_NLSFs.c
  gecko/src/media/libopus/silk/quant_LTP_gains.c
  gecko/src/media/libopus/silk/resampler.c
  gecko/src/media/libopus/silk/resampler_down2.c
  gecko/src/media/libopus/silk/resampler_down2_3.c
  gecko/src/media/libopus/silk/resampler_private_AR2.c
  gecko/src/media/libopus/silk/resampler_private_down_FIR.c
  gecko/src/media/libopus/silk/resampler_private_IIR_FIR.c
  gecko/src/media/libopus/silk/resampler_private_up2_HQ.c
  gecko/src/media/libopus/silk/resampler_rom.c
  gecko/src/media/libopus/silk/shell_coder.c
  gecko/src/media/libopus/silk/sigm_Q15.c
  gecko/src/media/libopus/silk/sort.c
  gecko/src/media/libopus/silk/stereo_decode_pred.c
  gecko/src/media/libopus/silk/stereo_encode_pred.c
  gecko/src/media/libopus/silk/stereo_find_predictor.c
  gecko/src/media/libopus/silk/stereo_LR_to_MS.c
  gecko/src/media/libopus/silk/stereo_MS_to_LR.c
  gecko/src/media/libopus/silk/stereo_quant_pred.c
  gecko/src/media/libopus/silk/sum_sqr_shift.c
  gecko/src/media/libopus/silk/table_LSF_cos.c
  gecko/src/media/libopus/silk/tables_gain.c
  gecko/src/media/libopus/silk/tables_LTP.c
  gecko/src/media/libopus/silk/tables_NLSF_CB_NB_MB.c
  gecko/src/media/libopus/silk/tables_NLSF_CB_WB.c
  gecko/src/media/libopus/silk/tables_other.c
  gecko/src/media/libopus/silk/tables_pitch_lag.c
  gecko/src/media/libopus/silk/tables_pulses_per_block.c
  gecko/src/media/libopus/silk/VAD.c
  gecko/src/media/libopus/silk/VQ_WMat_EC.c
  gecko/src/media/libopus/src/opus.c
  gecko/src/media/libopus/src/opus_decoder.c
  gecko/src/media/libopus/src/opus_encoder.c
  gecko/src/media/libopus/src/opus_multistream.c
  gecko/src/media/libopus/src/opus_multistream_decoder.c
  gecko/src/media/libopus/src/opus_multistream_encoder.c
  gecko/src/media/libopus/src/repacketizer.c
)

if(FEATURE_AUDIO_SAMPLE_TYPE_F32)
  target_include_directories(opus PRIVATE gecko/include/mozilla/media/libopus/silk/float)
  target_sources(opus PRIVATE
    gecko/src/media/libopus/silk/float/apply_sine_window_FLP.c
    gecko/src/media/libopus/silk/float/autocorrelation_FLP.c
    gecko/src/media/libopus/silk/float/burg_modified_FLP.c
    gecko/src/media/libopus/silk/float/bwexpander_FLP.c
    gecko/src/media/libopus/silk/float/corrMatrix_FLP.c
    gecko/src/media/libopus/silk/float/encode_frame_FLP.c
    gecko/src/media/libopus/silk/float/energy_FLP.c
    gecko/src/media/libopus/silk/float/find_LPC_FLP.c
    gecko/src/media/libopus/silk/float/find_LTP_FLP.c
    gecko/src/media/libopus/silk/float/find_pitch_lags_FLP.c
    gecko/src/media/libopus/silk/float/find_pred_coefs_FLP.c
    gecko/src/media/libopus/silk/float/inner_product_FLP.c
    gecko/src/media/libopus/silk/float/k2a_FLP.c
    gecko/src/media/libopus/silk/float/LPC_analysis_filter_FLP.c
    gecko/src/media/libopus/silk/float/LPC_inv_pred_gain_FLP.c
    gecko/src/media/libopus/silk/float/LTP_analysis_filter_FLP.c
    gecko/src/media/libopus/silk/float/LTP_scale_ctrl_FLP.c
    gecko/src/media/libopus/silk/float/noise_shape_analysis_FLP.c
    gecko/src/media/libopus/silk/float/pitch_analysis_core_FLP.c
    gecko/src/media/libopus/silk/float/process_gains_FLP.c
    gecko/src/media/libopus/silk/float/regularize_correlations_FLP.c
    gecko/src/media/libopus/silk/float/residual_energy_FLP.c
    gecko/src/media/libopus/silk/float/scale_copy_vector_FLP.c
    gecko/src/media/libopus/silk/float/scale_vector_FLP.c
    gecko/src/media/libopus/silk/float/schur_FLP.c
    gecko/src/media/libopus/silk/float/sort_FLP.c
    gecko/src/media/libopus/silk/float/warped_autocorrelation_FLP.c
    gecko/src/media/libopus/silk/float/wrappers_FLP.c
    gecko/src/media/libopus/src/analysis.c
    gecko/src/media/libopus/src/mlp.c
    gecko/src/media/libopus/src/mlp_data.c
  )
else()
  target_compile_definitions(opus PRIVATE
    FIXED_POINT=1
    DISABLE_FLOAT_API=1
  )
  target_include_directories(opus PRIVATE gecko/include/mozilla/media/libopus/silk/fixed)
  target_sources(opus PRIVATE
    gecko/src/media/libopus/silk/fixed/apply_sine_window_FIX.c
    gecko/src/media/libopus/silk/fixed/autocorr_FIX.c
    gecko/src/media/libopus/silk/fixed/burg_modified_FIX.c
    gecko/src/media/libopus/silk/fixed/corrMatrix_FIX.c
    gecko/src/media/libopus/silk/fixed/encode_frame_FIX.c
    gecko/src/media/libopus/silk/fixed/find_LPC_FIX.c
    gecko/src/media/libopus/silk/fixed/find_LTP_FIX.c
    gecko/src/media/libopus/silk/fixed/find_pitch_lags_FIX.c
    gecko/src/media/libopus/silk/fixed/find_pred_coefs_FIX.c
    gecko/src/media/libopus/silk/fixed/k2a_FIX.c
    gecko/src/media/libopus/silk/fixed/k2a_Q16_FIX.c
    gecko/src/media/libopus/silk/fixed/LTP_analysis_filter_FIX.c
    gecko/src/media/libopus/silk/fixed/LTP_scale_ctrl_FIX.c
    gecko/src/media/libopus/silk/fixed/noise_shape_analysis_FIX.c
    gecko/src/media/libopus/silk/fixed/pitch_analysis_core_FIX.c
    gecko/src/media/libopus/silk/fixed/process_gains_FIX.c
    gecko/src/media/libopus/silk/fixed/regularize_correlations_FIX.,
    gecko/src/media/libopus/silk/fixed/residual_energy16_FIX.c
    gecko/src/media/libopus/silk/fixed/residual_energy_FIX.c
    gecko/src/media/libopus/silk/fixed/schur64_FIX.c
    gecko/src/media/libopus/silk/fixed/schur_FIX.c
    gecko/src/media/libopus/silk/fixed/vector_ops_FIX.c
    gecko/src/media/libopus/silk/fixed/warped_autocorrelation_FIX.c
  )
endif()

if(X86 OR X86_64)
  target_compile_definitions(opus PRIVATE
    OPUS_HAVE_RTCD=
    OPUS_X86_MAY_HAVE_SSE=
    OPUS_X86_MAY_HAVE_SSE2=
    OPUS_X86_MAY_HAVE_SSE4_1=
    OPUS_X86_MAY_HAVE_AVX=
  )
  target_include_directories(opus PRIVATE gecko/include/mozilla/media/libopus/celt/x86)
  set(${CMAKE_C_FLAGS} "${CMAKE_C_FLAGS} -msse4.1")
  target_compile_options(opus PRIVATE "-msse4.1")

  target_sources(opus PRIVATE
    gecko/src/media/libopus/celt/x86/pitch_sse.c
    gecko/src/media/libopus/celt/x86/x86_celt_map.c
    gecko/src/media/libopus/celt/x86/x86cpu.c
    gecko/src/media/libopus/celt/x86/pitch_sse2.c
    gecko/src/media/libopus/celt/x86/vq_sse2.c
    gecko/src/media/libopus/celt/x86/celt_lpc_sse.c
    gecko/src/media/libopus/celt/x86/pitch_sse4_1.c
    gecko/src/media/libopus/silk/x86/NSQ_del_dec_sse.c
    gecko/src/media/libopus/silk/x86/NSQ_sse.c
    gecko/src/media/libopus/silk/x86/VAD_sse.c
    gecko/src/media/libopus/silk/x86/VQ_WMat_EC_sse.c
    gecko/src/media/libopus/silk/x86/x86_silk_map.c
  )
  if(NOT FEATURE_AUDIO_SAMPLE_TYPE_F32)
    target_sources(opus PRIVATE
      gecko/src/media/libopus/silk/fixed/x86/burg_modified_FIX_sse.c
      gecko/src/media/libopus/silk/fixed/x86/vector_ops_FIX_sse.c
    )
  endif()
endif()

if(ANDROID)
  target_sources(opus PRIVATE
    gecko/src/media/libopus/silk/arm/arm_silk_map.c
    gecko/src/media/libopus/silk/arm/biquad_alt_neon_intr.c
    gecko/src/media/libopus/silk/arm/LPC_inv_pred_gain_neon_intr.c
    gecko/src/media/libopus/silk/arm/NSQ_del_dec_neon_intr.c
    gecko/src/media/libopus/silk/arm/NSQ_neon.c
    gecko/src/media/libopus/silk/fixed/arm/warped_autocorrelation_FIX_neon_intr.c
    gecko/src/media/libopus/celt/arm/arm_celt_map.c
    gecko/src/media/libopus/celt/arm/armcpu.c
    gecko/src/media/libopus/celt/arm/celt_ne10_fft.c
    gecko/src/media/libopus/celt/arm/celt_ne10_mdct.c
    gecko/src/media/libopus/celt/arm/celt_neon_intr.c
    gecko/src/media/libopus/celt/arm/pitch_neon_intr.c
  )
endif()

target_compile_definitions(opus PRIVATE
  OPUS_BUILD=
  OPUS_VERSION="v1.2.1-mozilla"
  USE_ALLOCA=
  OPUS_EXPORT=
)

if(MACOS OR LINUX)
  target_compile_definitions(opus PRIVATE
    HAVE_LRINTF=
  )
endif()

target_include_directories(opus PRIVATE
  gecko/include/mozilla/media/libopus
  gecko/include/mozilla/media/libopus/celt
  gecko/include/mozilla/media/libopus/include
  gecko/include/mozilla/media/libopus/silk
  gecko/include/mozilla/media/libopus/src
)




############
# libvorbis
############
set(vorbis_srcs
  gecko/src/media/libvorbis/lib/vorbis_analysis.c
  gecko/src/media/libvorbis/lib/vorbis_bitrate.c
  gecko/src/media/libvorbis/lib/vorbis_block.c
  gecko/src/media/libvorbis/lib/vorbis_codebook.c
  gecko/src/media/libvorbis/lib/vorbis_envelope.c
  gecko/src/media/libvorbis/lib/vorbis_floor0.c
  gecko/src/media/libvorbis/lib/vorbis_floor1.c
  gecko/src/media/libvorbis/lib/vorbis_info.c
  gecko/src/media/libvorbis/lib/vorbis_lookup.c
  gecko/src/media/libvorbis/lib/vorbis_lpc.c
  gecko/src/media/libvorbis/lib/vorbis_lsp.c
  gecko/src/media/libvorbis/lib/vorbis_mapping0.c
  gecko/src/media/libvorbis/lib/vorbis_mdct.c
  gecko/src/media/libvorbis/lib/vorbis_psy.c
  gecko/src/media/libvorbis/lib/vorbis_registry.c
  gecko/src/media/libvorbis/lib/vorbis_res0.c
  gecko/src/media/libvorbis/lib/vorbis_sharedbook.c
  gecko/src/media/libvorbis/lib/vorbis_smallft.c
  gecko/src/media/libvorbis/lib/vorbis_synthesis.c
  gecko/src/media/libvorbis/lib/vorbis_window.c
  gecko/src/media/libvorbis/lib/vorbisenc.c
)

add_library(vorbis OBJECT ${vorbis_srcs})

target_include_directories(vorbis PRIVATE
  gecko/include/mozilla/media/libvorbis/lib
  gecko/include/mozilla/media/libvorbis/include
  gecko/include/mozilla/media/libogg/include/
)




############
# libstagefright
############
add_library(stagefright OBJECT
  gecko/src/media/libstagefright/binding/Adts.cpp
  gecko/src/media/libstagefright/binding/AnnexB.cpp
  gecko/src/media/libstagefright/binding/BitReader.cpp
  gecko/src/media/libstagefright/binding/Box.cpp
  gecko/src/media/libstagefright/binding/BufferStream.cpp
  gecko/src/media/libstagefright/binding/DecoderData.cpp
  gecko/src/media/libstagefright/binding/H264.cpp
  gecko/src/media/libstagefright/binding/Index.cpp
  gecko/src/media/libstagefright/binding/MP4Metadata.cpp
  gecko/src/media/libstagefright/binding/MoofParser.cpp
  gecko/src/media/libstagefright/binding/ResourceStream.cpp
  gecko/src/media/libstagefright/binding/SinfParser.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/DataSource.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/ESDS.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/MPEG4Extractor.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/MediaBuffer.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/MediaDefs.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/MediaSource.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/MetaData.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/SampleIterator.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/SampleTable.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/Utils.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/foundation/AAtomizer.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/foundation/ABitReader.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/foundation/ABuffer.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/foundation/AString.cpp
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright/foundation/hexdump.cpp
  # "media/libstagefright/gtest/TestInterval.cpp
  # "media/libstagefright/gtest/TestMP4Rust.cpp
  # "media/libstagefright/gtest/TestParser.cpp
  gecko/src/media/libstagefright/system/core/libutils/RefBase.cpp
  gecko/src/media/libstagefright/system/core/libutils/SharedBuffer.cpp
  gecko/src/media/libstagefright/system/core/libutils/Static.cpp
  gecko/src/media/libstagefright/system/core/libutils/String16.cpp
  gecko/src/media/libstagefright/system/core/libutils/String8.cpp
  gecko/src/media/libstagefright/system/core/libutils/Unicode.cpp
  gecko/src/media/libstagefright/system/core/libutils/VectorImpl.cpp
)

if(NOT ANDROID)
  target_compile_definitions(stagefright PRIVATE
    FAKE_LOG_DEVICE=1
  )
  target_sources(stagefright PRIVATE
    gecko/src/media/libstagefright/system/core/libcutils/strdup16to8.c
    gecko/src/media/libstagefright/system/core/liblog/fake_log_device.c
    gecko/src/media/libstagefright/system/core/liblog/logd_write.c
    gecko/src/media/libstagefright/system/core/liblog/logprint.c
  )
endif()

target_include_directories(stagefright PRIVATE
  gecko/glue/include
  gecko/glue/include/stl_wrappers
  gecko/glue/include/system_wrappers
  gecko/include
  gecko/include/mozilla/
  gecko/include/mozilla/media/libstagefright/binding/include
  gecko/include/mozilla/media/libstagefright/frameworks/av/include
  gecko/include/mozilla/media/libstagefright/frameworks/av/include/media/stagefright/foundation
  gecko/include/mozilla/media/libstagefright/frameworks/av/media/libstagefright
  gecko/include/mozilla/media/libstagefright/stubs/empty
  gecko/include/mozilla/media/libstagefright/stubs/include
  gecko/include/mozilla/media/libstagefright/stubs/include/media/stagefright/foundation/
  gecko/include/mozilla/media/libstagefright/system/core/include
  gecko/include/nspr
  gecko/include/system_wrappers
  gecko/src/media/libstagefright/binding/include
  gecko/src/media/libstagefright/frameworks/av/include
  gecko/src/media/libstagefright/frameworks/av/include/media/stagefright/foundation
  gecko/src/media/libstagefright/frameworks/av/media/libstagefright
  gecko/src/media/libstagefright/stubs/empty
  gecko/src/media/libstagefright/stubs/include
  gecko/src/media/libstagefright/stubs/include/media/stagefright/foundation
  gecko/src/media/libstagefright/system/core/include
)

if(MACOS OR LINUX)
  target_compile_definitions(stagefright PRIVATE
    HAVE_SYS_UIO_H=
    off64_t=off_t
  )
  target_include_directories(stagefright PRIVATE
    gecko/src/media/libstagefright/ports/darwin/include
  )
endif()

target_compile_options(stagefright PRIVATE -Wno-multichar)



##########
# libsoundtouch
##########
add_library(soundtouch OBJECT
  gecko/src/media/libsoundtouch/src/cpu_detect_x86.cpp
  gecko/src/media/libsoundtouch/src/RateTransposer.cpp
  gecko/src/media/libsoundtouch/src/InterpolateLinear.cpp
  gecko/src/media/libsoundtouch/src/AAFilter.cpp
  gecko/src/media/libsoundtouch/src/FIFOSampleBuffer.cpp
  gecko/src/media/libsoundtouch/src/FIRFilter.cpp
  gecko/src/media/libsoundtouch/src/SoundTouchFactory.cpp
  gecko/src/media/libsoundtouch/src/InterpolateCubic.cpp
  gecko/src/media/libsoundtouch/src/InterpolateShannon.cpp
  gecko/src/media/libsoundtouch/src/SoundTouch.cpp
  gecko/src/media/libsoundtouch/src/TDStretch.cpp
)

if(X86)
  if(FEATURE_AUDIO_SAMPLE_TYPE_F32)
    target_sources(soundtouch PRIVATE
      gecko/src/media/libsoundtouch/src/sse_optimized.cpp
    )
    target_compile_definitions(soundtouch PRIVATE
      SOUNDTOUCH_ALLOW_SSE=1
      #TODO: set SSE2 cflags
    )
  else()
    # TODO: Set MMX CFlags?
    target_sources(soundtouch PRIVATE
      gecko/src/media/libsoundtouch/src/mmx_optimized.cpp
    )
  endif()
endif() # if(X86)

if(WINDOWS)
  target_compile_definitions(soundtouch PRIVATE
    alloca=_alloca
  )
endif()

target_compile_definitions(soundtouch PRIVATE
  ST_NO_EXCEPTION_HANDLING=1
)
target_include_directories(soundtouch PRIVATE
  gecko/include/mozilla/media/libsoundtouch/src
  gecko/glue/include/soundtouch
  gecko/glue/include/stl_wrappers
  gecko/include
)




##########
# libspeex_resampler
##########
add_library(speex_resampler OBJECT
  gecko/src/media/libspeex_resampler/src/resample.c
  gecko/src/media/libspeex_resampler/src/simd_detect.cpp
)

if(FEATURE_AUDIO_SAMPLE_TYPE_F32 AND X86)
  target_sources(speex_resampler PRIVATE gecko/src/media/libspeex_resampler/src/resample_sse.c)
endif()

if(ARM)
  target_sources(speex_resampler PRIVATE gecko/src/media/libspeex_resampler/src/resample_neon.c)
endif()

target_compile_definitions(speex_resampler PRIVATE
  OUTSIDE_SPEEX=1
  EXPORT=
)
if(FEATURE_AUDIO_SAMPLE_TYPE_S16)
  target_compile_definitions(speex_resampler PRIVATE
    FIXED_POINT=1
  )
else()
  target_compile_definitions(speex_resampler PRIVATE
    FLOATING_POINT=1
  )
  if(X86)
    target_compile_definitions(speex_resampler PRIVATE
      _USE_SSE=1
      _USE_SSE2=1
    )
    # FIXME: set SSE2 cflags?
  endif()
  if(ARM)
    target_compile_definitions(speex_resampler PRIVATE
      _USE_NEON=1
    )
  endif()
endif()

target_include_directories(speex_resampler PRIVATE
  gecko/include/mozilla/media/libspeex_resampler/src
  gecko/glue/include/stl_wrappers
  gecko/include
)




##########
# libcubeb
##########
add_library(cubeb OBJECT
  gecko/src/media/libcubeb/src/cubeb.c
  gecko/src/media/libcubeb/src/cubeb_strings.c
  gecko/src/media/libcubeb/src/cubeb_log.cpp
  gecko/src/media/libcubeb/src/cubeb_mixer.cpp
  gecko/src/media/libcubeb/src/cubeb_panner.cpp
  gecko/src/media/libcubeb/src/cubeb_resampler.cpp
)

if(LINUX)
  # The only Mozilla supported audio backend on Linux is Pulse Audio.
  target_compile_definitions(cubeb PRIVATE
    USE_PULSE=1
    USE_PULSE_RUST=1
  )
  target_sources(cubeb PRIVATE
    gecko/src/media/libcubeb/src/cubeb_pulse.c
  )
endif()
if(MACOS)
  target_compile_definitions(cubeb PRIVATE USE_AUDIOUNIT=1)
  target_sources(cubeb PRIVATE
    gecko/src/media/libcubeb/src/cubeb_audiounit.cpp
    gecko/src/media/libcubeb/src/cubeb_osx_run_loop.c
  )
endif()
if(WINDOWS)
  target_compile_definitions(cubeb PRIVATE USE_WASAPI=1)
  target_sources(cubeb PRIVATE
    gecko/src/media/libcubeb/src/cubeb_wasapi.cpp
  )
endif()
if(ANDROID)
  target_compile_definitions(cubeb PRIVATE
    USE_OPENSL=1
    USE_AUDIOTRACK=1
  )
  target_sources(cubeb PRIVATE
    gecko/src/media/libcubeb/src/cubeb_audiotrack.c
    gecko/src/media/libcubeb/src/cubeb_opensl.c
  )
endif()

target_include_directories(cubeb PRIVATE
  gecko/include/mozilla/media/libcubeb/src
  gecko/glue/include/stl_wrappers
  gecko/include
  gecko/glue/include/
)



##########
# Tests
##########
add_library(tests OBJECT
  gecko/test/test.cpp
  gecko/test/MockMediaResource.cpp
  gecko/test/TestMediaDataDecoder.cpp
  gecko/test/TestMediaMIMETypes.cpp
  gecko/test/TestMP4Demuxer.cpp
)
target_include_directories(tests PRIVATE
    gecko/glue/include/stl_wrappers
    gecko/include/system_wrappers
    gecko/include/nspr
    gecko/include/nspr/private
    gecko/include
    gecko/glue/include
    gecko/include/mozilla/
    gecko/include/mozilla/double-conversion
    gecko/include/mozilla/media/libvorbis/include
    gecko/include/mozilla/media/libogg/include
    gecko/include/mozilla/media/libstagefright/
    gecko/include/mozilla/media/libstagefright/binding/include
    gecko/include/mozilla/media/libstagefright/frameworks/av/include
    gecko/include/mozilla/media/libstagefright/frameworks/av/include/media/stagefright/foundation
    gecko/include/mozilla/media/libstagefright/frameworks/av/media/libstagefright/
    gecko/include/mozilla/media/libstagefright/stubs/empty
    gecko/include/mozilla/media/libstagefright/stubs/include
    gecko/include/mozilla/media/libstagefright/stubs/include/media/stagefright/foundation
    gecko/include/mozilla/media/libstagefright/system/core/include
)




##########
# gecko_media
##########
set(FFVPX_OBJS )
set(FFMPEG57_OBJS )
if (LINUX OR MACOS)
  set(FFVPX_OBJS $<TARGET_OBJECTS:ffvpx>)
  set(FFMPEG57_OBJS $<TARGET_OBJECTS:ffmpeg57>)
endif()

add_library(gecko_media_cmake STATIC
  $<TARGET_OBJECTS:ogg>
  $<TARGET_OBJECTS:gecko_core>
  $<TARGET_OBJECTS:opus>
  $<TARGET_OBJECTS:vorbis>
  $<TARGET_OBJECTS:stagefright>
  $<TARGET_OBJECTS:soundtouch>
  $<TARGET_OBJECTS:speex_resampler>
  $<TARGET_OBJECTS:cubeb>
  $<TARGET_OBJECTS:tests>
  ${FFVPX_OBJS}
  ${FFMPEG57_OBJS}
)


install(TARGETS gecko_media_cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)
