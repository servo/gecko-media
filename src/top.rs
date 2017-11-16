// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use CanPlayType;
use TimeStamp;
use bindings::*;
use player::{Metadata, Plane, PlanarYCbCrImage, Player, PlayerEventSink, Region};
use std::ffi::CStr;
use std::ffi::CString;
use std::mem;
use std::ops::Drop;
use std::os::raw::c_void;
use std::slice;
use std::sync::Mutex;
use std::sync::atomic::{AtomicUsize, Ordering, ATOMIC_USIZE_INIT};
use std::sync::mpsc::{self, Sender};
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
        let sender = SENDER.lock().unwrap();
        match sender.clone() {
            Some(sender) => Ok(GeckoMedia { sender }),
            None => {
                OUTSTANDING_HANDLES.fetch_sub(1, Ordering::SeqCst);
                Err(())
            }
        }
    }

    /// Shuts down the media playback system. Call this when you're
    /// finished playing media. Returns Err() if any GeckoMedia or Player
    /// objects remain undropped.
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
        let callback = self.to_ffi_callback(sink);
        let media_data = to_ffi_vec(media_data);
        let mime_type = match CString::new(mime_type.as_bytes()) {
            Ok(mime_type) => mime_type,
            _ => return Err(()),
        };
        self.queue_task(move || unsafe {
            GeckoMedia_Player_CreateBlobPlayer(id, media_data, mime_type.as_ptr(), callback);
        });

        Ok(Player::new(handle, id))
    }

    fn to_ffi_callback(&self, sink: Box<PlayerEventSink>) -> PlayerCallbackObject {
        // Can't cast from *c_void to a Trait, so wrap in a concrete type
        // when we pass into C++ code.
        struct Wrapper {
            sink: Box<PlayerEventSink>,
        }
        unsafe extern "C" fn free(ptr: *mut c_void) {
            drop(Box::from_raw(ptr as *mut Wrapper));
        }
        unsafe extern "C" fn decode_error(ptr: *mut c_void) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.decode_error();
        }
        unsafe extern "C" fn playback_ended(ptr: *mut c_void) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.playback_ended();
        }
        unsafe extern "C" fn async_event(ptr: *mut c_void, name: *const i8) {
            let wrapper = &*(ptr as *mut Wrapper);
            let c_str: &CStr = CStr::from_ptr(name);
            wrapper.sink.async_event(c_str.to_str().unwrap());
        }
        unsafe extern "C" fn metadata_loaded(ptr: *mut c_void, metadata: GeckoMediaMetadata) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.metadata_loaded(
                Metadata { duration: metadata.mDuration },
            );
        }
        unsafe extern "C" fn duration_changed(ptr: *mut c_void, duration: f64) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.duration_changed(duration);
        }
        unsafe extern "C" fn loaded_data(ptr: *mut c_void) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.loaded_data();
        }
        unsafe extern "C" fn seek_started(ptr: *mut c_void) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.seek_started();
        }
        unsafe extern "C" fn seek_completed(ptr: *mut c_void) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.seek_completed();
        }
        unsafe extern "C" fn time_update(ptr: *mut c_void, time: f64) {
            let wrapper = &*(ptr as *mut Wrapper);
            wrapper.sink.time_update(time);
        }
        unsafe extern "C" fn update_current_images(ptr: *mut c_void, size: usize,
                                                   elements: *mut GeckoPlanarYCbCrImage) {
            let wrapper = &*(ptr as *mut Wrapper);
            let images = to_ffi_planar_ycbycr_images(size, elements);
            wrapper.sink.update_current_images(images);
        }

        PlayerCallbackObject {
            mContext: Box::into_raw(Box::new(Wrapper { sink: sink } )) as *mut c_void,
            mPlaybackEnded: Some(playback_ended),
            mDecodeError: Some(decode_error),
            mDurationChanged: Some(duration_changed),
            mAsyncEvent: Some(async_event),
            mMetadataLoaded: Some(metadata_loaded),
            mLoadedData: Some(loaded_data),
            mSeekStarted: Some(seek_started),
            mSeekCompleted: Some(seek_completed),
            mTimeUpdate: Some(time_update),
            mUpdateCurrentImages: Some(update_current_images),
            mFree: Some(free),
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

lazy_static! {
    static ref SENDER: Mutex<Option<Sender<GeckoMediaMsg>>> = {
        let (msg_sender, msg_receiver) = mpsc::channel();
        let (ok_sender, ok_receiver) = mpsc::channel();
        let msg_sender_clone = msg_sender.clone();
        Builder::new().name("GeckoMedia".to_owned()).spawn(move || {
            let thread_observer_object = thread_observer_object(msg_sender_clone);
            let services = RustServicesFnTable { mGetTimeNowFn: Some(GeckoMedia_Rust_TimeNow) };
            assert!(
                unsafe { GeckoMedia_Initialize(thread_observer_object, services) },
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

fn to_ffi_vec(bytes: Vec<u8>) -> RustVecU8Object {
    unsafe extern "C" fn free(ptr: *mut u8, len: usize) {
        let ptr = slice::from_raw_parts_mut(ptr, len) as *mut [u8];
        drop(Box::from_raw(ptr));
    }
    let mut bytes = bytes.into_boxed_slice();
    let data = bytes.as_mut_ptr();
    let len = bytes.len();
    mem::forget(bytes);

    RustVecU8Object {
        mData: data,
        mLength: len,
        mFree: Some(free),
    }
}

fn to_ffi_planar_ycbycr_images(size: usize, elements: *mut GeckoPlanarYCbCrImage) -> Vec<PlanarYCbCrImage> {
    let elements = unsafe { slice::from_raw_parts(elements, size) };
    elements.iter()
    .map(|&img| -> PlanarYCbCrImage {
        PlanarYCbCrImage {
            y_plane: Plane {
                pixels: img.mYChannel as *const u8,
                width: img.mYWidth,
                stride: img.mYStride,
                height: img.mYHeight,
                skip: img.mYSkip,
            },
            cb_plane: Plane {
                pixels: img.mCbChannel as *const u8,
                width: img.mCbCrWidth,
                stride: img.mCbCrStride,
                height: img.mCbCrHeight,
                skip: img.mCbSkip,
            },
            cr_plane: Plane {
                pixels: img.mCrChannel as *const u8,
                width: img.mCbCrWidth,
                stride: img.mCbCrStride,
                height: img.mCbCrHeight,
                skip: img.mCrSkip,
            },
            picture: Region {
                x: img.mPicX,
                y: img.mPicY,
                width: img.mPicWidth,
                height: img.mPicHeight,
            },
            time_stamp: TimeStamp(img.mTimeStamp),
            frame_id: img.mFrameID,
            gecko_image: img
        }
    })
    .collect::<Vec<PlanarYCbCrImage>>()
}
