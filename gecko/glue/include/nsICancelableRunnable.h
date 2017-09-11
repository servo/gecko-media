/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef gecko_media_nsICancelableRunnable_h
#define gecko_media_nsICancelableRunnable_h

#include "nsISupports.h"

class nsICancelableRunnable : public nsISupports
{
public:

  /*
   * Cancels a pending task.  If the task has already been executed this will
   * be a no-op.  Calling this method twice is considered an error.
   *
   * @throws NS_ERROR_UNEXPECTED
   *   Indicates that the runnable has already been canceled.
   */
  virtual nsresult Cancel() = 0;

protected:
  nsICancelableRunnable() { }
  virtual ~nsICancelableRunnable() {}
};

#endif // gecko_media_nsICancelableRunnable_h
