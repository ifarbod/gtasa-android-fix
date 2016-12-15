// Simple mathematic operations
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/MathDefs.hpp>

namespace Util
{

void SinCos(float angle, float& sin, float& cos)
{
	float angleRadians = angle * M_DEGTORAD;
#if defined(HAVE_SINCOSF)
	sincosf(angleRadians, &sin, &cos);
#elif defined(HAVE_UNDERSCORE_SINCOSF)
	__sincosf(angleRadians, &sin, &cos);
#else
	sin = sinf(angleRadians);
	cos = cosf(angleRadians);
#endif
}

}
