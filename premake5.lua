-- CTNorth build configuration script
-- Author(s):       iFarbod <ifarbod@outlook.com>
--
-- Copyright (c) 2015-2017 CTNorth Team
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

-- Add buildactions to path
premake.path = premake.path .. ";Utils/buildactions"
require "compose_files"
require "install_cef"
require "install_data"
dofile "buildoptions.lua"
dofile "findDirectX9.lua"

---
-- CTN main workspace
---
workspace "CTNorth"
    location "Build"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }

    flags { "StaticRuntime" }
    targetprefix ""
    buildoptions "/std:c++latest"
    --flags { "C++17" }
    symbols "On"
    characterset "Unicode"
    pic "On"
    systemversion "10.0.15063.0"
    startproject "Launcher"

    -- Preprocessor definitions
    defines
    {
        -- Enable wchar_t mode for pugixml
        "PUGIXML_WCHAR_MODE",

        -- No windows.h min()/max() macros
        "NOMINMAX",
        -- Exclude rarely-used stuff from Windows headers
        "WIN32_LEAN_AND_MEAN",

        -- Enable SSE
        "CTN_SSE",

        -- Test Versioning
        "CTN_MAJOR_VERSION=0",
        "CTN_MINOR_VERSION=1",
        "CTN_PATCH_VERSION=0"
    }

    if CI_BUILD then
    filter {}
        defines { "CI_BUILD=1" }
    end

    -- Find DirectX SDK
    --FindDirectX9()

    includedirs
    {
        "Base",
        "Vendor"
    }

    filter "platforms:x86"
        architecture "x86"
    filter "platforms:x64"
        architecture "x86_64"

    filter "configurations:Debug"
        defines { "CTN_DEBUG" }

    filter "configurations:Release"
        optimize "Speed"

    -- Generate PDB files at \Build\Symbols
    filter {"system:windows", "configurations:Release", "kind:not StaticLib"}
        os.mkdir("Build/Symbols")
        linkoptions "/PDB:\"Symbols\\$(ProjectName).pdb\""

    -- Enable visual styles
    filter { "system:windows", "kind:not StaticLib" }
        linkoptions "/manifestdependency:\"type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\""

    -- Disable deprecation warnings and errors
    filter "action:vs*"
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_DEPRECATE",
            "_CRT_NONSTDC_NO_WARNINGS",
            "_CRT_NONSTDC_NO_DEPRECATE",
            "_SCL_SECURE_NO_WARNINGS",
            "_SCL_SECURE_NO_DEPRECATE",
            "_WINSOCK_DEPRECATED_NO_WARNINGS"
        }

    -- Include the projects we are going to build
    group "Base"
    include "Base"

    group "Client"
    include "Client/Core"
    include "Client/Launcher"
    include "Client/Game"
    --include "Client/LauncherHelper"
    --include "Client/Update"
    if buildhost == "IFARBOD-PC" then
        include "Client/Network"
    end

    --group "Server"
    --include "Server/DS"
    --if buildhost == "IFARBOD-PC" then
        --include "Server/Network"
    --end

    group "Vendor"
    include "Vendor/angelscript"
    include "Vendor/cef3"
    --include "Vendor/freetype"
    --include "Vendor/jo"
    include "Vendor/libcpuid"
    include "Vendor/libcurl"
    include "Vendor/lz4"
    --include "Vendor/minhook"
    --include "Vendor/miniupnpc"
    --include "Vendor/opus"
    include "Vendor/pugixml"
    --include "Vendor/RakNet"
    include "Vendor/stb"
    --include "Vendor/sqlite"
    --include "Vendor/yaml-cpp"
    --include "Vendor/zlib"

-- Cleanup
if _ACTION == "clean" then
    os.rmdir("Bin");
    os.rmdir("Build");
end
