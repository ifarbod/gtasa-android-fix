// AnimBlendHierarchy.hpp
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

class CAnimBlendSequence;

class CAnimBlendHierarchy
{
public:
    u32 m_hashKey;
    CAnimBlendSequence* m_sequences;
    u16 m_seqCount;
    bool m_runningCompressed;
    s8 m_field0B;
    s32 m_animBlockId;
    f32 m_totalTime;
    s32 m_field14;
};

VALIDATE_SIZE(CAnimBlendHierarchy, 0x18);

}
