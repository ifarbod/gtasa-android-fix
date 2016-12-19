// Swaps two values
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace Util
{

class HashBase;
class ListBase;
class String;
class VectorBase;

// Swap two values.
template <class T>
inline void Swap(T& first, T& second)
{
    T temp = first;
    first = second;
    second = temp;
}

template <> void Swap<String>(String& first, String& second);
template <> void Swap<VectorBase>(VectorBase& first, VectorBase& second);
template <> void Swap<ListBase>(ListBase& first, ListBase& second);
template <> void Swap<HashBase>(HashBase& first, HashBase& second);

}
