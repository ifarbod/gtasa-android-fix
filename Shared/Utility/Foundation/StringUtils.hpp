// Useful string utilities
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Foundation/Variant.hpp>

namespace Util
{

// Parse a bool from a string. Check for the first non-empty character (converted to lowercase) being either 't', 'y' or '1'.
bool ToBool(const String& source);
// Parse a bool from a C string. Check for the first non-empty character (converted to lowercase) being either 't', 'y' or '1'.
bool ToBool(const char* source);
// Parse a float from a string.
float ToFloat(const String& source);
// Parse a float from a C string.
float ToFloat(const char* source);
// Parse a double from a string.
double ToDouble(const String& source);
// Parse a double from a C string.
double ToDouble(const char* source);
// Parse an integer from a string. Assumed to be decimal by default (base 10). Use base 0 to autodetect from string.
int ToInt(const String& source, int base = 10);
// Parse an integer from a C string. Assumed to be decimal by default (base 10). Use base 0 to autodetect from string.
int ToInt(const char* source, int base = 10);
// Parse an unsigned integer from a string. Assumed to be decimal by default (base 10). Use base 0 to autodetect from string.
unsigned ToUInt(const String& source, int base = 10);
// Parse an unsigned integer from a C string. Assumed to be decimal by default (base 10). Use base 0 to autodetect from string.
unsigned ToUInt(const char* source, int base = 10);
// Parse a Color from a string.
Color ToColor(const String& source);
// Parse a Color from a C string.
Color ToColor(const char* source);
// Parse an IntRect from a string.
IntRect ToIntRect(const String& source);
// Parse an IntRect from a C string.
IntRect ToIntRect(const char* source);
// Parse an IntVector2 from a string.
IntVector2 ToIntVector2(const String& source);
// Parse an IntVector2 from a C string.
IntVector2 ToIntVector2(const char* source);
// Parse a Quaternion from a string. If only 3 components specified, convert Euler angles (degrees) to quaternion.
Quaternion ToQuaternion(const String& source);
// Parse a Quaternion from a C string. If only 3 components specified, convert Euler angles (degrees) to quaternion.
Quaternion ToQuaternion(const char* source);
// Parse a Rect from a string.
Rect ToRect(const String& source);
// Parse a Rect from a C string.
Rect ToRect(const char* source);
// Parse a Vector2 from a string.
Vector2 ToVector2(const String& source);
// Parse a Vector2 from a C string.
Vector2 ToVector2(const char* source);
// Parse a Vector3 from a string.
Vector3 ToVector3(const String& source);
// Parse a Vector3 from a C string.
Vector3 ToVector3(const char* source);
// Parse a Vector4 from a string.
Vector4 ToVector4(const String& source, bool allowMissingCoords = false);
// Parse a Vector4 from a C string.
Vector4 ToVector4(const char* source, bool allowMissingCoords = false);
// Parse a float, Vector or Matrix variant from a string. Return empty variant on illegal input.
Variant ToVectorVariant(const String& source);
// Parse a float, Vector or Matrix variant from a C string. Return empty variant on illegal input.
Variant ToVectorVariant(const char* source);
// Parse a Matrix3 from a string.
Matrix3 ToMatrix3(const String& source);
// Parse a Matrix3 from a C string.
Matrix3 ToMatrix3(const char* source);
// Parse a Matrix3x4 from a string.
Matrix3x4 ToMatrix3x4(const String& source);
// Parse a Matrix3x4 from a C string.
Matrix3x4 ToMatrix3x4(const char* source);
// Parse a Matrix4 from a string.
Matrix4 ToMatrix4(const String& source);
// Parse a Matrix4 from a C string.
Matrix4 ToMatrix4(const char* source);
// Convert a pointer to string (returns hexadecimal.)
String ToString(void* value);
// Convert an unsigned integer to string as hexadecimal.
String ToStringHex(unsigned value);
// Convert a byte buffer to a string.
void BufferToString(String& dest, const void* data, unsigned size);
// Convert a string to a byte buffer.
void StringToBuffer(PODVector<unsigned char>& dest, const String& source);
// Convert a C string to a byte buffer.
void StringToBuffer(PODVector<unsigned char>& dest, const char* source);
// Return an index to a string list corresponding to the given string, or a default value if not found. The string list must be empty-terminated.
unsigned GetStringListIndex(const String& value, const String* strings, unsigned defaultIndex, bool caseSensitive = false);
// Return an index to a string list corresponding to the given C string, or a default value if not found. The string list must be empty-terminated.
unsigned GetStringListIndex(const char* value, const String* strings, unsigned defaultIndex, bool caseSensitive = false);
// Return an index to a C string list corresponding to the given C string, or a default value if not found. The string list must be empty-terminated.
unsigned GetStringListIndex(const char* value, const char** strings, unsigned defaultIndex, bool caseSensitive = false);
// Return a formatted string.
String ToString(const char* formatString, ...);
// Return whether a char is an alphabet letter.
bool IsAlpha(unsigned ch);
// Return whether a char is a digit.
bool IsDigit(unsigned ch);
// Return char in uppercase.
unsigned ToUpper(unsigned ch);
// Return char in lowercase.
unsigned ToLower(unsigned ch);
// Convert a memory size into a formatted size string, of the style "1.5 Mb".
String GetFileSizeString(unsigned long long memorySize);
// Parse type from a C string.
template <class T> T FromString(const char* source);

template <> inline const char* FromString<const char*>(const char* source) { return source; }
template <> inline String FromString<String>(const char* source) { return source; }
template <> inline bool FromString<bool>(const char* source) { return ToBool(source); }
template <> inline float FromString<float>(const char* source) { return ToFloat(source); }
template <> inline double FromString<double>(const char* source) { return ToDouble(source); }
template <> inline int FromString<int>(const char* source) { return ToInt(source); }
template <> inline unsigned FromString<unsigned>(const char* source) { return ToUInt(source); }
template <> inline Color FromString<Color>(const char* source) { return ToColor(source); }
template <> inline IntRect FromString<IntRect>(const char* source) { return ToIntRect(source); }
template <> inline IntVector2 FromString<IntVector2>(const char* source) { return ToIntVector2(source); }
template <> inline Quaternion FromString<Quaternion>(const char* source) { return ToQuaternion(source); }
template <> inline Rect FromString<Rect>(const char* source) { return ToRect(source); }
template <> inline Vector2 FromString<Vector2>(const char* source) { return ToVector2(source); }
template <> inline Vector3 FromString<Vector3>(const char* source) { return ToVector3(source); }
template <> inline Vector4 FromString<Vector4>(const char* source) { return ToVector4(source); }
template <> inline Variant FromString<Variant>(const char* source) { return ToVectorVariant(source); }
template <> inline Matrix3 FromString<Matrix3>(const char* source) { return ToMatrix3(source); }
template <> inline Matrix3x4 FromString<Matrix3x4>(const char* source) { return ToMatrix3x4(source); }
template <> inline Matrix4 FromString<Matrix4>(const char* source) { return ToMatrix4(source); }

// Parse type from a string.
template <class T> T FromString(const String& source) { return FromString<T>(source.CString()); }

// Variadic version of ToString.
String ToStringVariadic(const char* formatString, va_list args);

}
