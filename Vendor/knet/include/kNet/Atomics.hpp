// Abstracts platform-specific atomics operations.
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

// bool CmpXChgPointer(word *dst, word newVal, word cmp);
// Assigns *dst = newVal, but only if *dst == cmp before the assignment. This evaluation is done atomically.
// Returns true if the assignment succeeded.
// Do NOT call this macro with any side expressions on dst, newVal or cmp.

#ifdef _WIN32
// See http://msdn.microsoft.com/en-us/library/ms683568(VS.85).aspx
#define CmpXChgPointer(dst, newVal, cmp) (InterlockedCompareExchangePointer((dst), (newVal), (cmp)) == (cmp))
#else
// See http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html
#define CmpXChgPointer(dst, newVal, cmp) __sync_bool_compare_and_swap((dst), (cmp), (newVal))
#endif
