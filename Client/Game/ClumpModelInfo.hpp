// ClumpModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "BaseModelInfo.hpp"

namespace ctn::SA
{

class CClumpModelInfo : public CBaseModelInfo
{
public:
    union
    {
        char* m_animFileName;
        u32 m_animFileIndex; // m_animBlock
    };
};

VALIDATE_SIZE(CClumpModelInfo, 0x24);

}
