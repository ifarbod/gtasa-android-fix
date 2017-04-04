project "Core"
    language "C++"
    kind "SharedLib"
    targetname "Core"
    targetdir(buildpath("."))

    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"

    vpaths {
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = "**.rc",
        ["*"] = "premake5.lua"
    }

    libdirs {
        "../../Vendor/cef3/Release"
    }

    includedirs {
        ".",
        "../../Vendor/bass",
        "../../Vendor/lua",
        "../../Vendor/cef3"
    }

    links {
        "Base",
        "Game",
        "libcpuid",
        "../../Vendor/bass/lib/bass",
        "../../Vendor/bass/lib/bass_fx",
        "../../Vendor/bass/lib/bassmix",
        "../../Vendor/bass/lib/tags"
    }

    files {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.rc"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
