// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use CanPlayType;
use bindings::*;
use mse::mediasource::{MediaSource, MediaSourceImpl};
use mse::sourcebuffer::{SourceBuffer, SourceBufferImpl};
use mse::sourcebufferlist::{SourceBufferList, SourceBufferListImpl};
use player::{NetworkResource, Player, PlayerEventSink};
use std::ffi::CString;
use std::ops::Drop;
use std::os::raw::c_void;
use std::rc::Weak;
use std::sync::Mutex;
use std::sync::atomic::{AtomicUsize, Ordering, ATOMIC_USIZE_INIT};
use std::sync::mpsc::{self, Receiver, Sender};
use std::sync::{self, Once};
use std::thread::Builder;
use timestamp::GeckoMedia_Rust_TimeNow;

/// Represents the main connection to the media playback system.
pub struct GeckoMedia {
    sender: Sender<GeckoMediaMsg>,
}

impl GeckoMedia {
    /// Retrieves a handle to the media system.
    pub fn get() -> Result<Self, ()> {
        OUTSTANDING_HANDLES.fetch_add(1, Ordering::SeqCst);
        INITIALIZER.call_once(|| {
            let (msg_sender, msg_receiver) = mpsc::channel();
            let (ok_sender, ok_receiver) = mpsc::channel();
            let msg_sender_clone = msg_sender.clone();
            Builder::new()
                .name("GeckoMedia".to_owned())
                .spawn(move || {
                    let thread_observer_object = thread_observer_object(msg_sender_clone);
                    let services = RustServicesFnTable {
                        mGetTimeNowFn: Some(GeckoMedia_Rust_TimeNow),
                    };
                    assert!(
                        unsafe { GeckoMedia_Initialize(thread_observer_object, services) },
                        "failed to initialize GeckoMedia"
                    );
                    ok_sender.send(()).unwrap();
                    drop(ok_sender);
                    server_loop(msg_receiver);
                })
                .unwrap();
            ok_receiver.recv().unwrap();
            unsafe {
                SENDER = Box::into_raw(Box::new(Mutex::new(Some(msg_sender))));
            }
        });
        let sender = unsafe { &*SENDER }.lock().unwrap();
        match sender.clone() {
            Some(sender) => Ok(GeckoMedia {
                sender,
            }),
            None => {
                OUTSTANDING_HANDLES.fetch_sub(1, Ordering::SeqCst);
                Err(())
            },
        }
    }

    /// Shuts down the media playback system. Call this when you're
    /// finished playing media. Returns Err() if any GeckoMedia or Player
    /// objects remain undropped.
    pub fn shutdown() -> Result<(), ()> {
        INITIALIZER.call_once(|| unsafe {
            SENDER = Box::into_raw(Box::new(Mutex::new(None)));
        });
        let mut sender = unsafe { &*SENDER }.lock().unwrap();
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

    /// Reports whether GeckoMedia can play a specified MIME type.
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
            mData: Box::into_raw(Box::new(f)) as *mut c_void,
            mFunction: Some(call::<F>),
        };

        unsafe { GeckoMedia_QueueRustRunnable(runnable) }
    }

    /// Creates a Player to play the media file stored in bytes.
    pub fn create_blob_player(
        &self,
        media_data: Vec<u8>,
        mime_type: &str,
        sink: Box<PlayerEventSink>,
    ) -> Result<Player, ()> {
        let handle = GeckoMedia::get()?;
        let id = NEXT_PLAYER_ID.fetch_add(1, Ordering::SeqCst);
        Player::new(handle, id, media_data, mime_type, sink)
    }

    /// Creates a Player to play from a network resource.
    pub fn create_network_player(
        &self,
        resource: Box<NetworkResource>,
        mime_type: &str,
        sink: Box<PlayerEventSink>,
    ) -> Result<Player, ()> {
        let handle = GeckoMedia::get()?;
        let id = NEXT_PLAYER_ID.fetch_add(1, Ordering::SeqCst);
        Player::from_network_resource(handle, id, resource, mime_type, sink)
    }

    /// Creates a MediaSource instance and its corresponding GeckoMediaSource reflector.
    impl_gecko_media_struct_constructor!(
        create_media_source,
        MediaSourceImpl,
        MediaSource,
        NEXT_MEDIA_SOURCE_ID
    );

    /// Creates a SourceBuffer instance and its corresponding GeckoMediaSourceBuffer reflector.
    pub fn create_source_buffer(
        callbacks: Weak<SourceBufferImpl>,
        parent_id: usize,
        mime: &str,
    ) -> Result<SourceBuffer, ()> {
        let handle = GeckoMedia::get()?;
        let id = NEXT_SOURCE_BUFFER_ID.fetch_add(1, Ordering::SeqCst);
        if let Some(callbacks) = callbacks.upgrade() {
            SourceBuffer::new(handle, id, callbacks, parent_id, mime)
        } else {
            panic!("Callbacks gone")
        }
    }

    /// Creates a SourceBufferList instance and its corresponding GeckoMediaSourceBufferList reflector.
    impl_gecko_media_struct_constructor!(
        create_source_buffer_list,
        SourceBufferListImpl,
        SourceBufferList,
        NEXT_SOURCE_BUFFER_LIST_ID
    );

    /// Check to see whether the MediaSource is capable of creating SourceBuffer
    /// objects for the specified MIME type.
    pub fn is_type_supported(&self, mime_type: &str) -> bool {
        if let Ok(mime_type) = CString::new(mime_type.as_bytes()) {
            let (sender, receiver) = mpsc::channel();
            self.queue_task(move || unsafe {
                sender
                    .send(GeckoMedia_MediaSource_IsTypeSupported(mime_type.as_ptr()))
                    .unwrap();
            });
            receiver.recv().unwrap()
        } else {
            false
        }
    }

    #[cfg(test)]
    pub fn test(&self) {
        self.sender.send(GeckoMediaMsg::Test).unwrap();
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
    #[cfg(test)]
    Test,
    CallProcessGeckoEvents,
}

static OUTSTANDING_HANDLES: AtomicUsize = ATOMIC_USIZE_INIT;
static NEXT_PLAYER_ID: AtomicUsize = ATOMIC_USIZE_INIT;
static NEXT_MEDIA_SOURCE_ID: AtomicUsize = ATOMIC_USIZE_INIT;
static NEXT_SOURCE_BUFFER_ID: AtomicUsize = ATOMIC_USIZE_INIT;
static NEXT_SOURCE_BUFFER_LIST_ID: AtomicUsize = ATOMIC_USIZE_INIT;
static mut SENDER: *const Mutex<Option<Sender<GeckoMediaMsg>>> = 0 as *const _;
static INITIALIZER: Once = sync::ONCE_INIT;

fn server_loop(receiver: Receiver<GeckoMediaMsg>) {
    loop {
        match receiver.recv().unwrap() {
            GeckoMediaMsg::Exit(sender) => {
                unsafe { GeckoMedia_Shutdown() };
                sender.send(()).unwrap();
                break;
            },
            GeckoMediaMsg::CanPlayType(mime_type, sender) => unsafe {
                sender
                    .send(GeckoMedia_CanPlayType(mime_type.as_ptr()))
                    .unwrap();
            },
            GeckoMediaMsg::CallProcessGeckoEvents => {
                // Process any pending messages in Gecko's main thread
                // event queue.
                unsafe {
                    GeckoMedia_ProcessEvents();
                }
            },
            #[cfg(test)]
            GeckoMediaMsg::Test => {
                extern "C" {
                    fn TestGecko();
                }
                unsafe {
                    TestGecko();
                }
            },
        }
    }
}

fn thread_observer_object(sender: Sender<GeckoMediaMsg>) -> ThreadObserverObject {
    unsafe extern "C" fn on_dispatched_event(ptr: *mut c_void) {
        let sender = &*(ptr as *const Sender<GeckoMediaMsg>);
        sender.send(GeckoMediaMsg::CallProcessGeckoEvents).unwrap();
    }

    unsafe extern "C" fn free(ptr: *mut c_void) {
        drop(Box::from_raw(ptr as *mut Sender<GeckoMediaMsg>));
    }

    static VTABLE: ThreadObserverVtable = ThreadObserverVtable {
        mOnDispatchedEvent: Some(on_dispatched_event),
        mFree: Some(free),
    };

    ThreadObserverObject {
        mData: Box::into_raw(Box::new(sender)) as *mut c_void,
        mVtable: &VTABLE,
    }
}
