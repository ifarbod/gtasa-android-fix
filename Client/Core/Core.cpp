// Core module
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Core.hpp"

#include <Hooking/HookFunction.hpp>

Core::Core()
{
    ctn::Hook::HookFunction::RunAll();
}

Core::~Core()
{
}
