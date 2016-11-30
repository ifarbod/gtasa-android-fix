project "Updater"
    language "C++"
    kind "WindowedApp"
    targetname "Updater"
    targetdir(buildpath("."))

    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"

    flags { "WinMain" }

    links {
        "Utility",
        "comctl32"
    }

    vpaths {
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"**.rc", "res/sao.ico"},
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "*.hpp",
        "*.cpp",
        "**.rc",
        "**.ico"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
