project "LauncherHelper"
    language "C++"
    kind "SharedLib"
    targetname "LauncherHelper"
    targetdir(buildpath("."))

    links
    {
        "LZ4"
    }
    
    vpaths
    {
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = "**.rc",
        ["*"] = "premake5.lua"
    }

    files
    {
        "premake5.lua",
        "**.hpp",
        "**.cpp",
        "**.rc"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
