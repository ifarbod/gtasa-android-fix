project "Launcher"
    language "C++"
    kind "WindowedApp"
    targetname "CTN"
    targetdir(buildpath("."))
    debugdir(buildpath("."))

    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"

    flags { "NoIncrementalLink", "NoEditAndContinue" }
    linkoptions "/IGNORE:4254 /ENTRY:wmainCRTStartup /OPT:NOLBR /SAFESEH:NO /DYNAMICBASE:NO /LARGEADDRESSAWARE /LAST:.zdata"

    links {
        "Utility",
        "dbghelp", "psapi", "comctl32", "wininet", "winhttp", "Shlwapi",
        "PEFramework"
    }
    
    includedirs {
        "../../Vendor/eirrepo/"
    }
    
    vpaths {
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"**.rc", "res/ctn.ico"},
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
