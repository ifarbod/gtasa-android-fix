// Dummy variables to store game code
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//                  P3ti
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"

#pragma bss_seg(".cdummy")
char dummy_seg[0x2500000];

#pragma data_seg(".zdata")
char zdata[0x50000] = { 1 };
