/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_LAYERS_H
#define GFX_LAYERS_H

#include "nsISupportsImpl.h"

namespace mozilla {
namespace layers {

class KnowsCompositor;

class LayerManager {
  NS_INLINE_DECL_REFCOUNTING(LayerManager)
public:
  layers::KnowsCompositor* AsKnowsCompositor() { return nullptr; }
private:
  ~LayerManager() {}
};

} // namespace layers
} // namespace mozilla

#endif /* GFX_LAYERS_H */
