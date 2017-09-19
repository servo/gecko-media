/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsXULAppAPI.h"

bool
XRE_IsContentProcess()
{
  //return XRE_GetProcessType() == GeckoProcessType_Content;
  return true;
}

/**
 * Returns true in the e10s parent process and in the main process when e10s
 * is disabled.
 */
bool
XRE_IsParentProcess()
{
  return true;
}
