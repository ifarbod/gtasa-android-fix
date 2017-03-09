// Player ped entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Ped.hpp"

namespace ctn::SA
{

class CPlayerPed : public CPed
{
public:
    CPed* m_targetedPed;                  // +0x79C
    s32 m_timeWhenTargetMarkerDisappears; // +0x7A0 TimeInMS when mouse target marker disappears
};

//VALIDATE_SIZE(CPlayerPed, 0x7A4);

}
