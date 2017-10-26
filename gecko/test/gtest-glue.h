/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GTEST_GLUE_H
#define GTEST_GLUE_H

#define TEST(A, B) void Test_##A##B ()
#define EXPECT_TRUE(condition) assert((condition))
#define EXPECT_FALSE(condition) assert(!condition)
#define EXPECT_EQ(A, B) assert((A) == (B))
#define EXPECT_GT(A, B) assert((A) > (B))
#define EXPECT_LT(A, B) assert((A) < (B))
#define EXPECT_NE(A, B) assert((A) != (B))
#define EXPECT_STREQ(A, B) assert(strcmp(A, B) == 0)

#endif
