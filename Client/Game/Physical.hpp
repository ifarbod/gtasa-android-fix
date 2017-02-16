// Physical entity base
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Entity.hpp"

namespace ctn::SA
{

class CPhysical : public CEntity
{
public:
    f32 f038_;

};

//VALIDATE_SIZE(CPhysical, 0x138);

}
