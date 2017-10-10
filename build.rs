// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

fn build_libsoundtouch() {
    let mut cpp_builder = make_builder(true);

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
    cpp_builder.include("gecko/include/mozilla/media/libsoundtouch/src");
    cpp_builder.include("gecko/glue/include/soundtouch");
    cpp_builder.compile("gkmedia_soundtouch");
}

fn build_libspeex_resampler(cpp_builder: &mut cc::Build) {
    let mut c_builder = make_builder(false);

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

    c_builder.include("gecko/include/mozilla/media/libspeex_resampler/src");
    cpp_builder.include("gecko/include/mozilla/media/libspeex_resampler/src");

    c_builder.compile("gkmedia_speex_resampler_c");
}

fn build_libcubeb(cpp_builder: &mut cc::Build) {
    let mut c_builder = make_builder(false);

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

    c_builder.include("gecko/include/mozilla/media/libcubeb/src");
    cpp_builder.include("gecko/include/mozilla/media/libcubeb/src");
    c_builder.compile("gkmedia_cubeb_c");
}

fn build_libopus() {
    let mut c_builder = make_builder(false);
    c_builder.define("OPUS_BUILD", "");
    c_builder.define("OPUS_VERSION", "\"v1.2.1-mozilla\"");
    c_builder.define("USE_ALLOCA", "");
    c_builder.define("OPUS_EXPORT", "");

    #[cfg(target_os = "macos")]
    c_builder.define("HAVE_LRINTF", "");

    let mut src_files = vec![
        "media/libopus/celt/bands.c",
        "media/libopus/celt/celt.c",
        "media/libopus/celt/celt_decoder.c",
        "media/libopus/celt/celt_encoder.c",
        "media/libopus/celt/celt_lpc.c",
        "media/libopus/celt/cwrs.c",
        "media/libopus/celt/entcode.c",
        "media/libopus/celt/entdec.c",
        "media/libopus/celt/entenc.c",
        "media/libopus/celt/kiss_fft.c",
        "media/libopus/celt/laplace.c",
        "media/libopus/celt/mathops.c",
        "media/libopus/celt/mdct.c",
        "media/libopus/celt/modes.c",
        "media/libopus/celt/pitch.c",
        "media/libopus/celt/quant_bands.c",
        "media/libopus/celt/rate.c",
        "media/libopus/celt/vq.c",
        "media/libopus/silk/A2NLSF.c",
        "media/libopus/silk/ana_filt_bank_1.c",
        "media/libopus/silk/biquad_alt.c",
        "media/libopus/silk/bwexpander.c",
        "media/libopus/silk/bwexpander_32.c",
        "media/libopus/silk/check_control_input.c",
        "media/libopus/silk/CNG.c",
        "media/libopus/silk/code_signs.c",
        "media/libopus/silk/control_audio_bandwidth.c",
        "media/libopus/silk/control_codec.c",
        "media/libopus/silk/control_SNR.c",
        "media/libopus/silk/debug.c",
        "media/libopus/silk/dec_API.c",
        "media/libopus/silk/decode_core.c",
        "media/libopus/silk/decode_frame.c",
        "media/libopus/silk/decode_indices.c",
        "media/libopus/silk/decode_parameters.c",
        "media/libopus/silk/decode_pitch.c",
        "media/libopus/silk/decode_pulses.c",
        "media/libopus/silk/decoder_set_fs.c",
        "media/libopus/silk/enc_API.c",
        "media/libopus/silk/encode_indices.c",
        "media/libopus/silk/encode_pulses.c",
        "media/libopus/silk/gain_quant.c",
        "media/libopus/silk/HP_variable_cutoff.c",
        "media/libopus/silk/init_decoder.c",
        "media/libopus/silk/init_encoder.c",
        "media/libopus/silk/inner_prod_aligned.c",
        "media/libopus/silk/interpolate.c",
        "media/libopus/silk/lin2log.c",
        "media/libopus/silk/log2lin.c",
        "media/libopus/silk/LP_variable_cutoff.c",
        "media/libopus/silk/LPC_analysis_filter.c",
        "media/libopus/silk/LPC_fit.c",
        "media/libopus/silk/LPC_inv_pred_gain.c",
        "media/libopus/silk/NLSF2A.c",
        "media/libopus/silk/NLSF_decode.c",
        "media/libopus/silk/NLSF_del_dec_quant.c",
        "media/libopus/silk/NLSF_encode.c",
        "media/libopus/silk/NLSF_stabilize.c",
        "media/libopus/silk/NLSF_unpack.c",
        "media/libopus/silk/NLSF_VQ.c",
        "media/libopus/silk/NLSF_VQ_weights_laroia.c",
        "media/libopus/silk/NSQ.c",
        "media/libopus/silk/NSQ_del_dec.c",
        "media/libopus/silk/pitch_est_tables.c",
        "media/libopus/silk/PLC.c",
        "media/libopus/silk/process_NLSFs.c",
        "media/libopus/silk/quant_LTP_gains.c",
        "media/libopus/silk/resampler.c",
        "media/libopus/silk/resampler_down2.c",
        "media/libopus/silk/resampler_down2_3.c",
        "media/libopus/silk/resampler_private_AR2.c",
        "media/libopus/silk/resampler_private_down_FIR.c",
        "media/libopus/silk/resampler_private_IIR_FIR.c",
        "media/libopus/silk/resampler_private_up2_HQ.c",
        "media/libopus/silk/resampler_rom.c",
        "media/libopus/silk/shell_coder.c",
        "media/libopus/silk/sigm_Q15.c",
        "media/libopus/silk/sort.c",
        "media/libopus/silk/stereo_decode_pred.c",
        "media/libopus/silk/stereo_encode_pred.c",
        "media/libopus/silk/stereo_find_predictor.c",
        "media/libopus/silk/stereo_LR_to_MS.c",
        "media/libopus/silk/stereo_MS_to_LR.c",
        "media/libopus/silk/stereo_quant_pred.c",
        "media/libopus/silk/sum_sqr_shift.c",
        "media/libopus/silk/table_LSF_cos.c",
        "media/libopus/silk/tables_gain.c",
        "media/libopus/silk/tables_LTP.c",
        "media/libopus/silk/tables_NLSF_CB_NB_MB.c",
        "media/libopus/silk/tables_NLSF_CB_WB.c",
        "media/libopus/silk/tables_other.c",
        "media/libopus/silk/tables_pitch_lag.c",
        "media/libopus/silk/tables_pulses_per_block.c",
        "media/libopus/silk/VAD.c",
        "media/libopus/silk/VQ_WMat_EC.c",
        "media/libopus/src/opus.c",
        "media/libopus/src/opus_decoder.c",
        "media/libopus/src/opus_encoder.c",
        "media/libopus/src/opus_multistream.c",
        "media/libopus/src/opus_multistream_decoder.c",
        "media/libopus/src/opus_multistream_encoder.c",
        "media/libopus/src/repacketizer.c",
    ];

    if cfg!(feature = "audio-sample-type-f32") {
        src_files.extend([
            "media/libopus/silk/float/apply_sine_window_FLP.c",
            "media/libopus/silk/float/autocorrelation_FLP.c",
            "media/libopus/silk/float/burg_modified_FLP.c",
            "media/libopus/silk/float/bwexpander_FLP.c",
            "media/libopus/silk/float/corrMatrix_FLP.c",
            "media/libopus/silk/float/encode_frame_FLP.c",
            "media/libopus/silk/float/energy_FLP.c",
            "media/libopus/silk/float/find_LPC_FLP.c",
            "media/libopus/silk/float/find_LTP_FLP.c",
            "media/libopus/silk/float/find_pitch_lags_FLP.c",
            "media/libopus/silk/float/find_pred_coefs_FLP.c",
            "media/libopus/silk/float/inner_product_FLP.c",
            "media/libopus/silk/float/k2a_FLP.c",
            "media/libopus/silk/float/LPC_analysis_filter_FLP.c",
            "media/libopus/silk/float/LPC_inv_pred_gain_FLP.c",
            "media/libopus/silk/float/LTP_analysis_filter_FLP.c",
            "media/libopus/silk/float/LTP_scale_ctrl_FLP.c",
            "media/libopus/silk/float/noise_shape_analysis_FLP.c",
            "media/libopus/silk/float/pitch_analysis_core_FLP.c",
            "media/libopus/silk/float/process_gains_FLP.c",
            "media/libopus/silk/float/regularize_correlations_FLP.c",
            "media/libopus/silk/float/residual_energy_FLP.c",
            "media/libopus/silk/float/scale_copy_vector_FLP.c",
            "media/libopus/silk/float/scale_vector_FLP.c",
            "media/libopus/silk/float/schur_FLP.c",
            "media/libopus/silk/float/sort_FLP.c",
            "media/libopus/silk/float/warped_autocorrelation_FLP.c",
            "media/libopus/silk/float/wrappers_FLP.c",
            "media/libopus/src/analysis.c",
            "media/libopus/src/mlp.c",
            "media/libopus/src/mlp_data.c",
        ].iter());
        c_builder.include("gecko/include/mozilla/media/libopus/silk/float");

    } else {
        src_files.extend([
            "media/libopus/silk/fixed/apply_sine_window_FIX.c",
            "media/libopus/silk/fixed/autocorr_FIX.c",
            "media/libopus/silk/fixed/burg_modified_FIX.c",
            "media/libopus/silk/fixed/corrMatrix_FIX.c",
            "media/libopus/silk/fixed/encode_frame_FIX.c",
            "media/libopus/silk/fixed/find_LPC_FIX.c",
            "media/libopus/silk/fixed/find_LTP_FIX.c",
            "media/libopus/silk/fixed/find_pitch_lags_FIX.c",
            "media/libopus/silk/fixed/find_pred_coefs_FIX.c",
            "media/libopus/silk/fixed/k2a_FIX.c",
            "media/libopus/silk/fixed/k2a_Q16_FIX.c",
            "media/libopus/silk/fixed/LTP_analysis_filter_FIX.c",
            "media/libopus/silk/fixed/LTP_scale_ctrl_FIX.c",
            "media/libopus/silk/fixed/noise_shape_analysis_FIX.c",
            "media/libopus/silk/fixed/pitch_analysis_core_FIX.c",
            "media/libopus/silk/fixed/process_gains_FIX.c",
            "media/libopus/silk/fixed/regularize_correlations_FIX.c",
            "media/libopus/silk/fixed/residual_energy16_FIX.c",
            "media/libopus/silk/fixed/residual_energy_FIX.c",
            "media/libopus/silk/fixed/schur64_FIX.c",
            "media/libopus/silk/fixed/schur_FIX.c",
            "media/libopus/silk/fixed/vector_ops_FIX.c",
            "media/libopus/silk/fixed/warped_autocorrelation_FIX.c",
        ].iter());
        c_builder.define("FIXED_POINT", "1");
        c_builder.define("DISABLE_FLOAT_API", "");
        c_builder.include("gecko/include/mozilla/media/libopus/silk/fixed");
    }

    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        src_files.extend([
            "media/libopus/celt/x86/pitch_sse.c",
            "media/libopus/celt/x86/x86_celt_map.c",
            "media/libopus/celt/x86/x86cpu.c",
            "media/libopus/celt/x86/pitch_sse2.c",
            "media/libopus/celt/x86/vq_sse2.c",
            "media/libopus/celt/x86/celt_lpc_sse.c",
            "media/libopus/celt/x86/pitch_sse4_1.c",
            "media/libopus/silk/x86/NSQ_del_dec_sse.c",
            "media/libopus/silk/x86/NSQ_sse.c",
            "media/libopus/silk/x86/VAD_sse.c",
            "media/libopus/silk/x86/VQ_WMat_EC_sse.c",
            "media/libopus/silk/x86/x86_silk_map.c",
        ].iter());

        c_builder.define("OPUS_HAVE_RTCD", "");
        c_builder.define("OPUS_X86_MAY_HAVE_SSE", "");
        c_builder.define("OPUS_X86_MAY_HAVE_SSE2", "");
        c_builder.define("OPUS_X86_MAY_HAVE_SSE4_1", "");
        c_builder.define("OPUS_X86_MAY_HAVE_AVX", "");
        c_builder.include("gecko/include/mozilla/media/libopus/celt/x86");
        c_builder.flag("-msse4.1");

        #[cfg(not(feature = "audio-sample-type-f32"))]
        {
            c_builder.file("gecko/src/media/libopus/silk/fixed/x86/burg_modified_FIX_sse.c");
            c_builder.file("gecko/src/media/libopus/silk/fixed/x86/vector_ops_FIX_sse.c");
        }
    }

    #[cfg(target_arch = "arm")]
    {
        src_files.extend([
            "media/libopus/silk/arm/arm_silk_map.c",
            "media/libopus/silk/arm/biquad_alt_neon_intr.c",
            "media/libopus/silk/arm/LPC_inv_pred_gain_neon_intr.c",
            "media/libopus/silk/arm/NSQ_del_dec_neon_intr.c",
            "media/libopus/silk/arm/NSQ_neon.c",
            "media/libopus/silk/fixed/arm/warped_autocorrelation_FIX_neon_intr.c",
            "media/libopus/celt/arm/arm_celt_map.c",
            "media/libopus/celt/arm/armcpu.c",
            "media/libopus/celt/arm/celt_ne10_fft.c",
            "media/libopus/celt/arm/celt_ne10_mdct.c",
            "media/libopus/celt/arm/celt_neon_intr.c",
            "media/libopus/celt/arm/pitch_neon_intr.c",
        ].iter());
    }

    for file_path in src_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        c_builder.file(file_path);
    }

    c_builder.include("gecko/include/mozilla/media/libopus/");
    c_builder.include("gecko/include/mozilla/media/libopus/celt");
    c_builder.include("gecko/include/mozilla/media/libopus/include");
    c_builder.include("gecko/include/mozilla/media/libopus/silk");
    c_builder.include("gecko/include/mozilla/media/libopus/src");
    c_builder.compile("gkmedia_libopus");
}

fn build_libogg() {
    let mut c_builder = make_builder(false);
    let src_files = [
        "media/libogg/src/ogg_alloc.c",
        "media/libogg/src/ogg_bitwise.c",
        "media/libogg/src/ogg_framing.c",
    ];

    for file_path in src_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        c_builder.file(file_path);
    }
    c_builder.include("gecko/include/mozilla/media/libogg/include");
    c_builder.compile("gkmedia_libogg");
}

fn build_libvorbis() {
    let mut c_builder = make_builder(false);
    let src_files = [
        "media/libvorbis/lib/vorbis_analysis.c",
        "media/libvorbis/lib/vorbis_bitrate.c",
        "media/libvorbis/lib/vorbis_block.c",
        "media/libvorbis/lib/vorbis_codebook.c",
        "media/libvorbis/lib/vorbis_envelope.c",
        "media/libvorbis/lib/vorbis_floor0.c",
        "media/libvorbis/lib/vorbis_floor1.c",
        "media/libvorbis/lib/vorbis_info.c",
        "media/libvorbis/lib/vorbis_lookup.c",
        "media/libvorbis/lib/vorbis_lpc.c",
        "media/libvorbis/lib/vorbis_lsp.c",
        "media/libvorbis/lib/vorbis_mapping0.c",
        "media/libvorbis/lib/vorbis_mdct.c",
        "media/libvorbis/lib/vorbis_psy.c",
        "media/libvorbis/lib/vorbis_registry.c",
        "media/libvorbis/lib/vorbis_res0.c",
        "media/libvorbis/lib/vorbis_sharedbook.c",
        "media/libvorbis/lib/vorbis_smallft.c",
        "media/libvorbis/lib/vorbis_synthesis.c",
        "media/libvorbis/lib/vorbis_window.c",
        "media/libvorbis/lib/vorbisenc.c",
    ];
    for file_path in src_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        c_builder.file(file_path);
    }

    c_builder.include("gecko/include/mozilla/media/libvorbis/lib");
    c_builder.include("gecko/include/mozilla/media/libvorbis/include");
    c_builder.include("gecko/include/mozilla/media/libogg/include/");
    c_builder.compile("gkmedia_libvorbis");
}

fn build_libstagefright(cpp_builder: &mut cc::Build) {
    let mut c_builder = make_builder(false);

    let mut src_c_files: Vec<&str> = vec![];

    let src_cpp_files = [
        "media/libstagefright/binding/Adts.cpp",
        "media/libstagefright/binding/AnnexB.cpp",
        "media/libstagefright/binding/BitReader.cpp",
        "media/libstagefright/binding/Box.cpp",
        "media/libstagefright/binding/BufferStream.cpp",
        "media/libstagefright/binding/DecoderData.cpp",
        "media/libstagefright/binding/H264.cpp",
        "media/libstagefright/binding/Index.cpp",
        "media/libstagefright/binding/MP4Metadata.cpp",
        "media/libstagefright/binding/MoofParser.cpp",
        "media/libstagefright/binding/ResourceStream.cpp",
        "media/libstagefright/binding/SinfParser.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/DataSource.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/ESDS.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/MPEG4Extractor.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/MediaBuffer.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/MediaDefs.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/MediaSource.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/MetaData.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/SampleIterator.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/SampleTable.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/Utils.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/foundation/AAtomizer.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/foundation/ABitReader.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/foundation/ABuffer.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/foundation/AString.cpp",
        "media/libstagefright/frameworks/av/media/libstagefright/foundation/hexdump.cpp",
        //"media/libstagefright/gtest/TestInterval.cpp",
        //"media/libstagefright/gtest/TestMP4Rust.cpp",
        //"media/libstagefright/gtest/TestParser.cpp",
        "media/libstagefright/system/core/libutils/RefBase.cpp",
        "media/libstagefright/system/core/libutils/SharedBuffer.cpp",
        "media/libstagefright/system/core/libutils/Static.cpp",
        "media/libstagefright/system/core/libutils/String16.cpp",
        "media/libstagefright/system/core/libutils/String8.cpp",
        "media/libstagefright/system/core/libutils/Unicode.cpp",
        "media/libstagefright/system/core/libutils/VectorImpl.cpp",

    ];
    for file_path in src_cpp_files
        .iter()
        .map(|&p| "gecko/src/".to_owned() + p.clone())
    {
        cpp_builder.file(file_path);
    }

    let mut defines = Vec::<(&str, &str)>::new();
    let mut include_dirs = vec![
        "binding/include",
        "frameworks/av/include",
        "frameworks/av/include/media/stagefright/foundation",
        "frameworks/av/media/libstagefright/",
        "stubs/empty",
        "stubs/include",
        "stubs/include/media/stagefright/foundation",
        "system/core/include",
    ];

    #[cfg(any(target_os = "macos", target_os = "linux"))]
    defines.push(("HAVE_SYS_UIO_H", ""));

    #[cfg(target_os = "macos")] {
        defines.push(("off64_t", "off_t"));
        include_dirs.push("ports/darwin/include");
    }

    #[cfg(not(target_os = "android"))] {
        defines.push(("FAKE_LOG_DEVICE", "1"));
        src_c_files.extend([
            "media/libstagefright/system/core/libcutils/strdup16to8.c",
            "media/libstagefright/system/core/liblog/fake_log_device.c",
            "media/libstagefright/system/core/liblog/logd_write.c",
            "media/libstagefright/system/core/liblog/logprint.c",
        ].iter());
        for file_path in src_c_files.iter()
        {
            c_builder.file(format!("gecko/src/{}", file_path));
        }
    }

    for &(name, value) in defines.iter() {
        c_builder.define(name, value);
        cpp_builder.define(name, value);
    }


    for include_dir in include_dirs.iter()
        .map(|&p| "gecko/include/mozilla/media/libstagefright/".to_owned() + p.clone()) {
        c_builder.include(include_dir.clone());
        cpp_builder.include(include_dir);
    }

    c_builder.compile("gkmedia_stagefright");
}

fn compile_gecko_media() {
    let mut c_builder = make_builder(false);
    let mut cpp_builder = make_builder(true);

    build_libspeex_resampler(&mut cpp_builder);
    build_libcubeb(&mut cpp_builder);
    build_libsoundtouch();
    build_libogg();
    build_libvorbis();
    build_libopus();
    build_libstagefright(&mut cpp_builder);

    let src_cpp_files = [
        "dom/media/AudioStream.cpp",
        "dom/media/CubebUtils.cpp",
        "dom/media/MediaInfo.cpp",
        "dom/media/MediaPrefs.cpp",
        "dom/media/MediaResource.cpp",
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
