/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GeckoMediaDecoderOwner_h_
#define GeckoMediaDecoderOwner_h_

#include "MediaDecoderOwner.h"
#include "mozilla/UniquePtr.h"
#include "MediaInfo.h"

namespace mozilla {

class AbstractThread;
class GMPCrashHelper;
class VideoFrameContainer;
class MediaInfo;
class MediaResult;
class TaskQueue;

namespace dom {
class HTMLMediaElement;
} // namespace dom

class GeckoMediaDecoderOwner : public MediaDecoderOwner
{
public:
  GeckoMediaDecoderOwner() { }

  // Called by the media decoder to indicate that the download is progressing.
  virtual void DownloadProgressed() override { }

  // Dispatch an asynchronous event to the decoder owner
  virtual nsresult DispatchAsyncEvent(const nsAString& aName) override { return NS_OK; };

  // Triggers a recomputation of readyState.
  virtual void UpdateReadyState() override { }

  /**
   * Fires a timeupdate event. If aPeriodic is true, the event will only
   * be fired if we've not fired a timeupdate event (for any reason) in the
   * last 250ms, as required by the spec when the current time is periodically
   * increasing during playback.
   */
  virtual void FireTimeUpdate(bool aPeriodic) override { }

  // Return true if decoding should be paused
  virtual bool GetPaused() override { return false; }

  // Called by the video decoder object, on the main thread,
  // when it has read the metadata containing video dimensions,
  // etc.
  // Must take ownership of MetadataTags aTags argument.
  virtual void MetadataLoaded(const MediaInfo* aInfo,
                              UniquePtr<const MetadataTags> aTags) override { }

  // Called by the decoder object, on the main thread,
  // when it has read the first frame of the video or audio.
  virtual void FirstFrameLoaded() override { }

  // Called by the decoder object, on the main thread,
  // when the resource has a network error during loading.
  // The decoder owner should call Shutdown() on the decoder and drop the
  // reference to the decoder to prevent further calls into the decoder.
  virtual void NetworkError() override { }

  // Called by the decoder object, on the main thread, when the
  // resource has a decode error during metadata loading or decoding.
  // The decoder owner should call Shutdown() on the decoder and drop the
  // reference to the decoder to prevent further calls into the decoder.
  virtual void DecodeError(const MediaResult& aError) override { }

  // Called by the decoder object, on the main thread, when the
  // resource has a decode issue during metadata loading or decoding, but can
  // continue decoding.
  virtual void DecodeWarning(const MediaResult& aError) override { }

  // Return true if media element error attribute is not null.
  virtual bool HasError() const override { return false; }

  // Called by the video decoder object, on the main thread, when the
  // resource load has been cancelled.
  virtual void LoadAborted() override { }

  // Called by the video decoder object, on the main thread,
  // when the video playback has ended.
  virtual void PlaybackEnded() override { }

  // Called by the video decoder object, on the main thread,
  // when the resource has started seeking.
  virtual void SeekStarted() override { }

  // Called by the video decoder object, on the main thread,
  // when the resource has completed seeking.
  virtual void SeekCompleted() override { }

  // Called by the media stream, on the main thread, when the download
  // has been suspended by the cache or because the element itself
  // asked the decoder to suspend the download.
  virtual void DownloadSuspended() override { }

  // Called by the media decoder to indicate whether the media cache has
  // suspended the channel.
  virtual void NotifySuspendedByCache(bool aSuspendedByCache) override { }

  // called to notify that the principal of the decoder's media resource has changed.
  virtual void NotifyDecoderPrincipalChanged() override { }

  // The status of the next frame which might be available from the decoder
  enum NextFrameStatus {
    // The next frame of audio/video is available
    NEXT_FRAME_AVAILABLE,
    // The next frame of audio/video is unavailable because the decoder
    // is paused while it buffers up data
    NEXT_FRAME_UNAVAILABLE_BUFFERING,
    // The next frame of audio/video is unavailable for the decoder is seeking.
    NEXT_FRAME_UNAVAILABLE_SEEKING,
    // The next frame of audio/video is unavailable for some other reasons
    NEXT_FRAME_UNAVAILABLE,
    // Sentinel value
    NEXT_FRAME_UNINITIALIZED
  };

  // Check if the decoder owner is active.
  virtual bool IsActive() const override { return true; }

  // Check if the decoder owner is hidden.
  virtual bool IsHidden() const override { return false; }

  // Called by media decoder when the audible state changed
  virtual void SetAudibleState(bool aAudible) override { }

  // Notified by the decoder that XPCOM shutdown has begun.
  // The decoder owner should call Shutdown() on the decoder and drop the
  // reference to the decoder to prevent further calls into the decoder.
  virtual void NotifyXPCOMShutdown() override { }

  // Dispatches a "encrypted" event to the HTMLMediaElement, with the
  // provided init data. Actual dispatch may be delayed until HAVE_METADATA.
  // Main thread only.
  virtual void DispatchEncrypted(const nsTArray<uint8_t>& aInitData,
                                 const nsAString& aInitDataType) override { }

  // Called by the media decoder to create audio/video tracks and add to its
  // owner's track list.
  virtual void ConstructMediaTracks(const MediaInfo* aInfo) override { }

  // Called by the media decoder to removes all audio/video tracks from its
  // owner's track list.
  virtual void RemoveMediaTracks() override { }

  // Called by the media decoder to notify the owner to resolve a seek promise.
  virtual void AsyncResolveSeekDOMPromiseIfExists() override { }

  // Called by the media decoder to notify the owner to reject a seek promise.
  virtual void AsyncRejectSeekDOMPromiseIfExists() override { }

  // Notified by the decoder that a decryption key is required before emitting
  // further output.
  virtual void NotifyWaitingForKey() {}

  /*
   * Methods that are used only in Gecko go here. We provide defaul
   * implementations so they can compile in Servo without modification.
   */
  // Return an abstract thread on which to run main thread runnables.
  virtual AbstractThread* AbstractMainThread() const { return AbstractThread::MainThread(); }

  // Get the HTMLMediaElement object if the decoder is being used from an
  // HTML media element, and null otherwise.
  virtual dom::HTMLMediaElement* GetMediaElement() { return nullptr; }

  // Called by the media decoder and the video frame to get the
  // ImageContainer containing the video data.
  virtual VideoFrameContainer* GetVideoFrameContainer() { return nullptr; }

  // Return the decoder owner's owner document.
  // virtual nsIDocument* GetDocument() const { return nullptr; }

  // Called by the media decoder to create a GMPCrashHelper.
  virtual already_AddRefed<GMPCrashHelper> CreateGMPCrashHelper()
  {
    return nullptr;
  }

  /*
   * Servo only methods go here. Please provide default implementations so they
   * can build in Gecko without any modification.
   */
};

} // namespace mozilla

#endif

