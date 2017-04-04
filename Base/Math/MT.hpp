// Mersenne twister random device
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once
#include <cstdint> // for int types
#include <cstddef> // for size_t

namespace ctn
{

template <class UIntType, size_t w, size_t n, size_t m, size_t r, UIntType a, size_t u, UIntType d, size_t s,
    UIntType b, size_t t, UIntType c, size_t l, UIntType f>
class MersenneTwister
{
public:
    using ResultType = UIntType;

    static constexpr size_t wordSize_ = w;
    static constexpr size_t stateSize_ = n;
    static constexpr size_t shiftSize_ = m;
    static constexpr size_t maskBits_ = r;
    static constexpr UIntType xorMask_ = a;
    static constexpr size_t temperingU_ = u;
    static constexpr UIntType temperingD_ = d;
    static constexpr size_t temperingS_ = s;
    static constexpr UIntType temperingB_ = b;
    static constexpr size_t temperingT_ = t;
    static constexpr UIntType temperingC_ = c;
    static constexpr size_t temperingL = l;
    static constexpr UIntType initializationMultiplier_ = f;
    static constexpr UIntType defaultSeed_ = 5489u;

    MersenneTwister(UIntType _Seed = defaultSeed_) { Seed(_Seed); }

    // Sets the state x(0) to v mod 2w. Then, iteratively,
    // sets x(i) to
    // (i + f * (x(i-1) xor (x(i-1) rshift w-2))) mod 2<sup>w</sup>
    // for i = 1 .. n-1. x(n) is the first value to be returned by operator().
    void Seed(const UIntType& value)
    {
        // New seeding algorithm from
        // http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/emt19937ar.html
        // In the previous versions, MSBs of the seed affected only MSBs of the
        // state x[].
        const UIntType mask = (Max)();
        x[0] = value & mask;
        for (i = 1; i < n; i++)
        {
            x[i] = (f * (x[i - 1] ^ (x[i - 1] >> (w - 2))) + i) & mask;
        }

        NormalizeState();
    }

    void Seed() { Seed(defaultSeed_); }

    // Returns the smallest value that the generator can produce.
    static ResultType Min() { return 0; }

    // Returns the largest value that the generator can produce.
    static ResultType Max() { return WMSK_; }

    ResultType operator()()
    {
        if (i == n)
            Twist();
        // Step 4
        UIntType z = x[i];
        ++i;
        z ^= ((z >> u) & d);
        z ^= ((z << s) & b);
        z ^= ((z << t) & c);
        z ^= (z >> l);
        return z;
    }

    void Discard(uintmax_t z)
    {
        for (uintmax_t j = 0; j < z; ++j)
        {
            (*this)();
        }
    }

private:
    void Twist()
    {
        const UIntType upperMask = (~static_cast<UIntType>(0)) << r;
        const UIntType lowerMask = ~upperMask;

        const size_t unrollFactor = 6;
        const size_t unrollExtra1 = (n - m) % unrollFactor;
        const size_t unrollExtra2 = (m - 1) % unrollFactor;

        // Split loop to avoid costly modulo operations
        // Extra scope for MSVC brokenness w.r.t. for scope
        {
            for (size_t j = 0; j < n - m - unrollExtra1; j++)
            {
                UIntType y = (x[j] & upperMask) | (x[j + 1] & lowerMask);
                x[j] = x[j + m] ^ (y >> 1) ^ ((x[j + 1] & 1) * a);
            }
        }
        {
            for (size_t j = n - m - unrollExtra1; j < n - m; j++)
            {
                UIntType y = (x[j] & upperMask) | (x[j + 1] & lowerMask);
                x[j] = x[j + m] ^ (y >> 1) ^ ((x[j + 1] & 1) * a);
            }
        }
        {
            for (size_t j = n - m; j < n - 1 - unrollExtra2; j++)
            {
                UIntType y = (x[j] & upperMask) | (x[j + 1] & lowerMask);
                x[j] = x[j - (n - m)] ^ (y >> 1) ^ ((x[j + 1] & 1) * a);
            }
        }
        {
            for (size_t j = n - 1 - unrollExtra2; j < n - 1; j++)
            {
                UIntType y = (x[j] & upperMask) | (x[j + 1] & lowerMask);
                x[j] = x[j - (n - m)] ^ (y >> 1) ^ ((x[j + 1] & 1) * a);
            }
        }
        // Last iteration
        UIntType y = (x[n - 1] & upperMask) | (x[0] & lowerMask);
        x[n - 1] = x[m - 1] ^ (y >> 1) ^ ((x[0] & 1) * a);
        i = 0;
    }

    void NormalizeState()
    {
        const UIntType upperMask = (~static_cast<UIntType>(0)) << r;
        const UIntType lowerMask = ~upperMask;
        UIntType y0 = x[m - 1] ^ x[n - 1];
        if (y0 & (static_cast<UIntType>(1) << (w - 1)))
        {
            y0 = ((y0 ^ a) << 1) | 1;
        }
        else
        {
            y0 = y0 << 1;
        }
        x[0] = (x[0] & upperMask) | (y0 & lowerMask);

        // Fix up the state if it's all zeroes.
        for (size_t j = 0; j < n; ++j)
        {
            if (x[j] != 0)
                return;
        }

        x[0] = static_cast<UIntType>(1) << (w - 1);
    }

    UIntType x[n];
    size_t i;

    static constexpr UIntType WMSK_ = ~((~UIntType(0) << (w - 1)) << 1);
    static constexpr UIntType HMSK_ = (WMSK_ << r) & WMSK_;
    static constexpr UIntType LMSK_ = ~HMSK_ & WMSK_;
};

// Memory usage:
// mt11213b   : 352 * sizeof(uint32_t) = 352 * 4 = 1408 bytes = ~1 KB
// mt19937    : 625 * sizeof(uint32_t) = 625 * 4 = 2500 bytes = ~2 KB
// mt19937_64 : 312 * sizeof(uint64_t) = 312 * 8 = 2496 bytes = ~2 KB
using MT11213b = MersenneTwister<uint32_t, 32, 351, 175, 19, 0xCCAB8EE7, 11, 0xFFFFFFFF, 7, 0x31B6AB00, 15, 0xFFE50000,
    17, 1812433253>;

using MT19937 = MersenneTwister<uint32_t, 32, 624, 397, 31, 0x9908B0DF, 11, 0xFFFFFFFF, 7, 0x9D2C5680, 15, 0xEFC60000,
    18, 1812433253>;

using MT19937_64 =
    MersenneTwister<uint64_t, 64, 312, 156, 31, UINT64_C(0xB5026F5AA96619E9), 29, UINT64_C(0x5555555555555555), 17,
        UINT64_C(0x71D67FFFEDA60000), 37, UINT64_C(0xFFF7EEE000000000), 43, UINT64_C(6364136223846793005)>;

}
