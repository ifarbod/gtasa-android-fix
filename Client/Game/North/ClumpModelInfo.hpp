// ClumpModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
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
        char* animFileName_;
        unsigned int animFileIndex_; // animBlock_
    };
};

//VALIDATE_SIZE(CClumpModelInfo, 0x24);

}
