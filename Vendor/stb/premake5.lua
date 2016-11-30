project "STB"
    language "C++"
    kind "StaticLib"

    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = { "**.c", "**.cpp" },
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "*.h",
        "*.c",
        "*.cpp"
    }
