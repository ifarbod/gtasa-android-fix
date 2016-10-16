project "pugixml"
    kind "StaticLib"
    language "C++"
    targetname "pugixml"
    
    
    vpaths { 
        ["Headers/*"] = "**.hpp",
        ["Sources/*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "*.hpp",
        "*.cpp"
    }