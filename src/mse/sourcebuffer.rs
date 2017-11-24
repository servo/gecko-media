// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use GeckoMedia;
use bindings::{GeckoMedia_SourceBuffer_Create, GeckoMedia_SourceBuffer_Shutdown};
use bindings::GeckoMediaSourceBufferImpl;
use std::os::raw::c_void;

pub struct SourceBuffer {
    gecko_media: GeckoMedia,
    id: usize,
}

impl SourceBuffer {
    pub fn new(
        gecko_media: GeckoMedia,
        id: usize,
        source_buffer_impl: Box<SourceBufferImpl>,
    ) -> SourceBuffer {
        let callbacks = to_ffi_callbacks(source_buffer_impl);
        gecko_media.queue_task(move || unsafe {
            GeckoMedia_SourceBuffer_Create(id, callbacks);
        });
        SourceBuffer { gecko_media, id }
    }
}

impl Drop for SourceBuffer {
    fn drop(&mut self) {
        let sourcebuffer_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_SourceBuffer_Shutdown(sourcebuffer_id);
        })
    }
}

pub trait SourceBufferImpl {}

pub fn to_ffi_callbacks(source_buffer_impl: Box<SourceBufferImpl>) -> GeckoMediaSourceBufferImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.
    struct Wrapper {
        source_buffer_impl: Box<SourceBufferImpl>,
    }
    unsafe extern "C" fn free(ptr: *mut c_void) {
        drop(Box::from_raw(ptr as *mut Wrapper));
    }

    GeckoMediaSourceBufferImpl {
        mContext: Box::into_raw(Box::new(Wrapper { source_buffer_impl })) as *mut c_void,
        mFree: Some(free),
    }
}