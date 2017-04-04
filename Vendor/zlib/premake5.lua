project "zlib"
    language "C++"
    kind "StaticLib"
    targetname "zlib"

    vpaths
    {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files
    {
        "premake5.lua",
        "*.h",
        "*.c"
    }

project "Zip"
    language "C"
    kind "StaticLib"
    targetname "zip"

    links "zlib"
    
    includedirs
    {
        "../zlib"
    }

    vpaths
    {
        ["Headers/*"] = "**.h",
        ["Sources"] = "*.c",
        ["*"] = "premake5.lua"
    }

    files
    {
        "premake5.lua",
        "minizip/*.h",
        "minizip/*.c"
    }

    filter "system:linux"
        excludes
        {
            "minizip/iowin32.h",
            "minizip/iowin32.c"
        }
