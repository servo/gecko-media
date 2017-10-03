extern crate cc;

use std::env;

fn make_builder(cpp: bool) -> cc::Build {
    let mut b = cc::Build::new();
    b.cpp(cpp);

    if cpp {
        b.flag("-std=c++11");
    } else {
        #[cfg(target_arch = "x86_64")]
        {
            b.define("_LARGEFILE64_SOURCE", "");
            b.define("_FILE_OFFSET_BITS", "64");
        }
        b.define("_GNU_SOURCE", "");
    }

    b.flag("-Wno-unused-parameter");

    b.include("gecko/glue/include/stl_wrappers");
    b.include("gecko/include/system_wrappers");
    b.include("gecko/include/nspr");
    b.include("gecko/include/nspr/private");
    b.include("gecko/include");
    b.include("gecko/glue/include");
    b.include("gecko/include/mozilla/");
    b.include("gecko/include/mozilla/double-conversion");
    b.include("gecko/include/mozilla/media/libogg/include");
    b.include("gecko/include/mozilla/media/libsoundtouch/src");
    b.include("gecko/include/mozilla/media/libspeex_resampler/src");
    b.include("gecko/include/mozilla/media/libcubeb/src");

    #[cfg(target_os = "macos")] {
        // (lack of) space matters. This is not a bug, it'safeature.
        let flags = "-Xpreprocessor -includemozilla-config-x86_64-apple-darwin.h -Xpreprocessor -includeundefs.h";
        env::set_var("CFLAGS", flags);
        env::set_var("CXXFLAGS", flags);
        b.define("DARWIN", "");
        b.flag("-Wno-inline-new-delete");
        b.warnings(false);
    }

    #[cfg(target_os = "linux")]
    env::set_var(
        "CXXFLAGS",
        "--include gecko/glue/include/mozilla-config-x86_64-linux.h --include gecko/glue/include/undefs.h",
    );

    b.flag("-fno-exceptions");

    b.define("MOZILLA_INTERNAL_API", "1");
    b.define("_PR_PTHREADS", "1");
    b.define("GECKO_MEDIA_CRATE", "1");
    b.define("RUST_BINDGEN", "1");

    if let Ok(profile) = env::var("PROFILE") {
        if profile == "debug" {
            b.define("DEBUG", "1");
        }
    }

    b
}

fn compile_tests() {
    let mut b = make_builder(true);

    let src_files = ["test.cpp"];

    for file_path in src_files
        .iter()
        .map(|&p| "gecko/test/".to_owned() + p.clone())
    {
        b.file(file_path);
    }
    b.compile("geckotest");
}

fn configure_libsoundtouch(_c_builder: &mut cc::Build, cpp_builder: &mut cc::Build) {
    let src_files = [
        "media/libsoundtouch/src/cpu_detect_x86.cpp",
        "media/libsoundtouch/src/RateTransposer.cpp",
        "media/libsoundtouch/src/InterpolateLinear.cpp",
        "media/libsoundtouch/src/AAFilter.cpp",
        "media/libsoundtouch/src/FIFOSampleBuffer.cpp",
        "media/libsoundtouch/src/FIRFilter.cpp",
        "media/libsoundtouch/src/SoundTouchFactory.cpp",
        "media/libsoundtouch/src/InterpolateCubic.cpp",
        "media/libsoundtouch/src/InterpolateShannon.cpp",
        "media/libsoundtouch/src/SoundTouch.cpp",
        "media/libsoundtouch/src/TDStretch.cpp",
    ];

    for file_path in src_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        #[cfg(feature = "audio-sample-type-f32")]
        {
            // FIXME: set SSE2 cflags?
            cpp_builder.define("SOUNDTOUCH_ALLOW_SSE", "1");
            cpp_builder.file("gecko/src/media/libsoundtouch/src/sse_optimized.cpp");
        }
        #[cfg(not(feature = "audio-sample-type-f32"))]
        {
            // FIXME: set MMX cflags?
            cpp_builder.file("gecko/src/media/libsoundtouch/src/mmx_optimized.cpp");
        }
    }
    // #[cfg(not(target_os = "windows"))]
    // {
    //     env::set_var(
    //         "CXXFLAGS",
    //         "--include gecko/include/mozilla/media/libsoundtouch/src/soundtouch_perms.h",
    //     );
    // }
    #[cfg(target_os = "windows")]
    {
        cpp_builder.define("alloca", "_alloca");
    }

    cpp_builder.define("ST_NO_EXCEPTION_HANDLING", "1");
}

fn configure_libspeex_resampler(c_builder: &mut cc::Build, cpp_builder: &mut cc::Build) {
    c_builder.file("gecko/src/media/libspeex_resampler/src/resample.c");
    cpp_builder.file("gecko/src/media/libspeex_resampler/src/simd_detect.cpp");

    let mut defines = Vec::<(&str, &str)>::new();

    defines.push(("OUTSIDE_SPEEX", "1"));
    defines.push(("EXPORT", ""));

    #[cfg(feature = "audio-sample-type-s16")]
    {
        defines.push(("FIXED_POINT", "1"));
    }
    #[cfg(not(feature = "audio-sample-type-s16"))]
    {
        defines.push(("FLOATING_POINT", "1"));

        #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
        {
            defines.push(("_USE_SSE", "1"));
            defines.push(("_USE_SSE2", "1"));
            // FIXME: set SSE2 cflags?
            c_builder.file("gecko/src/media/libspeex_resampler/src/resample_sse.c");
        }
    }
    #[cfg(target_arch = "arm")]
    {
        // FIXME: set NEON cflags?
        defines.push(("_USE_NEON", "1"));
        c_builder.file("gecko/src/media/libspeex_resampler/src/resample_neon.c");
    }

    for &(name, value) in defines.iter() {
        c_builder.define(name, value);
        cpp_builder.define(name, value);
    }
}

fn configure_libcubeb(c_builder: &mut cc::Build, cpp_builder: &mut cc::Build) {
    let src_c_files = [
        "media/libcubeb/src/cubeb.c",
        "media/libcubeb/src/cubeb_strings.c",
    ];
    for file_path in src_c_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        c_builder.file(file_path);
    }
    let src_cpp_files = [
        "media/libcubeb/src/cubeb_log.cpp",
        "media/libcubeb/src/cubeb_mixer.cpp",
        "media/libcubeb/src/cubeb_panner.cpp",
    ];
    for file_path in src_cpp_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    let mut defines = Vec::<(&str, &str)>::new();

    #[cfg(feature = "alsa")]
    {
        c_builder.file("gecko/src/media/libcubeb/src/cubeb_alsa.c");
        defines.push(("USE_ALSA", "1"));
        c_builder.flag("-lasound");
        cpp_builder.flag("-lasound");
    }

    #[cfg(any(feature = "pulseaudio", feature = "jack"))]
    {
        cpp_builder.file("gecko/src/media/libcubeb/src/cubeb_resampler.cpp");
    }

    #[cfg(feature = "pulseaudio")]
    {
        c_builder.file("gecko/src/media/libcubeb/src/cubeb_pulse.c");
        defines.push(("USE_PULSE", "1"));
        defines.push(("USE_PULSE_RUST", "1"));
    }

    #[cfg(feature = "jack")]
    {
        cpp_builder.file("gecko/src/media/libcubeb/src/cubeb_jack.cpp");
        defines.push(("USE_JACK", "1"));
        // TODO: use lib speex
    }

    #[cfg(target_os = "openbsd")]
    {
        c_builder.file("gecko/src/media/libcubeb/src/cubeb_sndio.c");
        defines.push(("USE_SNDIO", "1"));
    }

    #[cfg(target_os = "macos")]
    {
        let src_files = [
            "media/libcubeb/src/cubeb_audiounit.cpp",
            "media/libcubeb/src/cubeb_resampler.cpp",
        ];
        for file_path in src_files
            .iter()
            .map(|&p| "gecko/src/".to_owned() + p.clone())
        {
            cpp_builder.file(file_path);
        }

        c_builder.file("gecko/src/media/libcubeb/src/cubeb_osx_run_loop.c");
        defines.push(("USE_AUDIOUNIT", "1"));
    }

    #[cfg(target_os = "windows")]
    {
        cpp_builder.file("gecko/src/media/libcubeb/src/cubeb_resampler.cpp");
        cpp_builder.file("gecko/src/media/libcubeb/src/cubeb_wasapi.cpp");
        c_builder.file("gecko/src/media/libcubeb/src/cubeb_winmm.c");
        defines.push(("USE_WINMM", "1"));
        defines.push(("USE_WASAPI", "1"));
        // TODO: add avrt to "OS_LIBS"
    }

    #[cfg(target_os = "android")]
    {
        c_builder.file("gecko/src/media/libcubeb/src/cubeb_audiotrack.c");
        c_builder.file("gecko/src/media/libcubeb/src/cubeb_opensl.c");
        cpp_builder.file("gecko/src/media/libcubeb/src/cubeb_resampler.cpp");
        defines.push(("USE_OPENSL", "1"));
        defines.push(("USE_AUDIOTRACK", "1"));
    }

    for &(name, value) in defines.iter() {
        c_builder.define(name, value);
        cpp_builder.define(name, value);
    }
}

fn compile_gecko_media() {
    let mut c_builder = make_builder(false);
    let mut cpp_builder = make_builder(true);

    configure_libspeex_resampler(&mut c_builder, &mut cpp_builder);
    configure_libcubeb(&mut c_builder, &mut cpp_builder);
    configure_libsoundtouch(&mut c_builder, &mut cpp_builder);

    let src_cpp_files = [
        "dom/media/AudioStream.cpp",
        "dom/media/CubebUtils.cpp",
        "dom/media/MediaInfo.cpp",
        #[cfg(target_os = "macos")]
        "dom/media/systemservices/OSXRunLoopSingleton.cpp",
        "memory/fallible/fallible.cpp",
        "memory/mozalloc/mozalloc.cpp",
        "memory/mozalloc/mozalloc_abort.cpp",
        "memory/mozalloc/mozalloc_oom.cpp",
        "mfbt/Assertions.cpp",
        "mfbt/ChaosMode.cpp",
        "mfbt/HashFunctions.cpp",
        "mfbt/Unused.cpp",
        "mozglue/misc/ConditionVariable_posix.cpp",
        "mozglue/misc/Printf.cpp",
        #[cfg(target_os = "macos")]
        "mozglue/misc/TimeStamp_darwin.cpp",
        #[cfg(target_os = "linux")]
        "mozglue/misc/TimeStamp_posix.cpp",
        #[cfg(target_os = "windows")]
        "mozglue/misc/TimeStamp_windows.cpp",
        "mozglue/misc/TimeStamp.cpp",
        #[cfg(unix)]
        "mozglue/misc/Mutex_posix.cpp",
        "toolkit/library/StaticXULComponentsStart.cpp",
        "toolkit/library/StaticXULComponentsEnd/StaticXULComponentsEnd.cpp",
        "xpcom/base/ClearOnShutdown.cpp",
        "xpcom/base/nsCOMPtr.cpp",
        "xpcom/base/nsClassInfoImpl.cpp",
        "xpcom/base/nsID.cpp",
        "xpcom/base/nsISupportsImpl.cpp",
        "xpcom/base/nsMemory.cpp",
        "xpcom/base/nsWeakReference.cpp",
        "xpcom/components/GenericFactory.cpp",
        "xpcom/ds/PLDHashTable.cpp",
        "xpcom/ds/nsCOMArray.cpp",
        "xpcom/ds/nsEnumeratorUtils.cpp",
        "xpcom/ds/nsQuickSort.cpp",
        "xpcom/ds/nsSupportsPrimitives.cpp",
        "xpcom/ds/nsTArray.cpp",
        "xpcom/ds/nsTObserverArray.cpp",
        "xpcom/io/nsDirectoryService.cpp",
        "xpcom/string/nsReadableUtilsSSE2.cpp",
        "xpcom/string/nsUTF8UtilsSSE2.cpp",
        "xpcom/string/unified.cpp",
        "xpcom/threads/EventQueue.cpp",
        "xpcom/threads/nsILabelableRunnable.cpp",
        "xpcom/threads/nsThreadManager.cpp",
        "xpcom/threads/nsThreadPool.cpp",
        "xpcom/threads/nsThreadUtils.cpp",
        "xpcom/threads/nsTimerImpl.cpp",
        "xpcom/threads/PrioritizedEventQueue.cpp",
        "xpcom/threads/SharedThreadPool.cpp",
        "xpcom/threads/RecursiveMutex.cpp",
        "xpcom/threads/SynchronizedEventQueue.cpp",
        "xpcom/threads/SystemGroup.cpp",
        "xpcom/threads/TaskQueue.cpp",
        "xpcom/threads/ThreadEventQueue.cpp",
        "xpcom/threads/ThreadEventTarget.cpp",
        "xpcom/threads/TimerThread.cpp",
    ];
    for file_path in src_cpp_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    let src_c_files = [
        "media/libogg/src/ogg_alloc.c",
        "media/libogg/src/ogg_bitwise.c",
        "media/libogg/src/ogg_framing.c",
        "nsprpub/pr/src/io/priometh.c",
        "nsprpub/pr/src/io/prfdcach.c",
        "nsprpub/pr/src/io/prlayer.c",
        "nsprpub/pr/src/io/prlog.c",
        "nsprpub/pr/src/io/prmapopt.c",
        "nsprpub/pr/src/io/prmmap.c",
        "nsprpub/pr/src/io/prmwait.c",
        "nsprpub/pr/src/io/prprf.c",
        "nsprpub/pr/src/linking/prlink.c",
        "nsprpub/pr/src/malloc/prmem.c",
        "nsprpub/pr/src/md/prosdep.c",
        #[cfg(target_os = "macos")]
        "nsprpub/pr/src/md/unix/darwin.c",
        #[cfg(target_os = "linux")]
        "nsprpub/pr/src/md/unix/linux.c",
        "nsprpub/pr/src/md/unix/unix.c",
        "nsprpub/pr/src/md/unix/unix_errors.c",
        "nsprpub/pr/src/memory/prseg.c",
        "nsprpub/pr/src/misc/pratom.c",
        "nsprpub/pr/src/misc/prdtoa.c",
        "nsprpub/pr/src/misc/prenv.c",
        "nsprpub/pr/src/misc/prerr.c",
        "nsprpub/pr/src/misc/prerror.c",
        "nsprpub/pr/src/misc/prerrortable.c",
        "nsprpub/pr/src/misc/prinit.c",
        "nsprpub/pr/src/misc/prinrval.c",
        "nsprpub/pr/src/misc/prnetdb.c",
        "nsprpub/pr/src/misc/prtime.c",
        "nsprpub/pr/src/pthreads/ptio.c",
        "nsprpub/pr/src/pthreads/ptmisc.c",
        "nsprpub/pr/src/pthreads/ptsynch.c",
        "nsprpub/pr/src/pthreads/ptthread.c",
        "nsprpub/pr/src/threads/prcmon.c",
        "nsprpub/pr/src/threads/prrwlock.c",
        "nsprpub/pr/src/threads/prtpd.c",
    ];
    for file_path in src_c_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        c_builder.file(file_path);
    }

    let glue_files = [
        "AbstractThread.cpp",
        "BlockingResourceBase.cpp",
        "CooperativeThreadPool.cpp",
        "ImageContainer.cpp",
        "InputEventStatistics.cpp",
        "nsDebugImpl.cpp",
        "LabeledEventQueue.cpp",
        "Logging.cpp",
        "MainThreadIdlePeriod.cpp",
        "MediaData.cpp",
        "nsAppRunner.cpp",
        "nsCategoryManager.cpp",
        "nsComponentManager.cpp",
        "nsComponentManagerUtils.cpp",
        "nsObserverList.cpp",
        "nsObserverService.cpp",
        "nsThread.cpp",
        "nsTraceRefcnt.cpp",
        "nsCRTGlue.cpp",
        "Preferences.cpp",
        "Scheduler.cpp",
        "SchedulerGroup.cpp",
        "Services.cpp",
        "Telemetry.cpp",
        "ThreadAnnotation.cpp",
        "VideoUtils.cpp",
        "XPCOMInit.cpp",
    ];
    for file_path in glue_files
        .iter()
        .map(|&p| "gecko/glue/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    #[cfg(target_os = "macos")] {
        let frameworks = vec!["CoreFoundation", "CoreAudio", "AudioUnit"];
        for framework in &frameworks {
            println!("cargo:rustc-link-lib=framework={}", framework);
        }
    }

    c_builder.compile("gecko_media_c");
    cpp_builder.compile("gecko_media_cpp");
}

fn main() {
    compile_gecko_media();
    compile_tests();
}
