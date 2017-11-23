// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use GeckoMedia;
use bindings::{GeckoMedia_MediaSource_Create, GeckoMedia_MediaSource_IsTypeSupported};
use bindings::{GeckoMedia_MediaSource_Shutdown, GeckoMediaSourceImpl};
use std::ffi::CString;
use std::os::raw::c_void;
use std::sync::mpsc;

pub struct MediaSource {
    gecko_media: GeckoMedia,
    id: usize,
}

impl MediaSource {
    pub fn new(gecko_media: GeckoMedia,
               id: usize,
               media_source_impl: Box<MediaSourceImpl>) -> MediaSource {
        let callbacks = to_ffi_callbacks(media_source_impl);
        gecko_media.queue_task(move || unsafe {
            GeckoMedia_MediaSource_Create(id, callbacks);
        });
        MediaSource {
            gecko_media,
            id,
        }
    }

    pub fn is_type_supported(&self, mime_type: &str) -> bool {
        if let Ok(mime_type) = CString::new(mime_type.as_bytes()) {
            let (sender, receiver) = mpsc::channel();
            self.gecko_media.queue_task(move || unsafe {
                sender.send(GeckoMedia_MediaSource_IsTypeSupported(mime_type.as_ptr())).unwrap();
            });
            receiver.recv().unwrap()
        } else {
            false
        }
    }
}

impl Drop for MediaSource {
    fn drop(&mut self) {
        let mediasource_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_MediaSource_Shutdown(mediasource_id);
        });
    }
}

/// Users of MediaSource pass in an implementation of this trait when creating
/// MediaSource objects.
pub trait MediaSourceImpl {
    /// MediaSource ready state getter.
    fn get_ready_state(&self) -> i32;
}

fn to_ffi_callbacks(media_source_impl: Box<MediaSourceImpl>) -> GeckoMediaSourceImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.
    struct Wrapper {
        media_source_impl: Box<MediaSourceImpl>,
    }
    unsafe extern "C" fn free(ptr: *mut c_void) {
        drop(Box::from_raw(ptr as *mut Wrapper));
    }
    unsafe extern "C" fn get_ready_state(ptr: *mut c_void) -> i32 {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.media_source_impl.get_ready_state()
    }

    GeckoMediaSourceImpl {
        mContext: Box::into_raw(Box::new(Wrapper { media_source_impl })) as *mut c_void,
        mFree: Some(free),
        mGetReadyState: Some(get_ready_state),
    }
}
