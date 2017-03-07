// String hash
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/MathDefs.hpp>
#include <Math/StringHash.hpp>

namespace ctn
{

const StringHash StringHash::ZERO;

StringHash::StringHash(const char* str) :
    value_(Calculate(str))
{
}

StringHash::StringHash(const String& str) :
    value_(Calculate(str.CString()))
{
}

unsigned StringHash::Calculate(const char* str)
{
    unsigned hash = 0;

    if (!str)
        return hash;

    while (*str)
    {
        // Perform the actual hashing as case-insensitive
        char c = *str;
        hash = SDBMHash(hash, (unsigned char)tolower(c));
        ++str;
    }

    return hash;
}

String StringHash::ToString() const
{
    return String::Format("%08X", value_);
}

}
