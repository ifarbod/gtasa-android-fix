// Config.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

// Platform stuff
#if defined(_WIN32)
    #define CTN_WINDOWS 1
    #if defined(WIN32) || defined(_M_IX86)
        #define CTN_WINDOWS32 1
    #elif defined(WIN64) || defined(_WIN64) || defined(_M_AMD64) || defined(_M_X64)
        #define CTN_WINDOWS64 1
    #endif
#endif

// Linux
#if defined(__linux__) && !defined(ANDROID) && !defined(__ANDROID__)
    #define CTN_LINUX 1
#endif

// BSD
#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__)
    #define CTN_BSD 1
    // For detecting the BSD version
    #if defined(__FreeBSD__)
        #define CTN_FREEBSD 1
    #elif defined(__DragonFly__)
        #define CTN_DRAGONFLY 1
    #elif defined(__NetBSD__)
        #define CTN_NETBSD 1
    #elif defined(__OpenBSD__)
        #define CTN_OPENBSD 1
    #elif defined(__bsdi__)
        #define CTN_BSDI 1
    #endif
    // It's not linux, but it seems most "Linux-only" code is BSD-compatible
    #define CTN_LINUX 1
#endif

// Apple
#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define CTN_IOS 1
    #elif TARGET_OS_MAC
        #define CTN_MACOSX 1
    #endif
#endif

// Android
#if defined(__ANDROID__) || defined(ANDROID)
    #define CTN_ANDROID 1
#endif

// Compiler stuff
#if __cplusplus >= 201103
    #define CTN_CXX11 1
#elif defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ == 8 && __GNUC_PATCHLEVEL__ == 1
    #define CTN_CXX11 1
#elif defined(_MSC_VER) && _MSC_VER >= 1800
    #define CTN_CXX11 1
// Clang 3.3 and later has full C++11 support
#elif defined(__clang__) && __clang_major__ == 3 && __clang_minor__ == 3
    #define CTN_CXX11 1
#endif

// C++14 support
#if __cplusplus >= 201402
    #define CTN_CXX14 1
#elif defined(__GNUC__) && __GNUC__ == 5 && __GNUC_MINOR__ == 0
    #define CTN_CXX14 1
#elif defined(_MSC_VER) && _MSC_VER >= 1900
    #define CTN_CXX14 1
// Clang 3.4 and later has full C++14 support
#elif defined(__clang__) && __clang_major__ == 3 && __clang_minor__ == 4
    #define CTN_CXX14 1
#endif

// This macro is used to avoid warnings about unused variables on certain compilers.
// Usually where the variables are only used in debug mode.
#ifndef CTN_UNUSED
    #define CTN_UNUSED(x) static_cast<void>(sizeof(x))
#endif

// DLL-stuff.
#ifdef CTN_WINDOWS
    #define CTN_API __declspec(dllexport)
#else
    #define CTN_API
#endif

// Check if we have a sane configuration
#if !defined(CTN_WINDOWS) && !defined(CTN_LINUX) && !defined(CTN_MACOSX) && !defined(CTN_ANDROID)
    #error Could not detect target platform
#endif
#if defined(CTN_IOS)
    #error Compiling for iOS is not allowed
#endif
#if !defined(CTN_CXX11) && !defined(CTN_CXX14)
    #error C++11 & C++14 is required to compile, use at least VS2015 with Update 3
#endif
