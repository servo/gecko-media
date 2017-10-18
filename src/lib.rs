// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

#[allow(unused_extern_crates)]
#[cfg(feature = "pulseaudio")]
extern crate cubeb_pulse;
extern crate libc;
extern crate mime;

use std::ffi::CString;

mod mime_parser_glue;

pub mod bindings {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

use bindings::{GeckoMedia_CanPlayType, GeckoMedia_Initialize, GeckoMedia_Shutdown};

#[doc(inline)]
pub use bindings::CanPlayTypeResult as CanPlayType;

pub use mime_parser_glue::mime_parser_new;
pub use mime_parser_glue::mime_parser_free;
pub use mime_parser_glue::mime_parser_get_param;
pub use mime_parser_glue::mime_parser_free_string;
pub use mime_parser_glue::mime_parser_get_type;

/// Initialize Gecko Media. Must be called on the thread which Gecko will
/// consider the "main" thread.
pub unsafe fn initialize() {
    GeckoMedia_Initialize();
}

/// Shutsdown Gecko Media.
pub unsafe fn shutdown() {
    GeckoMedia_Shutdown();
}

/// Reports whether Gecko Media can play the corresponding MIME type, as
/// expected by the HTMLMediaElement.canPlayType API.
pub unsafe fn can_play_type(mime_type: &str) -> CanPlayType {
    if let Ok(mime_type) = CString::new(mime_type.as_bytes()) {
        GeckoMedia_CanPlayType(mime_type.as_ptr())
    }  else {
        CanPlayType::No
    }
}

#[cfg(test)]
mod tests {
    use std::thread;
    use super::*;

    extern "C" {
        fn TestGecko();
    }

    unsafe fn test_can_play_type() {
        assert_eq!(can_play_type("bogus/bogus"), CanPlayType::No);
    }

    #[test]
    fn run_tests() {
        thread::spawn(move || {
            unsafe {
                initialize();
                TestGecko();
                test_can_play_type();
                shutdown();
            };
        }).join().unwrap();
    }
}
