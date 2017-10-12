// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use libc::c_char;
use mime::Mime;
use std::ffi::{CStr, CString};
use std::ptr;
use std::str::FromStr;

#[no_mangle]
pub extern "C" fn mime_parser_new(mime_str: *const c_char) -> *mut Mime {
    if mime_str.is_null() {
        return ptr::null_mut();
    }
    let m = unsafe { CStr::from_ptr(mime_str) };
    let mime_rust_str = m.to_str().unwrap();
    match Mime::from_str(mime_rust_str) {
        Ok(m) => Box::into_raw(Box::new(m)),
        Err(_) => ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn mime_parser_free(ptr: *mut Mime) {
    if ptr.is_null() {
        return;
    }
    unsafe {
        Box::from_raw(ptr);
    }
}

#[no_mangle]
pub extern "C" fn mime_parser_get_param(parser: *mut Mime, param: *const c_char) -> *mut c_char {
    if parser.is_null() || param.is_null() {
        return ptr::null_mut();
    }
    let parser = unsafe { &mut *parser };
    let param = unsafe { CStr::from_ptr(param) }.to_str();
    if let Err(_) = param {
        return ptr::null_mut();
    }
    match parser.get_param(param.unwrap()) {
        Some(value) => CString::new(value.as_str()).unwrap().into_raw(),
        None => ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn mime_parser_free_string(c_string: *mut c_char) {
    unsafe {
        if c_string.is_null() {
            return;
        }
        CString::from_raw(c_string)
    };
}

#[no_mangle]
pub extern "C" fn mime_parser_get_type(parser: *mut Mime) -> *mut c_char {
    if parser.is_null() {
        return ptr::null_mut();
    }
    let parser = unsafe { &mut *parser };
    let mut full_type: String = parser.type_().as_str().to_owned();
    full_type.push_str(&"/");
    full_type.push_str(parser.subtype().as_str());
    CString::new(full_type.as_str()).unwrap().into_raw()
}
