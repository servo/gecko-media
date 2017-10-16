// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

#[allow(unused_extern_crates)]
#[cfg(feature = "pulseaudio")]
extern crate cubeb_pulse;
extern crate libc;
extern crate mime;

mod mime_parser_glue;
pub use mime_parser_glue::mime_parser_new;
pub use mime_parser_glue::mime_parser_free;
pub use mime_parser_glue::mime_parser_get_param;
pub use mime_parser_glue::mime_parser_free_string;
pub use mime_parser_glue::mime_parser_get_type;

use std::ffi::CString;

/// Initialize Gecko Media. Must be called on the thread which Gecko will
/// consider the "main" thread.
pub fn initialize() {
    unsafe {
        GeckoMedia_Initialize();
    }
}

/// Shutsdown Gecko Media.
pub fn shutdown() {
    unsafe {
        GeckoMedia_Shutdown();
    }
}

/// Return type of can_play_type.
#[derive(PartialEq, Debug)]
pub enum CanPlayType {
    No,
    Maybe,
    Probably,
}

/// Reports whether Gecko Media can play the corresponding MIME type, as
/// expected by the HTMLMediaElement.canPlayType API.
pub fn can_play_type(mime_type: &str) -> CanPlayType {
    unsafe {
        if let Ok(mime_type) = CString::new(mime_type.as_bytes()) {
            match GeckoMedia_CanPlayType(mime_type.as_ptr()) {
                CanPlayTypeResult::No => CanPlayType::No,
                CanPlayTypeResult::Maybe => CanPlayType::Maybe,
                CanPlayTypeResult::Probably => CanPlayType::Probably,
            }
        }  else {
            CanPlayType::No
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    extern "C" {
        fn TestGecko();
    }

    fn test_can_play_type() {
        assert_eq!(can_play_type("bogus/bogus"), CanPlayType::No);
    }

    #[test]
    fn run_tests() {
        initialize();
        unsafe {
            TestGecko();
        };
        test_can_play_type();
        shutdown();
    }
}
