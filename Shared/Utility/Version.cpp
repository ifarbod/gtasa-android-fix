// SAO Version
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "PCH.hpp"

namespace Util
{

String MakeVersionStr(bool includeV)
{
	return (includeV) ? SAO_VERSION_STR : SAO_VERSION_STR_NOV;
}

}
