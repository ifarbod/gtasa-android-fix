project "CEFLauncherDLL"
    language "C++"
    kind "SharedLib"
    targetname "CEFLauncherDLL"
    targetdir(buildpath("SAO/cef"))
    
    includedirs { 
		"../../Vendor/cef" 
	}
    
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
    
    links { "CEF",  "../../Vendor/cef/Release/libcef.lib" }
    
    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
