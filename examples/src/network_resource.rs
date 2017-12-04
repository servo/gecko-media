// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use futures::{Future, Stream};
use gecko_media::CachedRangesSink;
use gecko_media::NetworkResource;
use hyper;
use hyper::header::{ByteRangeSpec, ContentLength, ContentRange};
use hyper::header::{ContentRangeSpec, Range as RangeHeader};
use hyper::{Client, Request, Method};
use std::cmp;
use std::io;
use std::ops::Range;
use std::sync::{Mutex, mpsc};
use std::thread;
use tokio_core::reactor::Core;


struct Cache {
    start: u64,
    data: Vec<u8>,
}

impl Cache {
    pub fn new() -> Cache {
        Cache {
            start: 0,
            data: vec![],
        }
    }

    pub fn append(&mut self, offset: u64, buffer: &[u8]) {
        if self.end() != offset {
            return;
        }
        self.data.extend_from_slice(buffer);
    }

    pub fn end(&self) -> u64 {
        self.start + self.data.len() as u64
    }

    pub fn read(&mut self, offset: u64, count: u32) -> Result<Vec<u8>, ()> {
        if self.start > offset || offset > self.end() {
            return Err(());
        }
        assert!(offset >= self.start);
        let start = (offset - self.start) as usize;
        let end = cmp::min(self.data.len(), start + count as usize);
        if end - start == 0 {
            return Err(());
        }
        Ok(self.data[start..end].to_vec())
    }

    pub fn reset(&mut self, offset: u64) {
        self.start = offset;
        self.data.clear();
    }

    pub fn ranges(&self) -> Vec<Range<u64>> {
        vec![
            Range {
                start: self.start,
                end: self.end(),
            },
        ]
    }
}

enum Response {
    Data(u64, Vec<u8>),
    ContentLength(Option<u64>),
    Error,
}

enum Commands {
    Read(u64, u32, mpsc::Sender<Response>),
    ReadFromCache(u64, u32, mpsc::Sender<Response>),
    AddToCache(u64, Vec<u8>),
    SetContentLength(u64),
    GetContentLength(mpsc::Sender<Response>),
    SetCachedRangesSink(CachedRangesSink),
    Shutdown,
}

const SEEK_VS_READ_THRESHOLD: u64 = 1_000_000;

struct Downloader {
    uri: String,
    // We tell the worker thread to terminate with this.
    stop_sender: Option<mpsc::Sender<()>>,

    // We keep a copy of the sender to send to the downloader threads
    // we spin up.
    command_sender: mpsc::Sender<Commands>,
}

impl Downloader {
    pub fn new(uri: String, command_sender: mpsc::Sender<Commands>) -> Downloader {
        let mut downloader = Downloader {
            uri,
            stop_sender: None,
            command_sender,
        };
        downloader.download_from(0);
        downloader
    }

    pub fn stop_download(&mut self) {
        self.stop_sender.as_ref().map(
            |sender| { sender.send(()).ok(); },
        );
        self.stop_sender = None;
    }

    pub fn download_from(&mut self, offset: u64) {
        self.stop_download();
        let (stop_sender, stop_receiver) = mpsc::channel();
        self.stop_sender = Some(stop_sender);
        let uri = self.uri.clone();
        let command_sender = self.command_sender.clone();
        thread::spawn(move || {
            let mut core = Core::new().unwrap();
            let client = Client::new(&core.handle());
            let uri = uri.parse().unwrap();
            let mut req = Request::new(Method::Get, uri);
            req.headers_mut().set(RangeHeader::Bytes(
                vec![ByteRangeSpec::AllFrom(offset)],
            ));
            let mut read_offset = offset;
            let work = client.request(req).and_then(|res| {
                let &ContentLength(length) = res.headers().get::<ContentLength>().unwrap();
                command_sender
                    .send(Commands::SetContentLength(length))
                    .unwrap();
                // Try to get the offset of the response from the HTTP header.
                // Otherwise, we'll use the offset we requested, and hope it's
                // what the server returned.
                {
                    let &ContentRange(ref range) = res.headers().get::<ContentRange>().unwrap();
                    if let &ContentRangeSpec::Bytes {
                        range,
                        instance_length: _instance_length,
                    } = range
                    {
                        if let Some((start, _end)) = range {
                            read_offset = start;
                        }
                    }
                }
                // Stream chunks to the cache, until we receive a cancel message.
                res.body().for_each(|chunk| {
                    if let Ok(()) = stop_receiver.try_recv() {
                      return Err(hyper::Error::Io(io::Error::new(
                          io::ErrorKind::TimedOut,
                          "Download canceled.",
                      )));
                    }
                    let len = chunk.len();
                    command_sender
                        .send(Commands::AddToCache(read_offset, chunk.to_vec()))
                        .unwrap();
                    read_offset += len as u64;
                    Ok(())
                })
            });
            core.run(work).ok();
        });
    }
}

pub struct DownloadController {
    command_sender: Mutex<mpsc::Sender<Commands>>,
}

impl DownloadController {
    pub fn new(uri: &str) -> DownloadController {
        // Create a thread to service read/get_buffered calls.
        // That thread in turn defers downloads to another thread, which sends the results back
        // to the service thread for caching.
        let (command_sender, command_receiver) = mpsc::channel();
        let command_sender_clone = command_sender.clone();
        let uri = uri.to_owned();
        thread::spawn(move || {
            let mut cache = Cache::new();
            let mut downloader = Downloader::new(uri, command_sender_clone);
            let mut content_length = None;
            let mut pending_reads = vec![];
            let mut cached_ranges_sink = None;
            loop {
                match command_receiver.recv() {
                    Ok(Commands::SetCachedRangesSink(sink)) => {
                        cached_ranges_sink = Some(sink);
                        cached_ranges_sink.as_ref().map(|sink| {
                            sink.update(&cache.ranges())
                        });
                    },
                    Ok(Commands::Read(offset, count, response_sender)) => {
                        if let Ok(bytes) = cache.read(offset, count) {
                            // Read serviced from cached data.
                            response_sender.send(Response::Data(offset, bytes)).unwrap();
                            continue;
                        }

                        // We need to wait for more data before we can service the read.
                        pending_reads.push((offset, count, response_sender));

                        if offset >= cache.end() && offset < cache.end() + SEEK_VS_READ_THRESHOLD {
                            // Requested read is close to the current download position.
                            // Just wait for the download to catch up.
                            continue;
                        }

                        // Otherwise, we need to seek the stream to the offset.
                        cache.reset(offset);
                        downloader.download_from(offset);
                    },
                    Ok(Commands::ReadFromCache(offset, count, response_sender)) => {
                        if let Ok(bytes) = cache.read(offset, count) {
                            // Read serviced from cached data.
                            response_sender.send(Response::Data(offset, bytes)).unwrap();
                            continue;
                        }
                        response_sender.send(Response::Error).unwrap();
                    },
                    Ok(Commands::AddToCache(offset, bytes)) => {
                        cache.append(offset, &bytes);
                        cached_ranges_sink.as_ref().map(|sink| {
                            sink.update(&cache.ranges())
                        });
                        // See if any reads waiting more data can now be serviced.
                        pending_reads = pending_reads
                            .into_iter()
                            .filter(|&(offset, count, ref response_sender)| {
                                if let Ok(bytes) = cache.read(offset, count) {
                                    // Read serviced.
                                    response_sender.send(Response::Data(offset, bytes)).ok();
                                    return false;
                                }
                                true
                            })
                            .collect();
                    },
                    Ok(Commands::SetContentLength(length)) => {
                        content_length = Some(length);
                    },
                    Ok(Commands::GetContentLength(response_sender)) => {
                        response_sender
                            .send(Response::ContentLength(content_length.clone()))
                            .ok();
                    },
                    Ok(Commands::Shutdown) => {
                        downloader.stop_download();
                        // Abort any pending reads. This is important, as
                        // threads in GeckoMedia may be blocked in our Rust
                        // DownloadController::read_from_cache() impl waiting
                        // for the result of a read.
                        for (_, _, ref response_sender) in pending_reads {
                            response_sender.send(Response::Error).ok();
                        }
                        break;
                    },
                    Err(_) => {},
                }
            }
        });

        DownloadController {
            command_sender: Mutex::new(command_sender),
        }
    }
}

impl Drop for DownloadController {
    fn drop(&mut self) {
        self.command_sender
            .lock()
            .unwrap()
            .send(Commands::Shutdown)
            .ok();
    }
}

impl NetworkResource for DownloadController {
    fn set_cached_ranges_sink(&self, sink: CachedRangesSink) {
        self.command_sender
            .lock()
            .unwrap()
            .send(Commands::SetCachedRangesSink(sink))
            .unwrap();
    }
    fn read_at(&self, offset: u64, buffer: &mut [u8]) -> Result<u32, ()> {
        let (sender, receiver) = mpsc::channel();
        self.command_sender
            .lock()
            .unwrap()
            .send(Commands::Read(offset, buffer.len() as u32, sender))
            .unwrap();
        match receiver.recv() {
            Ok(Response::Data(_, data)) => {
                let mut i = 0;
                let len = data.len();
                for byte in data {
                    buffer[i] = byte;
                    i += 1;
                }
                Ok(len as u32)
            },
            _ => Err(()),
        }
    }

    fn pin(&self) {
        // We don't evict data, so nothing to do here.
    }
    fn unpin(&self) {
        // We don't evict data, so nothing to do here.
    }

    fn len(&self) -> Option<u64> {
        let (sender, receiver) = mpsc::channel();
        self.command_sender
            .lock()
            .unwrap()
            .send(Commands::GetContentLength(sender))
            .unwrap();
        match receiver.recv() {
            Ok(Response::ContentLength(Some(length))) => Some(length),
            _ => None,
        }
    }

    fn read_from_cache(&self, offset: u64, buffer: &mut [u8]) -> Result<(), ()> {
        let (sender, receiver) = mpsc::channel();
        self.command_sender
            .lock()
            .unwrap()
            .send(Commands::ReadFromCache(offset, buffer.len() as u32, sender))
            .unwrap();
        match receiver.recv() {
            Ok(Response::Data(start, data)) => {
                assert!(start == offset);
                assert!(data.len() == buffer.len());
                let mut i = 0;
                for byte in data {
                    buffer[i] = byte;
                    i += 1;
                }
                Ok(())
            },
            _ => Err(()),
        }
    }
}
