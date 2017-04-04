// Linker configuration
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"

#pragma comment(linker, "/merge:.data=.cld")
#pragma comment(linker, "/merge:.rdata=.clr")
#pragma comment(linker, "/merge:.cl=.zdata")
#pragma comment(linker, "/merge:.text=.zdata")
#pragma comment(linker, "/section:.zdata,re")

// Prefer the higher performance GPU on Windows systems that use NVIDIA Optimus.
extern "C" __declspec(dllexport) u32 NvOptimusEnablement = 1;
// Same with AMD GPUs.
extern "C" __declspec(dllexport) u32 AmdPowerXpressRequestHighPerformance = 1;
