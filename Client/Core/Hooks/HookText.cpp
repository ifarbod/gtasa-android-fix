// Crash fixes
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

const char* __fastcall CText__Get_Custom(void* text, int dummy, const char* gxt)
{
    return gxt;
}

static Util::HookFunction hookFunction([]()
{
    // Disable CMenuManager::InitialiseChangedLanguageSettings
    MakeRet(0x573260);

    // Disable InitialiseLanguage
    MakeRet(0x7465B0);

    // Disable calling CText::Load in CGame::InitialiseEssentialsAfterRW
    MakeRangedNOP(0x5BA160, 0x5BA167 + 5);

    // Disable CText::LoadMissionText
    MakeRet(0x69FBF0);

    // Redirect CText::Get
    MakeJmp(0x6A0050, CText__Get_Custom);
});
