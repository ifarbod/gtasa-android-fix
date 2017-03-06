project "DedicatedServer"
    language "C++"
    kind "ConsoleApp"
	targetdir(buildpath("Server"))

    links { "kNet" }

    pchheader "Precompiled.hpp"
    pchsource "Precompiled.cpp"

    vpaths {
        ["Headers/*"] = { "**.h", "**.hpp" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"*.rc", "res/ctn_server.ico"},
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
        targetname "CTNDS"

    filter { "system:windows", "platforms:x64" }
        targetname "CTNDS.x64"
