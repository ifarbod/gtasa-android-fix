// Simple mathematic operations
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>

namespace Util
{

#undef M_PI
static const float M_PI = 3.14159265358979323846264338327950288f;
static const float M_HALF_PI = M_PI * 0.5f;
static const int M_MIN_INT = 0x80000000;
static const int M_MAX_INT = 0x7fffffff;
static const unsigned M_MIN_UNSIGNED = 0x00000000;
static const unsigned M_MAX_UNSIGNED = 0xffffffff;

static const float M_EPSILON = 0.000001f;
static const float M_LARGE_EPSILON = 0.00005f;
static const float M_MIN_NEARCLIP = 0.01f;
static const float M_MAX_FOV = 160.0f;
static const float M_LARGE_VALUE = 100000000.0f;
static const float M_INFINITY = (float)HUGE_VAL;
static const float M_DEGTORAD = M_PI / 180.0f;
static const float M_DEGTORAD_2 = M_PI / 360.0f; // M_DEGTORAD / 2.f
static const float M_RADTODEG = 1.0f / M_DEGTORAD;

// Intersection test result.
enum Intersection
{
	OUTSIDE,
	INTERSECTS,
	INSIDE
};

// Check whether two floating point values are equal within accuracy.
template <class T>
inline bool Equals(T lhs, T rhs) { return lhs + std::numeric_limits<T>::epsilon() >= rhs && lhs - std::numeric_limits<T>::epsilon() <= rhs; }

// Linear interpolation between two values.
template <class T, class U>
inline T Lerp(T lhs, T rhs, U t) { return lhs * (1.0 - t) + rhs * t; }

// Inverse linear interpolation between two values.
template <class T>
inline T InverseLerp(T lhs, T rhs, T x) { return (x - lhs) / (rhs - lhs); }

// Return the smaller of two values.
template <class T, class U>
inline T Min(T lhs, U rhs) { return lhs < rhs ? lhs : rhs; }

// Return the larger of two values.
template <class T, class U>
inline T Max(T lhs, U rhs) { return lhs > rhs ? lhs : rhs; }

// Return absolute value of a value
template <class T>
inline T Abs(T value) { return value >= 0.0 ? value : -value; }

// Return the sign of a float (-1, 0 or 1.)
template <class T>
inline T Sign(T value) { return value > 0.0 ? 1.0 : (value < 0.0 ? -1.0 : 0.0); }

// Check whether a floating point value is NaN.
inline bool IsNaN(float value) { return value != value; }

// Clamp a number to a range.
template <class T>
inline T Clamp(T value, T min, T max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

// Smoothly damp between values.
template <class T>
inline T SmoothStep(T lhs, T rhs, T t)
{
	t = Clamp((t - lhs) / (rhs - lhs), T(0.0), T(1.0)); // Saturate t
	return t * t * (3.0 - 2.0 * t);
}

// Return sine of an angle in degrees. Uses sinf() for floats, sin() for
// everything else
template <class T>
inline T Sin(T angle) { return sin(angle * M_DEGTORAD); }
template <> inline float Sin<float>(float angle) { return sinf(angle * M_DEGTORAD); }

// Return cosine of an angle in degrees. Uses cosf() for floats, cos() for
// everything else
template <class T>
inline T Cos(T angle) { return cos(angle * M_DEGTORAD); }
template <> inline float Cos<float>(float angle) { return cosf(angle * M_DEGTORAD); }

// Return tangent of an angle in degrees. Uses tanf() for floats, tan() for
// everything else
template <class T>
inline T Tan(T angle) { return tan(angle * M_DEGTORAD); }
template <> inline float Tan<float>(float angle) { return tanf(angle * M_DEGTORAD); }

// Return arc sine in degrees. Uses asinf() for floats, asin() for everything
// else
template <class T>
inline T Asin(T x) { return M_RADTODEG * asin(Clamp(x, -1.0, 1.0)); }
template <> inline float Asin<float>(float x) { return M_RADTODEG * asinf(Clamp(x, -1.0f, 1.0f)); }

// Return arc cosine in degrees. Uses acosf() for floats, acos() for
// everything else
template <class T>
inline T Acos(T x) { return M_RADTODEG * acos(Clamp(x, -1.0, 1.0)); }
template <> inline float Acos<float>(float x) { return M_RADTODEG * acosf(Clamp(x, -1.0f, 1.0f)); }

// Return arc tangent in degrees. Uses atanf() for floats, atan() for
// everything else
template <class T>
inline T Atan(T x) { return M_RADTODEG * atan(x); }
template <> inline float Atan<float>(float x) { return M_RADTODEG * atanf(x); }

// Return arc tangent of y/x in degrees. Uses atan2f() for floats, atan2()
// for everything else
template <class T>
inline T Atan2(T y, T x) { return M_RADTODEG * atan2(y, x); }
template <> inline float Atan2<float>(float y, float x) { return M_RADTODEG * atan2f(y, x); }

// Return X in power Y.
template <class T> T Pow(T x, T y) { return pow(x, y); }

// Return natural logarithm of X.
template <class T> T Ln(T x) { return log(x); }

// Return square root of X.
template <class T> T Sqrt(T x) { return sqrt(x); }

// Return floating-point remainder of X/Y.
template <class T> T Mod(T x, T y) { return fmod(x, y); }

// Return fractional part of passed value in range [0, 1).
template <class T> T Fract(T value) { return value - floor(value); }

// Round value down.
template <class T> T Floor(T x) { return floor(x); }

// Round value down. Returns integer value.
template <class T> int FloorToInt(T x) { return static_cast<int>(floor(x)); }

// Round value to nearest integer.
template <class T> T Round(T x) { return floor(x + T(0.5)); }

// Round value to nearest integer.
template <class T> int RoundToInt(T x) { return static_cast<int>(floor(x + T(0.5))); }

// Round value up.
template <class T> T Ceil(T x) { return ceil(x); }

// Round value up.
template <class T> int CeilToInt(T x) { return static_cast<int>(ceil(x)); }

// Check whether an unsigned integer is a power of two.
inline bool IsPowerOfTwo(unsigned value)
{
	return !(value & (value - 1));
}

// Round up to next power of two.
inline unsigned NextPowerOfTwo(unsigned value)
{
	--value;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	return ++value;
}

// Count the number of set bits in a mask.
inline unsigned CountSetBits(unsigned value)
{
	unsigned count = 0;
	for (count = 0; value; count++)
		value &= value - 1;
	return count;
}

// Update a hash with the given 8-bit value using the SDBM algorithm.
inline unsigned SDBMHash(unsigned hash, unsigned char c) { return c + (hash << 6) + (hash << 16) - hash; }

// Convert float to half float.
inline unsigned short FloatToHalf(float value)
{
	unsigned inu = *((unsigned*)&value);
	unsigned t1 = inu & 0x7fffffff;         // Non-sign bits
	unsigned t2 = inu & 0x80000000;         // Sign bit
	unsigned t3 = inu & 0x7f800000;         // Exponent

	t1 >>= 13;                              // Align mantissa on MSB
	t2 >>= 16;                              // Shift sign bit into position

	t1 -= 0x1c000;                          // Adjust bias

	t1 = (t3 < 0x38800000) ? 0 : t1;        // Flush-to-zero
	t1 = (t3 > 0x47000000) ? 0x7bff : t1;   // Clamp-to-max
	t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

	t1 |= t2;                               // Re-insert sign bit

	return (unsigned short)t1;
}

// Convert half float to float.
inline float HalfToFloat(unsigned short value)
{
	unsigned t1 = value & 0x7fff;           // Non-sign bits
	unsigned t2 = value & 0x8000;           // Sign bit
	unsigned t3 = value & 0x7c00;           // Exponent

	t1 <<= 13;                              // Align mantissa on MSB
	t2 <<= 16;                              // Shift sign bit into position

	t1 += 0x38000000;                       // Adjust bias

	t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

	t1 |= t2;                               // Re-insert sign bit

	float out;
	*((unsigned*)&out) = t1;
	return out;
}

// Calculate both sine and cosine, with angle in degrees.
void SinCos(float angle, float& sin, float& cos);

}
