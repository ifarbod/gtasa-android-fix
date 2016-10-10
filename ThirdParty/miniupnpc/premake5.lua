-- multi-threaded /mt
project "miniupnpc"
    language "C++"
    kind "StaticLib"
    targetname "miniupnpc"
    
    includedirs { "." }
	defines { "_LIB", "STATICLIB" }
    
    vpaths { 
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.c",
		["*"] = "premake5.lua"
	}
    
    files {
		"premake5.lua",
		"*.h",
		"*.c"
	}