project "PugiXML"
    kind "StaticLib"
    language "C++"


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
