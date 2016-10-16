// Memory helper utilities
// Author(s):       AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Util.Preprocessors.hpp"

_UTILS_BEGIN

// Safer Mem Copy with C++ + No need to * size bytes
template<class T>
void MemCopy(T* pvDestination, T* pvSource, u32 uiSize, u32 uiPosToStart = 0)
{
    if(uiSize == 0 || uiPosToStart >= uiSize) return;

    for(u32 ui = uiPosToStart; ui < uiSize; ui++) *pvDestination++ = *pvSource++;
}

_UTILS_END