project "Base"
    language "C++"
    kind "StaticLib"

    pchheader "PCH.hpp"
    pchsource "PCH.cpp"

    flags { "NoIncrementalLink", "NoEditAndContinue" }
    includedirs { "." }

    links
    {
        "AngelScript"
    }

    vpaths
    {
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }

    files
    {
        "premake5.lua",
        "**.hpp",
        "**.cpp"
    }
