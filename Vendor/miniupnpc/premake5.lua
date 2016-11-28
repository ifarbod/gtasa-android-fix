-- Link ws2_32.lib IPHlpApi.Lib before using this
project "MiniUPnPc"
    language "C++"
    kind "StaticLib"

    includedirs { "." }
    defines { "STATICLIB" }

    vpaths { 
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "connecthostport.c",
        "minixml.c",
        "receivedata.c",
        "upnpc.c",
        "miniwget.c",
        "minisoap.c",
        "miniupnpc.c",
        "portlistingparse.c",
        "igd_desc_parse.c",
        "upnpreplyparse.c",
        "upnpcommands.c",
        "upnperrors.c"
    }
