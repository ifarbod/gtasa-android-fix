project "Utility"
    language "C++"
    kind "StaticLib"
    targetname "Utility"
    
    pchheader "PCH.hpp"
    pchsource "PCH.cpp"
    
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
    
