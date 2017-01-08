// Black roads fix
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  DK22Pac
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

#if 0
unsigned int gCurrentLevel;
RwRaster* gCurrentRaster;

char* LockLevel(RwRaster *raster, unsigned int level, unsigned int mode)
{
    gCurrentLevel = level;
    gCurrentRaster = raster;
    return RwRasterLock(raster, level, mode);
}

unsigned int ReadLevel(RwStream* stream, void* pbuf, unsigned int size)
{
    // If we have a wrong level (and this texture is not corrupted)
    if (size == 0 && gCurrentLevel != 0)
    {
        // Copy the data from previous level
        if (gCurrentRaster->cType == rwRASTERTYPETEXTURE && gCurrentRaster->d3dRaster.hasCompression && gCurrentRaster->d3dRaster.texture)
        {
            D3DLOCKED_RECT rect;
            gCurrentRaster->d3dRaster.texture->LockRect(gCurrentLevel - 1, &rect, NULL, D3DLOCK_READONLY);
            unsigned int sz = 16;
            if (gCurrentRaster->d3dRaster.format == D3DFMT_DXT1)
                sz = 8;
            memcpy(pbuf, rect.pBits, sz);
            gCurrentRaster->d3dRaster.texture->UnlockRect(gCurrentLevel - 1);
            return size;
        }
    }
    return RwStreamRead(stream, pbuf, size);
}
#endif

static Util::HookFunction hookFunction([]()
{
    //MakeCALL(0x4CDCA4, LockLevel);
    //MakeCALL(0x4CDCD9, ReadLevel);
});
