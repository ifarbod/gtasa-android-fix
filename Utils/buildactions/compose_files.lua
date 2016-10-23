require 'utils'

premake.modules.compose_files = {}

-- Config variables
local BIN_DIR = "Bin"
local DATA_DIR = "Shared/Data"
local OUTPUT_DIR = "InstallFiles"
local WINDOWS = os.get() == "windows"

newaction {
    trigger = "compose_files",
    description = "Composes files that are required for building the installer",
    
    execute = function()
        os.mkdir(OUTPUT_DIR)
        
        -- Copy data files
        if WINDOWS then
            os.copydir(DATA_DIR.."/SAO", OUTPUT_DIR.."/SAO")
        end
        
        -- Copy the binaries
        if WINDOWS then
            os.copydir(BIN_DIR, OUTPUT_DIR, "**.exe")
            os.copydir(BIN_DIR, OUTPUT_DIR, "**.dll")
        else
            os.copydir(BIN_DIR, OUTPUT_DIR, "**.so")
            os.copydir(BIN_DIR, OUTPUT_DIR, "**sao-server*")
        end
    end
}

return premake.modules.compose_files