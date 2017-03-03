// EntryInfoNode.hpp
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

class CEntryInfoNode
{
public:
    CEntryInfoNode* m_field00; // +0x00 Previous?
    u32 m_field04;             // +0x04
    u32 m_field08;             // +0x08
    u32 m_field0C;             // +0x0C
    CEntryInfoNode* m_field10; // +0x10 Next?
};

VALIDATE_SIZE(CEntryInfoNode, 0x14);

}
