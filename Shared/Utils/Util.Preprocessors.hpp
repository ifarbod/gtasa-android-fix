// Some preprocessors
// Author(s):       AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

// Preprocessor to check is Unix or Win32 on compile
#ifdef __unix__
    #define SAO_UNIX
#elif defined(WIN32) || defined(_WIN32)
    #define SAO_WIN32
#endif

// Types
using s8  = char;               // [-128 to 127] (0 to 255 when compiled by using /J)
using s16 = short;              // [-32,768 to 32,767]
using s32 = int;                // [-2,147,483,648 to 2,147,483,647]
using s64 = long long;          // [–9,223,372,036,854,775,808 to 9,223,372,036,854,775,807]
using u8  = unsigned char;      // [0 to 255]
using u16 = unsigned short;     // [0 to 65535]
using u32 = unsigned int;       // [0 to 4,294,967,295]
using u64 = unsigned long long; // [0 to 18,446,744,073,709,551,615]
using f32 = float;              // [3.4E +/- 38 (7 digits)]
using f64 = double;             // [1.7E +/- 308 (15 digits)]

#define _UTILS_BEGIN namespace Utils {
#define _UTILS_END   }
