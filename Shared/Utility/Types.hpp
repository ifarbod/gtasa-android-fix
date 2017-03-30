// Numeral types
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "The size of " #struc " is not " #size)

#define OFFSETOF(struc, member) \
    (reinterpret_cast<size_t>(&reinterpret_cast<const volatile char&>(((static_cast<struc*>(nullptr))->member))))

#define VALIDATE_OFFSET(struc, member, offset) static_assert(OFFSETOF(struc, member) == offset, "The offset of " #struc "::" #member " is not " #offset)

#ifndef CTN_NO_FIXEDWIDTH_TYPES

// Types
using u8 = unsigned char;       // A single byte: 0-255.
using u16 = unsigned short;     // 2 bytes: 0 - 65535.
using u32 = unsigned int;       // 4 bytes: 0 - 4,294,967,295 ~ 4000 million or 4e9.
using u64 = unsigned long long; // 8 bytes: 18,446,744,073,709,551,615 ~1.8e19.

using s8 = signed char;         // A single byte: -128 - 127.
using s16 = signed short;       // 2 bytes: -32768 - 32767.
using s32 = int;                // 4 bytes signed: max 2,147,483,647 ~ 2000 million or 2e9.
using s64 = signed long long;   // 8 bytes signed. 9,223,372,036,854,775,807 ~ 9e18.

using f32 = float;              // Single-precision floating point number.
using f64 = double;             // Double-precision floating point number.

// Validate the type sizes
VALIDATE_SIZE(s8, 1);
VALIDATE_SIZE(u8, 1);
VALIDATE_SIZE(s16, 2);
VALIDATE_SIZE(u16, 2);
VALIDATE_SIZE(s32, 4);
VALIDATE_SIZE(u32, 4);
VALIDATE_SIZE(s64, 8);
VALIDATE_SIZE(u64, 8);
VALIDATE_SIZE(f32, 4);
VALIDATE_SIZE(f64, 8);

#endif
