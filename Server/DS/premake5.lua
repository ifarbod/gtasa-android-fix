project "DedicatedServer"
    language "C++"
    kind "ConsoleApp"
	targetdir(buildpath("Server"))

    --links { "Lua" }

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

    filter { "system:windows", "platforms:x86" }
        targetname "SAODS"

    filter { "system:windows", "platforms:x64" }
        targetname "SAODS.x64"
