// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoMediaSourceImpl;
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
}

/// Users of MediaSource pass in an implementation of this trait when creating
/// MediaSource objects.
pub trait MediaSourceImpl {
    /// MediaSource ready state getter.
    fn get_ready_state(&self) -> i32;
    /// MediaSource duration getter.
    fn get_duration(&self) -> f64;
}

fn to_ffi_callbacks(callbacks: Rc<MediaSourceImpl>) -> GeckoMediaSourceImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(Rc<MediaSourceImpl>);

    impl_simple_ffi_callback_wrapper!(get_ready_state, i32);
    impl_simple_ffi_callback_wrapper!(get_duration, f64);

    GeckoMediaSourceImpl {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
        mFree: Some(free),
        mGetReadyState: Some(get_ready_state),
        mGetDuration: Some(get_duration),
    }
}
