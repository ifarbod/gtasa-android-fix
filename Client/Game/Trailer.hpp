// Trailer vehicle entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
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
    //CColPoint m_field988[2];
    f32 m_field9E0;
    f32 m_field9E4;
    s8 m_field9E8[4];
    f32 m_field9EC;
    f32 m_field9F0;
};

//VALIDATE_SIZE(CTrailer, 0x9F4);

}
