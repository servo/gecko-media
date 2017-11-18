// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use GeckoMedia;
use bindings::GeckoPlanarYCbCrImage;
use bindings::{GeckoMedia_Player_Pause, GeckoMedia_Player_Play};
use bindings::{GeckoMedia_Player_Seek, GeckoMedia_Player_SetVolume, GeckoMedia_Player_Shutdown, PlaneType_Cb, PlaneType_Cr, PlaneType_Y};
use std::ops::Range;
use std::slice;
use timestamp::TimeStamp;

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
    pub pixels: *const u8,
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
    // pub y_plane: Plane,
    // pub cb_plane: Plane,
    // pub cr_plane: Plane,
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
            0 => (self.get_pixels(PlaneType_Y), img.mYStride as usize * img.mYHeight as usize),
            1 => (self.get_pixels(PlaneType_Cb), img.mCbCrStride as usize * img.mCbCrHeight as usize),
            2 => (self.get_pixels(PlaneType_Cr), img.mCbCrStride as usize * img.mCbCrHeight as usize),
             _ => panic!("Invalid channel_index"),
        };
        unsafe {
            slice::from_raw_parts(pixels, size)
        }
    }

    fn get_pixels(&self, plane: u32) -> *const u8 {
        let img = &self.gecko_image;
        let f = img.mGetPixelData.unwrap();
        unsafe {
            f(img.mFrameID, plane) as *const u8
        }
    }

    pub fn y_plane(&self) -> Plane {
        let img = &self.gecko_image;
        Plane {
            pixels: self.get_pixels(PlaneType_Y),
            width: img.mYWidth,
            stride: img.mYStride,
            height: img.mYHeight,
            skip: img.mYSkip,
        }
    }

    pub fn cb_plane(&self) -> Plane {
        let img = &self.gecko_image;
        Plane {
            pixels: self.get_pixels(PlaneType_Cb),
            width: img.mCbCrWidth,
            stride: img.mCbCrStride,
            height: img.mCbCrHeight,
            skip: img.mCbSkip,
        }
    }

    pub fn cr_plane(&self) -> Plane {
        let img = &self.gecko_image;
        Plane {
            pixels: self.get_pixels(PlaneType_Cr),
            width: img.mCbCrWidth,
            stride: img.mCbCrStride,
            height: img.mCbCrHeight,
            skip: img.mCrSkip,
        }
    }
}

impl Drop for PlanarYCbCrImage {
    fn drop(&mut self) {
        let frame_id = self.gecko_image.mFrameID;
        unsafe { (self.gecko_image.mFreePixelData.unwrap())(frame_id); };
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

impl Player {
    pub fn new(gecko_media: GeckoMedia, id: usize) -> Player {
        Player {
            gecko_media,
            id,
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
