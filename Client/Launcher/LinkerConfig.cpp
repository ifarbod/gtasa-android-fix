// Linker configuration
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"

#pragma comment(linker, "/merge:.data=.cld")
#pragma comment(linker, "/merge:.rdata=.clr")
#pragma comment(linker, "/merge:.cl=.zdata")
#pragma comment(linker, "/merge:.text=.zdata")
#pragma comment(linker, "/section:.zdata,re")

extern "C"
{

// Prefer the higher performance GPU on Windows systems that use nvidia Optimus.
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
// TODO: Re-evaluate in the future when the average integrated GPU in Optimus
// systems is less mediocre?
__declspec(dllexport) int NvOptimusEnablement = 1;

// Same with AMD GPUs.
// https://community.amd.com/thread/169965
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

}
