// CEF launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int __declspec(dllimport) InitCEF();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR strCmdShow, int nCmdShow)
{
    return InitCEF();
}
