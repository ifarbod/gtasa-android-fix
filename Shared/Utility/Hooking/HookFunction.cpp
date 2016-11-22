// Static hook installer
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "PCH.hpp"
#include "HookFunction.hpp"

namespace Util
{

static HookFunctionBase* hookFunctions;

void HookFunctionBase::Register()
{
    next_ = hookFunctions;
    hookFunctions = this;
}

void HookFunctionBase::RunAll()
{
    for (auto func = hookFunctions; func; func = func->next_)
    {
        func->Run();
    }
}

}
