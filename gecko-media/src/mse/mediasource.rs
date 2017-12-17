// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoMedia_MediaSource_EndOfStreamError;
use bindings::{GeckoMediaEndOfStreamError, GeckoMediaSourceImpl, GeckoMediaTimeInterval};
use bindings::{GeckoMedia_MediaSource_DecoderEnded, GeckoMedia_MediaSource_DurationChange};
use std::rc::Rc;

def_gecko_media_struct!(MediaSource);

impl_new_gecko_media_struct!(MediaSource, MediaSourceImpl, GeckoMedia_MediaSource_Create);

impl_drop_gecko_media_struct!(MediaSource, GeckoMedia_MediaSource_Shutdown);

impl MediaSource {
    pub fn duration_change(&self, duration: f64) {
        let id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_MediaSource_DurationChange(id, duration);
        });
    }

    pub fn decoder_ended(&self, ended: bool) {
        let id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_MediaSource_DecoderEnded(id, ended);
        });
    }

    pub fn end_of_stream_error(&self, error: GeckoMediaEndOfStreamError) {
        let id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_MediaSource_EndOfStreamError(id, error);
        })
    }
}

/// Users of MediaSource pass in an implementation of this trait when creating
/// MediaSource objects.
pub trait MediaSourceImpl {
    /// MediaSource ready state getter.
    fn get_ready_state(&self) -> i32;
    /// MediaSource ready state setter.
    fn set_ready_state(&self, i32);
    /// MediaSource duration getter.
    fn get_duration(&self) -> f64;
    /// Tell whether a media source has live seekable range or not.
    fn has_live_seekable_range(&self) -> bool;
    /// MediaSource live seekable range getter.
    fn get_live_seekable_range(&self) -> GeckoMediaTimeInterval;
    /// Get the list of SourceBuffer objects associated with this MediaSource.
    fn get_source_buffers(&self) -> *mut usize;
    /// Get the subset of sourceBuffers that are providing the selected video
    /// track, the enabled audio track(s), and the "showing" or "hidden" text
    /// track(s).
    fn get_active_source_buffers(&self) -> *mut usize;
    // Clear the list of SourceBuffer objects.
    fn clear_source_buffers(&self);
    // Clear the list of active SourceBuffer objects.
    fn clear_active_source_buffers(&self);
}

fn to_ffi_callbacks(callbacks: Rc<MediaSourceImpl>) -> GeckoMediaSourceImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(Rc<MediaSourceImpl>);

    impl_simple_ffi_getter_wrapper!(get_ready_state, i32);
    impl_simple_ffi_setter_wrapper!(set_ready_state, i32);
    impl_simple_ffi_getter_wrapper!(get_duration, f64);
    impl_simple_ffi_getter_wrapper!(has_live_seekable_range, bool);
    impl_simple_ffi_getter_wrapper!(get_live_seekable_range, GeckoMediaTimeInterval);
    impl_simple_ffi_getter_wrapper!(get_source_buffers, *mut usize);
    impl_simple_ffi_getter_wrapper!(get_active_source_buffers, *mut usize);
    impl_simple_ffi_callback_wrapper!(clear_source_buffers);
    impl_simple_ffi_callback_wrapper!(clear_active_source_buffers);

    GeckoMediaSourceImpl {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
        mFree: Some(free),
        mGetReadyState: Some(get_ready_state),
        mSetReadyState: Some(set_ready_state),
        mGetDuration: Some(get_duration),
        mHasLiveSeekableRange: Some(has_live_seekable_range),
        mGetLiveSeekableRange: Some(get_live_seekable_range),
        mGetSourceBuffers: Some(get_source_buffers),
        mGetActiveSourceBuffers: Some(get_active_source_buffers),
        mClearSourceBuffers: Some(clear_source_buffers),
        mClearActiveSourceBuffers: Some(clear_active_source_buffers),
    }
}
