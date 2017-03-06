// PlayerPed.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
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
    CPed* m_targettedPed; // +0x79C
    s32 m_field7A0;       // +0x7A0 TimeInMS related to 3rd person aiming
};

//VALIDATE_SIZE(CPlayerPed, 0x7A4);

}
