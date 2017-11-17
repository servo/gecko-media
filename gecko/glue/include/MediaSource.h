/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_MediaSource_h_
#define mozilla_dom_MediaSource_h_

#include "MediaSourceDecoder.h"
#include "mozilla/Assertions.h"
#include "mozilla/Attributes.h"
#include "mozilla/MozPromise.h"
#include "mozilla/dom/MediaSourceBinding.h"
#include "nsCOMPtr.h"
#include "nscore.h"
#include "TimeUnits.h"

namespace mozilla {

// TODO (gecko-media) class AbstractThread;
class ErrorResult;
template <typename T> class AsyncEventRunner;
class MediaResult;

namespace dom {

class SourceBuffer;
class SourceBufferList;
template <typename T> class Optional;

class MediaSource final
{
public:
  NS_INLINE_DECL_REFCOUNTING(MediaSource)

  static already_AddRefed<MediaSource>
  Constructor(ErrorResult& aRv);

  SourceBufferList* SourceBuffers();
  SourceBufferList* ActiveSourceBuffers();
  MediaSourceReadyState ReadyState();

  double Duration();
  void SetDuration(double aDuration, ErrorResult& aRv);

  already_AddRefed<SourceBuffer> AddSourceBuffer(const nsAString& aType, ErrorResult& aRv);
  void RemoveSourceBuffer(SourceBuffer& aSourceBuffer, ErrorResult& aRv);

  void EndOfStream(const MediaSourceEndOfStreamError& aError, ErrorResult& aRv);
  void EndOfStream(const MediaResult& aError);

  void SetLiveSeekableRange(double aStart, double aEnd, ErrorResult& aRv);
  void ClearLiveSeekableRange(ErrorResult& aRv);

  static bool IsTypeSupported(const nsAString& aType);
  static nsresult IsTypeSupported(const nsAString& aType, DecoderDoctorDiagnostics* aDiagnostics);

  static bool Enabled();

  // Attach this MediaSource to Decoder aDecoder.  Returns false if already attached.
  bool Attach(MediaSourceDecoder* aDecoder);
  void Detach();

  // Set mReadyState to aState and fire the required events at the MediaSource.
  void SetReadyState(MediaSourceReadyState aState);

 // Used by SourceBuffer to call CreateSubDecoder.
  MediaSourceDecoder* GetDecoder()
  {
    return mDecoder;
  }

  nsIPrincipal* GetPrincipal()
  {
    return nullptr;
  }

  // Returns a string describing the state of the MediaSource internal
  // buffered data. Used for debugging purposes.
  void GetMozDebugReaderData(nsAString& aString);

  bool HasLiveSeekableRange() const { return mLiveSeekableRange.isSome(); }
  media::TimeInterval LiveSeekableRange() const
  {
    return mLiveSeekableRange.value();
  }

  /* TODO (gecko-media)
  AbstractThread* AbstractMainThread() const
  {
    return mAbstractMainThread;
  }*/

  // Resolve all CompletionPromise pending.
  void CompletePendingTransactions();

private:
  // SourceBuffer uses SetDuration and SourceBufferIsActive
  friend class mozilla::dom::SourceBuffer;

  ~MediaSource();

  explicit MediaSource();

  friend class AsyncEventRunner<MediaSource>;
  void DispatchSimpleEvent(const char* aName);
  void QueueAsyncSimpleEvent(const char* aName);

  void DurationChange(double aNewDuration, ErrorResult& aRv);

  // SetDuration with no checks.
  void SetDuration(double aDuration);

  typedef MozPromise<bool, MediaResult, /* IsExclusive = */ true>
    ActiveCompletionPromise;
  // Mark SourceBuffer as active and rebuild ActiveSourceBuffers.
  // Return a MozPromise that will be resolved once all related operations are
  // completed, or can't progress any further.
  // Such as, transition of readyState from HAVE_NOTHING to HAVE_METADATA.
  RefPtr<ActiveCompletionPromise> SourceBufferIsActive(
    SourceBuffer* aSourceBuffer);

  RefPtr<SourceBufferList> mSourceBuffers;
  RefPtr<SourceBufferList> mActiveSourceBuffers;

  RefPtr<MediaSourceDecoder> mDecoder;
  // Ensures the media element remains alive to dispatch progress and
  // durationchanged events.
  // TODO (gecko-media) RefPtr<HTMLMediaElement> mMediaElement;

  // TODO (gecko-media) const RefPtr<AbstractThread> mAbstractMainThread;

  MediaSourceReadyState mReadyState;

  Maybe<media::TimeInterval> mLiveSeekableRange;
  nsTArray<MozPromiseHolder<ActiveCompletionPromise>> mCompletionPromises;
};

} // namespace dom

} // namespace mozilla

#endif /* mozilla_dom_MediaSource_h_ */
