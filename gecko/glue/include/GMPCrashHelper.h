/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined(GMPCrashHelper_h_)
#define GMPCrashHelper_h_

#include "MainThreadUtils.h"
#include "nsISupportsImpl.h"

class nsPIDOMWindowInner;

namespace mozilla {

class GMPCrashHelper
{
public:
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(GMPCrashHelper)

  // Called on the main thread.
  virtual already_AddRefed<nsPIDOMWindowInner> GetPluginCrashedEventTarget() {
    return nullptr;
  }

protected:
  virtual ~GMPCrashHelper()
  {
    MOZ_ASSERT(NS_IsMainThread());
  }
};

} // namespace mozilla

#endif // GMPCrashHelper_h_
