// Trailer vehicle entity
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

class CTrailer : public CAutomobile
{
public:
    //CColPoint f988[2];
    f32 f9E0;
    f32 f9E4;
    s8 f9E8[4];
    f32 f9EC;
    f32 f9F0;
};

//VALIDATE_SIZE(CTrailer, 0x9F4);

}
