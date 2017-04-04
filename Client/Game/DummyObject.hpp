// Dummy object entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Dummy.hpp"

namespace ctn::SA
{

class CDummyObject : public CDummy
{
public:

};

VALIDATE_SIZE(CDummyObject, 0x38);

}
