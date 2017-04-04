// AtomicModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "BaseModelInfo.hpp"

namespace ctn::SA
{

class CAtomicModelInfo : public CBaseModelInfo
{
public:
    // Methods, no additional members
};

VALIDATE_SIZE(CAtomicModelInfo, 0x20);

}
