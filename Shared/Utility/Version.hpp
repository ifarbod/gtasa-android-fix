// Versioning helpers
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Version.h>

// Some definitions
#define MOD_NAME "CtNorth"
#define MOD_NAME_ALT "CTN"
#define REGISTRY_AREA "SOFTWARE\\" MOD_NAME

// URI protocol for joining server/game sessions from browser
#define SHORT_URI_LAUNCH_1 "ctn://"
#define SHORT_URI_LAUNCH_2 "ctnorth://"
#define SHORT_URI_LAUNCH_4 "ctn"
#define SHORT_URI_LAUNCH_5 "ctnorth"

// Library debug suffix
#ifdef CTN_DEBUG
#define DEBUG_SUFFIX "_d"
#else
#define DEBUG_SUFFIX
#endif

// Library extension
#ifdef _WIN32
#define LIB_EXTENSION ".dll"
#else
#define LIB_EXTENSION ".so"
#endif

#define CLIENT_MPLAUNCHER "CTN"
#define CLIENT_CORE_NAME "Core"

// Version string generation
#define CTN_STRINGIZEX(x) #x
#define CTN_STRINGIZE(x) CTN_STRINGIZEX(x)

#define CTN_VERSION_DOT_RC CTN_VERSION_MAJOR.CTN_VERSION_MINOR.CTN_VERSION_PATCH.0
#define CTN_VERSION_COMMA_RC CTN_VERSION_MAJOR,CTN_VERSION_MINOR,CTN_VERSION_PATCH,0

#ifdef CTN_VERSION_PRE_RELEASE_TYPE
    #define CTN_VERSION_STR "v" CTN_STRINGIZE(CTN_VERSION_MAJOR) "." CTN_STRINGIZE(CTN_VERSION_MINOR) "." CTN_STRINGIZE(CTN_VERSION_PATCH) "-" CTN_VERSION_PRE_RELEASE_TYPE "." CTN_STRINGIZE(CTN_VERSION_PRE_RELEASE)
    #define CTN_VERSION_STR_NOV CTN_STRINGIZE(CTN_VERSION_MAJOR) "." CTN_STRINGIZE(CTN_VERSION_MINOR) "." CTN_STRINGIZE(CTN_VERSION_PATCH) "-" CTN_VERSION_PRE_RELEASE_TYPE "." CTN_STRINGIZE(CTN_VERSION_PRE_RELEASE)
#else
    #define CTN_VERSION_STR "v" CTN_STRINGIZE(CTN_VERSION_MAJOR) "." CTN_STRINGIZE(CTN_VERSION_MINOR) "." CTN_STRINGIZE(CTN_VERSION_PATCH)
    #define CTN_VERSION_STR_NOV CTN_STRINGIZE(CTN_VERSION_MAJOR) "." CTN_STRINGIZE(CTN_VERSION_MINOR) "." CTN_STRINGIZE(CTN_VERSION_PATCH)
#endif

#define CTN_VERSION_STR_RC CTN_STRINGIZE(CTN_VERSION_DOT_RC)
