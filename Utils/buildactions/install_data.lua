require 'utils'

premake.modules.install_data = {}

-- Config variables
local BIN_DIR = "Bin"
local DATA_DIR = "Shared/Data"
local CEF_DIR = "Vendor/cef3"

newaction {
    trigger = "install_data",
    description = "Installs data files",

    execute = function()
        -- Make Bin directory if it doesn't exists
        os.mkdir(BIN_DIR)
        os.mkdir(BIN_DIR .. "/SAO")

        -- Copy data files
        os.copydir(DATA_DIR, BIN_DIR)
        
        -- Copy cef3 data files
        os.copydir(CEF_DIR .. "/Release", BIN_DIR .. "/SAO", "*.dll")
        os.copydir(CEF_DIR .. "/Release", BIN_DIR .. "/SAO", "*.bin")
        os.copydir(CEF_DIR .. "/Resources", BIN_DIR .. "/SAO", "icudtl.dat")
        os.copydir(CEF_DIR .. "/Resources", BIN_DIR .. "/SAO/cef", "*.pak")
        os.copydir(CEF_DIR .. "/Resources/locales", BIN_DIR .. "/SAO/cef/locales", "en-US.pak")
    end
}

return premake.modules.install_data
