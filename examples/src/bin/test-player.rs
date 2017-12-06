// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

extern crate futures;
extern crate gecko_media;
extern crate gleam;
extern crate glutin;
extern crate hyper;
extern crate rand;
extern crate time;
extern crate tokio_core;
extern crate webrender;

use gecko_media::{GeckoMedia, Metadata, PlayerEventSink};
use gecko_media::{PlanarYCbCrImage, Plane, Player, TimeStamp};
use mirror::{Canonical, Mirror};
use rand::{thread_rng, Rng};
use std::env;
use std::ffi::CString;
use std::fs::File;
use std::io::prelude::*;
use std::mem;
use std::ops::Range;
use std::path::Path;
use std::sync::{Mutex, mpsc};
use std::thread;
use ui::HandyDandyRectBuilder;
use webrender::api::*;
use webrender::api::ImageData::*;

#[path = "../ui.rs"]
mod ui;
#[path = "../network_resource.rs"]
mod network_resource;
#[path = "../mirror.rs"]
mod mirror;

use network_resource::DownloadController;

enum PlayerEvent {
    BreakOutOfEventLoop,
    Error,
    Ended,
    AsyncEvent(CString),
    MetadataLoaded(Metadata),
    StartPlayback,
    PausePlayback,
    Seek(f64),
    SeekableRanges(Vec<Range<f64>>),
    UpdateCurrentImages(Vec<PlanarYCbCrImage>),
}

struct PlayerWrapper {
    sender: mpsc::Sender<PlayerEvent>,
    shutdown_receiver: mpsc::Receiver<()>,
    ended_receiver: mpsc::Receiver<()>,
    already_ended: bool,
    current_time_mirror: Mirror<f64>,
    buffered_ranges_mirror: Mirror<Vec<Range<f64>>>,
    duration_mirror: Mirror<f64>,
    video_dimensions_mirror: Mirror<Option<(i32, i32)>>,
}

impl PlayerWrapper {
    pub fn new<F>(player_creator: F, frame_sender: mpsc::Sender<Vec<PlanarYCbCrImage>>) -> PlayerWrapper
    where
        F: FnOnce(Box<PlayerEventSink>) -> Player,
    {
        let (sender, receiver) = mpsc::channel();
        let (current_time_mirror, current_time_canonical) = Mirror::new(0.0);
        let (buffered_ranges_mirror, buffered_ranges_canonical) = Mirror::new(vec![]);
        let (duration_mirror, duration_canonical) = Mirror::new(0.0);
        let (video_dimensions_mirror, video_dimensions_canonical) = Mirror::new(None);
        struct Sink {
            sender: Mutex<mpsc::Sender<PlayerEvent>>,
            current_time_canonical: Canonical<f64>,
            buffered_ranges_canonical: Canonical<Vec<Range<f64>>>,
            duration_canonical: Canonical<f64>,
            video_dimensions_canonical: Canonical<Option<(i32, i32)>>,
        }
        impl PlayerEventSink for Sink {
            fn playback_ended(&self) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(PlayerEvent::Ended)
                    .unwrap();
            }
            fn decode_error(&self) {
                println!("Decode error!");
                self.sender
                    .lock()
                    .unwrap()
                    .send(PlayerEvent::Error)
                    .unwrap();
            }
            fn async_event(&self, name: &str) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(PlayerEvent::AsyncEvent(CString::new(name).unwrap()))
                    .unwrap();
            }
            fn metadata_loaded(&self, metadata: Metadata) {
                self.duration_canonical.set(metadata.duration);
                self.video_dimensions_canonical.set(
                    metadata.video_dimensions,
                );
                self.sender
                    .lock()
                    .unwrap()
                    .send(PlayerEvent::MetadataLoaded(metadata))
                    .unwrap();
            }
            fn duration_changed(&self, duration: f64) {
                self.duration_canonical.set(duration);
            }
            fn loaded_data(&self) {}
            fn time_update(&self, time: f64) {
                self.current_time_canonical.set(time);
            }
            fn seek_started(&self) {}
            fn seek_completed(&self) {}
            fn update_current_images(&self, images: Vec<PlanarYCbCrImage>) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(PlayerEvent::UpdateCurrentImages(images))
                    .unwrap();
            }
            fn buffered(&self, ranges: Vec<Range<f64>>) {
                self.buffered_ranges_canonical.set(ranges);
            }
            fn seekable(&self, ranges: Vec<Range<f64>>) {
                self.sender
                    .lock()
                    .unwrap()
                    .send(PlayerEvent::SeekableRanges(ranges))
                    .unwrap();
            }
        }

        let (shutdown_sender, shutdown_receiver) = mpsc::channel();
        let (ended_sender, ended_receiver) = mpsc::channel();
        let wrapper_sender = sender.clone();
        let sink = Box::new(Sink {
            sender: Mutex::new(sender),
            current_time_canonical,
            buffered_ranges_canonical,
            duration_canonical,
            video_dimensions_canonical,
        });
        let player = player_creator(sink);
        thread::spawn(move || {
            player.set_volume(1.0);
            let mut duration = 0.0;
            loop {
                match receiver.recv().unwrap() {
                    PlayerEvent::Ended => {
                        println!("Ended");
                        ended_sender.send(()).unwrap();
                        // Send empty batch of frames to awaken the event loop.
                        frame_sender.send(vec![]).unwrap();
                        break;
                    },
                    PlayerEvent::Error => {
                        println!("Error");
                        break;
                    },
                    PlayerEvent::AsyncEvent(name) => {
                        println!("Event received: {:?}", name);
                    },
                    PlayerEvent::MetadataLoaded(metadata) => {
                        println!("MetadataLoaded; duration: {}", metadata.duration);
                        duration = metadata.duration;
                    },
                    PlayerEvent::StartPlayback => {
                        player.play();
                    },
                    PlayerEvent::PausePlayback => {
                        player.pause();
                    },
                    PlayerEvent::UpdateCurrentImages(images) => {
                        frame_sender.send(images).unwrap();
                    },
                    PlayerEvent::BreakOutOfEventLoop => {
                        break;
                    },
                    PlayerEvent::SeekableRanges(ranges) => {
                        println!("Seekable ranges: {:?}", ranges);
                    },
                    PlayerEvent::Seek(position) => {
                        player.seek(position * duration);
                    },
                };
            }
            // This drop ensures that the Player object is destroyed before
            // we send the shutdown notification. The Rust application's main
            // thread blocks waiting for the confirmation that the Player is
            // shutdown before shutting down GeckoMedia. If we don't drop
            // here, the Player may not have shutdown when the main thread
            // tries to shutdown GeckoMedia, which will assert if there are
            // Player objects alive.
            drop(player);
            shutdown_sender.send(()).unwrap();
        });

        PlayerWrapper {
            sender: wrapper_sender,
            shutdown_receiver,
            ended_receiver,
            already_ended: false,
            current_time_mirror,
            buffered_ranges_mirror,
            duration_mirror,
            video_dimensions_mirror,
        }
    }
    pub fn shutdown(&self) {
        self.sender.send(PlayerEvent::BreakOutOfEventLoop).ok();
        // Block until shutdown is complete. This ensures the Player object
        // and its reference to the GeckoMedia object have been dropped by the
        // time our main function calls GeckoMedia::Shutdown().
        self.shutdown_receiver.recv().unwrap();
    }
    pub fn playback_ended(&mut self) -> bool {
        if let Ok(_) = self.ended_receiver.try_recv() {
            self.already_ended = true;
        }
        self.already_ended
    }
    pub fn play(&self) {
        self.sender.send(PlayerEvent::StartPlayback).unwrap();
    }
    pub fn pause(&self) {
        self.sender.send(PlayerEvent::PausePlayback).unwrap();
    }
    pub fn video_dimensions(&self) -> Option<(i32, i32)> {
        self.video_dimensions_mirror.get()
    }
    pub fn seek(&self, position: f64) {
        self.sender.send(PlayerEvent::Seek(position)).unwrap();
    }
    pub fn current_time(&self) -> f64 {
        self.current_time_mirror.get()
    }
    pub fn buffered_ranges(&self) -> Vec<Range<f64>> {
        self.buffered_ranges_mirror.get()
    }
    pub fn duration(&self) -> f64 {
        self.duration_mirror.get()
    }
}

struct ImageGenerator {
    current_frame_receiver: mpsc::Receiver<PlanarYCbCrImage>,
    current_image: Option<PlanarYCbCrImage>,
}

impl webrender::ExternalImageHandler for ImageGenerator {
    fn lock(&mut self, _key: ExternalImageId, channel_index: u8) -> webrender::ExternalImage {
        if let Ok(v) = self.current_frame_receiver.try_recv() {
            self.current_image = Some(v);
        };
        assert!(self.current_image.is_some());
        webrender::ExternalImage {
            u0: 0.0,
            v0: 0.0,
            u1: 1.0,
            v1: 1.0,
            source: webrender::ExternalImageSource::RawData(
                self.current_image
                    .as_ref()
                    .map(|p| p.pixel_data(channel_index))
                    .unwrap(),
            ),
        }
    }
    fn unlock(&mut self, _key: ExternalImageId, _channel_index: u8) {}
}

const EXTERNAL_VIDEO_IMAGE_ID: ExternalImageId = ExternalImageId(1);

struct App {
    frame_receiver: mpsc::Receiver<Vec<PlanarYCbCrImage>>,
    current_frame_sender: Option<mpsc::Sender<PlanarYCbCrImage>>,
    frame_queue: Vec<PlanarYCbCrImage>,
    y_channel_key: Option<ImageKey>,
    cb_channel_key: Option<ImageKey>,
    cr_channel_key: Option<ImageKey>,
    last_frame_id: u32,
    player_wrapper: PlayerWrapper,
    playing: bool,
}

impl App {
    fn new<F>(player_creator: F) -> App
    where
        F: FnOnce(Box<PlayerEventSink>) -> Player,
    {
        // Channel for frames to pass between Gecko and player.
        let (frame_sender, frame_receiver) = mpsc::channel();
        App {
            frame_receiver: frame_receiver,
            frame_queue: vec![],
            current_frame_sender: None,
            y_channel_key: None,
            cb_channel_key: None,
            cr_channel_key: None,
            last_frame_id: 0,
            player_wrapper: PlayerWrapper::new(player_creator, frame_sender),
            playing: false,
        }
    }
    fn shutdown(&self) {
        self.player_wrapper.shutdown();
    }
    fn create_or_update_planar_image(
        api: &RenderApi,
        resources: &mut ResourceUpdates,
        image_key: Option<ImageKey>,
        plane: Plane,
        channel_index: u8,
    ) -> Option<ImageKey> {
        match image_key {
            Some(image_key) => {
                resources.update_image(
                    image_key,
                    ImageDescriptor {
                        format: ImageFormat::A8,
                        width: plane.width as u32,
                        height: plane.height as u32,
                        stride: Some(plane.stride as u32),
                        offset: 0,
                        is_opaque: true,
                    },
                    External(ExternalImageData {
                        id: EXTERNAL_VIDEO_IMAGE_ID,
                        channel_index,
                        image_type: ExternalImageType::ExternalBuffer,
                    }),
                    None,
                );
                Some(image_key)
            },
            None => {
                let image_key = api.generate_image_key();
                resources.add_image(
                    image_key,
                    ImageDescriptor {
                        format: ImageFormat::A8,
                        width: plane.width as u32,
                        height: plane.height as u32,
                        stride: Some(plane.stride as u32),
                        offset: 0,
                        is_opaque: true,
                    },
                    External(ExternalImageData {
                        id: EXTERNAL_VIDEO_IMAGE_ID,
                        channel_index,
                        image_type: ExternalImageType::ExternalBuffer,
                    }),
                    None,
                );
                Some(image_key)
            },
        }
    }

    fn draw_progess_bar(&mut self, builder: &mut DisplayListBuilder, layout_size: &LayoutSize) {
        let duration = self.player_wrapper.duration() as f32;
        if duration <= 0.0 || !duration.is_finite() {
            return;
        }

        let border_thickness: f32 = 4.0;
        let scrubber_thickness: i32 = 40;
        let margin: i32 = 10;
        let padding: f32 = 10.0;
        let width: i32 = layout_size.width as i32;
        let height: i32 = layout_size.height as i32;
        let background = LayoutPrimitiveInfo::new((margin, height - (margin + scrubber_thickness)).to(
            width -
                margin,
            height -
                margin,
        ));

        let opacity = 1.0;
        builder.push_rect(&background, ColorF::new(0.0, 0.0, 0.0, opacity));

        let border_side = BorderSide {
            color: ColorF::new(0.7, 0.7, 0.7, opacity),
            style: BorderStyle::Solid,
        };
        let border_widths = BorderWidths {
            top: border_thickness,
            left: border_thickness,
            bottom: border_thickness,
            right: border_thickness,
        };
        let border_details = BorderDetails::Normal(NormalBorder {
            top: border_side,
            right: border_side,
            bottom: border_side,
            left: border_side,
            radius: BorderRadius::zero(),
        });

        builder.push_border(&background, border_widths, border_details);

        let time = self.player_wrapper.current_time() as f32;
        let rect = background.rect;
        let proportion = time / duration;
        let x = rect.min_x() + padding;
        let y = rect.min_y() + padding;
        let width = rect.max_x() - rect.min_x() - 2.0 * padding;
        let height = rect.max_y() - rect.min_y() - 2.0 * padding;

        for range in self.player_wrapper.buffered_ranges().into_iter().map(|r| {
            Range {
                start: r.start as f32 / duration,
                end: r.end as f32 / duration,
            }
        })
        {
            let rect = LayoutPrimitiveInfo::new(((x + width * range.start) as i32, y as i32).to(
                (x + width * range.end) as
                    i32,
                (y + height) as
                    i32,
            ));
            builder.push_rect(&rect, ColorF::new(0.4, 0.4, 0.4, opacity));
        }

        let cursor_width = 5.0;
        let x = x + proportion * (width - cursor_width);
        let cursor = LayoutPrimitiveInfo::new((x as i32, y as i32).to(
            (x + cursor_width) as i32,
            (y + height) as i32,
        ));
        builder.push_rect(&cursor, ColorF::new(0.0, 0.0, 1.0, opacity));

    }
}

impl ui::Example for App {
    fn needs_repaint(&mut self) -> bool {
        if let Ok(v) = self.frame_receiver.try_recv() {
            self.frame_queue = v;
        }

        let time_now = TimeStamp(time::precise_time_ns());
        while self.frame_queue.len() > 1 && self.frame_queue[0].time_stamp > time_now {
            self.frame_queue.remove(0);
        }

        if let Some(first_frame) = self.frame_queue.first() {
            if self.last_frame_id != first_frame.frame_id {
                self.last_frame_id = first_frame.frame_id;
                self.current_frame_sender.as_ref().map(|sender| {
                    sender.send(first_frame.clone()).unwrap();
                });
                return true;
            }
        }

        false
    }

    fn should_close_window(&mut self) -> bool {
        self.player_wrapper.playback_ended()
    }

    fn video_dimensions(&self) -> Option<(i32, i32)> {
        self.player_wrapper.video_dimensions()
    }

    fn render(
        &mut self,
        api: &RenderApi,
        builder: &mut DisplayListBuilder,
        resources: &mut ResourceUpdates,
        layout_size: LayoutSize,
        _pipeline_id: PipelineId,
        _document_id: DocumentId,
    ) {
        let bounds = LayoutRect::new(LayoutPoint::zero(), layout_size);
        let info = LayoutPrimitiveInfo::new(bounds);
        builder.push_stacking_context(
            &info,
            ScrollPolicy::Scrollable,
            None,
            TransformStyle::Flat,
            None,
            MixBlendMode::Normal,
            Vec::new(),
        );

        if !self.frame_queue.is_empty() {
            let frame = &self.frame_queue[0];
            self.y_channel_key =
                App::create_or_update_planar_image(api, resources, self.y_channel_key, frame.y_plane(), 0);
            self.cb_channel_key =
                App::create_or_update_planar_image(api, resources, self.cb_channel_key, frame.cb_plane(), 1);
            self.cr_channel_key =
                App::create_or_update_planar_image(api, resources, self.cr_channel_key, frame.cr_plane(), 2);

            // We'll render the video at the intrinsic size, i.e. the size
            // at which the video frame is supposed to be rendered at after
            // scaling to respect the Pixel Aspect Ratio. If we don't know the
            // intrinsic size, we'll just use the frame size.
            let (width, height) = match self.video_dimensions() {
                Some((width, height)) => (width, height),
                None => (frame.picture.width, frame.picture.height),
            };

            // Resize so that the video is rendered as wide as the window.
            let aspect_ratio = width as f32 / height as f32;
            let mut render_size = LayoutSize::new(
                layout_size.width as f32,
                layout_size.width as f32 / aspect_ratio,
            );
            // If the above resize results in the video being taller than the
            // window, shrink so that the height matches the window. The width
            // should then be less than the window width.
            if render_size.height > layout_size.height {
                let aspect_ratio = render_size.height as f32 / render_size.width as f32;
                render_size = LayoutSize::new(
                    layout_size.height / aspect_ratio as f32,
                    layout_size.height as f32,
                );
            }
            // Render the image centered in the window.
            let render_location = LayoutPoint::new(
                (layout_size.width - render_size.width) / 2.0,
                (layout_size.height - render_size.height) / 2.0,
            );
            let info = LayoutPrimitiveInfo::with_clip_rect(LayoutRect::new(render_location, render_size), bounds);
            builder.push_yuv_image(
                &info,
                YuvData::PlanarYCbCr(
                    self.y_channel_key.unwrap(),
                    self.cb_channel_key.unwrap(),
                    self.cr_channel_key.unwrap(),
                ),
                YuvColorSpace::Rec601,
                ImageRendering::Auto,
            );
        }

        self.draw_progess_bar(builder, &layout_size);

        builder.pop_stacking_context();
    }

    fn on_event(&mut self, event: glutin::Event, _api: &RenderApi, _document_id: DocumentId) -> bool {
        match event {
            glutin::Event::KeyboardInput(glutin::ElementState::Pressed, _, Some(glutin::VirtualKeyCode::S)) => {
                let pos = thread_rng().next_f64();
                self.player_wrapper.seek(pos);
            },
            glutin::Event::KeyboardInput(glutin::ElementState::Pressed, _, Some(glutin::VirtualKeyCode::Space)) => {
                self.playing = !self.playing;
                if self.playing {
                    self.player_wrapper.play();
                } else {
                    self.player_wrapper.pause();
                }
            },
            _ => {},
        }
        true
    }

    fn get_external_image_handler(&mut self) -> Option<Box<webrender::ExternalImageHandler>> {
        // Channel for the current frame to be sent between the main
        // render loop and the external image handler.
        let (current_frame_sender, current_frame_receiver) = mpsc::channel();
        self.current_frame_sender = Some(current_frame_sender);
        Some(Box::new(ImageGenerator {
            current_frame_receiver,
            current_image: None,
        }))
    }

    fn init(&mut self, window_proxy: glutin::WindowProxy) {
        // Install a relay between the channel that receives new frames
        // and notify the event loop to wake it up after new frames come
        // in so they're rendered.
        let (sender, receiver) = mpsc::channel();
        let wrapped_receiver = mem::replace(&mut self.frame_receiver, receiver);
        thread::spawn(move || loop {
            match wrapped_receiver.recv() {
                Ok(frames) => {
                    if let Err(_) = sender.send(frames) {
                        break;
                    }
                    window_proxy.wakeup_event_loop();
                },
                Err(_) => {
                    break;
                },
            }
        });
        // Now that the UI is showing we can render video frames.
        // So start playback.
        self.playing = true;
        self.player_wrapper.play();
    }
}

fn main() {
    let args: Vec<_> = env::args().collect();
    let filename: &str = if args.len() == 2 {
        args[1].as_ref()
    } else {
        panic!("Usage: test-player file_path|URL")
    };

    let path = Path::new(filename);
    let mime = match path.extension().unwrap().to_str() {
        Some("wav") => "audio/wav",
        Some("mp3") => "audio/mp3",
        Some("flac") => "audio/flac",
        Some("ogg") => "audio/ogg; codecs=\"vorbis\"",
        Some("m4a") => "audio/mp4",
        Some("mp4") => "video/mp4",
        Some("webm") => "video/webm",
        _ => "",
    };
    assert!(
        mime != "",
        "Unknown file type. Audio codecs currently supported: wav, mp3, m4a, flac and vorbis.\
                Video codecs supported: h264, vp8 and vp9."
    );

    let mut app = if filename.starts_with("http://") {
        App::new(|sink: Box<PlayerEventSink>| {
            let downloader = Box::new(DownloadController::new(filename));
            GeckoMedia::get()
                .unwrap()
                .create_network_player(downloader, mime, sink)
                .unwrap()
        })
    } else {
        App::new(|sink: Box<PlayerEventSink>| {
            let mut file = File::open(filename).unwrap();
            let mut bytes = vec![];
            file.read_to_end(&mut bytes).unwrap();
            GeckoMedia::get()
                .unwrap()
                .create_blob_player(bytes, mime, sink)
                .unwrap()
        })
    };
    ui::main_wrapper(&mut app, None);
    app.shutdown();
    GeckoMedia::shutdown().unwrap();
}
