project "Core"
    language "C++"
    kind "SharedLib"
    targetname "SAO"
    targetdir(buildpath("."))
    
    --pchheader "StdH.hpp"
    --pchsource "StdH.cpp"
    
    vpaths { 
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = "**.rc",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "*.hpp",
        "*.cpp",
        "**.rc"
    }
    
    filter "architecture:x64"
        flags { "ExcludeFromBuild" } 
        
    filter "system:not windows"
        flags { "ExcludeFromBuild" } 