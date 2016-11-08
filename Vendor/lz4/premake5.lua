project "lz4"
    language "C++"
    kind "StaticLib"
    targetname "lz4"
    
    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }
    
    files {
        "premake5.lua",
        "**.h",
        "**.c"
    }
    
    --excludes {
    --}
