// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoMediaSourceImpl;
use std::rc::Rc;

def_gecko_media_struct!(MediaSource);

impl_new_gecko_media_struct!(MediaSource, MediaSourceImpl, GeckoMedia_MediaSource_Create);

impl_drop_gecko_media_struct!(MediaSource, GeckoMedia_MediaSource_Shutdown);

impl MediaSource {}

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

    unsafe extern "C" fn get_ready_state(ptr: *mut c_void) -> i32 {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.get_ready_state()
    }
    unsafe extern "C" fn get_duration(ptr: *mut c_void) -> f64 {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.get_duration()
    }

    GeckoMediaSourceImpl {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
        mFree: Some(free),
        mGetReadyState: Some(get_ready_state),
        mGetDuration: Some(get_duration),
    }
}
