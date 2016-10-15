project "Lua"
    language "C++"
    kind "StaticLib"
    targetname "Lua"
    
    defines { "_LIB" }
    
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
