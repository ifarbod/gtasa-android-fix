--require "utils"

--included here:
function os.file_exists(name)
    local f = io.open(name, "r")
    if f ~= nil then
        io.close(f)
        return true
    else
        return false
    end
end

function FindDirectX9()
    -- Get DirectX SDK directory from environment variables
    local dxsdkpath = os.getenv("DXSDK_DIR") or ""

    -- Check the path
    if dxsdkpath ~= "" then
        -- 'I'nclude or 'i'nclude? no, windows' fs ain't case-sensitive :(
        local filepath = string.format("%s%s", dxsdkpath, "Include/d3d9.h")
        if os.file_exists(filepath) then
            printf("Found DX9 SDK: '%s'", filepath)
            return true
        end
    end
    return false
end

function InitDirectX9()
    filter {}

    -- Get DirectX SDK directory from environment variables
    local dxsdkpath = os.getenv("DXSDK_DIR") or ""
        includedirs
        {
            dxsdkpath .. "Include"
        }

        filter "platforms:x86"
            libdirs
            {
                dxsdkpath .. "Lib/x86"
            }
        filter "platforms:x64"
            libdirs
            {
                dxsdkpath .. "Lib/x64"
            }

        filter {}

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
