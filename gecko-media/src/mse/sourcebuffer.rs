// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::{GeckoMediaSourceBufferImpl, GeckoMedia_SourceBuffer_Create};
use bindings::GeckoMedia_SourceBuffer_EvictData;
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
    ) -> Result<Self, ()> {
        let callbacks = to_ffi_callbacks(callbacks);
        let mime = match CString::new(mime.as_bytes()) {
            Ok(mime) => mime,
            _ => return Err(()),
        };
        gecko_media.queue_task(move || unsafe {
            GeckoMedia_SourceBuffer_Create(id, callbacks, parent_id, mime.as_ptr());
        });
        Ok(Self {
            gecko_media,
            id,
        })
    }

    pub fn evict_coded_frames(&self, parent_id: usize, len: usize, buffer_full: &mut bool) {
        let id = self.id;
        let (sender, receiver) = mpsc::channel();
        self.gecko_media.queue_task(move || unsafe {
            sender
                .send(GeckoMedia_SourceBuffer_EvictData(
                    id,
                    parent_id,
                    len as i64,
                    buffer_full,
                ))
                .unwrap();
        });
        receiver.recv().unwrap();
    }
}

impl_drop_gecko_media_struct!(SourceBuffer, GeckoMedia_SourceBuffer_Shutdown);

pub trait SourceBufferImpl {}

pub fn to_ffi_callbacks(callbacks: Rc<SourceBufferImpl>) -> GeckoMediaSourceBufferImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(Rc<SourceBufferImpl>);

    GeckoMediaSourceBufferImpl {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
        mFree: Some(free),
    }
}
