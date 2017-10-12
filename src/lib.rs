// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
