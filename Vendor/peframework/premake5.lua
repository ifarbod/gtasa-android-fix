project "PEFramework"
    language "C++"
    kind "StaticLib"

    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }
    
    includedirs {
        "include/",
        "../eirrepo/"
    }

    files {
        "premake5.lua",
        "src/*.cpp",
        "src/*.hxx",
        "include/*.h"
    }
