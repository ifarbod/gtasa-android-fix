project "TinyUI"
    language "C++"
    kind "StaticLib"
    targetname "TUI"
    
    pchheader "StdH.hpp"
    pchsource "StdH.cpp"
    
    includedirs { "." }
    
    vpaths {
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "**.h",
		"**.hpp",
        "**.cpp"
    }
