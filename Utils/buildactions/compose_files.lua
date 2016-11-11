require 'utils'

premake.modules.compose_files = {}

-- Config variables
local BIN_DIR = "Bin"
local DATA_DIR = "Shared/Data"
local OUTPUT_DIR = "InstallFiles"

newaction {
    trigger = "compose_files",
    description = "Composes files that are required for building the installer",
    
    execute = function()
        os.mkdir(OUTPUT_DIR)
        
        -- Copy data files
        os.copydir(DATA_DIR .. "/SAO", OUTPUT_DIR .. "/SAO")
        
        -- Copy the binaries
        os.copydir(BIN_DIR, OUTPUT_DIR, "**.exe")
        os.copydir(BIN_DIR, OUTPUT_DIR, "**.dll")
        os.copydir(BIN_DIR, OUTPUT_DIR, "**.pak")
        os.copydir(BIN_DIR, OUTPUT_DIR, "**.bin")
        os.copydir(BIN_DIR, OUTPUT_DIR, "**.dat")
    end
}

return premake.modules.compose_files
