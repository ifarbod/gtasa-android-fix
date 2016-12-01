// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Utils.hpp"
#include <Container/Str.hpp>
#include <Path.hpp>
#include <ProcessUtils.hpp>
#include <Registry.hpp>

using namespace Util;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR strCmdLine, int nCmdShow)
{
    // Check some stuff before launching
    if (!DoPreLaunchChecks())
    {
        return 1;
    }

    // Launch SA

    // Success
    return 0;
}
