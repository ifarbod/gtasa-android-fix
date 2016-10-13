project "Launcher"
    language "C++"
    kind "WindowedApp"
    targetname "SAO"
    targetdir(buildpath("."))
    
    --pchheader "StdH.hpp"
	--pchsource "StdH.cpp"
    
    flags { "WinMain" }
	
	includedirs {
		"../../Shared/Utils/",
		"../../Shared/Utils/Win32/"
	}
    
    vpaths { 
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"**.rc", "**.ico"},
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "*.hpp",
        "*.cpp"
    }
    
    filter "system:windows"
        files {
            "**.rc",
            "**.ico"
        }
    
    filter "architecture:x64"
        flags { "ExcludeFromBuild" } 
        
    filter "system:not windows"
        flags { "ExcludeFromBuild" } 