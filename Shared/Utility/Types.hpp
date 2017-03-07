// Numeral types
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#define CTN_INT8_MAX 0x7F
#define CTN_UINT8_MAX 0xFF
#define CTN_INT16_MAX 0x7FFF
#define CTN_UINT16_MAX 0xFFFF
#define CTN_INT32_MAX 0x7FFFFFFF
#define CTN_UINT32_MAX 0xFFFFFFFF
#define CTN_INT64_MAX 0x7FFFFFFFFFFFFFFF
#define CTN_UINT64_MAX 0xFFFFFFFFFFFFFFFF

// Types
using s8 = signed char;
using u8 = unsigned char;
using s16 = short;
using u16 = unsigned short;
using s32 = int;
using u32 = unsigned;
using s64 = long long;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;

using b32 = s32;

// Validate the type sizes
//VALIDATE_SIZE(s8, 1);
//VALIDATE_SIZE(u8, 1);
//VALIDATE_SIZE(s16, 2);
//VALIDATE_SIZE(u16, 2);
//VALIDATE_SIZE(s32, 4);
//VALIDATE_SIZE(u32, 4);
//VALIDATE_SIZE(s64, 8);
//VALIDATE_SIZE(u64, 8);
//
//VALIDATE_SIZE(f32, 4);
//VALIDATE_SIZE(f64, 8);
