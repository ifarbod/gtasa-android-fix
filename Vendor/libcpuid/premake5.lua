project "libcpuid"
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
