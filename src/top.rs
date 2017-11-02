// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use CanPlayType;
use bindings::*;
use std::ffi::CString;
use std::ops::Drop;
use std::sync::Mutex;
use std::sync::atomic::{AtomicUsize, Ordering, ATOMIC_USIZE_INIT};
use std::sync::mpsc::{self, Sender};
use std::thread::Builder;

pub struct GeckoMedia {
    sender: Sender<GeckoMediaMsg>,
}

impl GeckoMedia {
    pub fn get() -> Result<Self, ()> {
        OUTSTANDING_HANDLES.fetch_add(1, Ordering::SeqCst);
        let sender = SENDER.lock().unwrap();
        match sender.clone() {
            Some(sender) => Ok(GeckoMedia { sender }),
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

    #[cfg(test)]
    pub fn test(&self) {
        let (sender, receiver) = mpsc::channel();
        self.sender.send(GeckoMediaMsg::Test(sender)).unwrap();
        receiver.recv().unwrap();
    }
}

impl Drop for GeckoMedia {
    fn drop(&mut self) {
        OUTSTANDING_HANDLES.fetch_sub(1, Ordering::SeqCst);
    }
}

enum GeckoMediaMsg {
    Exit(Sender<()>),
    CanPlayType(CString, Sender<CanPlayType>),
    #[cfg(test)] Test(Sender<()>),
    CallProcessGeckoEvents,
}

static OUTSTANDING_HANDLES: AtomicUsize = ATOMIC_USIZE_INIT;

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
                    #[cfg(test)]
                    GeckoMediaMsg::Test(sender) => {
                        // To test threading, we pass the sender to Gecko
                        // for it to send a () over once the test completes
                        // asynchronously.

                        extern "C" { fn TestGecko(ptr: *mut rust_msg_sender_t); }
                        let raw_sender = Box::into_raw(Box::new(sender)) as *mut _;
                        unsafe { TestGecko(raw_sender); }
                    }
                }
            }
        }).unwrap();
        ok_receiver.recv().unwrap();
        Mutex::new(Some(msg_sender))
    };
}

#[cfg(test)]
#[no_mangle]
pub unsafe extern "C" fn finish_tests(ptr: *mut rust_msg_sender_t) {
    if ptr.is_null() {
        return;
    }
    let sender = Box::from_raw(ptr as *mut Sender<()>);
    sender.send(()).unwrap();
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
