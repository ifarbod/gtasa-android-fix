project "Launcher"
    language "C++"
    kind "WindowedApp"
    targetname "SAO"
    targetdir(buildpath("."))
    debugdir(buildpath("."))

    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"

    --flags { "WinMain" }
    --flags { "NoIncrementalLink", "NoEditAndContinue" }
    linkoptions "/IGNORE:4254 /SAFESEH:NO /DYNAMICBASE:NO /LARGEADDRESSAWARE /LAST:.zdata"

    links {
        "Utility",
        "../../Vendor/nvapi/x86/nvapi.lib",
        "dbghelp", "psapi", "comctl32", "wininet", "winhttp"
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
