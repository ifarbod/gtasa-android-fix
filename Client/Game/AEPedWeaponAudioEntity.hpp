// Ped weapon audio entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

struct CAEPedWeaponAudioEntity
{
    u8 pad[0xA8];
};

VALIDATE_SIZE(CAEPedWeaponAudioEntity, 0xA8);

}
