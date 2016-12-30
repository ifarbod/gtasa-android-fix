project "libcef_dll_wrapper"
    language "C++"
    kind "StaticLib"

    includedirs { "." }

    defines {
        "__STDC_CONSTANT_MACROS", "__STDC_FORMAT_MACROS", "_FILE_OFFSET_BITS=64",
        "_WINDOWS", "UNICODE", "_UNICODE", "WINVER=0x0602", "_WIN32_WINNT=0x602", "NOMINMAX", "WIN32_LEAN_AND_MEAN", "_HAS_EXCEPTIONS=0",
        "PSAPI_VERSION=1",
        "WRAPPING_CEF_SHARED" -- introduced in 3.2883.1539.gd7f087e
    }

    files {
        "premake5.lua",
        "**.cc",
        "**.h",
    }

    excludes {
        "libcef_dll/base/cef_string16.cc"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }
    filter "system:not windows"
        flags { "ExcludeFromBuild" }
