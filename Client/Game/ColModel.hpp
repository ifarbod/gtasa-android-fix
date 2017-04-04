// ColModel.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "BoundingBox.hpp"
//#include "CollSphere.hpp"
#include "CollisionData.hpp"

namespace ctn::SA
{

class CColModel
{
public:
    // Memory allocation in pool.
    void* operator new(u32 size);
    // Memory deallocation in pool.
    void operator delete(void* ptr);

    CBoundingBox m_boundBox;    // +0x00
    //CColSphere m_boundSphere;   // +0x00
    CCollisionData* m_colData;  // +0x2C
};

//VALIDATE_SIZE(CColModel, 0x30);

}
