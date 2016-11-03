project "DedicatedServer"
    language "C++"
    kind "ConsoleApp"
    
    filter { "system:windows", "platforms:x86" }
        targetdir(buildpath("Server"))
        targetname "SAOServer"
    
    filter { "system:windows", "platforms:x64" }
        targetdir(buildpath("Server.x64"))
        targetname "SAOServer.x64"
    
    links { "Lua" }
    
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
