// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

#[cfg(target_os = "android")]
#[allow(unused_extern_crates)]
extern crate android_ffi;
#[allow(unused_extern_crates)]
extern crate cubeb_pulse;
#[allow(unused_extern_crates)]
extern crate encoding_c;
extern crate libc;
extern crate mime;
#[allow(unused_extern_crates)]
extern crate mp4parse_capi;

#[macro_use]
mod macros;
pub mod mime_parser_glue;
pub mod mse;
pub mod player;
pub mod timestamp;
mod top;

pub mod bindings {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[doc(inline)]
pub use bindings::CanPlayTypeResult as CanPlayType;
pub use bindings::GeckoMediaTimeInterval;
pub use mse::mediasource::{MediaSource as GeckoMediaSource, MediaSourceImpl as GeckoMediaSourceImpl};
pub use mse::sourcebuffer::SourceBuffer as GeckoMediaSourceBuffer;
pub use mse::sourcebuffer::SourceBufferImpl as GeckoMediaSourceBufferImpl;
pub use mse::sourcebufferlist::SourceBufferList as GeckoMediaSourceBufferList;
pub use mse::sourcebufferlist::SourceBufferListImpl as GeckoMediaSourceBufferListImpl;
pub use player::{CachedRangesSink, Metadata, NetworkResource, PlanarYCbCrImage};
pub use player::{Plane, Player, PlayerEventSink, Region};
pub use top::GeckoMedia;
pub use timestamp::TimeStamp;

#[cfg(test)]
mod tests {
    use player::{Metadata, PlanarYCbCrImage, Player, PlayerEventSink};
    use std::cell::Cell;
    use std::ffi::CString;
    use std::fs::File;
    use std::io::prelude::*;
    use std::ops::Range;
    use std::os::raw::c_void;
    use std::ptr;
    use std::rc::Rc;
    use std::sync::{Mutex, mpsc};
    use {CanPlayType, GeckoMedia, GeckoMediaTimeInterval};
    use {GeckoMediaSource, GeckoMediaSourceImpl};
    use {GeckoMediaSourceBuffer, GeckoMediaSourceBufferImpl};

    fn test_can_play_type() {
        let gecko_media = GeckoMedia::get().unwrap();
        assert_eq!(gecko_media.can_play_type("bogus/bogus"), CanPlayType::No);
        assert_eq!(
            gecko_media.can_play_type("audio/mp4; codecs=\"bogus\""),
            CanPlayType::No
        );
        assert_eq!(gecko_media.can_play_type("audio/aac"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("audio/wav"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("audio/mp3"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("audio/mp4"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("audio/flac"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("audio/ogg"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("audio/webm"), CanPlayType::Maybe);
        assert_eq!(
            gecko_media.can_play_type("audio/ogg; codecs=\"vorbis\""),
            CanPlayType::Probably
        );
        assert_eq!(
            gecko_media.can_play_type("video/ogg; codecs=\"vorbis, theora\""),
            CanPlayType::Probably
        );
        assert_eq!(gecko_media.can_play_type("video/ogg"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("video/mp4"), CanPlayType::Maybe);
        assert_eq!(gecko_media.can_play_type("video/webm"), CanPlayType::Maybe);
    }

    enum Status {
        Error,
        Ended,
        AsyncEvent(CString),
        MetadataLoaded(Metadata),
        DurationChanged(f64),
        LoadedData,
        TimeUpdate(f64),
        SeekStarted,
        SeekComplete,
        UpdateImages(Vec<PlanarYCbCrImage>),
        Buffered(Vec<Range<f64>>),
        Seekable(Vec<Range<f64>>),
    }

    fn create_test_player(path: &str, mime: &str) -> (Player, mpsc::Receiver<Status>) {
        let (sender, receiver) = mpsc::channel();
        struct Sink {
            sender: Mutex<mpsc::Sender<Status>>,
        }
        impl PlayerEventSink for Sink {
            fn playback_ended(&self) {
                self.sender.lock().unwrap().send(Status::Ended).unwrap();
            }
            fn decode_error(&self) {
                self.sender.lock().unwrap().send(Status::Error).unwrap();
            }
            fn async_event(&self, name: &str) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::AsyncEvent(CString::new(name).unwrap()))
                    .unwrap();
            }
            fn metadata_loaded(&self, metadata: Metadata) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::MetadataLoaded(metadata))
                    .unwrap();
            }
            fn duration_changed(&self, duration: f64) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::DurationChanged(duration))
                    .unwrap();
            }
            fn loaded_data(&self) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::LoadedData)
                    .unwrap();
            }
            fn time_update(&self, time: f64) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::TimeUpdate(time))
                    .unwrap();
            }
            fn seek_started(&self) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::SeekStarted)
                    .unwrap();
            }
            fn seek_completed(&self) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::SeekComplete)
                    .unwrap();
            }
            fn update_current_images(&self, images: Vec<PlanarYCbCrImage>) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::UpdateImages(images))
                    .unwrap();
            }
            fn buffered(&self, ranges: Vec<Range<f64>>) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::Buffered(ranges))
                    .unwrap();
            }
            fn seekable(&self, ranges: Vec<Range<f64>>) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(Status::Seekable(ranges))
                    .unwrap();
            }
        }
        let sink = Box::new(Sink {
            sender: Mutex::new(sender),
        });
        let mut file = File::open(path).unwrap();
        let mut bytes = vec![];
        file.read_to_end(&mut bytes).unwrap();
        let player = GeckoMedia::get()
            .unwrap()
            .create_blob_player(bytes, mime, sink)
            .unwrap();
        player.set_volume(0.001);
        (player, receiver)
    }

    fn test_basic_playback() {
        let (player, receiver) = create_test_player("gecko/test/audiotest.wav", "audio/wav");
        player.play();
        let ok;
        loop {
            match receiver.recv().unwrap() {
                Status::Ended => {
                    ok = true;
                    break;
                },
                Status::Error => {
                    ok = false;
                    break;
                },
                _ => {},
            };
        }
        assert!(ok);
    }

    fn test_seeking() {
        let (player, receiver) = create_test_player("gecko/test/audiotest.wav", "audio/wav");
        let mut duration = 0.0;
        let mut current_time = 0.0;
        let mut reached_metadata_loaded = false;
        let mut reached_loaded_data = false;
        let mut reached_seek_started = false;
        let mut reached_seek_complete = false;
        let mut reached_ended = false;
        let mut reached_error = false;
        let mut reached_buffered = false;
        let mut reached_seekable = false;
        loop {
            match receiver.recv().unwrap() {
                Status::MetadataLoaded(metadata) => {
                    reached_metadata_loaded = true;
                    duration = metadata.duration;
                    assert!(duration > 0.0);
                },
                Status::LoadedData => {
                    reached_loaded_data = true;
                    assert!(reached_metadata_loaded);
                    player.seek(duration / 2.0);
                },
                Status::TimeUpdate(time) => {
                    current_time = time;
                },
                Status::SeekStarted => {
                    reached_seek_started = true;
                },
                Status::SeekComplete => {
                    reached_seek_complete = true;
                    assert!(reached_seek_started);
                    // The current time should have updated to approximately
                    // the seek target of (duration / 2.0).
                    let delta = (current_time - duration / 2.0).abs();
                    assert!(delta < 0.0001);
                    player.play();
                },
                Status::Ended => {
                    reached_ended = true;
                    break;
                },
                Status::Error => {
                    reached_error = true;
                    break;
                },
                Status::UpdateImages(_images) => {},
                Status::Buffered(_ranges) => {
                    reached_buffered = true;
                },
                Status::Seekable(_ranges) => {
                    reached_seekable = true;
                },
                _ => {},
            };
        }
        assert!(reached_metadata_loaded);
        assert!(reached_loaded_data);
        assert!(reached_seek_started);
        assert!(reached_seek_complete);
        assert!(reached_ended);
        assert!(!reached_error);
        assert!(reached_buffered);
        assert!(reached_seekable);
    }

    #[derive(Clone, Copy)]
    #[allow(dead_code)]
    enum MediaSourceReadyState {
        Closed,
        Open,
        Ended,
    }

    impl From<MediaSourceReadyState> for i32 {
        fn from(state: MediaSourceReadyState) -> Self {
            match state {
                MediaSourceReadyState::Closed => 0,
                MediaSourceReadyState::Open => 1,
                MediaSourceReadyState::Ended => 2,
            }
        }
    }

    impl From<i32> for MediaSourceReadyState {
        fn from(state: i32) -> Self {
            match state {
                0 => MediaSourceReadyState::Closed,
                1 => MediaSourceReadyState::Open,
                2 => MediaSourceReadyState::Ended,
                _ => unreachable!(),
            }
        }
    }

    struct MediaSourceValues {
        pub ready_state: Cell<MediaSourceReadyState>,
        pub duration: f64,
    }

    struct MediaSourceDom {
        pub values: Rc<MediaSourceValues>,
        pub media_source_impl: Rc<MediaSourceImpl>,
        pub gecko_media_source: GeckoMediaSource,
    }
    impl MediaSourceDom {
        pub fn new() -> Self {
            let values = Rc::new(MediaSourceValues {
                ready_state: Cell::new(MediaSourceReadyState::Closed),
                duration: 0.,
            });
            let media_source_impl = Rc::new(MediaSourceImpl {
                values: values.clone(),
            });
            let weak_impl = Rc::downgrade(&media_source_impl);
            Self {
                values,
                media_source_impl,
                gecko_media_source: GeckoMedia::create_media_source(weak_impl).unwrap(),
            }
        }

        pub fn id(&self) -> usize {
            self.gecko_media_source.get_id()
        }
    }
    struct MediaSourceImpl {
        pub values: Rc<MediaSourceValues>,
    }
    impl GeckoMediaSourceImpl for MediaSourceImpl {
        fn get_ready_state(&self) -> i32 {
            self.values.ready_state.get().into()
        }
        fn set_ready_state(&self, state: i32) {
            self.values.ready_state.set(state.into());
        }
        fn get_duration(&self) -> f64 {
            self.values.duration
        }
        fn has_live_seekable_range(&self) -> bool {
            false
        }
        fn get_live_seekable_range(&self) -> GeckoMediaTimeInterval {
            GeckoMediaTimeInterval {
                mStart: 0.,
                mEnd: 0.,
            }
        }
        fn get_source_buffers(&self) -> *mut usize {
            ptr::null_mut()
        }
        fn get_active_source_buffers(&self) -> *mut usize {
            ptr::null_mut()
        }
    }

    fn test_media_source() {
        let _ = MediaSourceDom::new();

        let gecko_media = GeckoMedia::get().unwrap();

        assert_eq!(gecko_media.is_type_supported("bogus/bogus"), false);
        assert_eq!(
            gecko_media.is_type_supported("audio/mp4; codecs=\"bogus\""),
            false
        );
        assert_eq!(gecko_media.is_type_supported("video/mp4"), true);
        assert_eq!(gecko_media.is_type_supported("audio/mp4"), true);
        assert_eq!(gecko_media.is_type_supported("audio/mp3"), false);
        assert_eq!(gecko_media.is_type_supported("audio/flac"), false);
        assert_eq!(gecko_media.is_type_supported("audio/ogg"), false);
        assert_eq!(
            gecko_media.is_type_supported("audio/ogg; codecs=\"vorbis\""),
            false
        );
        assert_eq!(gecko_media.is_type_supported("audio/wav"), false);
    }

    struct SourceBufferAttributes {}

    impl SourceBufferAttributes {
        pub fn new() -> Self {
            SourceBufferAttributes {}
        }
    }

    impl GeckoMediaSourceBufferImpl for SourceBufferAttributes {
        fn owner(&self) -> *mut c_void {
            ptr::null_mut()
        }
        fn get_append_window_start(&self) -> f64 {
            0.
        }
        fn set_append_window_start(&self, _: f64) {}
        fn get_append_window_end(&self) -> f64 {
            0.
        }
        fn set_append_window_end(&self, _: f64) {}
        fn get_timestamp_offset(&self) -> f64 {
            0.
        }
        fn set_timestamp_offset(&self, _: f64) {}
        fn get_append_mode(&self) -> i32 {
            0
        }
        fn set_append_mode(&self, _: i32) {}
        fn get_group_start_timestamp(&self, _: *mut f64) {}
        fn set_group_start_timestamp(&self, _: f64) {}
        fn have_group_start_timestamp(&self) -> bool {
            false
        }
        fn reset_group_start_timestamp(&self) {}
        fn restart_group_start_timestamp(&self) {}
        fn get_group_end_timestamp(&self) -> f64 {
            0.
        }
        fn set_group_end_timestamp(&self, _: f64) {}
        fn get_append_state(&self) -> i32 {
            0
        }
        fn set_append_state(&self, _: i32) {}
        fn get_updating(&self) -> bool {
            false
        }
        fn set_updating(&self, _: bool) {}
        fn get_active(&self) -> bool {
            false
        }
        fn set_active(&self, _: bool) {}
    }

    struct SourceBufferDom {
        pub attributes: Rc<SourceBufferAttributes>,
        pub gecko_media: GeckoMediaSourceBuffer,
    }

    impl SourceBufferDom {
        pub fn new(parent_media_source: &MediaSourceDom, mime: &str) -> Self {
            let attributes = Rc::new(SourceBufferAttributes::new());
            let weak_attributes = Rc::downgrade(&(&attributes));
            Self {
                attributes,
                gecko_media: GeckoMedia::create_source_buffer(weak_attributes, parent_media_source.id(), mime, false)
                    .unwrap(),
            }
        }

        pub fn append_data<S, E>(&self, data: *const u8, len: usize, success_cb: S, error_cb: E)
        where
            S: Fn(),
            E: Fn(u32),
        {
            self.gecko_media.append_data(
                data,
                len,
                success_cb,
                error_cb,
            );
        }
    }

    fn test_source_buffer() {
        let media_source = MediaSourceDom::new();
        let source_buffer = SourceBufferDom::new(&media_source, "video/mp4");
        let empty: [u8; 0] = [];
        // TODO For now this only tests that the mechanism to send closures through FFI works.
        // Should throw error because no decoder is attached yet.
        source_buffer.append_data(empty.as_ptr(), empty.len(), || unreachable!(), |_| {
            // TODO check error code.
            assert!(true);
        });
    }

    #[test]
    fn run_tests() {
        GeckoMedia::get().unwrap();
        test_can_play_type();
        GeckoMedia::get().unwrap().test();
        let (sender, receiver) = mpsc::channel();
        GeckoMedia::get().unwrap().queue_task(
            move || sender.send(()).unwrap(),
        );
        receiver.recv().unwrap();
        test_basic_playback();
        test_seeking();
        test_media_source();
        test_source_buffer();
        GeckoMedia::shutdown().unwrap();
    }
}
