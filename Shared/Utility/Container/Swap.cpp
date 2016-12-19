// Swaps two values
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include "ListBase.hpp"

namespace Util
{

template <> void Swap<String>(String& first, String& second)
{
    first.Swap(second);
}

template <> void Swap<VectorBase>(VectorBase& first, VectorBase& second)
{
    first.Swap(second);
}

template <> void Swap<ListBase>(ListBase& first, ListBase& second)
{
    first.Swap(second);
}

template <> void Swap<HashBase>(HashBase& first, HashBase& second)
{
    first.Swap(second);
}

}
