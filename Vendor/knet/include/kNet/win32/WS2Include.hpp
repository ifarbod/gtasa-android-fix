// WS2Include.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

// Use this file to include winsock2.h. This file fixes the issue where winsock2.h has to be included before windows.h.
//
//    The source of this error comes from the fact that if <windows.h> is included before <winsock2.h>, <windows.h> will include
//    <winsock.h>, which is the older version of the winsock library. After that, if the user includes winsock2.h, he will get
//    compilation errors.
//
//    To fix this problem, do one of the following:
//
//    1) Alter the inclusion order in the compilation unit(s) so that you explicitly #include <winsock2.h> before including
//       windows.h.
//    2) Add a #define _WINSOCKAPI_ either to project settings, or at the top of the offending compilation unit (before including
//       windows.h). Use this solution when you don't want to alter the inclusion order to the proper desired order (winsock2.h
//       before windows.h)

#if defined(_WINDOWS_) && !defined(_WINSOCK2API_) && defined(FD_CLR)
#error Error: Trying to include winsock2.h after windows.h! This is not allowed! See this file for fix instructions.
#endif

// Remove the manually added #define if it exists so that winsock2.h includes OK.
#if !defined(_WINSOCK2API_) && defined(_WINSOCKAPI_)
#undef _WINSOCKAPI_
#endif

#ifdef __MINGW32__
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
