// Abstract task
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Task.hpp"

namespace ctn::SA
{

void* CTask::operator new(u32 size)
{
    return nullptr;
}

void CTask::operator delete(void* ptr)
{
}

}
