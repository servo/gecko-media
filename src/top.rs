// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use CanPlayType;
use bindings::*;
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

/// Represents the main connection to the media playback system.
pub struct GeckoMedia {
    sender: Sender<GeckoMediaMsg>,
}

/// Plays a media resource.
pub struct Player {
    gecko_media: GeckoMedia,
    id: usize,
}

/// Holds useful metadata extracted from a media resource during loading.
pub struct Metadata {
    /// Duration of the media in seconds, as described either by metadata
    /// in the container, or an estimate if no better inforation exists.
    pub duration: f64,
}

/// Holds pixel data and coordinates of a plane of data.
///
/// skip, enable various output formats from hardware decoder. They
/// are per-pixel skips in the source image.
///
/// For example when image width is 640, stride is 670, skip is 3,
/// the pixel data looks like:
///
///     |<------------------------- stride ----------------------------->|
///     |<-------------------- width ------------------>|
///      0   3   6   9   12  15  18  21                659             669
///     |----------------------------------------------------------------|
///     |Y___Y___Y___Y___Y___Y___Y___Y...                      |%%%%%%%%%|
///     |Y___Y___Y___Y___Y___Y___Y___Y...                      |%%%%%%%%%|
///     |Y___Y___Y___Y___Y___Y___Y___Y...                      |%%%%%%%%%|
///     |            |<->|
///                skip
///
pub struct Plane {
    pixels: *const u8,
    /// The width of a line of pixels in bytes.
    pub width: i32,
    /// The stride of a line of pixels in bytes.
    pub stride: i32,
    /// The height of the plane in lines.
    pub height: i32,
    /// The skip bytes per pixel. This is the number of bytes to skip between
    /// each sample. So if skip is 3, bytes {0,3,6,...} contain pixels.
    pub skip: i32,
}

impl Plane {
    /// Returns a slice storing the raw pixel data.
    pub fn data<'a>(&'a self) -> &'a [u8] {
        unsafe {
            let size = self.stride as usize * self.height as usize;
            slice::from_raw_parts(self.pixels, size)
        }
    }
}

/// A subregion of an image buffer.
pub struct Region {
    // X coordinate of the origin of the region.
    pub x: u32,
    // Y coordinate of the origin of the region.
    pub y: u32,
    // Width of region.
    pub width: i32,
    // Height of the region.
    pub height: i32,
}

/// Stores a planar YCbCr image.
///
/// We assume that the image data is in the REC 470M color space (see
/// Theora specification, section 4.3.1).
///
/// The YCbCr format can be:
///
/// * 4:4:4 - CbCr width/height are the same as Y.
/// * 4:2:2 - CbCr width is half that of Y. Height is the same.
/// * 4:2:0 - CbCr width and height is half that of Y.
///
/// The color format is detected based on the height/width ratios
/// defined above.
pub struct PlanarYCbCrImage {
    /// Pixel data for the Y channel.
    pub y_plane: Plane,
    /// Pixel data for the Cb channel.
    pub cb_plane: Plane,
    /// Pixel data for the Cr channel.
    pub cr_plane: Plane,
    /// The sub-region of the buffer which contains the image to be rendered.
    pub picture: Region,
    /// The time at which this image should be renderd.
    pub time_stamp: i64,
    /// A stream-unique identifier.
    pub frame_id: u32,
    gecko_image: GeckoPlanarYCbCrImage,
}

impl Drop for PlanarYCbCrImage {
    fn drop(&mut self) {
        unsafe { GeckoMedia_FreeImage(self.gecko_image); };
    }
}

/// Users of Player pass in an implementation of this trait when creating
/// Player objects. When events happen in the Player, users will receive
/// callbacks upon the trait implementation, notifying them of the event.
pub trait PlayerEventSink {
    /// Called when playback has reached the end of media. Playback can
    /// be resumed from the start of media by calling Player::Play, or by
    /// seeking.
    fn playback_ended(&self);
    /// Called if playback has encountered a fatal error. The Player can
    /// no longer function, and should be dropped.
    fn decode_error(&self);
    /// Called when the HTML simple event corresponding to `name` should
    /// be fired at the HTMLMediaElement.
    fn async_event(&self, name: &str);
    /// Called when initial metadata has been loaded.
    fn metadata_loaded(&self, metadata: Metadata);
    /// Called if the duration has changed. This could happen if the Player's
    /// estimate of the duration becomes more accurate, or if playing a live
    /// or unbounded stream. Note this can be called after metadata_loaded()
    /// reports the initial duration.
    fn duration_changed(&self, duration: f64);
    /// Called when the initial video frame and audio sample have been loaded.
    fn loaded_data(&self);
    /// Called when the current playback positions changes, reporting the
    /// current playback position in seconds. This is called whenever the
    /// playback position changes due to significant events (such as seeking)
    /// or roughly once per frame while play media. The value reported here
    /// is HTMLMediaElement.currentTime.
    fn time_update(&self, time: f64);
    /// Called when the Player has started to seek.
    fn seek_started(&self);
    /// Called when the Player has stopped seeking.
    fn seek_completed(&self);
    /// Called when new video frames need to be rendered. The vector of
    /// frames should be passed to the client's compositor for rendering.
    /// Each frame has a timestamp at which it should be rendered.
    ///
    /// Note: this is the complete queue of decoded frames; frames may
    /// be passed multiple times as new frames are added. For example,
    /// the client may receive calls with frames with frame_ids as:
    ///
    ///     update_current_images([1,2,3,4])
    ///     update_current_images([2,3,4,5])
    ///     update_current_images([3,4,5,6])
    ///     ... etc..
    ///
    /// Clients should be careful if making extra copies of the image data to
    /// check the frame_id field to avoid making unnecessay copies.
    ///
    /// TODO: The timestamp needs to be converted into something client
    /// can use.
    fn update_current_images(&self, images: Vec<PlanarYCbCrImage>);
}

impl Player {
    /// Starts playback of the media resource. While playing,
    /// PlayerEventSink::time_update() will be called once per frame,
    /// or every 40 milliseconds if there is no video.
    /// PlayerEventSink::playback_ended() will be called when playback
    /// reaches the end of the resource.
    pub fn play(&self) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_Play(player_id);
        });
    }
    /// Pauses playback of the media resource.
    pub fn pause(&self) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_Pause(player_id);
        });
    }
    /// Seeks the media resource to a time offset from the beginning of the
    /// resource in seconds. Calls PlayerEventSink::seek_started when the
    /// playback engine begins seeking, and PlayerEventSink::seek_completed
    /// when the seek completes. A PlayerEventSink::time_update() will be
    /// called just before seek_completed() with the current time after
    /// the seek.
    pub fn seek(&self, time_offset_seconds: f64) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_Seek(player_id, time_offset_seconds);
        });
    }
    // Changes the volume. Volume is in the range [0, 1.0].
    pub fn set_volume(&self, volume: f64) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_SetVolume(player_id, volume);
        });
    }
}

impl Drop for Player {
    fn drop(&mut self) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_Shutdown(player_id);
        });
    }
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

        Ok(Player {
            gecko_media: handle,
            id,
        })
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
            assert!(
                unsafe { GeckoMedia_Initialize(thread_observer_object) },
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
            time_stamp: img.mTimeStamp,
            frame_id: img.mFrameID,
            gecko_image: img
        }
    })
    .collect::<Vec<PlanarYCbCrImage>>()
}
