project "Lua"
    language "C++"
    kind "StaticLib"
    targetname "Lua"
    
    defines { "_LIB" }
    
    vpaths {
        ["Headers/*"] = { "**.h", "**.hpp" },
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "**.h",
		"**.hpp",
        "**.c"
    }
    
    excludes {
        "lua.c",
        "luac.c"
    }
