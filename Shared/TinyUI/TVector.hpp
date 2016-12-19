// TinyUI vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace TinyUI
{

template <class T>
struct _Vector2D
{
	_Vector2D() :
		x_(0),
		y_(0)
	{
	}

	_Vector2D(T x, T y) :
		x_(x),
		y_(y)
	{
	}

	// Coordinates.
	T x_, y_;
};

template <class T>
struct _Vector3D
{
	_Vector3D() :
		x_(0),
		y_(0),
		z_(0)
	{
	}

	_Vector3D(T x, T y, T z) :
		x_(x),
		y_(y),
		z_(z)
	{
	}

	// Coordinates.
	T x_, y_, z_;
};

template <class T>
struct _Vector4D
{
	_Vector4D() :
		x_(0),
		y_(0),
		z_(0),
		w_(0)
	{
	}

	_Vector4D(T x, T y, T z, T w) :
		x_(x),
		y_(y),
		z_(z),
		w_(w)
	{
	}

	// Coordinates.
	T x_, y_, z_, w_;
};

using Vector2D = _Vector2D<float>;
using Vector3D = _Vector3D<float>;
using Vector4D = _Vector4D<float>;
using IntVector2D = _Vector2D<int>;
using IntVector3D = _Vector3D<int>;
using IntVector4D = _Vector4D<int>;
using UIntVector2D = _Vector2D<unsigned>;
using UIntVector3D = _Vector3D<unsigned>;
using UIntVector4D = _Vector4D<unsigned>;

}
