require 'utils'

premake.modules.compose_files = {}

-- Config variables
local BIN_DIR = "Bin"
local DATA_DIR = "Shared/Data"
local CEF_DIR = "Vendor/cef3"
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
        
        -- Copy cef3 data files
        os.copydir(CEF_DIR .. "/Release", OUTPUT_DIR .. "/SAO", "*.dll")
        os.copydir(CEF_DIR .. "/Release", OUTPUT_DIR .. "/SAO", "*.bin")
        os.copydir(CEF_DIR .. "/Resources", OUTPUT_DIR .. "/SAO", "icudtl.dat")
        os.copydir(CEF_DIR .. "/Resources", OUTPUT_DIR .. "/SAO/cef", "*.pak")
        os.copydir(CEF_DIR .. "/Resources/locales", OUTPUT_DIR .. "/SAO/cef/locales", "en-US.pak")
    end
}

return premake.modules.compose_files
