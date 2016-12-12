// Quick sort algorithm
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Swap.hpp>
#include <Container/VectorBase.hpp>

namespace Util
{

static const int QUICKSORT_THRESHOLD = 16;

// Perform insertion sort on an array.
template <class T> void InsertionSort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end)
{
    for (RandomAccessIterator<T> i = begin + 1; i < end; ++i)
    {
        T temp = *i;
        RandomAccessIterator<T> j = i;
        while (j > begin && temp < *(j - 1))
        {
            *j = *(j - 1);
            --j;
        }
        *j = temp;
    }
}

// Perform insertion sort on an array using a compare function.
template <class T, class U> void InsertionSort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end, U compare)
{
    for (RandomAccessIterator<T> i = begin + 1; i < end; ++i)
    {
        T temp = *i;
        RandomAccessIterator<T> j = i;
        while (j > begin && compare(temp, *(j - 1)))
        {
            *j = *(j - 1);
            --j;
        }
        *j = temp;
    }
}

// Perform quick sort initial pass on an array. Does not sort fully.
template <class T> void InitialQuickSort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end)
{
    while (end - begin > QUICKSORT_THRESHOLD)
    {
        // Choose the pivot by median
        RandomAccessIterator<T> pivot = begin + ((end - begin) / 2);
        if (*begin < *pivot && *(end - 1) < *begin)
            pivot = begin;
        else if (*(end - 1) < *pivot && *begin < *(end - 1))
            pivot = end - 1;

        // Partition and sort recursively
        RandomAccessIterator<T> i = begin - 1;
        RandomAccessIterator<T> j = end;
        T pivotValue = *pivot;
        for (;;)
        {
            while (pivotValue < *(--j));
            while (*(++i) < pivotValue);
            if (i < j)
                Swap(*i, *j);
            else
                break;
        }

        InitialQuickSort(begin, j + 1);
        begin = j + 1;
    }
}

// Perform quick sort initial pass on an array using a compare function. Does not sort fully.
template <class T, class U> void InitialQuickSort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end, U compare)
{
    while (end - begin > QUICKSORT_THRESHOLD)
    {
        // Choose the pivot by median
        RandomAccessIterator<T> pivot = begin + ((end - begin) / 2);
        if (compare(*begin, *pivot) && compare(*(end - 1), *begin))
            pivot = begin;
        else if (compare(*(end - 1), *pivot) && compare(*begin, *(end - 1)))
            pivot = end - 1;

        // Partition and sort recursively
        RandomAccessIterator<T> i = begin - 1;
        RandomAccessIterator<T> j = end;
        T pivotValue = *pivot;
        for (;;)
        {
            while (compare(pivotValue, *(--j)));
            while (compare(*(++i), pivotValue));
            if (i < j)
                Swap(*i, *j);
            else
                break;
        }

        InitialQuickSort(begin, j + 1, compare);
        begin = j + 1;
    }
}

// Sort in ascending order using quicksort for initial passes, then an insertion sort to finalize.
template <class T> void Sort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end)
{
    InitialQuickSort(begin, end);
    InsertionSort(begin, end);
}

// Sort in ascending order using quicksort for initial passes, then an insertion sort to finalize, using a compare function.
template <class T, class U> void Sort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end, U compare)
{
    InitialQuickSort(begin, end, compare);
    InsertionSort(begin, end, compare);
}

}
