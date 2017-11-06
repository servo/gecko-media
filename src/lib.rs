// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

#[allow(unused_extern_crates)]
extern crate cubeb_pulse;
#[macro_use]
extern crate lazy_static;
extern crate libc;
extern crate mime;
#[allow(unused_extern_crates)]
extern crate mp4parse_capi;

pub mod mime_parser_glue;
mod top;

pub mod bindings {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[doc(inline)]
pub use bindings::CanPlayTypeResult as CanPlayType;
pub use top::GeckoMedia;

#[cfg(test)]
mod tests {
    use std::sync::mpsc;
    use {CanPlayType, GeckoMedia};

    fn test_can_play_type() {
        let gecko_media = GeckoMedia::get().unwrap();
        assert_eq!(gecko_media.can_play_type("bogus/bogus"), CanPlayType::No);
    }

    #[test]
    fn run_tests() {
        GeckoMedia::get().unwrap();
        test_can_play_type();
        GeckoMedia::get().unwrap().test();
        let (sender, receiver) = mpsc::channel();
        GeckoMedia::get().unwrap().queue_task(move || sender.send(()).unwrap());
        receiver.recv().unwrap();
        GeckoMedia::shutdown().unwrap();
    }
}
