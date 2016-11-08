project "bzip2"
    language "C++"
    kind "StaticLib"
    targetname "bzip2"
    
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
        "bzip2.c",
        "bzip2recover.c"
    }
