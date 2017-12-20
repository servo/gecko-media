// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoMediaSourceBufferListImpl;
use std::os::raw::c_void;
use std::rc::Rc;

def_gecko_media_struct!(SourceBufferList);

impl_new_gecko_media_struct!(
    SourceBufferList,
    SourceBufferListImpl,
    GeckoMedia_SourceBufferList_Create
);

impl_drop_gecko_media_struct!(SourceBufferList, GeckoMedia_SourceBufferList_Shutdown);

pub trait SourceBufferListImpl {
    /// Get the internal identifier of the index'th SourceBuffer in the list if found.
    fn indexed_getter(&self, index: u32, source_buffer: *mut usize) -> bool;
    /// Get the number of SourceBuffer objects in the list.
    fn length(&self) -> u32;
    /// Append a new SourceBuffer object to the list and optionaly trigger the addsourcebuffer event.
    fn append(&self, source_buffer: *mut c_void, notify: bool);
    /// Clear the list of SourceBuffer objects.
    fn clear(&self);
}

pub fn to_ffi_callbacks(callbacks: Rc<SourceBufferListImpl>) -> GeckoMediaSourceBufferListImpl {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(Rc<SourceBufferListImpl>);

    impl_simple_ffi_getter_wrapper!(length, u32);
    impl_simple_ffi_callback_wrapper!(clear);

    unsafe extern "C" fn indexed_getter(ptr: *mut c_void, index: u32, id: *mut usize) -> bool {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.indexed_getter(index, id)
    }

    unsafe extern "C" fn append(ptr: *mut c_void, source_buffer: *mut c_void, notify: bool) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.append(source_buffer, notify);
    }

    GeckoMediaSourceBufferListImpl {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
        mFree: Some(free),
        mIndexedGetter: Some(indexed_getter),
        mLength: Some(length),
        mAppend: Some(append),
        mClear: Some(clear),
    }
}
