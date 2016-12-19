// Initializer lists
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace Util
{

// List of pointers to elements.
template <class T>
class InitializerList
{
public:
    using ValueType = T;
    using Reference = const T&;
    using ConstReference = const T&;
    using SizeType = size_t;

    using Iterator = const T*;
    using ConstIterator = const T*;

    // Empty list
    constexpr InitializerList() :
        first_(0),
        last_(0)
    {
    }

    // Construct with pointers.
    constexpr InitializerList(const T* firstArg, const T *lastArg) :
        first_(firstArg),
        last_(lastArg)
    {
    }

    // Get beginning of list.
    constexpr const T *Begin() const
    {
        return (first_);
    }

    // Get end of list.
    constexpr const T *End() const
    {
        return (last_);
    }

    // Get length of list.
    constexpr size_t Size() const
    {
        return ((size_t)(last_ - first_));
    }

private:
    const T* first_;
    const T* last_;
};

template <class T>
typename Util::InitializerList<T>::Iterator begin(InitializerList<T> v)
{
    return v.Begin();
}

template <class T>
typename Util::InitializerList<T>::Iterator end(InitializerList<T> v)
{
    return v.End();
}

template <class T>
typename Util::InitializerList<T>::ConstIterator begin(InitializerList<T> v)
{
    return v.Begin();
}

template <class T>
typename Util::InitializerList<T>::ConstIterator end(InitializerList<T> v)
{
    return v.End();
}

}
