project "GameNorth"
    language "C++"
    kind "StaticLib"
    
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
    