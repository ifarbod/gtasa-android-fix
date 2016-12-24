/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.2
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        eirrepo/sdk/MacroUtils.h
*  PURPOSE:     Common macros in the SDK
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef _COMMON_MACRO_UTILITIES_
#define _COMMON_MACRO_UTILITIES_

// Basic always inline definition.
#ifndef AINLINE
#ifdef _MSC_VER
#define AINLINE __forceinline
#elif __linux__
#define AINLINE __attribute__((always_inline))
#else
#define AINLINE
#endif
#endif

#endif //_COMMON_MACRO_UTILITIES_