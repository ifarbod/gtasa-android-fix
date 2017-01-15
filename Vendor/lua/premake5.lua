project "Lua"
    language "C++"
    kind "StaticLib"

    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "**.h",
        "**.c"
    }

    excludes {
        "lua.c",
        "luac.c"
    }

project "lua_interpreter"
    language "C++"
    kind "ConsoleApp"
    
    links {
        "Lua"
    }
    
    vpaths {
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "lua.c"
    }

project "luac"
    language "C++"
    kind "ConsoleApp"
    
    links {
        "Lua"
    }
    
    vpaths {
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "luac.c"
    }