// TimeInfo.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "TimeInfo.hpp"

namespace ctn::SA
{

CTimeInfo* CTimeInfo::FindOtherTimeModel(const char* name)
{
#if 0
    String str = name;
    String suffix;

    if (str.EndsWith("_nt"))
    {
        suffix = "_nt";
    }
    else if (str.EndsWith("_dy"))
    {
        suffix = "_dy";
    }
    else
    {
        return nullptr;
    }

    // ...
#endif

    return nullptr;
}

}
