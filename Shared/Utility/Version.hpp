// SAO Version
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Version.h"

// Some definitions
#define MOD_NAME "San Andreas Online"
#define MOD_NAME2 "SAO"
#define MOD_NAME3 "SA:Online"
#define MOD_NAME4 "SA: Online"
#define REGISTRY_AREA "SOFTWARE\\" MOD_NAME

// URI protocol for joining server/game sessions from browser
// Format:      sao://<ip>:<optional:port=7733>
// Examples:    sao://localhost
//              saonline://127.0.0.1:3377
//              sanandreasonline://play.myserver.com:37530
#define SHORT_URI_LAUNCH_1 "sao://"
#define SHORT_URI_LAUNCH_2 "saonline://"
#define SHORT_URI_LAUNCH_3 "sanandreasonline://"
#define SHORT_URI_LAUNCH_4 "sao"
#define SHORT_URI_LAUNCH_5 "saonline"
#define SHORT_URI_LAUNCH_6 "sanandreasonline"

// Library debug suffix
#ifdef SAO_DEBUG
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

#define CLIENT_MPLAUNCHER "SAO"
#define CLIENT_CORE_NAME "Core"

// Version string generation
#define SAO_STRINGIZEX(x) #x
#define SAO_STRINGIZE(x) SAO_STRINGIZEX(x)

#define SAO_VERSION_DOT SAO_VERSION_MAJOR.SAO_VERSION_MINOR.SAO_VERSION_PATCH
#define SAO_VERSION_DOT_RC SAO_VERSION_MAJOR.SAO_VERSION_MINOR.SAO_VERSION_PATCH.0
#define SAO_VERSION_COMMA_RC SAO_VERSION_MAJOR,SAO_VERSION_MINOR,SAO_VERSION_PATCH,0

#ifdef SAO_VERSION_PRE_RELEASE_TYPE
    #define SAO_VERSION_STR SAO_STRINGIZE(SAO_VERSION_MAJOR) "." SAO_STRINGIZE(SAO_VERSION_MINOR) "." SAO_STRINGIZE(SAO_VERSION_PATCH) "-" SAO_VERSION_PRE_RELEASE_TYPE "." SAO_STRINGIZE(SAO_VERSION_PRE_RELEASE)
#else
    #define SAO_VERSION_STR SAO_STRINGIZE(SAO_VERSION_MAJOR) "." SAO_STRINGIZE(SAO_VERSION_MINOR) "." SAO_STRINGIZE(SAO_VERSION_PATCH)
#endif

#define SAO_VERSION_STR_RC SAO_STRINGIZE(SAO_VERSION_DOT)

namespace Util
{

//String MakeVersionString();

}
