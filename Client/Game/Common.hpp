// Common stuff
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Hooking/Hook.hpp>

// Padding
#define PAD(prefix, name, size) u8 _##prefix##_##name##_[size]

using b32 = s32;                // 4-bytes, to represent a boolean with 1 as true, and 0 as false.
