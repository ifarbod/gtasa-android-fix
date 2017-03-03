// EntryInfoList.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "EntryInfoNode.hpp"

namespace ctn::SA
{

class CEntryInfoList
{
public:
    CEntryInfoNode* m_lastEntry; // +0x0
};

VALIDATE_SIZE(CEntryInfoList, 0x4);

}
