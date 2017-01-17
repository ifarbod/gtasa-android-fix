project "toluapp"
    language "C++"
    kind "StaticLib"

    defines {
        "TOLUA_RELEASE"
    }

    includedirs {
        ".",
        "include"
    }

    if USE_LUAJIT then
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

project "tolua++"
    language "C++"
    kind "ConsoleApp"
    targetsuffix ""
    targetdir "src/tool"

    defines {
        "TOLUA_RELEASE"
    }

    links {
        "Lua",
        "toluapp"
    }

    includedirs {
        ".",
        "include"
    }

    -- Use normal Lua wheather using LuaJIT or not
    includedirs { "../lua" }

    vpaths {
        ["Headers/*"] = "src/tool/**.h",
        ["Sources/*"] = "src/tool/**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "src/tool/tolua.c",
        "src/tool/toluabind.c",
        "src/tool/toluabind.h"
    }
