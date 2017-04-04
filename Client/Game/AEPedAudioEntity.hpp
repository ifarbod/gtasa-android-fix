// Ped audio entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

struct CAEPedAudioEntity
{
    u8 pad[0x15C];
};

VALIDATE_SIZE(CAEPedAudioEntity, 0x15C);

}
