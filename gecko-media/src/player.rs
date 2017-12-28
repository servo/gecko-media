// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use bindings::GeckoMedia_Player_CreateMediaSourcePlayer;
use bindings::NetworkResourceObject;
use bindings::{CachedRangesObserverObject, FrameAllocatorObject, GeckoImagePlane};
use bindings::{GeckoMediaByteRange, GeckoPlanarYCbCrImage, GeckoPlanarYCbCrImageData};
use bindings::{GeckoMediaMetadata, GeckoMediaTimeInterval};
use bindings::{GeckoMedia_Player_CreateBlobPlayer, GeckoMedia_Player_CreateNetworkPlayer};
use bindings::{GeckoMedia_Player_Pause, GeckoMedia_Player_Play};
use bindings::{GeckoMedia_Player_Seek, GeckoMedia_Player_SetPlaybackRate, GeckoMedia_Player_SetVolume};
use bindings::{PlayerCallbackObject, RustVecU8Object};
use std::ffi::CStr;
use std::ffi::CString;
use std::mem;
use std::ops::Range;
use std::os::raw::c_char;
use std::slice;
use std::sync::{mpsc, Arc, Mutex};
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

/// Holds pixel data and geometry of a non-interleaved plane of data.
pub struct Plane {
    /// Pixel sample data. One byte per sample. Will contain stride*height byte.
    pub pixels: Vec<u8>,
    /// The width of a line of pixels in bytes.
    pub width: usize,
    /// The stride of a line of pixels in bytes.
    pub stride: usize,
    /// The height of the plane in lines.
    pub height: usize,
}

struct PlanarYCbCrImageData {
    pub y_plane: Plane,
    pub cb_plane: Plane,
    pub cr_plane: Plane,
    pub picture: Region,
}

/// A subregion of an image buffer.
#[derive(Clone)]
pub struct Region {
    /// X coordinate of the origin of the region.
    pub x: usize,
    /// Y coordinate of the origin of the region.
    pub y: usize,
    /// Width of region.
    pub width: usize,
    /// Height of the region.
    pub height: usize,
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
    timestamp: TimeStamp,
    frame_id: u32,
    image_data: Arc<PlanarYCbCrImageData>,
}

// When cloning, we need to ensure we increment the reference count on
// the pixel data on the C++ side, so that our Drop impl can decrement
// the reference count appropriately.
impl Clone for PlanarYCbCrImage {
    fn clone(&self) -> Self {
        PlanarYCbCrImage {
            timestamp: self.timestamp.clone(),
            frame_id: self.frame_id,
            image_data: self.image_data.clone(),
        }
    }
}

impl PlanarYCbCrImage {
    /// Returns the time at which this image should be rendered.
    pub fn timestamp(&self) -> TimeStamp {
        self.timestamp.clone()
    }
    /// Returns the image identifier, which is a stream-unique identifier
    /// for this frame. This can be used to distinguish images.
    pub fn frame_id(&self) -> u32 {
        self.frame_id
    }
    /// Returns the sub-region of the buffer which contains the image to
    /// be rendered.
    pub fn picture_region(&self) -> &Region {
        &self.image_data.picture
    }
    /// Returns the image plane for the image channels.
    /// Pass 0 for Y, 1 for Cb, 2 for Cr. Other values panic.
    pub fn plane_for_channel(&self, channel_index: u8) -> &Plane {
        match channel_index {
            0 => self.y_plane(),
            1 => self.cb_plane(),
            2 => self.cr_plane(),
            _ => panic!("Invalid planar index"),
        }
    }
    /// Returns the Y plane data.
    pub fn y_plane(&self) -> &Plane {
        &self.image_data.y_plane
    }
    /// Returns the Cb plane data.
    pub fn cb_plane(&self) -> &Plane {
        &self.image_data.cb_plane
    }
    /// Returns the Cr plane data.
    pub fn cr_plane(&self) -> &Plane {
        &self.image_data.cr_plane
    }
}

/// Users of Player pass in an implementation of this trait when creating
/// Player objects. When events happen in the Player, users will receive
/// callbacks upon the trait implementation, notifying them of the event.
pub trait PlayerEventSink: Send + Sync {
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

struct SharedVideoFrame {
    id: usize,
    frame: Arc<PlanarYCbCrImageData>,
}

// Allocates video frames in Rust code that can be referenced via ID
// in C++ code.
struct VideoFrameAllocator {
    allocated_frames: Vec<SharedVideoFrame>,
    next_frame_id: usize,
}

impl VideoFrameAllocator {
    pub fn new() -> VideoFrameAllocator {
        VideoFrameAllocator {
            allocated_frames: vec![],
            next_frame_id: 1,
        }
    }

    unsafe fn to_plane(plane: &GeckoImagePlane) -> Plane {
        assert!((*plane).mWidth > 0);
        assert!((*plane).mHeight > 0);
        assert!((*plane).mStride > 0);
        let size = (*plane).mStride as usize * (*plane).mHeight as usize;
        let src = slice::from_raw_parts((*plane).mPixelData, size);
        Plane {
            pixels: src.to_vec(),
            width: (*plane).mWidth as usize,
            stride: (*plane).mStride as usize,
            height: (*plane).mHeight as usize,
        }
    }

    pub fn allocate_and_copy_image(&mut self, image: *const GeckoPlanarYCbCrImageData) -> usize {
        let id = self.next_frame_id;
        self.next_frame_id += 1;

        let frame = unsafe {
            assert!((*image).mPicWidth > 0);
            assert!((*image).mPicHeight > 0);
            SharedVideoFrame {
                id,
                frame: Arc::new(PlanarYCbCrImageData {
                    y_plane: Self::to_plane(&(*image).mYPlane),
                    cb_plane: Self::to_plane(&(*image).mCbPlane),
                    cr_plane: Self::to_plane(&(*image).mCrPlane),
                    picture: Region {
                        x: (*image).mPicX as usize,
                        y: (*image).mPicY as usize,
                        width: (*image).mPicWidth as usize,
                        height: (*image).mPicHeight as usize,
                    },
                }),
            }
        };
        self.allocated_frames.push(frame);
        id
    }

    fn frame_position(&self, id: usize) -> Option<usize> {
        self.allocated_frames.iter().position(|p| p.id == id)
    }

    pub fn drop_frame(&mut self, id: usize) {
        if let Some(index) = self.frame_position(id) {
            self.allocated_frames.remove(index);
        }
    }

    pub fn get_frame(&self, id: usize) -> Option<Arc<PlanarYCbCrImageData>> {
        match self.frame_position(id) {
            Some(index) => Some(self.allocated_frames[index].frame.clone()),
            None => None,
        }
    }
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
        let video_frame_allocator = Arc::new(Mutex::new(VideoFrameAllocator::new()));
        let callback = to_ffi_callback(sink, video_frame_allocator.clone());
        let media_data = to_ffi_vec(media_data);
        let mime_type = match CString::new(mime_type.as_bytes()) {
            Ok(mime_type) => mime_type,
            _ => return Err(()),
        };
        let ffi_frame_allocator = to_ffi_frame_allocator(video_frame_allocator.clone());
        let player = Player { gecko_media, id };
        player.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_CreateBlobPlayer(
                id,
                media_data,
                mime_type.as_ptr(),
                callback,
                ffi_frame_allocator,
            );
        });
        Ok(player)
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
        let video_frame_allocator = Arc::new(Mutex::new(VideoFrameAllocator::new()));
        let callback = to_ffi_callback(sink, video_frame_allocator.clone());
        let ffi_frame_allocator = to_ffi_frame_allocator(video_frame_allocator.clone());
        let resource = to_ffi_resource(resource);
        let player = Player { gecko_media, id };
        player.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_CreateNetworkPlayer(
                id,
                resource,
                mime_type.as_ptr(),
                callback,
                ffi_frame_allocator,
            );
        });
        Ok(player)
    }

    pub fn from_media_source(
        gecko_media: GeckoMedia,
        id: usize,
        media_source_id: usize,
        sink: Box<PlayerEventSink>,
    ) -> Result<Player, ()> {
        let video_frame_allocator = Arc::new(Mutex::new(VideoFrameAllocator::new()));
        let callback = to_ffi_callback(sink, video_frame_allocator.clone());
        let ffi_frame_allocator = to_ffi_frame_allocator(video_frame_allocator.clone());
        let player = Player { gecko_media, id };
        let (sender, receiver) = mpsc::channel();
        player.gecko_media.queue_task(move || unsafe {
            sender
                .send(GeckoMedia_Player_CreateMediaSourcePlayer(
                    id,
                    media_source_id,
                    callback,
                    ffi_frame_allocator,
                ))
                .unwrap();
        });
        if receiver.recv().unwrap() {
            Ok(player)
        } else {
            Err(())
        }
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
    /// Changes the volume. Volume is in the range [0, 1.0].
    pub fn set_volume(&self, volume: f64) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_SetVolume(player_id, volume);
        });
    }

    /// Changes the playback rate. 1.0 is "normal speed," values lower than 1.0
    /// make the media play slower than normal, higher values make it play
    /// faster.
    pub fn set_playback_rate(&self, rate: f64) {
        let player_id = self.id;
        self.gecko_media.queue_task(move || unsafe {
            GeckoMedia_Player_SetPlaybackRate(player_id, rate);
        });
    }
}

/// Passed to NetworkResource shortly after startup. NetworkResource uses
/// this object to inform the Player when the byte ranges of the resource
/// that are cached has changed.
pub struct CachedRangesSink {
    observer: CachedRangesObserverObject,
}

impl CachedRangesSink {
    /// Whenever the byte ranges that are cached changes, for example if more
    /// data is downloaded, or if data is evicted from the cache, the
    /// NetworkResource implementation should call update() to inform the
    /// Player of the change. The Player will then update the buffered
    /// ranges, and internal state that reflects the cached ranges.
    pub fn update(&self, ranges: &[Range<u64>]) {
        let mut data = vec![];
        for ref r in ranges.iter() {
            data.push(GeckoMediaByteRange {
                mStart: r.start,
                mEnd: r.end,
            });
        }
        unsafe {
            self.observer
                .mUpdate
                .as_ref()
                .map(|f| f(self.observer.mResourceID, data.as_ptr(), data.len()));
        }
    }
}

pub trait NetworkResource: Send + Sync {
    /// Sets the sender to which the implementation should send updates
    /// whenever the cached ranges change. Whenever new data is added to
    /// or removed from the cache, the NetworkResource should send a
    /// message informing GeckcoMedia of the new cached ranges.
    fn set_cached_ranges_sink(&self, sink: CachedRangesSink);
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
}

fn to_ffi_callback(
    callbacks: Box<PlayerEventSink>,
    video_frame_allocator: Arc<Mutex<VideoFrameAllocator>>,
) -> PlayerCallbackObject {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.
    use std::os::raw::c_void;
    struct Wrapper {
        callbacks: Box<PlayerEventSink>,
        video_frame_allocator: Arc<Mutex<VideoFrameAllocator>>,
    }
    unsafe extern "C" fn free(ptr: *mut c_void) {
        drop(Box::from_raw(ptr as *mut Wrapper));
    }

    impl_simple_ffi_callback_wrapper!(decode_error);
    impl_simple_ffi_callback_wrapper!(playback_ended);
    impl_simple_ffi_callback_wrapper!(loaded_data);
    impl_simple_ffi_callback_wrapper!(seek_started);
    impl_simple_ffi_callback_wrapper!(seek_completed);

    impl_simple_ffi_setter_wrapper!(duration_changed, f64);
    impl_simple_ffi_setter_wrapper!(time_update, f64);

    unsafe extern "C" fn async_event(ptr: *mut c_void, name: *const c_char) {
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
    unsafe extern "C" fn update_current_images(ptr: *mut c_void, size: usize, elements: *mut GeckoPlanarYCbCrImage) {
        let wrapper = &*(ptr as *mut Wrapper);
        let images = to_ffi_planar_ycbycr_images(size, elements, &wrapper.video_frame_allocator);
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
            video_frame_allocator,
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

fn to_ffi_frame_allocator(video_frame_allocator: Arc<Mutex<VideoFrameAllocator>>) -> FrameAllocatorObject {
    def_gecko_callbacks_ffi_wrapper!(Arc<Mutex<VideoFrameAllocator>>);

    unsafe extern "C" fn drop_frame(ptr: *mut c_void, id: usize) {
        let wrapper = &*(ptr as *mut Wrapper);
        let mut allocator = wrapper.callbacks.lock().unwrap();
        allocator.drop_frame(id)
    }

    unsafe extern "C" fn allocate_and_copy_image(ptr: *mut c_void, image: *const GeckoPlanarYCbCrImageData) -> usize {
        let wrapper = &*(ptr as *mut Wrapper);
        let mut allocator = wrapper.callbacks.lock().unwrap();
        allocator.allocate_and_copy_image(image)
    }

    FrameAllocatorObject {
        mContext: Box::into_raw(Box::new(Wrapper {
            callbacks: video_frame_allocator,
        })) as *mut c_void,
        mAllocateFrame: Some(allocate_and_copy_image),
        mDropFrame: Some(drop_frame),
        mFree: Some(free),
    }
}

fn to_ffi_resource(callbacks: Box<NetworkResource>) -> NetworkResourceObject {
    // Can't cast from *c_void to a Trait, so wrap in a concrete type
    // when we pass into C++ code.

    def_gecko_callbacks_ffi_wrapper!(Box<NetworkResource>);

    unsafe extern "C" fn set_ranges_observer(ptr: *mut c_void, observer: CachedRangesObserverObject) {
        let wrapper = &*(ptr as *mut Wrapper);
        wrapper
            .callbacks
            .set_cached_ranges_sink(CachedRangesSink { observer })
    }

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
    NetworkResourceObject {
        mSetRangesObserver: Some(set_ranges_observer),
        mReadAt: Some(read_at),
        mPin: Some(pin),
        mUnPin: Some(unpin),
        mLength: Some(length),
        mReadFromCache: Some(read_from_cache),
        mFree: Some(free),
        mData: Box::into_raw(Box::new(Wrapper { callbacks })) as *mut c_void,
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

fn to_ffi_planar_ycbycr_images(
    size: usize,
    elements: *mut GeckoPlanarYCbCrImage,
    video_frame_allocator: &Arc<Mutex<VideoFrameAllocator>>,
) -> Vec<PlanarYCbCrImage> {
    let elements = unsafe { slice::from_raw_parts(elements, size) };
    let mut frames = vec![];
    let video_frame_allocator = video_frame_allocator.lock().unwrap();
    for &img in elements {
        let image_data = match video_frame_allocator.get_frame(img.mImageHandle) {
            Some(image_data) => image_data,
            None => continue,
        };
        frames.push(PlanarYCbCrImage {
            timestamp: TimeStamp(img.mTimeStamp),
            frame_id: img.mFrameID,
            image_data,
        });
    }
    frames
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
