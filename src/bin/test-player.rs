// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

extern crate gecko_media;

use gecko_media::{GeckoMedia, Metadata, PlayerEventSink};
use std::env;
use std::ffi::CString;
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;
use std::sync::mpsc;

fn main() {
    let args: Vec<_> = env::args().collect();
    let filename: &str = if args.len() == 2 {
        args[1].as_ref()
    } else {
        panic!("Usage: test-player file_path")
    };

    GeckoMedia::get().unwrap();
    let (sender, receiver) = mpsc::channel();
    GeckoMedia::get().unwrap().queue_task(
        move || sender.send(()).unwrap(),
    );
    receiver.recv().unwrap();
    {
        enum Status {
            Error,
            Ended,
            AsyncEvent(CString),
            MetadataLoaded(Metadata),
        }
        let (sender, receiver) = mpsc::channel();
        struct Sink {
            sender: mpsc::Sender<Status>,
        }
        impl PlayerEventSink for Sink {
            fn playback_ended(&self) {
                self.sender.send(Status::Ended).unwrap();
            }
            fn decode_error(&self) {
                self.sender.send(Status::Error).unwrap();
            }
            fn async_event(&self, name: &str) {
                self.sender
                    .send(Status::AsyncEvent(CString::new(name).unwrap()))
                    .unwrap();
            }
            fn metadata_loaded(&self, metadata: Metadata) {
                self.sender.send(Status::MetadataLoaded(metadata)).unwrap();
            }
            fn duration_changed(&self, _duration: f64) {}
            fn loaded_data(&self) {}
            fn time_update(&self, _time: f64) {}
            fn seek_started(&self) {}
            fn seek_completed(&self) {}
        }
        let sink = Box::new(Sink { sender: sender });

        let g = GeckoMedia::get().unwrap();
        let player = g.create_player(sink).unwrap();
        let mut file = File::open(filename).unwrap();
        let mut bytes = vec![];
        file.read_to_end(&mut bytes).unwrap();

        let path = Path::new(filename);
        let mime = match path.extension().unwrap().to_str() {
            Some("wav") => "audio/wav",
            Some("mp3") => "audio/mp3",
            Some("flac") => "audio/flac",
            Some("ogg") => "audio/ogg; codecs=\"vorbis\"",
            Some("m4a") => "audio/mp4",
            _ => "",
        };
        if mime != "" {
            player.load_blob(bytes, mime).unwrap();
            player.play();
            player.set_volume(1.0);
            loop {
                match receiver.recv().unwrap() {
                    Status::Ended => {
                        println!("Ended");
                        break;
                    }
                    Status::Error => {
                        println!("Error");
                        break;
                    }
                    Status::AsyncEvent(name) => {
                        println!("Event received: {:?}", name);
                    }
                    Status::MetadataLoaded(metadata) => {
                        println!("MetadataLoaded; duration: {:?}", metadata.duration);
                    }
                };
            }
        } else {
            panic!(
                "Unknown file type. Currently supported: wav, mp3, m4a, flac and ogg/vorbis files."
            )
        }
    }

    GeckoMedia::shutdown().unwrap();
}
