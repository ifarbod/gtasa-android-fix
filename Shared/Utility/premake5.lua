project "Utility"
    language "C++"
    kind "StaticLib"
    targetname "Utility"
    
    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"
    
    vpaths {
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
		"**.hpp",
        "**.cpp"
    }
    
