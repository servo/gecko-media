#[allow(unused_extern_crates)]

#[cfg(feature = "pulseaudio")]
extern crate cubeb_pulse;

#[cfg(test)]
mod tests {

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
