// Useful macros
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

// Bit manipulation
#define BITMASK_SET(a, b) (a |= b)
#define BITMASK_RESET(a, b) (a &= ~(b))
#define BITMASK_FLIP(a, b) (a ^= b)
#define BITMASK_TEST(a, b) ((a & b) != 0)
#define CALC_BITMASK(n) (u32(1 << n))
#define CALC_BITMASK8(n) (u8(1 << n))
#define CALC_BITMASK16(n) (u16(1 << n))

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

#ifdef _WIN32
#define DIRECTORY_SEPARATOR_CHAR "\\"
#else
#define DIRECTORY_SEPARATOR_CHAR "/"
#endif
