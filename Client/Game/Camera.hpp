// Camera.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Placeable.hpp"
#include "Cam.hpp"

namespace ctn::SA
{

class CCamera : public CPlaceable
{
public:
    virtual ~CCamera() = 0;

public:
    CCam m_cams[3];
};

//VALIDATE_SIZE(CCamera, 0xD78);

}
