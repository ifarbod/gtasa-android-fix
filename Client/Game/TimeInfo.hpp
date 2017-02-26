// Time info for modelinfo
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CTimeInfo
{
public:
    CTimeInfo* FindOtherTimeModel(const char* name);

public:
    u8 m_timeOn;
    u8 m_timeOff;
    u16 m_otherModelId;
};

}
