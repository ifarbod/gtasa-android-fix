// Core module
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Core.hpp"

#include <Hooking/HookFunction.hpp>

using namespace Util;

Core::Core()
{
    HookFunction::RunAll();
}

Core::~Core()
{
}
