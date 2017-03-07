// PedAttractor.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Entity.hpp"

namespace ctn::SA
{

class CPedAttractor
{
public:
    virtual ~CPedAttractor() = 0;

private:
    class C2dEffect* m_2dEffect;     // +0x04
    CEntity* m_entity;                // +0x08
    PAD(CPedAttractor, pad0C, 0xB8); // +0x0C
};

// Not actual size, but biggest child of this class, for pool allocation
VALIDATE_SIZE(CPedAttractor, 0xC4);

}
