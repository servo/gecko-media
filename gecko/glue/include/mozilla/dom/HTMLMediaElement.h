/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#ifndef dom_media_HTMLMediaElement_h__
#define dom_media_HTMLMediaElement_h__

#include "nsSize.h"
#include "mozilla/AbstractMainThread.h"

namespace mozilla {

class nsIFrame;
class VideoFrameContainer;
class PrincipalHandle;

namespace dom {

class HTMLMediaElement {
public:

  void PrincipalHandleChangedForVideoFrameContainer(VideoFrameContainer* aContainer,
                                                    const PrincipalHandle& aNewPrincipalHandle)
  {
  }

  nsIFrame* GetPrimaryFrame() const {
    return  nullptr;
  }

  void UpdateMediaSize(const nsIntSize& aSize) {
  }

  AbstractMainThread* AbstractMainThread() {
    return AbstractThread::MainThread();
  }

private:
  RefPtr<AbstractMainThread> mMainThread;
};

} // namespace dom
} // namespace mozilla

#endif // dom_media_HTMLMediaElement_h__
