// Numeral types
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

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

union Float32
{
    f32 f;
    u32 i;
    s32 s;

    Float32(f32 n) { f = n; }
    Float32(u32 n) { i = n; }
    Float32(s32 n) { s = n; }
};

union Float64
{
    f64 f;
    u64 i;
    s64 s;

    Float64(f64 n) { f = n; }
    Float64(u64 n) { i = n; }
    Float64(s64 n) { s = n; }
};
