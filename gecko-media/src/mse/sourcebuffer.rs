// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoMedia_SourceBuffer_EvictData;
use bindings::{GeckoMediaSourceBufferImpl, GeckoMedia_SourceBuffer_Create};
use std::ffi::CString;
use std::rc::Rc;
use std::sync::mpsc;

def_gecko_media_struct!(SourceBuffer);

impl SourceBuffer {
    pub fn new(
        gecko_media: GeckoMedia,
        id: usize,
        callbacks: Rc<SourceBufferImpl>,
        parent_id: usize,
        mime: &str,
        generate_timestamps: bool,
    ) -> Result<Self, ()> {
        let callbacks = to_ffi_callbacks(callbacks);
        let mime = match CString::new(mime.as_bytes()) {
            Ok(mime) => mime,
            _ => return Err(()),
        };
        gecko_media.queue_task(move || unsafe {
            GeckoMedia_SourceBuffer_Create(id, callbacks, parent_id, mime.as_ptr(), generate_timestamps);
        });
        Ok(Self {
            gecko_media,
            id,
        })
    }

    pub fn evict_coded_frames(&self, len: usize, buffer_full: &mut bool) {
        let id = self.id;
        let (sender, receiver) = mpsc::channel();
        self.gecko_media.queue_task(move || unsafe {
            sender
                .send(GeckoMedia_SourceBuffer_EvictData(
                    id,
                    len as i64,
                    buffer_full,
                ))
                .unwrap();
        });
        receiver.recv().unwrap();
    }
}

impl_drop_gecko_media_struct!(SourceBuffer, GeckoMedia_SourceBuffer_Shutdown);

/// Users of SourceBuffer pass in an implementation of this trait when creating
/// SourceBuffer objects.
pub trait SourceBufferImpl {
    fn get_append_window_start(&self) -> f64;
    fn set_append_window_start(&self, f64);
    fn get_append_window_end(&self) -> f64;
    fn set_append_window_end(&self, f64);
    fn get_timestamp_offset(&self) -> f64;
    fn set_timestamp_offset(&self, f64);
    fn get_append_mode(&self) -> i32;
    fn set_append_mode(&self, i32);
    fn get_group_start_timestamp(&self, *mut f64);
    fn set_group_start_timestamp(&self, f64);
    fn have_group_start_timestamp(&self) -> bool;
    fn reset_group_start_timestamp(&self);
    fn restart_group_start_timestamp(&self);
    fn get_group_end_timestamp(&self) -> f64;
    fn set_group_end_timestamp(&self, f64);
    fn get_append_state(&self) -> i32;
    fn set_append_state(&self, i32);
}

pub fn to_ffi_callbacks(callbacks: Rc<SourceBufferImpl>) -> GeckoMediaSourceBufferImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(Rc<SourceBufferImpl>);

    impl_simple_ffi_getter_wrapper!(get_append_window_start, f64);
    impl_simple_ffi_setter_wrapper!(set_append_window_start, f64);
    impl_simple_ffi_getter_wrapper!(get_append_window_end, f64);
    impl_simple_ffi_setter_wrapper!(set_append_window_end, f64);
    impl_simple_ffi_getter_wrapper!(get_timestamp_offset, f64);
    impl_simple_ffi_setter_wrapper!(set_timestamp_offset, f64);
    impl_simple_ffi_getter_wrapper!(get_append_mode, i32);
    impl_simple_ffi_setter_wrapper!(set_append_mode, i32);
    impl_simple_ffi_setter_wrapper!(set_group_start_timestamp, f64);
    impl_simple_ffi_getter_wrapper!(have_group_start_timestamp, bool);
    impl_simple_ffi_callback_wrapper!(reset_group_start_timestamp);
    impl_simple_ffi_callback_wrapper!(restart_group_start_timestamp);
    impl_simple_ffi_getter_wrapper!(get_group_end_timestamp, f64);
    impl_simple_ffi_setter_wrapper!(set_group_end_timestamp, f64);
    impl_simple_ffi_getter_wrapper!(get_append_state, i32);
    impl_simple_ffi_setter_wrapper!(set_append_state, i32);

    unsafe extern "C" fn get_group_start_timestamp(ptr: *mut c_void, timestamp: *mut f64) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.get_group_start_timestamp(timestamp);
    }

    GeckoMediaSourceBufferImpl {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
        mFree: Some(free),
        mGetAppendWindowStart: Some(get_append_window_start),
        mSetAppendWindowStart: Some(set_append_window_start),
        mGetAppendWindowEnd: Some(get_append_window_end),
        mSetAppendWindowEnd: Some(set_append_window_end),
        mGetTimestampOffset: Some(get_timestamp_offset),
        mSetTimestampOffset: Some(set_timestamp_offset),
        mGetAppendMode: Some(get_append_mode),
        mSetAppendMode: Some(set_append_mode),
        mGetGroupStartTimestamp: Some(get_group_start_timestamp),
        mSetGroupStartTimestamp: Some(set_group_start_timestamp),
        mHaveGroupStartTimestamp: Some(have_group_start_timestamp),
        mResetGroupStartTimestamp: Some(reset_group_start_timestamp),
        mRestartGroupStartTimestamp: Some(restart_group_start_timestamp),
        mGetGroupEndTimestamp: Some(get_group_end_timestamp),
        mSetGroupEndTimestamp: Some(set_group_end_timestamp),
        mGetAppendState: Some(get_append_state),
        mSetAppendState: Some(set_append_state),
    }
}
