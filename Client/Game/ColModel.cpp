// ColModel.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "ColModel.hpp"

namespace ctn::SA
{

void* CColModel::operator new(u32 size)
{
    return nullptr;
}

void CColModel::operator delete(void* ptr)
{
}

}
