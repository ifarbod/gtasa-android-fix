// Win32 necessary headers
// Author(s):       AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

// Macros
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#ifdef WINVER
	#undef WINVER
#endif
#define WINVER 0x0601 // Win7

#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0601 // Win7

// Headers
#include <Windows.h>

// Until custom implementation
#include <cassert>