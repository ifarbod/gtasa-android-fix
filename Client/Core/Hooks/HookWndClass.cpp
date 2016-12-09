// Window class
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

ATOM __stdcall InitApplication(HINSTANCE instance)
{
    WNDCLASS WndClass;

    WndClass.style = CS_BYTEALIGNWINDOW;
    WndClass.lpfnWndProc = (WNDPROC)0x747EB0;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = instance;
    WndClass.hIcon = LoadIconW(instance, (wchar_t*)1);
    WndClass.hCursor = LoadCursorW(instance, (wchar_t*)0x7F00);
    WndClass.hbrBackground = 0;
    WndClass.lpszMenuName = 0;
    WndClass.lpszClassName = L"Grand theft auto San Andreas"; // TODO: Change this
    return RegisterClassW(&WndClass);
}

static HookFunction hookFunction([]()
{
    // Use our custom icon and WndProc
    //MakeCALL(0x74875B, InitApplication);
});
