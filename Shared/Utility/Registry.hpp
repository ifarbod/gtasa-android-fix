// Wrapper for win32's registry functions
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Str.hpp"

namespace Util
{

enum KeyRoot : unsigned
{
    HKCR = 0x80000000,
    HKCU = 0x80000001,
    HKLM = 0x80000002,
    HKU = 0x80000003
};

bool WriteRegStr(KeyRoot keyRoot, const String& subKey, const String& keyName, const String& value, bool flush = false);
String ReadRegStr(KeyRoot keyRoot, const String& subKey, const String& keyName, bool *result = nullptr);

}
