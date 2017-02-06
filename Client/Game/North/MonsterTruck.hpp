// MonsterTruck.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Automobile.hpp"

namespace ctn::SA
{

class CMonsterTruck : public CAutomobile
{
public:
    f32 f988_;
    f32 f98C_;
    f32 f990_;
    f32 f994_;
    f32 f998_;
};

//VALIDATE_SIZE(CMonsterTruck, 0x99C);

}
