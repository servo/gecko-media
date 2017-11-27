/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsPluginHost_h__
#define nsPluginHost_h__

#include "nsISupportsImpl.h"
#include "RefPtr.h"

class nsPluginHost {
public:

  NS_INLINE_DECL_REFCOUNTING(nsPluginHost)

  static already_AddRefed<nsPluginHost> GetInst() {
    return RefPtr<nsPluginHost>(new nsPluginHost()).forget();
  }

  bool HavePluginForType(const nsCString& aType) const {
    return false;
  }
private:
  nsPluginHost() {}
  ~nsPluginHost() {}
};

#endif // nsPluginHost_h__