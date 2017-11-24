// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::{GeckoMedia_MediaSource_IsTypeSupported, GeckoMediaSourceImpl};
use std::ffi::CString;
use std::sync::mpsc;

def_gecko_media_struct!(MediaSource);

impl_new_gecko_media_struct!(MediaSource, MediaSourceImpl, GeckoMedia_MediaSource_Create);

impl_drop_gecko_media_struct!(MediaSource, GeckoMedia_MediaSource_Shutdown);

impl MediaSource {
    pub fn is_type_supported(&self, mime_type: &str) -> bool {
        if let Ok(mime_type) = CString::new(mime_type.as_bytes()) {
            let (sender, receiver) = mpsc::channel();
            self.gecko_media.queue_task(move || unsafe {
                sender
                    .send(GeckoMedia_MediaSource_IsTypeSupported(mime_type.as_ptr()))
                    .unwrap();
            });
            receiver.recv().unwrap()
        } else {
            false
        }
    }
}

/// Users of MediaSource pass in an implementation of this trait when creating
/// MediaSource objects.
pub trait MediaSourceImpl {
    /// MediaSource ready state getter.
    fn get_ready_state(&self) -> i32;
}

fn to_ffi_callbacks(callbacks: Box<MediaSourceImpl>) -> GeckoMediaSourceImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(MediaSourceImpl);

    unsafe extern "C" fn get_ready_state(ptr: *mut c_void) -> i32 {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.get_ready_state()
    }

    GeckoMediaSourceImpl {
        mContext: Box::into_raw(Box::new(Wrapper { callbacks })) as *mut c_void,
        mFree: Some(free),
        mGetReadyState: Some(get_ready_state),
    }
}
