// Memory allocation debugging helper
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#if defined(_MSC_VER) && defined(CTN_DEBUG)

#define _CRTDBG_MAP_ALLOC

#ifdef _malloca
#undef _malloca
#endif

#include <crtdbg.h>

#define CTN_DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new CTN_DEBUG_NEW

#endif
