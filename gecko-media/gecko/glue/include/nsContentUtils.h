/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsContentUtils_h__
#define nsContentUtils_h__

#include "mozilla/RefPtr.h"
#include "mozilla/Move.h"
#include "nsThreadUtils.h"

namespace mozilla {
namespace layers {
class LayerManager;
} // namespace layers
} // namespace mozilla

class nsIDocument;

class nsContentUtils
{
public:
  static void RunInStableState(already_AddRefed<nsIRunnable> aRunnable) {
    // RunInStableState ensures no JS contexts are running at the
    // time when this runnable runs. Inside GeckoMedia, we don't
    // run JS on the main thread, we let Servo run JS on its own
    // script thread, so we can just dispatch the task to the current
    // thread. Note: we can't just run it, as FireTailDispatcher()
    // clears mTailDispatcher which would make the ref() call below
    // fail.
    NS_DispatchToCurrentThread(mozilla::Move(aRunnable));
  }
  static mozilla::layers::LayerManager* LayerManagerForDocument(nsIDocument* aDocument) {
    return nullptr;
  }

  static void RegisterShutdownObserver(nsIObserver* aObserver) {}
  static void UnregisterShutdownObserver(nsIObserver* aObserver) {}
};

#endif // nsContentUtils_h__
