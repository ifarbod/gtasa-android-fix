project "libcurl"
    language "C++"
    kind "StaticLib"

    includedirs { "include" }

    vpaths {
        ["Headers/*"] = { "include/curl/**.h", "lib/**.h" },
        ["Sources/*"] = "lib/**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "**.c",
        "**.h",
        "premake5.lua"
    }

    defines {
        "BUILDING_LIBCURL",
        "CURL_STATICLIB"
    }

    excludes {
        "lib/vtls/*.*",
        "lib/vauth/*.*"
    }
