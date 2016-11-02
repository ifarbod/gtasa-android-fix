project "DedicatedServer"
    language "C++"
    kind "ConsoleApp"
    targetdir(buildpath("Server"))
    
    targetname "SAOServer_%{cfg.platform}"
    
    links { "Lua", "RakNet" }
    
    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"
    
    vpaths { 
        ["Headers/*"] = { "**.h", "**.hpp" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"*.rc", "res/sao_server.ico"},
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "**.hpp",
        "**.cpp",
        "**.rc",
        "**.ico"
    }
