// Linker configuration
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define M_MIN(a,b) (((a) < (b)) ? (a) : (b))

#include <stdint.h>
#include <stdio.h>

#include <io.h>
#include <Commdlg.h>
#include <Shlwapi.h>

#include "Utils.h"
#include "ExecutableLoader.h"

#include "Game.h"

#define SAO_REG_KEY HKEY_CURRENT_USER
#define SAO_REG_SUBKEY "SOFTWARE\\San Andreas Online"

#define GAME_EXECUTABLE_NAME "SA4g.exe"

extern char g_szGamePath[MAX_PATH];
extern char g_szGameExecutable[MAX_PATH];
extern char g_szExecutablePath[MAX_PATH];
