// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

extern crate gecko_media;

use gecko_media::{GeckoMedia, Metadata, PlayerEventSink};
use gecko_media::{PlanarYCbCrImage, Plane, TimeStamp};
use std::env;
use std::ffi::CString;
use std::fs::File;
use std::io::prelude::*;
use std::mem;
use std::ops::Range;
use std::path::Path;
use std::sync::mpsc;
use std::thread;

extern crate gleam;
extern crate glutin;
extern crate time;
extern crate webrender;

use webrender::api::*;
use webrender::api::ImageData::*;

mod ui;

static mut CLOSE_WINDOW: bool = false;

enum PlayerEvent {
    BreakOutOfEventLoop,
    Error,
    Ended,
    AsyncEvent(CString),
    MetadataLoaded(Metadata),
    BufferedRanges(Vec<Range<f64>>),
    SeekableRanges(Vec<Range<f64>>),
    UpdateCurrentImages(Vec<PlanarYCbCrImage>),
}

struct PlayerWrapper {
    sender: mpsc::Sender<PlayerEvent>,
    shutdown_receiver: mpsc::Receiver<()>,
}

impl PlayerWrapper {
    pub fn new(
        bytes: Vec<u8>,
        mime: &'static str,
        frame_sender: mpsc::Sender<Vec<PlanarYCbCrImage>>,
    ) -> PlayerWrapper {
        let (sender, receiver) = mpsc::channel();

        #[derive(Clone)]
        struct Sink {
            sender: mpsc::Sender<PlayerEvent>,
            frame_sender: mpsc::Sender<Vec<PlanarYCbCrImage>>,
        }
        impl PlayerEventSink for Sink {
            fn playback_ended(&self) {
                self.sender.send(PlayerEvent::Ended).unwrap();
            }
            fn decode_error(&self) {
                self.sender.send(PlayerEvent::Error).unwrap();
            }
            fn async_event(&self, name: &str) {
                self.sender
                    .send(PlayerEvent::AsyncEvent(CString::new(name).unwrap()))
                    .unwrap();
            }
            fn metadata_loaded(&self, metadata: Metadata) {
                self.sender
                    .send(PlayerEvent::MetadataLoaded(metadata))
                    .unwrap();
            }
            fn duration_changed(&self, _duration: f64) {}
            fn loaded_data(&self) {}
            fn time_update(&self, _time: f64) {}
            fn seek_started(&self) {}
            fn seek_completed(&self) {}
            fn update_current_images(&self, images: Vec<PlanarYCbCrImage>) {
                self.sender.send(PlayerEvent::UpdateCurrentImages(images)).unwrap();
            }
            fn buffered(&self, ranges: Vec<Range<f64>>) {
                self.sender.send(PlayerEvent::BufferedRanges(ranges)).unwrap();
            }
            fn seekable(&self, ranges: Vec<Range<f64>>) {
                self.sender.send(PlayerEvent::SeekableRanges(ranges)).unwrap();
            }
        }

        let (shutdown_sender, shutdown_receiver) = mpsc::channel();
        let wrapper_sender = sender.clone();
        thread::spawn(move || {
            let sink = Box::new(Sink {
                sender: sender,
                frame_sender: frame_sender,
            });
            let sink_clone = sink.clone();
            let player = GeckoMedia::get()
                .unwrap()
                .create_blob_player(bytes, mime, sink_clone)
                .unwrap();
            player.play();
            player.set_volume(1.0);
            let mut metadata_loaded = false;
            loop {
                match receiver.recv().unwrap() {
                    PlayerEvent::Ended => {
                        println!("Ended");
                        unsafe { CLOSE_WINDOW = true; };
                        break;
                    }
                    PlayerEvent::Error => {
                        println!("Error");
                        break;
                    }
                    PlayerEvent::AsyncEvent(name) => {
                        println!("Event received: {:?}", name);
                    }
                    PlayerEvent::MetadataLoaded(metadata) => {
                        metadata_loaded = true;
                        println!("MetadataLoaded; duration: {:?}", metadata.duration);
                    }
                    PlayerEvent::UpdateCurrentImages(images) => {
                        if metadata_loaded {
                            sink.frame_sender.send(images).unwrap();
                        }
                    }
                    PlayerEvent::BreakOutOfEventLoop => {
                        break;
                    }
                    PlayerEvent::BufferedRanges(ranges) => {
                        println!("Buffered ranges: {:?}", ranges);
                    }
                    PlayerEvent::SeekableRanges(ranges) => {
                        println!("Seekable ranges: {:?}", ranges);
                    }
                };
            }
            drop(player);
            sink.frame_sender.send(vec![]).unwrap();
            shutdown_sender.send(()).unwrap();
        });

        PlayerWrapper {
            sender: wrapper_sender,
            shutdown_receiver: shutdown_receiver,
        }
    }
    pub fn shutdown(&self) {
        match self.sender.send(PlayerEvent::BreakOutOfEventLoop) {
            Ok(_) => (),
            Err(_) => (),
        }
        // Block until shutdown is complete. This ensures the Player object
        // and its reference to the GeckoMedia object have been dropped by the
        // time our main function calls GeckoMedia::Shutdown().
        self.shutdown_receiver.recv().unwrap();
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
        if let None = self.current_image {
            panic!("Don't have a current image!");
        }
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
    image_handler: Option<Box<webrender::ExternalImageHandler>>,
    frame_receiver: mpsc::Receiver<Vec<PlanarYCbCrImage>>,
    current_frame_sender: mpsc::Sender<PlanarYCbCrImage>,
    frame_queue: Vec<PlanarYCbCrImage>,
    y_channel_key: Option<ImageKey>,
    cb_channel_key: Option<ImageKey>,
    cr_channel_key: Option<ImageKey>,
    last_frame_id: u32,
}

impl App {
    fn new() -> (App, mpsc::Sender<Vec<PlanarYCbCrImage>>) {
        // Channel for frames to pass between Gecko and player.
        let (frame_sender, frame_receiver) = mpsc::channel();
        // Channel for the current frame to be sent between the main
        // render loop and the external image handler.
        let (current_frame_sender, current_frame_receiver) = mpsc::channel();
        let handler = Box::new(ImageGenerator {
            current_frame_receiver: current_frame_receiver,
            current_image: None,
        });
        let app = App {
            image_handler: Some(handler),
            frame_receiver: frame_receiver,
            frame_queue: vec![],
            current_frame_sender: current_frame_sender,
            y_channel_key: None,
            cb_channel_key: None,
            cr_channel_key: None,
            last_frame_id: 0,
        };
        (app, frame_sender)
    }
}

fn create_or_update_planar_image(
    api: &RenderApi,
    resources: &mut ResourceUpdates,
    image_key: &mut Option<ImageKey>,
    plane: Plane,
    channel_index: u8,
) {
    match image_key {
        &mut Some(ref image_key) => {
            resources.update_image(
                *image_key,
                ImageDescriptor::new(
                    plane.width as u32,
                    plane.height as u32,
                    ImageFormat::A8,
                    true,
                ),
                External(ExternalImageData {
                    id: EXTERNAL_VIDEO_IMAGE_ID,
                    channel_index: channel_index,
                    image_type: ExternalImageType::ExternalBuffer,
                }),
                None,
            );
        }
        &mut None => {
            let key = api.generate_image_key();
            *image_key = Some(key);
            resources.add_image(
                key,
                ImageDescriptor::new(
                    plane.width as u32,
                    plane.height as u32,
                    ImageFormat::A8,
                    true,
                ),
                External(ExternalImageData {
                    id: EXTERNAL_VIDEO_IMAGE_ID,
                    channel_index: channel_index,
                    image_type: ExternalImageType::ExternalBuffer,
                }),
                None,
            );
        }
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

        if self.frame_queue.len() > 0 {
            if self.last_frame_id != self.frame_queue[0].frame_id {
                self.last_frame_id = self.frame_queue[0].frame_id;
                self.current_frame_sender
                    .send(self.frame_queue[0].clone())
                    .unwrap();
                return true;
            }
        }

        false
    }

    fn should_close_window(&self) -> bool {
        unsafe { CLOSE_WINDOW }
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

        if self.frame_queue.len() > 0 {
            // Assume dimensions of first frame.
            let frame = &self.frame_queue[0];

            create_or_update_planar_image(
                api,
                resources,
                &mut self.y_channel_key,
                frame.y_plane(),
                0,
            );
            create_or_update_planar_image(
                api,
                resources,
                &mut self.cb_channel_key,
                frame.cb_plane(),
                1,
            );
            create_or_update_planar_image(
                api,
                resources,
                &mut self.cr_channel_key,
                frame.cr_plane(),
                2,
            );

            let aspect_ratio = frame.picture.width as f32 / frame.picture.height as f32;
            let render_size = LayoutSize::new(
                layout_size.width as f32,
                layout_size.width as f32 / aspect_ratio,
            );
            let render_location =
                LayoutPoint::new(0.0, (layout_size.height - render_size.height) / 2.0);
            let info = LayoutPrimitiveInfo::with_clip_rect(
                LayoutRect::new(render_location, render_size),
                bounds,
            );
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

        builder.pop_stacking_context();
    }

    fn on_event(
        &mut self,
        _event: glutin::Event,
        _api: &RenderApi,
        _document_id: DocumentId,
    ) -> bool {
        true
    }

    fn get_external_image_handler(&mut self) -> Option<Box<webrender::ExternalImageHandler>> {
        mem::replace(&mut self.image_handler, None)
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
                    match sender.send(frames) {
                        Ok(_) => (),
                        Err(_) => break,
                    }
                    window_proxy.wakeup_event_loop();
                }
                Err(_) => {
                    break;
                }
            }
        });
    }
}


fn main() {
    let args: Vec<_> = env::args().collect();
    let filename: &str = if args.len() == 2 {
        args[1].as_ref()
    } else {
        panic!("Usage: test-player file_path")
    };

    let path = Path::new(filename);
    let mime = match path.extension().unwrap().to_str() {
        Some("wav") => "audio/wav",
        Some("mp3") => "audio/mp3",
        Some("flac") => "audio/flac",
        Some("ogg") => "audio/ogg; codecs=\"vorbis\"",
        Some("m4a") => "audio/mp4",
        Some("mp4") => "video/mp4",
        _ => "",
    };
    if mime == "" {
        panic!(
            "Unknown file type. Currently supported: wav, mp3, m4a, flac and ogg/vorbis files.\
                Video files supported: mp4."
        )
    }

    let mut file = File::open(filename).unwrap();
    let mut bytes = vec![];
    file.read_to_end(&mut bytes).unwrap();

    let (mut app, frame_sender) = App::new();

    let player = PlayerWrapper::new(bytes, mime, frame_sender);
    ui::main_wrapper(&mut app, None);
    player.shutdown();
    GeckoMedia::shutdown().unwrap();
}
