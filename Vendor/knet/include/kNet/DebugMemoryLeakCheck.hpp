// DebugMemoryLeakCheck.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file DebugMemoryLeakCheck.h
    @brief Provides overloads of operators new and delete for tracking memory leaks. */

#if defined(_MSC_VER) && defined(_DEBUG) && defined(KNET_MEMORY_LEAK_CHECK)

#include <new>
#include <crtdbg.h>

// On MSVC2008, include these files beforehand to avoid compilation errors from our operator new redefine.
#if _MSC_VER == 1500
#include <ios>
#include <map>
#endif

#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif

__forceinline void *operator new(size_t size, const char *file, int line)
{
    return _malloc_dbg(size, _NORMAL_BLOCK, file, line);
}

__forceinline void *operator new[](size_t size, const char *file, int line)
{
    return _malloc_dbg(size, _NORMAL_BLOCK, file, line);
}

__forceinline void operator delete(void *ptr, const char *, int)
{
    _free_dbg(ptr, _NORMAL_BLOCK);
}

__forceinline void operator delete[](void *ptr, const char *, int)
{
    _free_dbg(ptr, _NORMAL_BLOCK);
}

__forceinline void *operator new(size_t size)
{
#ifdef DEBUG_CPP_NAME
    return _malloc_dbg(size, _NORMAL_BLOCK, DEBUG_CPP_NAME, 1);
#else
    return _malloc_dbg(size, _NORMAL_BLOCK, "(No CPP Name)", 1);
#endif
}

__forceinline void *operator new[](size_t size)
{
#ifdef DEBUG_CPP_NAME
    return _malloc_dbg(size, _NORMAL_BLOCK, DEBUG_CPP_NAME " new[]", 1);
#else
    return _malloc_dbg(size, _NORMAL_BLOCK, "(No CPP Name new[])", 1);
#endif
}

__forceinline void operator delete(void *ptr)
{
    _free_dbg(ptr, _NORMAL_BLOCK);
}

__forceinline void operator delete[](void *ptr)
{
    _free_dbg(ptr, _NORMAL_BLOCK);
}

#define new new (__FILE__, __LINE__)

#endif

