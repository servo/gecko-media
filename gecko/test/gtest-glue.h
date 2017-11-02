/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GTEST_GLUE_H
#define GTEST_GLUE_H

#include "mozilla/Assertions.h"

// Note: We use release mode assertions so that tests in release mode
// actually test. We also evaluate the A,B arguments to ensure there
// are no macro processing issues when evaluating the inner macros here.
#define TEST(A, B) void Test_##A##B ()
#define EXPECT_TRUE(condition) { auto c = condition; MOZ_RELEASE_ASSERT(c); }
#define EXPECT_FALSE(condition) { auto c = condition; MOZ_RELEASE_ASSERT(!c); }
#define EXPECT_EQ(A, B) { auto a = (A); auto b = (B); MOZ_RELEASE_ASSERT(a == b); }
#define EXPECT_GT(A, B) { auto a = (A); auto b = (B); MOZ_RELEASE_ASSERT(a > b); }
#define EXPECT_LT(A, B) { auto a = (A); auto b = (B); MOZ_RELEASE_ASSERT(a < b); }
#define EXPECT_NE(A, B) { auto a = (A); auto b = (B); MOZ_RELEASE_ASSERT(a != b); }
#define EXPECT_STREQ(A, B) { auto a = (A); auto b = (B); MOZ_RELEASE_ASSERT(strcmp(a, b) == 0); }

#endif
