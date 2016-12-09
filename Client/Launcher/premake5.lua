project "Launcher"
    language "C++"
    kind "WindowedApp"
    targetname "SAO"
    targetdir(buildpath("."))
    debugdir(buildpath("."))
    
    characterset "MBCS"

    flags { "NoIncrementalLink", "NoEditAndContinue" }
    linkoptions "/IGNORE:4254 /ENTRY:wmainCRTStartup /SAFESEH:NO /DYNAMICBASE:NO /LARGEADDRESSAWARE /LAST:.zdata"

    links {
        "dbghelp", "psapi", "comctl32", "wininet", "winhttp", "Shlwapi"
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
