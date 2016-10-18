project "Utils"
    language "C++"
    kind "StaticLib"
    targetname "Utils"
    
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
    
