// Automobile vehicle entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vehicle.hpp"
#include "DamageManager.hpp"

// Vortex-related fields (4D Vector/Quaternion) @ 0x6C9357:
// TODO: Check them
// CAutomobile+0x968
// CAutomobile+0x96C
// CAutomobile+0x970
// CAutomobile+0x974

namespace ctn::SA
{

class CAutomobile : public CVehicle
{
public:

private:
    CDamageManager m_damageManager;
    //CDoor m_doors[6];
    //RwFrame* m_aCarNodes[CAR_NUM_NODES];
    //CPanel m_panels[3];
    //CDoor m_swingingChassis;
    //CColPoint m_wheelColPoint[4];
};

//VALIDATE_SIZE(CAutomobile, 0x988);

}
