// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoPlanarYCbCrImage;
use bindings::{GeckoMediaMetadata, GeckoMediaByteIntervalSet, GeckoMediaTimeInterval};
use bindings::{GeckoMedia_Player_CreateBlobPlayer, GeckoMedia_Player_CreateNetworkPlayer};
use bindings::{GeckoMedia_Player_Pause, GeckoMedia_Player_Play};
use bindings::{GeckoMedia_Player_Seek, GeckoMedia_Player_SetVolume};
use bindings::{NetworkResourceObject, PlaneType_Cb, PlaneType_Cr, PlaneType_Y};
use bindings::{PlayerCallbackObject, RustVecU8Object};
use std::ffi::CStr;
use std::ffi::CString;
use std::mem;
use std::ops::Range;
use std::slice;
use timestamp::TimeStamp;

/// Holds useful metadata extracted from a media resource during loading.
pub struct Metadata {
    /// Duration of the media in seconds, as described either by metadata
    /// in the container, or an estimate if no better information exists.
    pub duration: f64,
    /// Video image width and height in pixels, as described by metadata in container,
    /// if the container contains a video track. None otherwise.
    pub video_dimensions: Option<(i32, i32)>,
}

/// Holds pixel data and coordinates of a non-interleaved plane of data.
///
pub struct Plane {
    pub pixels: *const u8,
    /// The width of a line of pixels in bytes.
    pub width: i32,
    /// The stride of a line of pixels in bytes.
    pub stride: i32,
    /// The height of the plane in lines.
    pub height: i32,
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
#[derive(Clone)]
pub struct Region {
    // X coordinate of theExternalImage origin of the region.
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
    /// The sub-region of the buffer which contains the image to be rendered.
    pub picture: Region,
    /// The time at which this image should be renderd.
    pub time_stamp: TimeStamp,
    /// A stream-unique identifier.
    pub frame_id: u32,
    pub gecko_image: GeckoPlanarYCbCrImage,
}

// When cloning, we need to ensure we increment the reference count on
// the pixel data on the C++ side, so that our Drop impl can decrement
// the reference count appropriately.
impl Clone for PlanarYCbCrImage {
    fn clone(&self) -> Self {
        unsafe {
            (self.gecko_image.mAddRefPixelData.unwrap())(self.frame_id);
        }
        PlanarYCbCrImage {
            picture: self.picture.clone(),
            time_stamp: self.time_stamp.clone(),
            frame_id: self.frame_id,
            gecko_image: self.gecko_image,
        }
    }
}

impl PlanarYCbCrImage {
    /// Returns a slice storing the raw pixel data.
    pub fn pixel_data<'a>(&'a self, channel_index: u8) -> &'a [u8] {
        let img = &self.gecko_image;
        let (pixels, size) = match channel_index {
            0 => (
                self.get_pixels(PlaneType_Y),
                img.mYStride as usize * img.mYHeight as usize,
            ),
            1 => (
                self.get_pixels(PlaneType_Cb),
                img.mCbCrStride as usize * img.mCbCrHeight as usize,
            ),
            2 => (
                self.get_pixels(PlaneType_Cr),
                img.mCbCrStride as usize * img.mCbCrHeight as usize,
            ),
            _ => panic!("Invalid channel_index"),
        };
        unsafe { slice::from_raw_parts(pixels, size) }
    }

    fn get_pixels(&self, plane: u32) -> *const u8 {
        let img = &self.gecko_image;
        let f = img.mGetPixelData.unwrap();
        unsafe { f(img.mFrameID, plane) as *const u8 }
    }
    pub fn y_plane(&self) -> Plane {
        let img = &self.gecko_image;
        Plane {
            pixels: self.get_pixels(PlaneType_Y),
            width: img.mYWidth,
            stride: img.mYStride,
            height: img.mYHeight,
        }
    }

    pub fn cb_plane(&self) -> Plane {
        let img = &self.gecko_image;
        Plane {
            pixels: self.get_pixels(PlaneType_Cb),
            width: img.mCbCrWidth,
            stride: img.mCbCrStride,
            height: img.mCbCrHeight,
        }
    }

    pub fn cr_plane(&self) -> Plane {
        let img = &self.gecko_image;
        Plane {
            pixels: self.get_pixels(PlaneType_Cr),
            width: img.mCbCrWidth,
            stride: img.mCbCrStride,
            height: img.mCbCrHeight,
        }
    }
}

impl Drop for PlanarYCbCrImage {
    fn drop(&mut self) {
        let frame_id = self.gecko_image.mFrameID;
        unsafe {
            (self.gecko_image.mFreePixelData.unwrap())(frame_id);
        };
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
    fn update_current_images(&self, images: Vec<PlanarYCbCrImage>);
    /// Called when the Player buffered ranges were updated.
    fn buffered(&self, ranges: Vec<Range<f64>>);
    /// Called when the Player seekable ranges were updated.
    fn seekable(&self, ranges: Vec<Range<f64>>);
}

/// Plays a media resource.
def_gecko_media_struct!(Player);

impl_drop_gecko_media_struct!(Player, GeckoMedia_Player_Shutdown);

impl Player {
    pub fn new(
        gecko_media: GeckoMedia,
        id: usize,
        media_data: Vec<u8>,
        mime_type: &str,
        sink: Box<PlayerEventSink>,
    ) -> Result<Player, ()> {
        let callback = to_ffi_callback(sink);
        let media_data = to_ffi_vec(media_data);
        let mime_type = match CString::new(mime_type.as_bytes()) {
            Ok(mime_type) => mime_type,
            _ => return Err(()),
        };
        gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_CreateBlobPlayer(id, media_data, mime_type.as_ptr(), callback);
        });
        Ok(Player {
            gecko_media,
            id,
        })
    }

    pub fn from_network_resource(
        gecko_media: GeckoMedia,
        id: usize,
        resource: Box<NetworkResource>,
        mime_type: &str,
        sink: Box<PlayerEventSink>,
    ) -> Result<Player, ()> {
        let mime_type = match CString::new(mime_type.as_bytes()) {
            Ok(mime_type) => mime_type,
            _ => return Err(()),
        };
        let callback = to_ffi_callback(sink);
        let resource = to_ffi_resource(resource);
        gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_CreateNetworkPlayer(id, resource, mime_type.as_ptr(), callback);
        });
        Ok(Player {
            gecko_media,
            id,
        })
    }

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

pub trait NetworkResource {
    /// Attempts to fill buffer with bytes from offset. Reads as many
    /// bytes as possible, blocks if no bytes are available at offset,
    /// until bytes become available.
    fn read_at(&self, offset: u64, buffer: &mut [u8]) -> Result<u32, ()>;
    /// Directs the implementation to not evict any cached data until
    /// unpin() is called. The Player calls this during seeking to ensure
    /// the seek target doesn't disappear.
    fn pin(&self);
    /// Directs the implementation that it is now OK to evict cached data.
    /// Follows a pin() call.
    fn unpin(&self);
    /// Returns length of the stream, or None if the stream is unbounded or
    /// length is unknown.
    fn len(&self) -> Option<u64>;
    /// Reads data from a cached range.
    fn read_from_cache(&self, offset: u64, buffer: &mut [u8]) -> Result<(), ()>;
    /// Reports the byte ranges that are cached by the implementation.
    fn cached_ranges(&self) -> Vec<Range<u64>>;
}

fn to_ffi_callback(callbacks: Box<PlayerEventSink>) -> PlayerCallbackObject {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(PlayerEventSink);

    unsafe extern "C" fn decode_error(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.decode_error();
    }
    unsafe extern "C" fn playback_ended(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.playback_ended();
    }
    unsafe extern "C" fn async_event(ptr: *mut c_void, name: *const i8) {
        let wrapper = &*(ptr as *mut Wrapper);
        let c_str: &CStr = CStr::from_ptr(name);
        wrapper.callbacks.async_event(c_str.to_str().unwrap());
    }
    unsafe extern "C" fn metadata_loaded(ptr: *mut c_void, gecko_metadata: GeckoMediaMetadata) {
        let wrapper = &*(ptr as *mut Wrapper);
        let mut metadata = Metadata {
            duration: gecko_metadata.mDuration,
            video_dimensions: None,
        };
        if gecko_metadata.mVideoWidth != 0 && gecko_metadata.mVideoHeight != 0 {
            metadata.video_dimensions = Some((gecko_metadata.mVideoWidth, gecko_metadata.mVideoHeight));
        }
        wrapper.callbacks.metadata_loaded(metadata);
    }
    unsafe extern "C" fn duration_changed(ptr: *mut c_void, duration: f64) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.duration_changed(duration);
    }
    unsafe extern "C" fn loaded_data(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.loaded_data();
    }
    unsafe extern "C" fn seek_started(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.seek_started();
    }
    unsafe extern "C" fn seek_completed(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.seek_completed();
    }
    unsafe extern "C" fn time_update(ptr: *mut c_void, time: f64) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.time_update(time);
    }
    unsafe extern "C" fn update_current_images(ptr: *mut c_void, size: usize, elements: *mut GeckoPlanarYCbCrImage) {
        let wrapper = &*(ptr as *mut Wrapper);
        let images = to_ffi_planar_ycbycr_images(size, elements);
        wrapper.callbacks.update_current_images(images);
    }
    unsafe extern "C" fn notify_buffered(ptr: *mut c_void, size: usize, ranges: *mut GeckoMediaTimeInterval) {
        let wrapper = &*(ptr as *mut Wrapper);
        let ranges = to_ffi_time_ranges(size, ranges);
        wrapper.callbacks.buffered(ranges);
    }
    unsafe extern "C" fn notify_seekable(ptr: *mut c_void, size: usize, ranges: *mut GeckoMediaTimeInterval) {
        let wrapper = &*(ptr as *mut Wrapper);
        let ranges = to_ffi_time_ranges(size, ranges);
        wrapper.callbacks.seekable(ranges);
    }

    PlayerCallbackObject {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
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
        mNotifyBuffered: Some(notify_buffered),
        mNotifySeekable: Some(notify_seekable),
        mFree: Some(free),
    }
}

fn to_ffi_resource(callbacks: Box<NetworkResource>) -> NetworkResourceObject {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.
    def_gecko_callbacks_ffi_wrapper!(NetworkResource);

    unsafe extern "C" fn read_at(
        ptr: *mut c_void,
        offset: u64,
        bytes: *mut u8,
        len: u32,
        out_bytes_read: *mut u32,
    ) -> bool {
        let wrapper = &*(ptr as *mut Wrapper);
        let buffer = slice::from_raw_parts_mut(bytes, len as usize);
        match wrapper.callbacks.read_at(offset, buffer) {
            Ok(bytes_read) => {
                *out_bytes_read = bytes_read;
                true
            },
            Err(_) => false,
        }
    }
    unsafe extern "C" fn pin(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.pin();
    }
    unsafe extern "C" fn unpin(ptr: *mut c_void) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper.callbacks.unpin();
    }
    unsafe extern "C" fn length(ptr: *mut c_void) -> i64 {
        let wrapper = &*(ptr as *mut Wrapper);
        match wrapper.callbacks.len() {
            Some(len) if len < i64::max_value() as u64 => len as i64,
            _ => -1,
        }
    }
    unsafe extern "C" fn read_from_cache(ptr: *mut c_void, offset: u64, bytes: *mut u8, len: u32) -> bool {
        let wrapper = &*(ptr as *mut Wrapper);
        let buffer = slice::from_raw_parts_mut(bytes, len as usize);
        match wrapper.callbacks.read_from_cache(offset, buffer) {
            Ok(_) => true,
            Err(_) => false,
        }
    }
    unsafe extern "C" fn cached_ranges(ptr: *mut c_void, interval_set: *mut GeckoMediaByteIntervalSet) {
        let wrapper = &*(ptr as *mut Wrapper);
        for range in wrapper.callbacks.cached_ranges() {
            (*interval_set).mAdd.as_ref().map(|f| {
                f((*interval_set).mData, range.start, range.end)
            });
        }
    }

    NetworkResourceObject {
        mReadAt: Some(read_at),
        mPin: Some(pin),
        mUnPin: Some(unpin),
        mLength: Some(length),
        mReadFromCache: Some(read_from_cache),
        mCachedRanges: Some(cached_ranges),
        mFree: Some(free),
        mData: Box::into_raw(Box::new(Wrapper {
            callbacks,
        })) as *mut c_void,
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
    elements
        .iter()
        .map(|&img| -> PlanarYCbCrImage {
            PlanarYCbCrImage {
                picture: Region {
                    x: img.mPicX,
                    y: img.mPicY,
                    width: img.mPicWidth,
                    height: img.mPicHeight,
                },
                time_stamp: TimeStamp(img.mTimeStamp),
                frame_id: img.mFrameID,
                gecko_image: img,
            }
        })
        .collect::<Vec<PlanarYCbCrImage>>()
}

fn to_ffi_time_ranges(size: usize, elements: *mut GeckoMediaTimeInterval) -> Vec<Range<f64>> {
    let ranges = unsafe { slice::from_raw_parts(elements, size) };
    ranges
        .iter()
        .map(|&range| -> Range<f64> {
            Range {
                start: range.mStart,
                end: range.mEnd,
            }
        })
        .collect::<Vec<Range<f64>>>()
}
