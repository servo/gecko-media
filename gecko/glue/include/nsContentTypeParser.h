/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsContentTypeParser_h
#define nsContentTypeParser_h

#include "nsString.h"

typedef struct mime_parser_S mime_parser_t;

class nsContentTypeParser {
public:
  explicit nsContentTypeParser(const nsAString& aString);
  ~nsContentTypeParser();

  nsresult GetParameter(const char* aParameterName, nsAString& aResult) const;
  nsresult GetType(nsAString& aResult) const;

private:
  mime_parser_t* mParser = nullptr;
};

#endif

