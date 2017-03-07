// Crash fixes
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/Hook.hpp>
#include <Hooking/HookFunction.hpp>

using namespace ctn;
using namespace ctn::Hook;

const char* __fastcall CText__Get_Custom(void* text, int dummy, const char* gxt)
{
    return gxt;
}

static HookFunction hookFunction([]()
{
    // Disable CMenuManager::InitialiseChangedLanguageSettings
    MakeRet(0x573260);

    // Disable InitialiseLanguage
    MakeRet(0x7465B0);

    // Disable calling CText::Load in CGame::InitialiseEssentialsAfterRW
    MakeRangedNop(0x5BA160, 0x5BA167 + 5);

    // Disable CText::LoadMissionText
    MakeRet(0x69FBF0);

    // Redirect CText::Get
    MakeJmp(0x6A0050, CText__Get_Custom);
});
