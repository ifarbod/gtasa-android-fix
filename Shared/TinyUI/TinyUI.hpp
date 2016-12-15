// TinyUI - A minimalistic Win32 GUI library
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "TConfig.hpp"

#ifndef TINYUI_WCHAR_MODE
#   ifdef UNICODE
#       define TINYUI_WCHAR_MODE
#   endif
#endif

#ifdef TINYUI_WCHAR_MODE
#   define TINYUI_TEXT(x) L ## x
#   define TINYUI_CHAR wchar_t
#else
#   define TINYUI_TEXT(x) x
#   define TINYUI_CHAR char
#endif

#ifndef TINYUI_API
#    ifdef TINYUI_BUILD_SHAREDLIBRARY
#        ifdef TINYUI_EXPORTS
#            define TINYUI_API __declspec(dllexport)
#        else
#            define TINYUI_API __declspec(dllimport)
#        endif
#    else
#        define TINYUI_API
#    endif
#endif

#ifndef TINYUI_CLASS
#   define TINYUI_CLASS TINYUI_API
#endif

#ifndef TINYUI_FUNCTION
#   define TINYUI_FUNCTION TINYUI_API
#endif

#include "TVector.hpp"

namespace TinyUI
{

using TChar = TINYUI_CHAR;

}

#ifndef TINYUI_NO_TUI
namespace TUI = TinyUI;
#endif
