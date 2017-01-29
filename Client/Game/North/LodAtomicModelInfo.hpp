// LodAtomicModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "AtomicModelInfo.hpp"

namespace ctn::SA
{

class CLodAtomicModelInfo : public CAtomicModelInfo
{
public:
    u16 f20_;
    u16 f22_;
};

//VALIDATE_SIZE(CLodAtomicModelInfo, 0x24);

}
