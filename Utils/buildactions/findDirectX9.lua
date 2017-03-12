require "utils"

function findDirectX9()
    -- Get DirectX SDK directory from environment variables
    local dxsdkpath = os.getenv("DXSDK_DIR") or ""
    
    -- Check the path
    if dxsdkpath ~= "" then
        local filepath = string.format("%s%s", dxsdkpath, "Include/d3d9.h")
        if os.file_exists(filepath) then
            printf("Found DX9 SDK: '%s'", filepath)
            return true
        end
    end
    return false
end

function initDirectX9()
    configuration {}
    
    -- Get DirectX SDK directory from environment variables
    local dxsdkpath = os.getenv("DXSDK_DIR") or ""
        includedirs
        {
            dxsdkpath .. "Include"
        }
    
        configuration "x32"
            libdirs
            {
                dxsdkpath .. "Lib/x86"
            }
        configuration "x64"
            libdirs
            {
                dxsdkpath .. "Lib/x64"
            }

        configuration {}

        links
        {
            "d3dcompiler",
            "dxerr",
            "dxguid",
            "d3dx9",
            "d3d9",
            "winmm",
            "comctl32"
        }
    return true
end