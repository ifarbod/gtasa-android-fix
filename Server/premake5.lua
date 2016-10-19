project "DedicatedServer"
    language "C++"
    kind "ConsoleApp"
    targetdir(buildpath("Server"))
    
    vpaths { 
        ["Headers/*"] = { "**.h", "**.hpp" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"*.rc", "res/sao_icon.ico"},
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "**.hpp",
        "**.cpp",
        "**.rc",
        "**.ico"
    }