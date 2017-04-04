// LodTimeModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "LodAtomicModelInfo.hpp"
#include "TimeInfo.hpp"

namespace ctn::SA
{

class CLodTimeModelInfo : public CLodAtomicModelInfo
{
public:
    CTimeInfo m_timeInfo;
};

VALIDATE_SIZE(CLodTimeModelInfo, 0x28);

}
