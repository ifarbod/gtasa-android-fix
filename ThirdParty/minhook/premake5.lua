project "minhook"
    language "C++"
    kind "StaticLib"
    
    includedirs { "include" }
    
    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "**.h",
        "buffer.c",
        "hook.c",
        "trampoline.c",
        "HDE/hde32.c"
    }
    
    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
