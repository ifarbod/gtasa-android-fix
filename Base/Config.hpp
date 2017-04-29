// Cross-platform build configuration
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

// This file adds defines about the platform we're currently building on.
//  Operating System:
//    CTN_OS_WIN / CTN_OS_MACOSX / CTN_OS_LINUX / CTN_OS_POSIX (MACOSX or LINUX) /
//  Compiler:
//    COMPILER_MSVC / COMPILER_GCC / COMPILER_CLANG
//  Processor:
//    ARCH_CPU_X86 / ARCH_CPU_X86_64 / ARCH_CPU_X86_FAMILY (X86 or X86_64)
//    ARCH_CPU_32_BITS / ARCH_CPU_64_BITS

#pragma once

#include <Types.hpp>
#include <Version.hpp>

// A set of macros to use for platform detection.
// Android
#if defined(__ANDROID__) || defined(ANDROID)
    #define CTN_OS_ANDROID 1

// Apple
// Only include TargetConditions after testing ANDROID as some android builds
// on mac don't have this header available and it's not needed unless the target
// is really mac/ios.
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
#define CTN_OS_MACOSX 1

#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
    #define CTN_OS_IOS 1
#endif

#elif defined(_WIN32)
    #define CTN_OS_WINDOWS 1

#elif defined(__linux__) && !defined(ANDROID) && !defined(__ANDROID__)
    #define CTN_OS_LINUX 1
    // Include a system header to pull in features.h for glibc/uclibc macros.
    #include <unistd.h>
    #if defined(__GLIBC__) && !defined(__UCLIBC__)
        // We really are using glibc, not uClibc pretending to be glibc
        #define LIBC_GLIBC 1
    #endif

#elif defined(__FreeBSD__)
    #define CTN_OS_FREEBSD 1
#elif defined(__NetBSD__)
    #define CTN_OS_NETBSD 1
#elif defined(__OpenBSD__)
    #define CTN_OS_OPENBSD 1
#elif defined(__DragonFly__)
    #define CTN_OS_DRAGONFLY 1
#elif defined(__bsdi__)
    #define CTN_OS_BSDI 1
#elif defined(__sun)
    #define CTN_OS_SOLARIS 1
#elif defined(__QNXNTO__)
    #define CTN_OS_QNX 1
#else
    #error Please add support for your platform in Base/Config.hpp
#endif

// For access to standard BSD features, use OS_BSD instead of a
// more specific macro.
#if defined(CTN_OS_FREEBSD) || defined(CTN_OS_NETBSD) || defined(CTN_OS_OPENBSD) || defined(CTN_OS_DRAGONFLY) ||       \
    defined(CTN_OS_BSDI)
    #define CTN_OS_BSD 1
#endif

// For access to standard POSIXish features, use OS_POSIX instead of a
// more specific macro.
#if defined(CTN_OS_MACOSX) || defined(CTN_OS_LINUX) || defined(CTN_OS_ANDROID) || defined(CTN_OS_FREEBSD) || defined(CTN_OS_NETBSD) ||     \
    defined(CTN_OS_OPENBSD) || defined(CTN_OS_SOLARIS) || defined(CTN_OS_QNX)

    #define CTN_OS_POSIX 1
#endif

// Compiler detection.
#if defined(__GNUC__)
    #define CTN_COMPILER_GCC 1
#elif defined(_MSC_VER)
    #define CTN_COMPILER_MSVC 1
#elif defined(__clang__)
    #define CTN_COMPILER_CLANG 1
#else
    #error Please add support for your compiler in Base/Config.hpp
#endif


// Processor architecture detection.  For more info on what's defined, see:
// http://msdn.microsoft.com/en-us/library/b0084kay.aspx
// http://www.agner.org/optimize/calling_conventions.pdf
// or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_IX86) || defined(__i386__) || defined(__i386) || defined(_X86_) || defined(__I86__) || defined(__INTEL__)
#define CTN_ARCH_CPU_X86_FAMILY 1
#define CTN_ARCH_CPU_X86 1
#define CTN_ARCH_CPU_32_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_X64) || defined(__x86_64__)
#define CTN_ARCH_CPU_X86_FAMILY 1
#define CTN_ARCH_CPU_X86_64 1
#define CTN_ARCH_CPU_64_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__s390x__)
#define CTN_ARCH_CPU_S390_FAMILY 1
#define CTN_ARCH_CPU_S390X 1
#define CTN_ARCH_CPU_64_BITS 1
#define CTN_ARCH_CPU_BIG_ENDIAN 1
#elif defined(__s390__)
#define CTN_ARCH_CPU_S390_FAMILY 1
#define CTN_ARCH_CPU_S390 1
#define CTN_ARCH_CPU_31_BITS 1
#define CTN_ARCH_CPU_BIG_ENDIAN 1
#elif defined(__PPC64__) && defined(__BIG_ENDIAN__)
#define CTN_ARCH_CPU_PPC64_FAMILY 1
#define CTN_ARCH_CPU_PPC64 1
#define CTN_ARCH_CPU_64_BITS 1
#define CTN_ARCH_CPU_BIG_ENDIAN 1
#elif defined(__PPC64__) && defined(__LITTLE_ENDIAN__)
#define CTN_ARCH_CPU_PPC64_FAMILY 1
#define CTN_ARCH_CPU_PPC64 1
#define CTN_ARCH_CPU_64_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__PPC__)
#define CTN_ARCH_CPU_PPC_FAMILY 1
#define CTN_ARCH_CPU_PPC 1
#define CTN_ARCH_CPU_32_BITS 1
#define CTN_ARCH_CPU_BIG_ENDIAN 1
#elif defined(__ARMEL__) || defined(_M_ARM) || defined(__arm__) || defined(__aarch32__)
#define CTN_ARCH_CPU_ARM_FAMILY 1
#define CTN_ARCH_CPU_ARMEL 1
#define CTN_ARCH_CPU_32_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm64__)
#define CTN_ARCH_CPU_ARM_FAMILY 1
#define CTN_ARCH_CPU_ARM64 1
#define CTN_ARCH_CPU_64_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__pnacl__)
#define CTN_ARCH_CPU_32_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__MIPSEL__)
#if defined(__LP64__)
#define CTN_ARCH_CPU_MIPS_FAMILY 1
#define CTN_ARCH_CPU_MIPS64EL 1
#define CTN_ARCH_CPU_64_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#else
#define CTN_ARCH_CPU_MIPS_FAMILY 1
#define CTN_ARCH_CPU_MIPSEL 1
#define CTN_ARCH_CPU_32_BITS 1
#define CTN_ARCH_CPU_LITTLE_ENDIAN 1
#endif
#else
#error Please add support for your architecture in Base/Config.hpp
#endif

// C++17 support
#if defined(__cplusplus) && __cplusplus >= 201703
    #define CTN_CXX17 1
// G++ 7.0 and later has full C++17 support
#elif defined(__GNUC__) && __GNUC__ == 7 && __GNUC_MINOR__ == 0
    #define CTN_CXX17 1
// MSVC 2017 and later has partial C++17 support
#elif defined(_MSC_VER) && _MSC_FULL_VER >= 191025017
    #define CTN_CXX17 1
// Clang 4.0 and later has full C++17 support
#elif defined(__clang__) && __clang_major__ == 4 && __clang_minor__ == 0
    #define CTN_CXX17 1
#else
#error Please add support for your compiler in Base/Config.hpp
#endif

// This macro is used to avoid warnings about unused variables on certain compilers.
// Usually where the variables are only used in debug mode.
#ifndef CTN_UNUSED
    #define CTN_UNUSED(x) static_cast<void>(sizeof(x))
#endif

// Shared library stuff
#ifdef CTN_COMPILE_SHARED_LIB
    #ifdef CTN_OS_WINDOWS
        #ifdef CTN_BASE_IMPLEMENTATION
            #define CTN_API __declspec(dllexport)
        #else
            #define CTN_API __declspec(dllimport)
        #endif
    #else
        #ifdef CTN_BASE_IMPLEMENTATION
            #define CTN_API __attribute__((visibility("default")))
        #else
            #define CTN_API
        #endif
    #endif
#else
    #define CTN_API
#endif
