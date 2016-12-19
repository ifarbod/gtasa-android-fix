-- Premake5 script
-- Author(s):       iFarbod <ifarbod@outlook.com>
--
-- Copyright (c) 2015-2017 Project CtNorth
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

-- Add buildactions to path
premake.path = premake.path .. ";utils/buildactions"
require "compose_files"
require "install_cef"
require "install_data"

-- Set CI Build global
local ci = os.getenv("CI")
if ci and ci:lower() == "true" then
    CI_BUILD = true
else
    CI_BUILD = false
end

workspace "CtNorth"
    location "Build"
    platforms { "x86", "x64" }
    targetprefix ""
    configurations { "Debug", "Release" }
    flags { "C++14" }
    symbols "On"
    characterset "Unicode"
    pic "On"
    startproject "Launcher"

    -- Preprocessor definitions
    defines {
        -- Disable deprecation warnings and errors
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_SECURE_NO_DEPRECATE",
        "_CRT_NONSTDC_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
        "_SCL_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_DEPRECATE",
        "_WINSOCK_DEPRECATED_NO_WARNINGS",

        -- Enable wchar_t mode for pugixml
        "PUGIXML_WCHAR_MODE",

        -- No windows.h min()/max() macros
        "NOMINMAX",
        
        -- Enable SSE
        "CTN_SSE"
    }

    -- Get DirectX SDK directory from environment variables
    dxdir = os.getenv("DXSDK_DIR") or ""

    includedirs {
        "Vendor",
        "Shared/Utility"
    }

    filter "platforms:x86"
        architecture "x86"
    filter "platforms:x64"
        architecture "x86_64"

    filter "configurations:Debug"
        defines { "CTN_DEBUG" }
        targetsuffix "_d"

    filter "configurations:Release"
        optimize "Speed"

    if CI_BUILD then
        filter {}
            defines { "CI_BUILD=1" }
    end

    -- Generate PDB files at \Build\Symbols
    filter {"system:windows", "configurations:Release", "kind:not StaticLib"}
        os.mkdir("Build/Symbols")
        linkoptions "/PDB:\"Symbols\\$(ProjectName).pdb\""

    filter "system:windows"
        toolset "v140" -- TODO: Change to v141 after VS2017 RTM release
        flags { "StaticRuntime" }

    filter { "system:windows", "platforms:x86" }
        defines { "WIN32", "_WIN32" }

        includedirs {
            dxdir .. "Include"
        }
        libdirs {
            dxdir .. "Lib/x86"
        }

    filter { "system:windows", "platforms:x64" }
        defines { "WIN64", "_WIN64" }

    filter { "system:windows", "kind:StaticLib" }
        defines { "_LIB" }

    -- Enable visual styles
    filter { "system:windows", "platforms:x86", "kind:not StaticLib" }
        linkoptions "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\""

    --filter { "system:windows", "kind:not StaticLib", "kind:not SharedLib" }
        --linkoptions "/MANIFESTUAC:\"level='requireAdministrator\'"

    -- Helper functions for output path
    buildpath = function(p) return "%{wks.location}/../Bin/"..p.."/" end
    copy = function(p) return "{COPY} %{cfg.buildtarget.abspath} %{wks.location}../Bin/" .. p .. "/" end

    -- Include the projects we are going to build
    group "Client"
    include "Client/Core"
    include "Client/Launcher"
    --include "Client/Updater"

    group "Vendor"
    --include "Vendor/angelscript"
    --include "Vendor/bzip2"
    --include "Vendor/cef3"
    --include "Vendor/cryptopp"
    --include "Vendor/freetype"
    --include "Vendor/jo"
    include "Vendor/libcpuid"
    include "Vendor/libcurl"
    --include "Vendor/lz4"
    --include "Vendor/lzma"
    --include "Vendor/minhook"
    --include "Vendor/miniupnpc"
    include "Vendor/pugixml"
    --include "Vendor/RakNet"
    include "Vendor/stb"
    --include "Vendor/sqlite"
    --include "Vendor/yaml-cpp"
    --include "Vendor/zlib"
    --include "Vendor/zip"

    group "Server"
    include "Server/DS"

    group "Shared"
    include "Shared/Utility"
