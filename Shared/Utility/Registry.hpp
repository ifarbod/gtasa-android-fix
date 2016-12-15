// Wrapper for win32's registry functions
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Str.hpp>

namespace Util
{

enum KeyRoot : unsigned
{
    HKCR = 0x80000000,
    HKCU = 0x80000001,
    HKLM = 0x80000002,
    HKU = 0x80000003,
    HKCC = 0x80000005
};

bool WriteRegStr(KeyRoot rootKey, const String& subKey, const String& valueName, const String& value, bool flush = false);
String ReadRegStr(KeyRoot rootKey, const String& subKey, const String& valueName, bool *result = nullptr);
bool DeleteRegKey(KeyRoot rootKey, const String& subKey);
bool DeleteRegValue(KeyRoot rootKey, const String& subKey, const String& valueName);

// These functions read from SAO's registry area
bool RegWriteStr(const String& valueName, const String& value);
String RegReadStr(const String& valueName);
bool RegDeleteStr(const String& valueName);

}
