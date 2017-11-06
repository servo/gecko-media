// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use CanPlayType;
use bindings::*;
use std::ffi::CString;
use std::mem;
use std::ops::Drop;
use std::os::raw::c_void;
use std::sync::Mutex;
use std::sync::atomic::{AtomicUsize, Ordering, ATOMIC_USIZE_INIT};
use std::sync::mpsc::{self, Sender};
use std::thread::Builder;

pub struct GeckoMedia {
    sender: Sender<GeckoMediaMsg>,
    id: usize,
}

impl GeckoMedia {
    pub fn get() -> Result<Self, ()> {
        OUTSTANDING_HANDLES.fetch_add(1, Ordering::SeqCst);
        let id = INSTANCE_COUNT.fetch_add(1, Ordering::SeqCst);
        let sender = SENDER.lock().unwrap();
        match sender.clone() {
            Some(sender) => Ok(GeckoMedia { sender, id }),
            None => {
                OUTSTANDING_HANDLES.fetch_sub(1, Ordering::SeqCst);
                Err(())
            }
        }
    }

    pub fn shutdown() -> Result<(), ()> {
        let mut sender = SENDER.lock().unwrap();
        if OUTSTANDING_HANDLES.load(Ordering::SeqCst) > 0 {
            return Err(());
        }
        if let Some(ref sender) = *sender {
            let (ok_sender, ok_receiver) = mpsc::channel();
            let _ = sender.send(GeckoMediaMsg::Exit(ok_sender));
            ok_receiver.recv().unwrap();
        }
        *sender = None;
        Ok(())
    }

    pub fn can_play_type(&self, mime_type: &str) -> CanPlayType {
        if let Ok(mime_type) = CString::new(mime_type.as_bytes()) {
            let (sender, receiver) = mpsc::channel();
            self.sender
                .send(GeckoMediaMsg::CanPlayType(mime_type, sender))
                .unwrap();
            receiver.recv().unwrap()
        } else {
            CanPlayType::No
        }
    }

    pub fn load(&self, blob: Vec<u8>, mime_type: &str) -> Result<(), ()> {
        match CString::new(mime_type.as_bytes()) {
            Ok(mime_type) => {
                self.sender
                    .send(GeckoMediaMsg::Load(self.id, blob, mime_type))
                    .unwrap();
                Ok(())
            }
            _ => Err(()),
        }
    }

    pub fn queue_task<F>(&self, f: F)
    where
        F: FnOnce(),
    {
        unsafe extern "C" fn call<F>(ptr: *mut c_void)
        where
            F: FnOnce(),
        {
            Box::from_raw(ptr as *mut F)()
        }

        let runnable = RustRunnable {
            data: Box::into_raw(Box::new(f)) as *mut c_void,
            function: Some(call::<F>),
        };

        unsafe { GeckoMedia_QueueRustRunnable(runnable) }
    }

    #[cfg(test)]
    pub fn test(&self) {
        self.sender.send(GeckoMediaMsg::Test).unwrap();
    }
}

impl Drop for GeckoMedia {
    fn drop(&mut self) {
        self.sender.send(GeckoMediaMsg::Unload(self.id)).unwrap();
        OUTSTANDING_HANDLES.fetch_sub(1, Ordering::SeqCst);
    }
}

enum GeckoMediaMsg {
    Exit(Sender<()>),
    CanPlayType(CString, Sender<CanPlayType>),
    Load(usize, Vec<u8>, CString),
    Unload(usize),
    #[cfg(test)] Test,
    CallProcessGeckoEvents,
}

static OUTSTANDING_HANDLES: AtomicUsize = ATOMIC_USIZE_INIT;
static INSTANCE_COUNT: AtomicUsize = ATOMIC_USIZE_INIT;

lazy_static! {
    static ref SENDER: Mutex<Option<Sender<GeckoMediaMsg>>> = {
        let (msg_sender, msg_receiver) = mpsc::channel();
        let (ok_sender, ok_receiver) = mpsc::channel();
        let msg_sender_clone = msg_sender.clone();
        Builder::new().name("GeckoMedia".to_owned()).spawn(move || {
            let ptr = Box::into_raw(Box::new(msg_sender_clone));
            let raw_msg_sender = ptr as *mut rust_msg_sender_t;
            assert!(
                unsafe { GeckoMedia_Initialize(raw_msg_sender) },
                "failed to initialize GeckoMedia"
            );
            ok_sender.send(()).unwrap();
            drop(ok_sender);
            loop {
                match msg_receiver.recv().unwrap() {
                    GeckoMediaMsg::Exit(sender) => {
                        unsafe { GeckoMedia_Shutdown() };
                        sender.send(()).unwrap();
                        break;
                    },
                    GeckoMediaMsg::CanPlayType(mime_type, sender) => {
                        unsafe {
                            sender.send(GeckoMedia_CanPlayType(mime_type.as_ptr())).unwrap();
                        }
                    },
                    GeckoMediaMsg::CallProcessGeckoEvents => {
                        // Process any pending messages in Gecko's main thread
                        // event queue.
                        unsafe {
                            GeckoMedia_ProcessEvents();
                        }
                    },
                    GeckoMediaMsg::Load(id, blob, mime_type) => {
                        unsafe {
                            let ptr = blob.as_ptr();
                            let len = blob.len();
                            // C++ code is responsible for calling back to deallocate
                            // memory when finished with it.
                            mem::forget(blob);
                            MediaDecoder_Load(id, ptr, len, mime_type.as_ptr());
                        }
                    },
                    GeckoMediaMsg::Unload(id) => {
                        unsafe {
                            MediaDecoder_Unload(id);
                        }
                    },
                    #[cfg(test)]
                    GeckoMediaMsg::Test => {
                        extern "C" { fn TestGecko(); }
                        unsafe { TestGecko(); }
                    }
                }
            }
        }).unwrap();
        ok_receiver.recv().unwrap();
        Mutex::new(Some(msg_sender))
    };
}

#[no_mangle]
pub unsafe extern "C" fn call_gecko_process_events(ptr: *mut rust_msg_sender_t) {
    if ptr.is_null() {
        return;
    }
    let sender = &*(ptr as *const Sender<GeckoMediaMsg>);
    sender.send(GeckoMediaMsg::CallProcessGeckoEvents).unwrap();
}

#[no_mangle]
pub unsafe extern "C" fn free_gecko_process_events_sender(ptr: *mut rust_msg_sender_t) {
    if !ptr.is_null() {
        return;
    }
    drop(Box::from_raw(ptr as *mut Sender<GeckoMediaMsg>));
}

#[no_mangle]
pub unsafe extern "C" fn free_rust_vec_u8(ptr: *mut u8, len: usize) {
    if !ptr.is_null() {
        return;
    }
    drop(Vec::from_raw_parts(ptr, len, len));
}
