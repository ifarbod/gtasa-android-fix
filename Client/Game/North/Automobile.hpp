// Automobile.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vehicle.hpp"

// Vortex-related fields (4D Vector/Quaternion) @ 0x6C9357:
// CAutomobile+0x968
// CAutomobile+0x96C
// CAutomobile+0x970
// CAutomobile+0x974

namespace ctn::SA
{

class CAutomobile : public CVehicle
{
public:

};

//VALIDATE_SIZE(CAutomobile, 0x988);

}
