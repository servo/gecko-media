/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef gecko_media_prefs
#define gecko_media_prefs

namespace mozilla {

struct BoolPref
{
  const char* mName;
  const bool mValue;
};

struct IntPref
{
  const char* mName;
  const int32_t mValue;
};

struct StringPref
{
  const char* mName;
  const char* mValue;
};

} // namespace mozilla

#endif
