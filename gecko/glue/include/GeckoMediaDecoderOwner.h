/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GeckoMediaDecoderOwner_h_
#define GeckoMediaDecoderOwner_h_

#include "GeckoMedia.h"
#include "GeckoMediaDecoder.h"
#include "MediaDecoderOwner.h"
#include "MediaInfo.h"
#include "mozilla/UniquePtr.h"
#include "nsAString.h"
#include "VideoFrameContainer.h"

namespace mozilla {

class AbstractThread;
class GMPCrashHelper;
class MediaInfo;
class MediaResult;
class TaskQueue;

namespace dom {
class HTMLMediaElement;
} // namespace dom

class GeckoMediaDecoderOwner : public MediaDecoderOwner
{
public:

  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(GeckoMediaDecoderOwner)

  GeckoMediaDecoderOwner(PlayerCallbackObject aCallback);
  GeckoMediaDecoderOwner();

  // Called by the media decoder to indicate that the download is progressing.
  void DownloadProgressed() override;

  // Dispatch an asynchronous event to the decoder owner
  void DispatchAsyncEvent(const nsAString& aName) override;

  // Triggers a recomputation of readyState.
  void UpdateReadyState() override;

  /**
   * Fires a timeupdate event. If aPeriodic is true, the event will only
   * be fired if we've not fired a timeupdate event (for any reason) in the
   * last 250ms, as required by the spec when the current time is periodically
   * increasing during playback.
   */
  void FireTimeUpdate(bool aPeriodic) override;

  // Return true if decoding should be paused
  bool GetPaused() override;

  // Called by the video decoder object, on the main thread,
  // when it has read the metadata containing video dimensions,
  // etc.
  // Must take ownership of MetadataTags aTags argument.
  void MetadataLoaded(const MediaInfo* aInfo,
                      UniquePtr<const MetadataTags> aTags) override;

  // Called by the decoder object, on the main thread,
  // when it has read the first frame of the video or audio.
  void FirstFrameLoaded() override;

  // Called by the decoder object, on the main thread,
  // when the resource has a network error during loading.
  // The decoder owner should call Shutdown() on the decoder and drop the
  // reference to the decoder to prevent further calls into the decoder.
  void NetworkError() override;

  // Called by the decoder object, on the main thread, when the
  // resource has a decode error during metadata loading or decoding.
  // The decoder owner should call Shutdown() on the decoder and drop the
  // reference to the decoder to prevent further calls into the decoder.
  void DecodeError(const MediaResult& aError) override;

  // Called by the decoder object, on the main thread, when the
  // resource has a decode issue during metadata loading or decoding, but can
  // continue decoding.
  void DecodeWarning(const MediaResult& aError) override;

  // Return true if media element error attribute is not null.
  bool HasError() const override;

  // Called by the video decoder object, on the main thread, when the
  // resource load has been cancelled.
  void LoadAborted() override;

  // Called by the video decoder object, on the main thread,
  // when the video playback has ended.
  void PlaybackEnded() override;

  // Called by the video decoder object, on the main thread,
  // when the resource has started seeking.
  void SeekStarted() override;

  // Called by the video decoder object, on the main thread,
  // when the resource has completed seeking.
  void SeekCompleted() override;

  // Called by the media stream, on the main thread, when the download
  // has been suspended by the cache or because the element itself
  // asked the decoder to suspend the download.
  void DownloadSuspended() override;

  // Called by the media decoder to indicate whether the media cache has
  // suspended the channel.
  void NotifySuspendedByCache(bool aSuspendedByCache) override;

  // called to notify that the principal of the decoder's media resource has
  // changed.
  void NotifyDecoderPrincipalChanged() override;

  // The status of the next frame which might be available from the decoder
  enum NextFrameStatus
  {
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

  // Called by media decoder when the audible state changed
  void SetAudibleState(bool aAudible) override;

  // Notified by the decoder that XPCOM shutdown has begun.
  // The decoder owner should call Shutdown() on the decoder and drop the
  // reference to the decoder to prevent further calls into the decoder.
  void NotifyXPCOMShutdown() override;

  // Dispatches a "encrypted" event to the HTMLMediaElement, with the
  // provided init data. Actual dispatch may be delayed until HAVE_METADATA.
  // Main thread only.
  void DispatchEncrypted(const nsTArray<uint8_t>& aInitData,
                         const nsAString& aInitDataType) override;

  // Called by the media decoder to create audio/video tracks and add to its
  // owner's track list.
  void ConstructMediaTracks(const MediaInfo* aInfo) override;

  // Called by the media decoder to removes all audio/video tracks from its
  // owner's track list.
  void RemoveMediaTracks() override;

  // Called by the media decoder to notify the owner to resolve a seek promise.
  void AsyncResolveSeekDOMPromiseIfExists() override;

  // Called by the media decoder to notify the owner to reject a seek promise.
  void AsyncRejectSeekDOMPromiseIfExists() override;

  // Notified by the decoder that a decryption key is required before emitting
  // further output.
  void NotifyWaitingForKey() override;

  /*
   * Methods that are used only in Gecko go here. We provide defaul
   * implementations so they can compile in Servo without modification.
   */
  // Return an abstract thread on which to run main thread runnables.
  AbstractThread* AbstractMainThread() const override;

  // Get the HTMLMediaElement object if the decoder is being used from an
  // HTML media element, and null otherwise.
  dom::HTMLMediaElement* GetMediaElement() override;

  // Called by the media decoder and the video frame to get the
  // ImageContainer containing the video data.
  VideoFrameContainer* GetVideoFrameContainer() override;

  // Return the decoder owner's owner document.
  // nsIDocument* GetDocument() const { return nullptr; }

  // Called by the media decoder to create a GMPCrashHelper.
  already_AddRefed<GMPCrashHelper> CreateGMPCrashHelper() override;

  void Invalidate(bool aImageSizeChanged,
                  Maybe<nsIntSize>& aNewIntrinsicSize,
                  bool aForceInvalidate) override;

  // Called after the MediaStream we're playing rendered a frame to aContainer
  // with a different principalHandle than the previous frame.
  void PrincipalHandleChangedForVideoFrameContainer(VideoFrameContainer* aContainer,
                                                    const PrincipalHandle& aNewPrincipalHandle) override;

  void SetDecoder(GeckoMediaDecoder* aDecoder);

  void UpdateCurrentImages(nsTArray<GeckoPlanarYCbCrImage> aImages);

  void NotifyBuffered() const override;
  void NotifySeekable() const override;

  void Shutdown();

private:
  ~GeckoMediaDecoderOwner() {}

  bool mHasError = false;
  PlayerCallbackObject mCallback = { 0 };
  RefPtr<GeckoMediaDecoder> mDecoder;
  RefPtr<VideoFrameContainer> mVideoFrameContainer;
};

} // namespace mozilla

#endif
