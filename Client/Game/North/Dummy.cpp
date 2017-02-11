// Dummy.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Dummy.hpp"

namespace ctn::SA
{

void* CDummy::operator new(u32 size)
{
    return nullptr;
}

void CDummy::operator delete(void* ptr)
{
}

}
