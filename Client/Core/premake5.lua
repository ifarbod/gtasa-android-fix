project "Core"
    language "C++"
    kind "SharedLib"
    targetname "Core"
    targetdir(buildpath("SAO"))
    
    pchheader "pch.hpp"
    pchsource "pch.cpp"
    
    links { "RakNet", "Lua" }
    
    vpaths { 
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = "**.rc",
        ["*"] = "premake5.lua"
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