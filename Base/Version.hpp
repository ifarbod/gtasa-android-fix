// Versioning helpers
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

// Version string generation
#define CTN_STRINGIZE_HELPER(x) #x
#define CTN_STRINGIZE(x) CTN_STRINGIZE_HELPER(x)

#define CTN_VERSION_DOT_RC CTN_MAJOR_VERSION.CTN_MINOR_VERSION.CTN_PATCH_VERSION.0
#define CTN_VERSION_COMMA_RC CTN_MAJOR_VERSION,CTN_MINOR_VERSION,CTN_PATCH_VERSION,0
#define CTN_VERSION_STR_RC CTN_STRINGIZE(CTN_VERSION_DOT_RC)

#ifdef CTN_VERSION_PRE_RELEASE_TYPE

#define CTN_VERSION_STRING  CTN_STRINGIZE(CTN_MAJOR_VERSION) "." \
                            CTN_STRINGIZE(CTN_MINOR_VERSION) "." \
                            CTN_STRINGIZE(CTN_PATCH_VERSION) "-" \
                            CTN_VERSION_PRE_RELEASE_TYPE "." CTN_STRINGIZE(CTN_VERSION_PRE_RELEASE)

#else

#define CTN_VERSION_STRING  CTN_STRINGIZE(CTN_MAJOR_VERSION) "." \
                            CTN_STRINGIZE(CTN_MINOR_VERSION) "." \
                            CTN_STRINGIZE(CTN_PATCH_VERSION)

#endif

#define CTN_VERSION "v" CTN_VERSION_STRING

#define CTN_VERSION_AT_LEAST(major, minor, patch) \
  (( (major) < CTN_MAJOR_VERSION) \
  || ((major) == CTN_MAJOR_VERSION && (minor) < CTN_MINOR_VERSION) \
  || ((major) == CTN_MAJOR_VERSION && (minor) == CTN_MINOR_VERSION \
      && (patch) <= CTN_PATCH_VERSION))
