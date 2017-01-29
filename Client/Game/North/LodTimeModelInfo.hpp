// LodTimeModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "LodAtomicModelInfo.hpp"

namespace ctn::SA
{

class CLodTimeModelInfo : public CLodAtomicModelInfo
{
public:
    //CTimeInfo timeInfo;
};

//VALIDATE_SIZE(CLodTimeModelInfo, 0x24);

}
