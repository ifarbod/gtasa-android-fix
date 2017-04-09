project "AngelScript"
    language "C++"
    kind "StaticLib"

    includedirs
    {
        "include"
    }

    vpaths
    {
        ["Headers/*"] = { "include/**.h", "source/**.h" },
        ["Sources/*"] = "source/**.cpp",
        ["*"] = "premake5.lua"
    }

    files
    {
        "**.cpp",
        "**.h",
        "premake5.lua"
    }
