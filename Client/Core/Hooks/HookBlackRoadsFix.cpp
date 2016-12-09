// Black roads fix
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  DK22Pac
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

static Util::HookFunction hookFunction([]()
{
    //MakeCALL(0x4CDCA4, LockLevel);
    //MakeCALL(0x4CDCD9, ReadLevel);
});
