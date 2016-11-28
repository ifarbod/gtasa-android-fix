project "yaml-cpp"
    language "C++"
    kind "StaticLib"
    
    includedirs { "include" }
    
    vpaths {
        ["Headers/*"] = { "include/yaml-cpp/**.h", "src/**.h" },
        ["Sources/*"] = "src/**.cpp",
        ["*"] = "premake5.lua"
    }
    
    files {
        "**.cpp",
        "**.h",
        "premake5.lua"
    }
