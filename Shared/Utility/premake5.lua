project "Utility"
    language "C++"
    kind "StaticLib"
    targetname "Utility"

    pchheader "PCH.hpp"
    pchsource "PCH.cpp"

    includedirs { "." }

    vpaths {
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp"
    }
    
    excludes {
        "Foundation/Variant.cpp"
    }

