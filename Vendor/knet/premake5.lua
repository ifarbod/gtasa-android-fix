project "kNet"
    language "C++"
    kind "StaticLib"

    includedirs { "include" }

    vpaths {
        ["Headers/*"] = { "include/**.hpp", "src/**.hpp" },
        ["Sources/*"] = "src/**.cpp",
        ["*"] = "premake5.lua"
    }

    files {
        "src/*.cpp",
        "**.hpp",
        "premake5.lua"
    }

    filter { "system:windows" }
        files "src/win32/*.cpp"
        links { "ws2_32.lib" }
