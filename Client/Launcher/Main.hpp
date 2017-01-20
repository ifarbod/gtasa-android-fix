// Linker configuration
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdint.h>
#include <stdio.h>

#include <io.h>
#include <Commdlg.h>
#include <Shlwapi.h>

#include "Utils.hpp"
#include "ExecutableLoader.hpp"

#include "GameLauncher.hpp"

#define CTN_REG_KEY HKEY_CURRENT_USER
#define CTN_REG_SUBKEY "SOFTWARE\\CtNorth"

#define GAME_EXECUTABLE_NAME "gta_sa.exe"

extern char g_szGamePath[MAX_PATH];
extern char g_szGameExecutable[MAX_PATH];
extern char g_szExecutablePath[MAX_PATH];
