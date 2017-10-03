#[allow(unused_extern_crates)]

#[cfg(feature = "pulseaudio")]
extern crate cubeb_pulse;

#[cfg(test)]
mod tests {

    #[cfg(target_os = "macos")]
    #[link(name = "CoreFoundation", kind = "framework")]
    #[link(name = "CoreAudio", kind = "framework")]
    #[link(name = "AudioUnit", kind = "framework")]
    extern "C" {
        fn TestGecko();
    }

    #[test]
    fn it_works() {
        unsafe {
            TestGecko();
        };
    }
}
