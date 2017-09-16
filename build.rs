extern crate gcc;

use std::env;

fn make_builder(cpp: bool) -> gcc::Build {
    let mut b = gcc::Build::new();
    b.cpp(cpp);

    if cpp {
        b.flag("-std=c++11");
    } else {
        #[cfg(target_arch = "x86_64")]
        {
            b.define("_LARGEFILE64_SOURCE", "");
            b.define("_FILE_OFFSET_BITS", "64");
        }
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
    b.include("gecko/include/mozilla/media/libsoundtouch/src");
    b.include("gecko/include/mozilla/media/libspeex_resampler/src");
    b.include("gecko/include/mozilla/media/libcubeb/src");

    #[cfg(target_os = "macos")]
    env::set_var(
        "CXXFLAGS",
        "--include gecko/glue/include/mozilla-config-x86_64-apple-darwin.h",
    );

    #[cfg(target_os = "linux")]
    env::set_var(
        "CXXFLAGS",
        "--include gecko/glue/include/mozilla-config-x86_64-linux.h",
    );

    b.flag("-fno-exceptions");

    b.define("MOZILLA_INTERNAL_API", "1");

    b.define("_GNU_SOURCE", "");
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

fn configure_libsoundtouch(c_builder: &mut gcc::Build, cpp_builder: &mut gcc::Build) {
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

fn configure_libspeex_resampler(c_builder: &mut gcc::Build, cpp_builder: &mut gcc::Build) {
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

fn configure_libcubeb(c_builder: &mut gcc::Build, cpp_builder: &mut gcc::Build) {
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
        // FIXME: import cubeb pulse-rs
        //defines.push(("USE_PULSE_RUST", "1"));
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

        #[cfg(feature = "widget-toolkit-cocoa")]
        {
            c_builder.file("gecko/src/media/libcubeb/src/cubeb_osx_run_loop.c");
        }
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
        "memory/fallible/fallible.cpp",
        "memory/mozalloc/mozalloc.cpp",
        "memory/mozalloc/mozalloc_abort.cpp",
        "memory/mozalloc/mozalloc_oom.cpp",
        "mfbt/Assertions.cpp",
        "mfbt/ChaosMode.cpp",
        "mfbt/Unused.cpp",
        "mozglue/misc/ConditionVariable_posix.cpp",
        #[cfg(macos)]
        "mozglue/misc/TimeStamp_darwin.cpp",
        #[cfg(all(unix, not(macos)))]
        "mozglue/misc/TimeStamp_posix.cpp",
        #[cfg(windows)]
        "mozglue/misc/TimeStamp_windows.cpp",
        "mozglue/misc/TimeStamp.cpp",
        "mozglue/misc/Mutex_posix.cpp",
        "xpcom/base/nsISupportsImpl.cpp",
        "xpcom/ds/nsTArray.cpp",
        "xpcom/ds/PLDHashTable.cpp",
        "xpcom/string/unified.cpp",
        "xpcom/threads/BlockingResourceBase.cpp",
        "xpcom/threads/nsThreadManager.cpp",
    ];
    for file_path in src_cpp_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    let src_c_files = [
        "nsprpub/pr/src/io/priometh.c",
        "nsprpub/pr/src/io/prfdcach.c",
        "nsprpub/pr/src/io/prlayer.c",
        "nsprpub/pr/src/io/prlog.c",
        "nsprpub/pr/src/io/prmapopt.c",
        "nsprpub/pr/src/io/prmwait.c",
        "nsprpub/pr/src/io/prprf.c",
        "nsprpub/pr/src/linking/prlink.c",
        "nsprpub/pr/src/malloc/prmem.c",
        "nsprpub/pr/src/md/prosdep.c",
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
        "nsDebugImpl.cpp",
        "Logging.cpp",
        "nsAppRunner.cpp",
        "nsCOMPtr.cpp",
        "nsThreadUtils.cpp",
        "nsTraceRefcnt.cpp",
        "nsCRTGlue.cpp",
        "nsStringBundleService.cpp",
        "Telemetry.cpp",
    ];
    for file_path in glue_files
        .iter()
        .map(|&p| "gecko/glue/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    c_builder.compile("gecko_media_c");
    cpp_builder.compile("gecko_media_cpp");
}

fn main() {
    compile_gecko_media();
    compile_tests();
}
