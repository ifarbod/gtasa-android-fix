project "minilua"
    language "C++"
    kind "ConsoleApp"
    targetsuffix ""

    vpaths {
        ["Sources/*"] = "src/host/minilua.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "src/host/minilua.c"
    }

project "buildvm"
    language "C++"
    kind "ConsoleApp"
    targetsuffix ""

    defines {
        "TOLUA_RELEASE"
    }

    links {
        "minilua"
    }
    
    includedirs {
        "src"
    }

    vpaths {
        ["Headers/*"] = "src/host/**.h",
        ["Sources/*"] = "src/host/**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "src/host/buildvm*.h",
        "src/host/buildvm*.c"
    }
    
    excludes {
        "src/host/buildvm.h"
    }

project "LuaJIT"
    language "C++"
    kind "StaticLib"

    defines {
        --"TOLUA_RELEASE"
    }
    
    links {
    
    }

    includedirs {
        ".",
        "include"
    }

    if USE_LJ then
        includedirs { "../luajit/src" }
    else
        includedirs { "../lua" }
    end

    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "src/lib/**.h",
        "src/lib/**.c"
    }
