/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsContentTypeParser.h"

extern "C" {
  mime_parser_t* mime_parser_new(char const* aMimeType);
  void mime_parser_free(mime_parser_t* aParser);
  char* mime_parser_get_param(mime_parser_t* aParser, const char* aParam);
  char* mime_parser_get_type(mime_parser_t* aParser);
  void mime_parser_free_string(char* aValue);
}

nsContentTypeParser::nsContentTypeParser(const nsAString& aString)
{
  mParser = mime_parser_new(NS_ConvertUTF16toUTF8(aString).get());
}

nsContentTypeParser::~nsContentTypeParser()
{
  mime_parser_free(mParser);
}

nsresult
nsContentTypeParser::GetParameter(const char* aParameterName,
                                  nsAString& aResult) const
{
  NS_ENSURE_TRUE(mParser, NS_ERROR_FAILURE);

  char* value = mime_parser_get_param(mParser, aParameterName);
  if (!value) {
    return NS_ERROR_FAILURE;
  }
  aResult = NS_ConvertUTF8toUTF16(nsDependentCString(value));
  mime_parser_free_string(value);
  return NS_OK;
}

nsresult
nsContentTypeParser::GetType(nsAString& aResult) const
{
  NS_ENSURE_TRUE(mParser, NS_ERROR_FAILURE);
  char* value = mime_parser_get_type(mParser);
  if (!value) {
    return NS_ERROR_FAILURE;
  }
  aResult = NS_ConvertUTF8toUTF16(nsDependentCString(value));
  mime_parser_free_string(value);
  return NS_OK;
}

