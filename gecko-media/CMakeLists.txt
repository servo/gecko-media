cmake_minimum_required (VERSION 2.8)
project (gecko_media)

enable_language(ASM)

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

if($ENV{TARGET} MATCHES "android")
  set(ANDROID 1) # Note: *Doesn't* imply LINUX nor MACOS nor WINDOWS
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

if (X86 OR X86_64)
  find_program(YASM_EXE NAMES yasm)
endif()

if (X86_64)
  add_definitions(-DHAVE_64BIT_BUILD=1)
endif()

if (MACOS AND NOT ANDROID)
  add_definitions(-DXP_DARWIN=1)
  add_definitions(-DDARWIN=1)
endif()

if (LINUX OR ANDROID)
  add_definitions(-DXP_UNIX=1)
endif()

if (ANDROID)
  add_definitions(-DANDROID=1)
  add_definitions(-DHAVE_FCNTL_FILE_LOCKING=1)
endif()

add_definitions(-DGECKO_MEDIA_CRATE=1)
add_definitions(-DMOZILLA_INTERNAL_API=1)
add_definitions(-D_PR_PTHREADS=1)
add_definitions(-DRUST_BINDGEN=1)

if (LINUX)
  if (X86_64)
    set(yasm_binary_format elf64)
  else()
    set(yasm_binary_format elf32)
  endif()
elseif (MACOS)
  if (X86_64)
    set(yasm_binary_format macho64)
  else()
    set(yasm_binary_format macho32)
  endif()
endif()

############
# ffvpx
############

set(yasm_flags -P${CMAKE_CURRENT_SOURCE_DIR}/gecko/src/media/ffvpx/defaults_disabled.asm)
if (MACOS AND NOT ANDROID)
  set(yasm_config -P${CMAKE_CURRENT_SOURCE_DIR}/gecko/src/media/ffvpx/config_darwin64.asm)
  set(yasm_flags ${yasm_flags} -DPREFIX)
elseif (LINUX)
  set(yasm_config -P${CMAKE_CURRENT_SOURCE_DIR}/gecko/src/media/ffvpx/config_unix64.asm)
elseif (WINDOWS)
  if (X86_64)
    set(yasm_config -P${CMAKE_CURRENT_SOURCE_DIR}/gecko/src/media/ffvpx/config_win64.asm)
  else()
    set(yasm_config -P${CMAKE_CURRENT_SOURCE_DIR}/gecko/src/media/ffvpx/config_win32.asm)
  endif()
endif()


set(LIBAVUTIL_ASM_OBJS )

add_library(libavutil OBJECT
  gecko/src/media/ffvpx/libavutil/adler32.c
  gecko/src/media/ffvpx/libavutil/atomic.c
  gecko/src/media/ffvpx/libavutil/avstring.c
  gecko/src/media/ffvpx/libavutil/base64.c
  gecko/src/media/ffvpx/libavutil/bprint.c
  gecko/src/media/ffvpx/libavutil/buffer.c
  gecko/src/media/ffvpx/libavutil/channel_layout.c
  gecko/src/media/ffvpx/libavutil/color_utils.c
  gecko/src/media/ffvpx/libavutil/cpu.c
  gecko/src/media/ffvpx/libavutil/crc.c
  gecko/src/media/ffvpx/libavutil/dict.c
  gecko/src/media/ffvpx/libavutil/dummy_funcs.c
  gecko/src/media/ffvpx/libavutil/error.c
  gecko/src/media/ffvpx/libavutil/eval.c
  gecko/src/media/ffvpx/libavutil/fifo.c
  gecko/src/media/ffvpx/libavutil/fixed_dsp.c
  gecko/src/media/ffvpx/libavutil/float_dsp.c
  gecko/src/media/ffvpx/libavutil/frame.c
  gecko/src/media/ffvpx/libavutil/imgutils.c
  gecko/src/media/ffvpx/libavutil/integer.c
  gecko/src/media/ffvpx/libavutil/intmath.c
  gecko/src/media/ffvpx/libavutil/lls.c
  gecko/src/media/ffvpx/libavutil/log.c
  gecko/src/media/ffvpx/libavutil/log2_tab.c
  gecko/src/media/ffvpx/libavutil/mathematics.c
  gecko/src/media/ffvpx/libavutil/mem.c
  gecko/src/media/ffvpx/libavutil/opt.c
  gecko/src/media/ffvpx/libavutil/parseutils.c
  gecko/src/media/ffvpx/libavutil/pixdesc.c
  gecko/src/media/ffvpx/libavutil/pixelutils.c
  gecko/src/media/ffvpx/libavutil/rational.c
  gecko/src/media/ffvpx/libavutil/reverse.c
  gecko/src/media/ffvpx/libavutil/samplefmt.c
  gecko/src/media/ffvpx/libavutil/slicethread.c
  gecko/src/media/ffvpx/libavutil/threadmessage.c
  gecko/src/media/ffvpx/libavutil/time.c
  gecko/src/media/ffvpx/libavutil/timecode.c
  gecko/src/media/ffvpx/libavutil/utils.c
)

target_compile_definitions(libavutil PRIVATE
  HAVE_AV_CONFIG_H=1
  HAVE_LOCALTIME_R=1
)

if (X64_64)
  target_compile_definitions(libavutil PRIVATE
    HAVE_64BIT_BUILD=
  )
endif()

if (ANDROID)
  # This define is needed to enable the config_android32.h #include in config.h.
  set(ffvpx_compile_flags "-DMOZ_WIDGET_ANDROID")
elseif (LINUX)
  set(ffvpx_compile_flags "-Wno-parentheses -Wno-pointer-sign -Wno-sign-compare -Wno-switch -Wno-type-limits -Wno-unused-function -Wno-deprecated-declarations -Wno-maybe-uninitialized -U_GNU_SOURCE -D_XOPEN_SOURCE=600  --include ${CMAKE_SOURCE_DIR}/gecko/include/mozilla/media/ffvpx/libavutil_visibility.h")
elseif (MACOS)
  set(ffvpx_compile_flags "-Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/include/mozilla/media/ffvpx/libavutil_visibility.h")
endif()

set_target_properties(libavutil PROPERTIES
  COMPILE_FLAGS ${ffvpx_compile_flags}
)

target_include_directories(libavutil PRIVATE
  gecko/include/mozilla/media/ffvpx/
  gecko/include/mozilla/media/ffvpx/libavutil/
)

if (ARM)
  add_library(libavutil-arm OBJECT
    gecko/src/media/ffvpx/libavutil/arm/cpu.c
    gecko/src/media/ffvpx/libavutil/arm/float_dsp_init_arm.c
    gecko/src/media/ffvpx/libavutil/arm/float_dsp_init_neon.c
    gecko/src/media/ffvpx/libavutil/arm/float_dsp_init_vfp.c
    gecko/src/media/ffvpx/libavutil/arm/float_dsp_neon.S
    gecko/src/media/ffvpx/libavutil/arm/float_dsp_vfp.S
  )
  target_include_directories(libavutil-arm PRIVATE
    gecko/src/media/ffvpx/
    gecko/include/mozilla/media/ffvpx/
    gecko/include/mozilla/media/ffvpx/libavutil/arm/
    gecko/include/mozilla/media/ffvpx/libavutil/
  )
  set(LIBAVUTIL_ARCH_OBJS $<TARGET_OBJECTS:libavutil-arm>)
endif()

if (X86)
  add_library(libavutil-x86 OBJECT
    gecko/src/media/ffvpx/libavutil/x86/cpu.c
    gecko/src/media/ffvpx/libavutil/x86/fixed_dsp_init.c
    gecko/src/media/ffvpx/libavutil/x86/float_dsp_init.c
    gecko/src/media/ffvpx/libavutil/x86/imgutils_init.c
    gecko/src/media/ffvpx/libavutil/x86/lls_init.c
    gecko/src/media/ffvpx/libavutil/x86/pixelutils_init.c
  )
  set(asm_sources
    gecko/src/media/ffvpx/libavutil/x86/cpuid.asm
    gecko/src/media/ffvpx/libavutil/x86/emms.asm
    gecko/src/media/ffvpx/libavutil/x86/fixed_dsp.asm
    gecko/src/media/ffvpx/libavutil/x86/float_dsp.asm
    gecko/src/media/ffvpx/libavutil/x86/imgutils.asm
    gecko/src/media/ffvpx/libavutil/x86/lls.asm
    gecko/src/media/ffvpx/libavutil/x86/pixelutils.asm
    gecko/src/media/ffvpx/libavutil/x86/x86inc.asm
    gecko/src/media/ffvpx/libavutil/x86/x86util.asm
  )
  target_include_directories(libavutil-x86 PRIVATE
    gecko/include/mozilla/media/ffvpx/
    gecko/include/mozilla/media/ffvpx/libavutil/x86/
    gecko/include/mozilla/media/ffvpx/libavutil/
  )
  set(LIBAVUTIL_ARCH_OBJS $<TARGET_OBJECTS:libavutil-x86>)

  set(LIBAVUTIL_ASM_OBJS )
  foreach(asm_file ${asm_sources})
    get_filename_component(file_name ${asm_file} NAME)
    set(compiled_object ${CMAKE_CURRENT_BINARY_DIR}/ffvpx-${file_name}.o)
    add_custom_command(OUTPUT ${compiled_object}
      COMMAND ${YASM_EXE}
      ARGS -f ${yasm_binary_format} ${yasm_config} ${yasm_flags} -DPIC -Igecko/src/media/ffvpx/ -o ${compiled_object} ./${asm_file}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM)
    list(APPEND LIBAVUTIL_ASM_OBJS ${compiled_object})
  endforeach()

  set_source_files_properties(
    ${LIBAVUTIL_ASM_OBJS}
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
  )
  set_target_properties(libavutil-x86 PROPERTIES COMPILE_FLAGS ${ffvpx_compile_flags})

endif()

add_library(mozavutil SHARED
  $<TARGET_OBJECTS:libavutil>
  ${LIBAVUTIL_ARCH_OBJS}
  ${LIBAVUTIL_ASM_OBJS}
)

install(TARGETS mozavutil
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)

set_target_properties(mozavutil PROPERTIES
  LINKER_LANGUAGE C
  POSITION_INDEPENDENT_CODE ON
  SOVERSION 57
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
)


set(LIBAVCODEC_ASM_OBJS )

add_library(gecko_media_mozavcodec OBJECT
  gecko/src/media/ffvpx/libavcodec/allcodecs.c
  gecko/src/media/ffvpx/libavcodec/audioconvert.c
  gecko/src/media/ffvpx/libavcodec/avpacket.c
  gecko/src/media/ffvpx/libavcodec/avpicture.c
  gecko/src/media/ffvpx/libavcodec/bitstream.c
  gecko/src/media/ffvpx/libavcodec/bitstream_filter.c
  gecko/src/media/ffvpx/libavcodec/bitstream_filters.c
  gecko/src/media/ffvpx/libavcodec/bsf.c
  gecko/src/media/ffvpx/libavcodec/codec_desc.c
  gecko/src/media/ffvpx/libavcodec/decode.c
  gecko/src/media/ffvpx/libavcodec/dummy_funcs.c
  gecko/src/media/ffvpx/libavcodec/flac.c
  gecko/src/media/ffvpx/libavcodec/flacdata.c
  gecko/src/media/ffvpx/libavcodec/flacdec.c
  gecko/src/media/ffvpx/libavcodec/flacdsp.c
  gecko/src/media/ffvpx/libavcodec/golomb.c
  gecko/src/media/ffvpx/libavcodec/h264pred.c
  gecko/src/media/ffvpx/libavcodec/imgconvert.c
  gecko/src/media/ffvpx/libavcodec/log2_tab.c
  gecko/src/media/ffvpx/libavcodec/mathtables.c
  gecko/src/media/ffvpx/libavcodec/null_bsf.c
  gecko/src/media/ffvpx/libavcodec/options.c
  gecko/src/media/ffvpx/libavcodec/parser.c
  gecko/src/media/ffvpx/libavcodec/profiles.c
  gecko/src/media/ffvpx/libavcodec/pthread.c
  gecko/src/media/ffvpx/libavcodec/pthread_frame.c
  gecko/src/media/ffvpx/libavcodec/pthread_slice.c
  gecko/src/media/ffvpx/libavcodec/qsv_api.c
  gecko/src/media/ffvpx/libavcodec/raw.c
  gecko/src/media/ffvpx/libavcodec/resample.c
  gecko/src/media/ffvpx/libavcodec/resample2.c
  gecko/src/media/ffvpx/libavcodec/reverse.c
  gecko/src/media/ffvpx/libavcodec/utils.c
  gecko/src/media/ffvpx/libavcodec/videodsp.c
  gecko/src/media/ffvpx/libavcodec/vorbis_parser.c
  gecko/src/media/ffvpx/libavcodec/vp56rac.c
  gecko/src/media/ffvpx/libavcodec/vp8.c
  gecko/src/media/ffvpx/libavcodec/vp8_parser.c
  gecko/src/media/ffvpx/libavcodec/vp8dsp.c
  gecko/src/media/ffvpx/libavcodec/vp9.c
  gecko/src/media/ffvpx/libavcodec/vp9_parser.c
  gecko/src/media/ffvpx/libavcodec/vp9block.c
  gecko/src/media/ffvpx/libavcodec/vp9data.c
  gecko/src/media/ffvpx/libavcodec/vp9dsp.c
  gecko/src/media/ffvpx/libavcodec/vp9dsp_10bpp.c
  gecko/src/media/ffvpx/libavcodec/vp9dsp_12bpp.c
  gecko/src/media/ffvpx/libavcodec/vp9dsp_8bpp.c
  gecko/src/media/ffvpx/libavcodec/vp9lpf.c
  gecko/src/media/ffvpx/libavcodec/vp9mvs.c
  gecko/src/media/ffvpx/libavcodec/vp9prob.c
  gecko/src/media/ffvpx/libavcodec/vp9recon.c
  gecko/src/media/ffvpx/libavcodec/xiph.c
)

target_compile_definitions(gecko_media_mozavcodec PRIVATE
  HAVE_AV_CONFIG_H=1
)

target_include_directories(gecko_media_mozavcodec PRIVATE
  gecko/src/media/ffvpx/
  gecko/include/mozilla/media/ffvpx/
  gecko/include/mozilla/media/ffvpx/libavcodec/
)

if (X64_64)
  target_compile_definitions(gecko_media_mozavcodec PRIVATE
    HAVE_64BIT_BUILD=
  )
endif()

if (LINUX OR ANDROID)
  target_compile_definitions(gecko_media_mozavcodec PRIVATE
    HAVE_LOCALTIME_R=1
)
endif()

if (ANDROID)
  # This define is needed to enable the config_android32.h #include in config.h.
  set_target_properties(gecko_media_mozavcodec PROPERTIES COMPILE_FLAGS "-DMOZ_WIDGET_ANDROID")
endif()

if (ARM)
  add_library(libavcodec-arm OBJECT
    gecko/src/media/ffvpx/libavcodec/arm/flacdsp_arm.S
    gecko/src/media/ffvpx/libavcodec/arm/flacdsp_init_arm.c
  )
  target_include_directories(libavcodec-arm PRIVATE
    gecko/src/media/ffvpx/
    gecko/include/mozilla/media/ffvpx/libavcodec/arm/
    gecko/include/mozilla/media/ffvpx
  )

  target_compile_definitions(libavcodec-arm PRIVATE
    HAVE_AV_CONFIG_H=1
  )
  set(LIBAVCODEC_ARCH_OBJS $<TARGET_OBJECTS:libavcodec-arm>)
endif()

if (X86)
  add_library(libavcodec-x86 OBJECT
    gecko/src/media/ffvpx/libavcodec/x86/constants.c
    gecko/src/media/ffvpx/libavcodec/x86/flacdsp_init.c
    gecko/src/media/ffvpx/libavcodec/x86/h264_intrapred_init.c
    gecko/src/media/ffvpx/libavcodec/x86/videodsp_init.c
    gecko/src/media/ffvpx/libavcodec/x86/vp8dsp_init.c
    gecko/src/media/ffvpx/libavcodec/x86/vp9dsp_init.c
    gecko/src/media/ffvpx/libavcodec/x86/vp9dsp_init_10bpp.c
    gecko/src/media/ffvpx/libavcodec/x86/vp9dsp_init_12bpp.c
    gecko/src/media/ffvpx/libavcodec/x86/vp9dsp_init_16bpp.c
  )

  set_source_files_properties(gecko/src/media/ffvpx/libavcodec/x86/constants.c PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden"
  )

  set(asm_sources
    gecko/src/media/ffvpx/libavcodec/x86/flacdsp.asm
    gecko/src/media/ffvpx/libavcodec/x86/h264_intrapred.asm
    gecko/src/media/ffvpx/libavcodec/x86/h264_intrapred_10bit.asm
    gecko/src/media/ffvpx/libavcodec/x86/videodsp.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp8dsp.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp8dsp_loopfilter.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9intrapred.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9intrapred_16bpp.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9itxfm.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9itxfm_16bpp.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9lpf.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9lpf_16bpp.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9mc.asm
    gecko/src/media/ffvpx/libavcodec/x86/vp9mc_16bpp.asm
  )
  target_include_directories(libavcodec-x86 PRIVATE
    gecko/include/mozilla/media/ffvpx/libavutil/x86/
    gecko/include/mozilla/media/ffvpx/libavcodec/x86/
    gecko/include/mozilla/media/ffvpx
  )
  target_compile_definitions(libavcodec-x86 PRIVATE
    HAVE_AV_CONFIG_H=1
  )
  set(LIBAVCODEC_ARCH_OBJS $<TARGET_OBJECTS:libavcodec-x86>)

  foreach(asm_file ${asm_sources})
    get_filename_component(file_name ${asm_file} NAME)
    set(compiled_object ${CMAKE_CURRENT_BINARY_DIR}/ffvpx-${file_name}.o)
    add_custom_command(OUTPUT ${compiled_object}
      COMMAND ${YASM_EXE}
      ARGS -f ${yasm_binary_format} ${yasm_config} ${yasm_flags} -DPIC -Igecko/src/media/ffvpx/ -o ${compiled_object} ./${asm_file}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM)
    list(APPEND LIBAVCODEC_ASM_OBJS ${compiled_object})
  endforeach()

  set_source_files_properties(
    ${LIBAVCODEC_ASM_OBJS}
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
  )
  set_target_properties(libavcodec-x86 PROPERTIES
    COMPILE_FLAGS ${ffvpx_compile_flags}
  )

endif()

add_library(mozavcodec SHARED
  $<TARGET_OBJECTS:gecko_media_mozavcodec>
  ${LIBAVCODEC_ARCH_OBJS}
  ${LIBAVCODEC_ASM_OBJS}
)

target_link_libraries(mozavcodec mozavutil)

install(TARGETS mozavcodec
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)

set_target_properties(mozavcodec PROPERTIES
  LINKER_LANGUAGE C
  POSITION_INDEPENDENT_CODE ON
  SOVERSION 57
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fno-exceptions")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
if(X86_64)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64")
endif()

if (ANDROID)
  set(GECKO_MEDIA_PREPROCESSOR_INCLUDES "--include ${CMAKE_SOURCE_DIR}/gecko/glue/include/mozilla-config-android.h --include ${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
elseif (LINUX)
  set(GECKO_MEDIA_PREPROCESSOR_INCLUDES "--include ${CMAKE_SOURCE_DIR}/gecko/glue/include/mozilla-config-x86_64-linux.h --include ${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
else(MACOS)
  set(GECKO_MEDIA_PREPROCESSOR_INCLUDES "-Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/glue/include/mozilla-config-x86_64-apple-darwin.h -Xpreprocessor -include${CMAKE_SOURCE_DIR}/gecko/glue/include/undefs.h")
endif()

if(MACOS AND NOT ANDROID)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inline-new-delete")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-inline-new-delete")
endif()

if (ANDROID)
  if (ARM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=softfp -mfpu=neon")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=softfp -mfpu=neon")
  endif()
endif()


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
    gecko/include/mozilla/media/libvpx/libvpx
  )
  target_compile_definitions(ffvpx PRIVATE
    FFVPX_VERSION=46465650
    USING_MOZFFVPX=1
  )

  set_target_properties(ffvpx PROPERTIES COMPILE_FLAGS ${GECKO_MEDIA_PREPROCESSOR_INCLUDES})
endif()


############
# libogg
############

add_library(ogg OBJECT
  gecko/src/media/libogg/src/ogg_alloc.c
  gecko/src/media/libogg/src/ogg_bitwise.c
  gecko/src/media/libogg/src/ogg_framing.c
  )
target_include_directories(ogg PRIVATE
  gecko/include
  gecko/include/mozilla/media/libogg/include
)




############
# Gecko core
############

add_library(gecko_core OBJECT
  gecko/glue/AsyncShutdown.cpp
  gecko/glue/Benchmark.cpp
  gecko/glue/CooperativeThreadPool.cpp
  gecko/glue/ContainerParser.cpp
  gecko/glue/CubebUtils.cpp
  gecko/glue/FFmpegRuntimeLinker.cpp
  gecko/glue/GeckoMedia.cpp
  gecko/glue/GeckoMediaDecoder.cpp
  gecko/glue/GeckoMediaDecoderOwner.cpp
  gecko/glue/GeckoMediaSource.cpp
  gecko/glue/GeckoMediaSourceBuffer.cpp
  gecko/glue/GeckoMediaSourceBufferList.cpp
  gecko/glue/GlueStubs.cpp
  gecko/glue/ImageContainer.cpp
  gecko/glue/InputEventStatistics.cpp
  gecko/glue/nsDebugImpl.cpp
  gecko/glue/LabeledEventQueue.cpp
  gecko/glue/Logging.cpp
  gecko/glue/MainThreadIdlePeriod.cpp
  gecko/glue/MediaData.cpp
  gecko/glue/MediaSource.cpp
  gecko/glue/MediaSourceDecoder.cpp
  gecko/glue/MediaStreamGraph.cpp
  gecko/glue/PDMFactory.cpp
  gecko/glue/Player.cpp
  gecko/glue/mozalloc_abort.cpp
  gecko/glue/nsAppRunner.cpp
  gecko/glue/nsContentTypeParser.cpp
  gecko/glue/nsCategoryManager.cpp
  gecko/glue/nsComponentManager.cpp
  gecko/glue/nsLocalFileCommon.cpp
  gecko/glue/nsLocalFileUnix.cpp
  gecko/glue/nsObserverList.cpp
  gecko/glue/nsObserverService.cpp
  gecko/glue/nsThread.cpp
  gecko/glue/nsTraceRefcnt.cpp
  gecko/glue/Preferences.cpp
  gecko/glue/RustServices.cpp
  gecko/glue/RustMediaResource.cpp
  gecko/glue/Scheduler.cpp
  gecko/glue/SchedulerGroup.cpp
  gecko/glue/Services.cpp
  gecko/glue/SourceBuffer.cpp
  gecko/glue/SourceBufferList.cpp
  gecko/glue/Telemetry.cpp
  gecko/glue/ThreadEventQueue.cpp
  gecko/glue/ThreadAnnotation.cpp
  gecko/glue/TrackBuffersManager.cpp
  gecko/glue/VideoSegment.cpp
  gecko/glue/WebAudioUtils.cpp
  gecko/glue/XPCOMInit.cpp
  gecko/src/dom/media/ADTSDecoder.cpp
  gecko/src/dom/media/ADTSDemuxer.cpp
  gecko/src/dom/media/AudioConverter.cpp
  gecko/src/dom/media/AudioSegment.cpp
  gecko/src/dom/media/AudioStream.cpp
  gecko/src/dom/media/BitReader.cpp
  gecko/src/dom/media/DecoderTraits.cpp
  gecko/src/dom/media/MediaDecoder.cpp
  gecko/src/dom/media/MediaDecoderStateMachine.cpp
  gecko/src/dom/media/MediaFormatReader.cpp
  gecko/src/dom/media/MediaContainerType.cpp
  gecko/src/dom/media/MediaInfo.cpp
  gecko/src/dom/media/MediaMIMETypes.cpp
  gecko/src/dom/media/MediaShutdownManager.cpp
  gecko/src/dom/media/MediaPrefs.cpp
  gecko/src/dom/media/MediaResource.cpp
  gecko/src/dom/media/MediaStreamListener.cpp
  gecko/src/dom/media/MediaStreamVideoSink.cpp
  gecko/src/dom/media/MediaTimer.cpp
  gecko/src/dom/media/QueueObject.cpp
  gecko/src/dom/media/ReaderProxy.cpp
  gecko/src/dom/media/SeekJob.cpp
  gecko/src/dom/media/StreamTracks.cpp
  gecko/src/dom/media/doctor/DDLifetime.cpp
  gecko/src/dom/media/doctor/DDLifetimes.cpp
  gecko/src/dom/media/doctor/DDLogCategory.cpp
  gecko/src/dom/media/doctor/DDLogMessage.cpp
  gecko/src/dom/media/doctor/DDLogObject.cpp
  gecko/src/dom/media/doctor/DDLogUtils.cpp
  gecko/src/dom/media/doctor/DDLogValue.cpp
  gecko/src/dom/media/doctor/DDMediaLog.cpp
  gecko/src/dom/media/doctor/DDMediaLogs.cpp
  gecko/src/dom/media/doctor/DDTimeStamp.cpp
  gecko/src/dom/media/doctor/DecoderDoctorLogger.cpp
  gecko/src/dom/media/mp4/MP4Decoder.cpp
  gecko/src/dom/media/mp4/MP4Demuxer.cpp
  gecko/src/dom/media/mediasink/AudioSink.cpp
  gecko/src/dom/media/mediasink/VideoSink.cpp
  gecko/src/dom/media/mediasink/AudioSinkWrapper.cpp
  gecko/src/dom/media/mediasink/DecodedStream.cpp
  gecko/src/dom/media/mediasink/OutputStreamManager.cpp
  gecko/src/dom/media/mediasource/MediaSourceDemuxer.cpp
  gecko/src/dom/media/mediasource/MediaSourceUtils.cpp
  gecko/src/dom/media/mediasource/ResourceQueue.cpp
  gecko/src/dom/media/mediasource/SourceBufferResource.cpp
  gecko/src/dom/media/mp3/MP3Decoder.cpp
  gecko/src/dom/media/mp3/MP3Demuxer.cpp
  gecko/src/dom/media/mp3/MP3FrameParser.cpp
  gecko/src/dom/media/mp4/Box.cpp
  gecko/src/dom/media/mp4/BufferStream.cpp
  gecko/src/dom/media/mp4/DecoderData.cpp
  gecko/src/dom/media/mp4/Index.cpp
  gecko/src/dom/media/mp4/MP4Decoder.cpp
  gecko/src/dom/media/mp4/MP4Demuxer.cpp
  gecko/src/dom/media/mp4/MP4Metadata.cpp
  gecko/src/dom/media/mp4/MoofParser.cpp
  gecko/src/dom/media/mp4/ResourceStream.cpp
  gecko/src/dom/media/mp4/SinfParser.cpp
  gecko/src/dom/media/ogg/OpusParser.cpp
  gecko/src/dom/media/ogg/OpusParser.cpp
  gecko/src/dom/media/ogg/OggDecoder.cpp
  gecko/src/dom/media/ogg/OggDemuxer.cpp
  gecko/src/dom/media/ogg/OggCodecState.cpp
  gecko/src/dom/media/ogg/OggCodecStore.cpp
  gecko/src/dom/media/flac/FlacDecoder.cpp
  gecko/src/dom/media/flac/FlacDemuxer.cpp
  gecko/src/dom/media/flac/FlacFrameParser.cpp
  gecko/src/dom/media/XiphExtradata.cpp
  gecko/src/dom/media/platforms/agnostic/bytestreams/Adts.cpp
  gecko/src/dom/media/platforms/agnostic/bytestreams/AnnexB.cpp
  gecko/src/dom/media/platforms/agnostic/bytestreams/H264.cpp
  gecko/src/dom/media/platforms/agnostic/AgnosticDecoderModule.cpp
  gecko/src/dom/media/platforms/agnostic/BlankDecoderModule.cpp
  gecko/src/dom/media/platforms/agnostic/DummyMediaDataDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/NullDecoderModule.cpp
  gecko/src/dom/media/platforms/agnostic/OpusDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/TheoraDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/VorbisDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/VPXDecoder.cpp
  gecko/src/dom/media/platforms/agnostic/WAVDecoder.cpp
  gecko/src/dom/media/platforms/ffmpeg/FFmpegLibWrapper.cpp
  gecko/src/dom/media/platforms/wrappers/H264Converter.cpp
  gecko/src/dom/media/VideoUtils.cpp
  gecko/src/dom/media/VideoFrameContainer.cpp
  gecko/src/dom/media/wave/WaveDecoder.cpp
  gecko/src/dom/media/wave/WaveDemuxer.cpp
  gecko/src/dom/media/webm/WebMBufferedParser.cpp
  gecko/src/dom/media/webm/WebMDecoder.cpp
  gecko/src/dom/media/webm/WebMDemuxer.cpp
  gecko/src/memory/mozalloc/mozalloc.cpp
  gecko/src/memory/mozalloc/mozalloc_oom.cpp
  gecko/src/mfbt/Assertions.cpp
  gecko/src/mfbt/ChaosMode.cpp
  gecko/src/mfbt/HashFunctions.cpp
  gecko/src/mfbt/JSONWriter.cpp
  gecko/src/mfbt/Poison.cpp
  gecko/src/mfbt/Unused.cpp
  gecko/src/mfbt/double-conversion/double-conversion/bignum.cc
  gecko/src/mfbt/double-conversion/double-conversion/bignum-dtoa.cc
  gecko/src/mfbt/double-conversion/double-conversion/cached-powers.cc
  gecko/src/mfbt/double-conversion/double-conversion/diy-fp.cc
  gecko/src/mfbt/double-conversion/double-conversion/double-conversion.cc
  gecko/src/mfbt/double-conversion/double-conversion/fast-dtoa.cc
  gecko/src/mfbt/double-conversion/double-conversion/fixed-dtoa.cc
  gecko/src/mfbt/double-conversion/double-conversion/strtod.cc
  gecko/src/mozglue/misc/ConditionVariable_posix.cpp
  gecko/src/mozglue/misc/Printf.cpp
  gecko/src/mozglue/misc/TimeStamp.cpp
  gecko/src/toolkit/library/StaticXULComponentsStart.cpp
  gecko/src/toolkit/library/StaticXULComponentsEnd/StaticXULComponentsEnd.cpp
  gecko/src/xpcom/base/ClearOnShutdown.cpp
  gecko/src/xpcom/base/ErrorNames.cpp
  gecko/src/xpcom/base/nsCOMPtr.cpp
  gecko/src/xpcom/base/nsCRTGlue.cpp
  gecko/src/xpcom/base/nsClassInfoImpl.cpp
  gecko/src/xpcom/base/nsID.cpp
  gecko/src/xpcom/base/nsISupportsImpl.cpp
  gecko/src/xpcom/base/nsMemory.cpp
  gecko/src/xpcom/base/nsWeakReference.cpp
  gecko/src/xpcom/components/nsComponentManagerUtils.cpp
  gecko/src/xpcom/components/GenericFactory.cpp
  gecko/src/xpcom/ds/PLDHashTable.cpp
  gecko/src/xpcom/ds/nsCOMArray.cpp
  gecko/src/xpcom/ds/nsCRT.cpp
  gecko/src/xpcom/ds/nsDeque.cpp
  gecko/src/xpcom/ds/nsEnumeratorUtils.cpp
  gecko/src/xpcom/ds/nsQuickSort.cpp
  gecko/src/xpcom/ds/nsSupportsPrimitives.cpp
  gecko/src/xpcom/ds/nsTArray.cpp
  gecko/src/xpcom/ds/nsTObserverArray.cpp
  gecko/src/xpcom/io/nsDirectoryService.cpp
  gecko/src/xpcom/io/nsNativeCharsetUtils.cpp
  gecko/src/xpcom/string/unified.cpp
  gecko/src/xpcom/threads/AbstractThread.cpp
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
  gecko/src/nsprpub/lib/libc/src/base64.c
  gecko/src/nsprpub/lib/libc/src/strcase.c
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
  gecko/src/nsprpub/pr/src/misc/prlog2.c
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

if(X86 OR X86_64)
  target_sources(gecko_core PRIVATE
    gecko/src/xpcom/string/nsReadableUtilsSSE2.cpp
    gecko/src/xpcom/string/nsUTF8UtilsSSE2.cpp
  )
endif()

if(LINUX OR ANDROID)
  target_sources(gecko_core PRIVATE
    gecko/src/mozglue/misc/Mutex_posix.cpp
    gecko/src/mozglue/misc/TimeStamp_posix.cpp
    gecko/src/nsprpub/pr/src/md/unix/linux.c
  )
endif()
if(MACOS AND NOT ANDROID)
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
    gecko/glue/CocoaFileUtils.mm
  )
  set_source_files_properties(gecko/glue/CocoaFileUtils.mm PROPERTIES COMPILE_FLAGS "-fno-objc-exceptions")

endif()
if(WINDOWS)
  target_sources(gecko_core PRIVATE
    gecko/src/mozglue/misc/TimeStamp_windows.cpp
  )
endif()
if(ANDROID)
  target_compile_definitions(gecko_core PRIVATE
    __GNU_LIBRARY__=
  )
  target_sources(gecko_core PRIVATE
    gecko/src/dom/media/systemservices/OpenSLESProvider.cpp
    gecko/src/mozglue/misc/Mutex_posix.cpp
  )
endif()

target_include_directories(gecko_core PRIVATE
    gecko/glue/include/
    gecko/glue/include/stl_wrappers
    gecko/include/system_wrappers
    gecko/include/
    gecko/include/nspr
    gecko/include/nspr/private
    gecko/include/mediasink
    gecko/include/mozilla/
    gecko/include/mozilla/gfx
    gecko/include/mozilla/double-conversion
    gecko/include/mozilla/media/libvorbis/include
    gecko/include/mozilla/media/libnestegg/include
    gecko/include/mozilla/media/libogg/include
    gecko/include/mozilla/media/libtheora/include
    gecko/include/mozilla/media/libtremor/include
    gecko/include/mozilla/media/libvpx/libvpx
)

target_compile_definitions(gecko_core PRIVATE
  MOZILLA_INTERNAL_API=1
  _PR_PTHREADS=1
  GECKO_MEDIA_CRATE=1
  RUST_BINDGEN=1
)

set_target_properties(gecko_core PROPERTIES COMPILE_FLAGS ${GECKO_MEDIA_PREPROCESSOR_INCLUDES})


############
# ffmpeg
############
if (LINUX OR MACOS AND NOT ANDROID)
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
    gecko/include/mozilla/media/libvpx/libvpx
)

set_target_properties(ffmpeg57 PROPERTIES COMPILE_FLAGS ${GECKO_MEDIA_PREPROCESSOR_INCLUDES})

endif()

############
# libnestegg
############

add_library(nestegg OBJECT
  gecko/src/media/libnestegg/src/nestegg.c
)

target_include_directories(nestegg PRIVATE
  gecko/include/
  gecko/include/mozilla/media/libnestegg/include
)

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
    gecko/src/media/libopus/silk/fixed/regularize_correlations_FIX.c
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
  target_include_directories(opus PRIVATE gecko/include/mozilla/media/libopus/silk/fixed)
  target_sources(opus PRIVATE
    gecko/src/media/libopus/silk/arm/arm_silk_map.c
    gecko/src/media/libopus/silk/arm/biquad_alt_neon_intr.c
    gecko/src/media/libopus/silk/arm/LPC_inv_pred_gain_neon_intr.c
    gecko/src/media/libopus/silk/arm/NSQ_del_dec_neon_intr.c
    gecko/src/media/libopus/silk/arm/NSQ_neon.c
    gecko/src/media/libopus/silk/fixed/arm/warped_autocorrelation_FIX_neon_intr.c
    gecko/src/media/libopus/celt/arm/arm_celt_map.c
    gecko/src/media/libopus/celt/arm/armcpu.c
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
  gecko/include/
  gecko/include/mozilla/media/libopus
  gecko/include/mozilla/media/libopus/celt
  gecko/include/mozilla/media/libopus/include
  gecko/include/mozilla/media/libopus/silk
  gecko/include/mozilla/media/libopus/src
)


############
# libtremor
############

if (ANDROID)
  set(tremor_srcs
    gecko/src/media/libtremor/lib/tremor_block.c
    gecko/src/media/libtremor/lib/tremor_codebook.c
    gecko/src/media/libtremor/lib/tremor_floor0.c
    gecko/src/media/libtremor/lib/tremor_floor1.c
    gecko/src/media/libtremor/lib/tremor_info.c
    gecko/src/media/libtremor/lib/tremor_mapping0.c
    gecko/src/media/libtremor/lib/tremor_mdct.c
    gecko/src/media/libtremor/lib/tremor_registry.c
    gecko/src/media/libtremor/lib/tremor_res012.c
    gecko/src/media/libtremor/lib/tremor_sharedbook.c
    gecko/src/media/libtremor/lib/tremor_synthesis.c
    gecko/src/media/libtremor/lib/tremor_window.c
  )

  add_library(tremor OBJECT ${tremor_srcs})

  if (ARM)
    target_compile_definitions(tremor PRIVATE
      _ARM_ASSEM_=
    )
  endif()

  target_include_directories(tremor PRIVATE
    gecko/include/mozilla/media/libtremor/lib
    gecko/include/mozilla/media/libtremor/include/tremor
    gecko/include/mozilla/media/libogg/include
  )
endif()

############
# libvorbis
############
if (NOT ANDROID) 
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
    gecko/include/
    gecko/include/mozilla/media/libvorbis/lib
    gecko/include/mozilla/media/libvorbis/include
    gecko/include/mozilla/media/libogg/include/
  )
endif()

############
# libtheora
############
set(theora_srcs
  gecko/src/media/libtheora/lib/apiwrapper.c
  gecko/src/media/libtheora/lib/bitpack.c
  gecko/src/media/libtheora/lib/decapiwrapper.c
  gecko/src/media/libtheora/lib/decinfo.c
  gecko/src/media/libtheora/lib/decode.c
  gecko/src/media/libtheora/lib/dequant.c
  gecko/src/media/libtheora/lib/fragment.c
  gecko/src/media/libtheora/lib/huffdec.c
  gecko/src/media/libtheora/lib/idct.c
  gecko/src/media/libtheora/lib/info.c
  gecko/src/media/libtheora/lib/internal.c
  gecko/src/media/libtheora/lib/quant.c
  gecko/src/media/libtheora/lib/state.c
)

add_library(theora OBJECT ${theora_srcs})

target_compile_definitions(theora PRIVATE
  THEORA_DISABLE_ENCODE=
)

if(X86 OR X86_64)
  # TODO: add mmx x86_vc files for Windows target.
  if (X86)
    target_compile_definitions(theora PRIVATE
      OC_X86_ASM=
    )
  endif()
  if (X86_64)
    target_compile_definitions(theora PRIVATE
      OC_X86_64_ASM=
    )
  endif()

  target_sources(theora PRIVATE
    gecko/src/media/libtheora/lib/x86/x86state.c
    gecko/src/media/libtheora/lib/x86/mmxfrag.c
    gecko/src/media/libtheora/lib/x86/sse2idct.c
    gecko/src/media/libtheora/lib/x86/x86cpu.c
    gecko/src/media/libtheora/lib/x86/mmxstate.c
    gecko/src/media/libtheora/lib/x86/mmxidct.c
    )
endif()

if (ARM)
  target_sources(theora PRIVATE
    gecko/src/media/libtheora/lib/arm/armstate.c
    gecko/src/media/libtheora/lib/arm/armcpu.c
    gecko/glue/libtheora-arm/armbits-gnu.s
    gecko/glue/libtheora-arm/armfrag-gnu.s
    gecko/glue/libtheora-arm/armidct-gnu.s
    gecko/glue/libtheora-arm/armloop-gnu.s
  )
  target_compile_definitions(theora PRIVATE
    OC_ARM_ASM=
    OC_ARM_ASM_EDSP=
    OC_ARM_ASM_MEDIA=
    OC_ARM_ASM_NEON=
    )
  target_include_directories(theora PRIVATE
    gecko/include/mozilla/media/libtheora/lib/arm
    gecko/glue/libtheora-arm
  )
  set(CMAKE_ASM_FLAGS "-mfpu=neon -march=armv7-a")
endif()

target_include_directories(theora PRIVATE
  gecko/include/
  gecko/include/mozilla/media/libvorbis/lib
  gecko/include/mozilla/media/libvorbis/include
  gecko/include/mozilla/media/libogg/include/
  gecko/include/mozilla/media/libtheora/lib
  gecko/include/mozilla/media/libtheora/lib/x86
  gecko/include/mozilla/media/libtheora/include
)




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
      MOZ_SAMPLE_TYPE_FLOAT32=
    )
    set_source_files_properties(gecko/src/media/libsoundtouch/src/sse_optimized.cpp PROPERTIES COMPILE_FLAGS "-msse2")
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
  gecko/include
)

set_target_properties(soundtouch PROPERTIES COMPILE_FLAGS ${GECKO_MEDIA_PREPROCESSOR_INCLUDES})


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
  target_compile_definitions(speex_resampler PRIVATE
    _USE_NEON=1
  )
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

if(LINUX AND NOT ANDROID)
  # The only Mozilla supported audio backend on Linux is Pulse Audio.
  target_compile_definitions(cubeb PRIVATE
    USE_PULSE=1
    USE_PULSE_RUST=1
  )
  target_sources(cubeb PRIVATE
    gecko/src/media/libcubeb/src/cubeb_pulse.c
  )
endif()
if(MACOS AND NOT ANDROID)
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

set_target_properties(cubeb PROPERTIES COMPILE_FLAGS ${GECKO_MEDIA_PREPROCESSOR_INCLUDES})


##########
# libvpx
##########
set (VPX_OBJS )

add_library(vpx OBJECT
  gecko/src/media/libvpx/libvpx/vp8/common/alloccommon.c
  gecko/src/media/libvpx/libvpx/vp8/common/blockd.c
  gecko/src/media/libvpx/libvpx/vp8/common/copy_c.c
  gecko/src/media/libvpx/libvpx/vp8/common/dequantize.c
  gecko/src/media/libvpx/libvpx/vp8/common/entropy.c
  gecko/src/media/libvpx/libvpx/vp8/common/entropymode.c
  gecko/src/media/libvpx/libvpx/vp8/common/entropymv.c
  gecko/src/media/libvpx/libvpx/vp8/common/extend.c
  gecko/src/media/libvpx/libvpx/vp8/common/filter.c
  gecko/src/media/libvpx/libvpx/vp8/common/findnearmv.c
  gecko/src/media/libvpx/libvpx/vp8/common/generic/systemdependent.c
  gecko/src/media/libvpx/libvpx/vp8/common/idct_blk.c
  gecko/src/media/libvpx/libvpx/vp8/common/idctllm.c
  gecko/src/media/libvpx/libvpx/vp8/common/loopfilter_filters.c
  gecko/src/media/libvpx/libvpx/vp8/common/mbpitch.c
  gecko/src/media/libvpx/libvpx/vp8/common/modecont.c
  gecko/src/media/libvpx/libvpx/vp8/common/quant_common.c
  gecko/src/media/libvpx/libvpx/vp8/common/reconinter.c
  gecko/src/media/libvpx/libvpx/vp8/common/reconintra.c
  gecko/src/media/libvpx/libvpx/vp8/common/reconintra4x4.c
  gecko/src/media/libvpx/libvpx/vp8/common/rtcd.c
  gecko/src/media/libvpx/libvpx/vp8/common/setupintrarecon.c
  gecko/src/media/libvpx/libvpx/vp8/common/swapyv12buffer.c
  gecko/src/media/libvpx/libvpx/vp8/common/treecoder.c
  gecko/src/media/libvpx/libvpx/vp8/common/vp8_loopfilter.c
  gecko/src/media/libvpx/libvpx/vp8/decoder/dboolhuff.c
  gecko/src/media/libvpx/libvpx/vp8/decoder/decodeframe.c
  gecko/src/media/libvpx/libvpx/vp8/decoder/decodemv.c
  gecko/src/media/libvpx/libvpx/vp8/decoder/detokenize.c
  gecko/src/media/libvpx/libvpx/vp8/decoder/onyxd_if.c
  gecko/src/media/libvpx/libvpx/vp8/decoder/threading.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/bitstream.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/boolhuff.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/dct.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/denoising.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/encodeframe.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/encodeintra.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/encodemb.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/encodemv.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/ethreading.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/lookahead.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/mcomp.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/modecosts.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/mr_dissim.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/onyx_if.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/pickinter.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/picklpf.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/ratectrl.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/rdopt.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/segmentation.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/tokenize.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/treewriter.c
  gecko/src/media/libvpx/libvpx/vp8/encoder/vp8_quantize.c
  gecko/src/media/libvpx/libvpx/vp8/vp8_cx_iface.c
  gecko/src/media/libvpx/libvpx/vp8/vp8_dx_iface.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_alloccommon.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_blockd.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_common_data.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_entropy.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_entropymode.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_entropymv.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_filter.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_frame_buffers.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_idct.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_loopfilter.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_mvref_common.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_pred_common.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_quant_common.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_reconinter.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_reconintra.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_rtcd.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_scale.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_scan.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_seg_common.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_thread_common.c
  gecko/src/media/libvpx/libvpx/vp9/common/vp9_tile_common.c
  gecko/src/media/libvpx/libvpx/vp9/decoder/vp9_decodeframe.c
  gecko/src/media/libvpx/libvpx/vp9/decoder/vp9_decodemv.c
  gecko/src/media/libvpx/libvpx/vp9/decoder/vp9_decoder.c
  gecko/src/media/libvpx/libvpx/vp9/decoder/vp9_detokenize.c
  gecko/src/media/libvpx/libvpx/vp9/decoder/vp9_dsubexp.c
  gecko/src/media/libvpx/libvpx/vp9/decoder/vp9_dthread.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_alt_ref_aq.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_aq_360.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_aq_complexity.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_aq_cyclicrefresh.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_aq_variance.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_bitstream.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_context_tree.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_cost.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_dct.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_encodeframe.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_encodemb.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_encodemv.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_encoder.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_ethread.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_extend.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_firstpass.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_lookahead.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_mbgraph.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_mcomp.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_noise_estimate.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_picklpf.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_pickmode.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_quantize.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_ratectrl.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_rd.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_rdopt.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_resize.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_segmentation.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_skin_detection.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_speed_features.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_subexp.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_svc_layercontext.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_temporal_filter.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_tokenize.c
  gecko/src/media/libvpx/libvpx/vp9/encoder/vp9_treewriter.c
  gecko/src/media/libvpx/libvpx/vp9/vp9_cx_iface.c
  gecko/src/media/libvpx/libvpx/vp9/vp9_dx_iface.c
  gecko/src/media/libvpx/libvpx/vpx/src/vpx_codec.c
  gecko/src/media/libvpx/libvpx/vpx/src/vpx_decoder.c
  gecko/src/media/libvpx/libvpx/vpx/src/vpx_encoder.c
  gecko/src/media/libvpx/libvpx/vpx/src/vpx_image.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/avg.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/bitreader.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/bitreader_buffer.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/bitwriter.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/bitwriter_buffer.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/fwd_txfm.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/intrapred.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/inv_txfm.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/loopfilter.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/prob.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/psnr.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/quantize.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/sad.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/subtract.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/sum_squares.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/variance.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/vpx_convolve.c
  gecko/src/media/libvpx/libvpx/vpx_dsp/vpx_dsp_rtcd.c
  gecko/src/media/libvpx/libvpx/vpx_mem/vpx_mem.c
  gecko/src/media/libvpx/libvpx/vpx_scale/generic/gen_scalers.c
  gecko/src/media/libvpx/libvpx/vpx_scale/generic/vpx_scale.c
  gecko/src/media/libvpx/libvpx/vpx_scale/generic/yv12config.c
  gecko/src/media/libvpx/libvpx/vpx_scale/generic/yv12extend.c
  gecko/src/media/libvpx/libvpx/vpx_scale/vpx_scale_rtcd.c
  gecko/src/media/libvpx/libvpx/vpx_util/vpx_thread.c
)

# TODO: set include path for windows.
if (LINUX OR ANDROID)
  if (ANDROID OR ARM)
    set(vpx_asm_include_dir "gecko/src/media/libvpx/config/linux/arm/")
    set(vpx_include_dir "gecko/include/mozilla/media/libvpx/config/linux/arm/")
  elseif (X86_64)
    set(vpx_asm_include_dir "gecko/src/media/libvpx/config/linux/x64/")
    set(vpx_include_dir "gecko/include/mozilla/media/libvpx/config/linux/x64/")
  else()
    set(vpx_asm_include_dir "gecko/src/media/libvpx/config/linux/ia32/")
    set(vpx_include_dir "gecko/include/mozilla/media/libvpx/config/linux/ia32/")
  endif()
elseif (MACOS)
  if (X86_64)
    set(vpx_asm_include_dir "gecko/src/media/libvpx/config/mac/x64/")
    set(vpx_include_dir "gecko/include/mozilla/media/libvpx/config/mac/x64")
  else()
    set(vpx_asm_include_dir "gecko/src/media/libvpx/config/mac/ia32/")
    set(vpx_include_dir "gecko/include/mozilla/media/libvpx/config/mac/ia32/")
  endif()
endif()

target_include_directories(vpx PRIVATE
  gecko/include/
  gecko/include/mozilla/media/libvpx/config
  gecko/include/mozilla/media/libvpx/libvpx
  gecko/include/mozilla/media/libvpx/libvpx/vp8/
  gecko/include/mozilla/media/libvpx/libvpx/vp8/common
  gecko/include/mozilla/media/libvpx/libvpx/vp8/decoder
  gecko/include/mozilla/media/libvpx/libvpx/vp8/encoder
  gecko/include/mozilla/media/libvpx/libvpx/vpx_dsp
  gecko/include/mozilla/media/libvpx/libvpx/vpx_mem
  gecko/include/mozilla/media/libvpx/libvpx/vpx_util
  ${vpx_include_dir}
)

if (X86_64)
  target_sources(vpx PRIVATE
    gecko/src/media/libvpx/libvpx/vp8/common/mfqe.c
    gecko/src/media/libvpx/libvpx/vp8/common/postproc.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/filter_x86.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idct_blk_mmx.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idct_blk_sse2.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/loopfilter_x86.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/vp8_asm_stubs.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/firstpass.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/temporal_filter.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/denoising_sse2.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_sse4.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_ssse3.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_enc_stubs_mmx.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_enc_stubs_sse2.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_quantize_sse2.c
    gecko/src/media/libvpx/libvpx/vp9/common/vp9_mfqe.c
    gecko/src/media/libvpx/libvpx/vp9/common/vp9_postproc.c
    gecko/src/media/libvpx/libvpx/vp9/common/x86/vp9_idct_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_ssse3.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_diamond_search_sad_avx.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_error_intrin_avx2.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_frame_scale_ssse3.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_quantize_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/add_noise.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/deblock.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/avg_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/inv_txfm_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/loopfilter_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/loopfilter_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/quantize_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad4d_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sum_squares_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_impl_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_asm_stubs.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_intrin_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_intrin_ssse3.c
  )

  set(asm_sources
    gecko/src/media/libvpx/libvpx/vp8/common/x86/copy_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/copy_sse3.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/dequantize_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idctllm_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idctllm_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/iwalsh_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/loopfilter_block_sse2_x86_64.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/loopfilter_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/mfqe_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/recon_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/recon_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/subpixel_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/subpixel_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/subpixel_ssse3.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/dct_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/encodeopt.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/fwalsh_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/temporal_filter_apply_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/common/x86/vp9_mfqe_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_error_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_quantize_ssse3_x86_64.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_temporal_filter_apply_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/add_noise_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/avg_ssse3_x86_64.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/deblock_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_ssse3_x86_64.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/intrapred_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/intrapred_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/inv_txfm_ssse3_x86_64.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/inv_wht_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/quantize_avx_x86_64.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/quantize_ssse3_x86_64.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad4d_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_sse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_sse4.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/ssim_opt_x86_64.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/subpel_variance_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/subtract_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_convolve_copy_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_bilinear_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_bilinear_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_ports/emms.asm
    gecko/src/media/libvpx/libvpx/vpx_ports/x86_abi_support.asm
  )

  foreach(asm_file ${asm_sources})
    get_filename_component(file_name ${asm_file} NAME)
    set(obj_file ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.o)
    add_custom_command(OUTPUT ${obj_file}
      COMMAND ${YASM_EXE}
      ARGS -f ${yasm_binary_format} -Igecko/src/media/libvpx/libvpx/ -I${vpx_asm_include_dir} -o ${obj_file} ./${asm_file}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM)
    list(APPEND VPX_OBJS ${obj_file})
  endforeach()

  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/x86/idct_blk_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/x86/denoising_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_sse4.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse4.1")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_ssse3.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mssse3")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_enc_stubs_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_quantize_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/common/x86/vp9_idct_intrin_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_intrin_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_ssse3.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mssse3")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_diamond_search_sad_avx.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_error_intrin_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_frame_scale_ssse3.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mssse3")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_quantize_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/avg_intrin_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/inv_txfm_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/loopfilter_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/loopfilter_intrin_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/quantize_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad4d_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sum_squares_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_impl_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_sse2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -msse2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_intrin_avx2.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mavx2")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_intrin_ssse3.c PROPERTIES COMPILE_FLAGS "-I${vpx_asm_include_dir} -mssse3")

  target_include_directories(vpx PRIVATE
    gecko/include/mozilla/media/libvpx/libvpx/vp8/common/x86
  )
elseif (X86)
  target_sources(vpx PRIVATE
    gecko/src/media/libvpx/libvpx/vp8/common/mfqe.c
    gecko/src/media/libvpx/libvpx/vp8/common/postproc.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/filter_x86.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idct_blk_mmx.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idct_blk_sse2.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/loopfilter_x86.c
    gecko/src/media/libvpx/libvpx/vp8/common/x86/vp8_asm_stubs.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/firstpass.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/temporal_filter.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/denoising_sse2.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_sse4.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_ssse3.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_enc_stubs_mmx.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_enc_stubs_sse2.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/vp8_quantize_sse2.c
    gecko/src/media/libvpx/libvpx/vp9/common/vp9_mfqe.c
    gecko/src/media/libvpx/libvpx/vp9/common/vp9_postproc.c
    gecko/src/media/libvpx/libvpx/vp9/common/x86/vp9_idct_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_ssse3.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_diamond_search_sad_avx.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_error_intrin_avx2.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_frame_scale_ssse3.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_quantize_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/add_noise.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/deblock.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/avg_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/fwd_txfm_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/inv_txfm_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/loopfilter_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/loopfilter_intrin_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/quantize_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad4d_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sum_squares_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_impl_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/variance_sse2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_asm_stubs.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_intrin_avx2.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_intrin_ssse3.c
  )

  set(asm_sources
    gecko/src/media/libvpx/libvpx/vp8/common/x86/copy_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/copy_sse3.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/dequantize_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idctllm_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/idctllm_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/iwalsh_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/loopfilter_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/mfqe_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/recon_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/recon_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/subpixel_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/subpixel_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/common/x86/subpixel_ssse3.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/dct_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/encodeopt.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/fwalsh_sse2.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/quantize_mmx.asm
    gecko/src/media/libvpx/libvpx/vp8/encoder/x86/temporal_filter_apply_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/common/x86/vp9_mfqe_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_dct_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_temporal_filter_apply_sse2.asm
    gecko/src/media/libvpx/libvpx/vp9/encoder/x86/vp9_error_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/add_noise_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/deblock_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/intrapred_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/intrapred_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/inv_wht_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad4d_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_sse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_sse4.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/sad_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/subpel_variance_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/subtract_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_convolve_copy_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_8t_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_bilinear_sse2.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/x86/vpx_subpixel_bilinear_ssse3.asm
    gecko/src/media/libvpx/libvpx/vpx_ports/emms.asm
    gecko/src/media/libvpx/libvpx/vpx_ports/x86_abi_support.asm
  )
  foreach(asm_file ${asm_sources})
    get_filename_component(file_name ${asm_file} NAME)
    set(obj_file ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.o)
    add_custom_command(OUTPUT ${obj_file}
      COMMAND ${YASM_EXE}
      ARGS -f ${yasm_binary_format} -Igecko/src/media/libvpx/libvpx/ -I${vpx_asm_include_dir} -o ${obj_file} ./${asm_file}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM)
    list(APPEND VPX_OBJS ${obj_file})
  endforeach()

elseif (ARM)
  set(asm_sources
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct16x16_1_add_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct16x16_add_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct4x4_1_add_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct4x4_add_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct8x8_1_add_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct8x8_add_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/intrapred_neon_asm.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/loopfilter_16_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/loopfilter_4_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/loopfilter_8_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/save_reg_neon.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/vpx_convolve8_avg_neon_asm.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/vpx_convolve8_neon_asm.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/vpx_convolve_avg_neon_asm.asm
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/vpx_convolve_copy_neon_asm.asm
  )

  set(VPX_GENERATED_AS_SRCS )
  foreach(asm_file ${asm_sources})
    get_filename_component(file_name ${asm_file} NAME)
    get_filename_component(directory ${asm_file} DIRECTORY)
    set(asm_dst_path ${CMAKE_CURRENT_BINARY_DIR}/${asm_file}.S)
    add_custom_target(build-time-make-directory-${file_name} ALL
      COMMAND ${CMAKE_COMMAND} -E make_directory ${directory})
    add_custom_command(OUTPUT ${asm_dst_path}
      COMMAND gecko/src/media/libvpx/libvpx/build/make/ads2gas.pl
      ARGS < ./${asm_file} > ${asm_dst_path}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      DEPENDS build-time-make-directory-${file_name}
      VERBATIM)
    set_source_files_properties(${asm_dst_path} PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/gecko/src/media/libvpx/libvpx/ -I${vpx_asm_include_dir} -march=armv7-a -mfpu=neon")
    list(APPEND VPX_GENERATED_AS_SRCS ${asm_dst_path})
  endforeach()

  target_sources(vpx PRIVATE
    gecko/src/media/libvpx/libvpx/vp8/common/arm/loopfilter_arm.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/bilinearpredict_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/copymem_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/dc_only_idct_add_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/dequant_idct_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/dequantizeb_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/idct_blk_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/idct_dequant_0_2x_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/idct_dequant_full_2x_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/iwalsh_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/loopfiltersimplehorizontaledge_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/loopfiltersimpleverticaledge_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/mbloopfilter_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/shortidct4x4llm_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/sixtappredict_neon.c
    gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/vp8_loopfilter_neon.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/denoising_neon.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/fastquantizeb_neon.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/shortfdct_neon.c
    gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/vp8_shortwalsh4x4_neon.c
    gecko/src/media/libvpx/libvpx/vp9/common/arm/neon/vp9_iht4x4_add_neon.c
    gecko/src/media/libvpx/libvpx/vp9/common/arm/neon/vp9_iht8x8_add_neon.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/arm/neon/vp9_dct_neon.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/arm/neon/vp9_error_neon.c
    gecko/src/media/libvpx/libvpx/vp9/encoder/arm/neon/vp9_quantize_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/avg_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/fwd_txfm_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/hadamard_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct16x16_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_135_add_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_1_add_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_34_add_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_add_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/intrapred_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/sad4d_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/sad_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/subpel_variance_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/subtract_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/variance_neon.c
    gecko/src/media/libvpx/libvpx/vpx_dsp/arm/vpx_convolve_neon.c
    gecko/src/media/libvpx/libvpx/vpx_ports/arm_cpudetect.c
    ${VPX_GENERATED_AS_SRCS}
  )

  target_include_directories(vpx PRIVATE
    gecko/include/mozilla/media/libvpx/config/linux/arm
    ${vpx_asm_include_dir}
  )

  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_ports/arm_cpudetect.c PROPERTIES COMPILE_FLAGS "-I$ENV{ANDROID_NDK_HOME}/sources/android/cpufeatures/")

  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/bilinearpredict_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/copymem_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/dc_only_idct_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/dequant_idct_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/dequantizeb_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/idct_blk_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/idct_dequant_0_2x_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/idct_dequant_full_2x_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/iwalsh_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/loopfiltersimplehorizontaledge_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/loopfiltersimpleverticaledge_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/mbloopfilter_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/shortidct4x4llm_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/sixtappredict_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/common/arm/neon/vp8_loopfilter_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/denoising_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/fastquantizeb_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/shortfdct_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp8/encoder/arm/neon/vp8_shortwalsh4x4_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/common/arm/neon/vp9_iht4x4_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/common/arm/neon/vp9_iht8x8_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/arm/neon/vp9_dct_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/arm/neon/vp9_error_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vp9/encoder/arm/neon/vp9_quantize_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/avg_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/fwd_txfm_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/hadamard_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct16x16_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_135_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_1_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_34_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/idct32x32_add_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/intrapred_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/sad4d_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/sad_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/subpel_variance_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/subtract_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/variance_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")
  set_source_files_properties(gecko/src/media/libvpx/libvpx/vpx_dsp/arm/vpx_convolve_neon.c PROPERTIES COMPILE_FLAGS "-march=armv7-a -mfpu=neon")

else()
  target_include_directories(vpx PRIVATE gecko/include/mozilla/media/libvpx/config/generic)
endif()

if (ANDROID)
  target_sources(vpx PRIVATE
    $ENV{ANDROID_NDK_HOME}/sources/android/cpufeatures/cpu-features.c
  )
endif()


#########
# lgpllibs
#########

add_library(lgpllibs SHARED
  $<TARGET_OBJECTS:soundtouch>
)

install(TARGETS lgpllibs
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)

set_target_properties(lgpllibs PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
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
    gecko/include/mozilla/media/libtheora/include
)

set_target_properties(tests PROPERTIES COMPILE_FLAGS ${GECKO_MEDIA_PREPROCESSOR_INCLUDES})


##########
# gecko_media
##########
set(FFVPX_OBJS )
set(FFMPEG57_OBJS )
if (LINUX OR MACOS)
  set(FFVPX_OBJS $<TARGET_OBJECTS:ffvpx>)
endif()

if (NOT ANDROID)
  set(FFMPEG57_OBJS $<TARGET_OBJECTS:ffmpeg57>)
endif()

set(VORBIS_OBJS)
if (ANDROID)
  set(VORBIS_OBJS $<TARGET_OBJECTS:tremor>)
else()
  set(VORBIS_OBJS $<TARGET_OBJECTS:vorbis>)
endif()

add_library(gecko_media_cmake STATIC
  $<TARGET_OBJECTS:ogg>
  $<TARGET_OBJECTS:gecko_core>
  $<TARGET_OBJECTS:nestegg>
  $<TARGET_OBJECTS:opus>
  ${VORBIS_OBJS}
  $<TARGET_OBJECTS:theora>
  $<TARGET_OBJECTS:speex_resampler>
  $<TARGET_OBJECTS:cubeb>
  $<TARGET_OBJECTS:vpx>
  ${VPX_OBJS}
  $<TARGET_OBJECTS:tests>
  ${FFVPX_OBJS}
  ${FFMPEG57_OBJS}
)


install(TARGETS gecko_media_cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)

target_link_libraries(gecko_media_cmake lgpllibs)
