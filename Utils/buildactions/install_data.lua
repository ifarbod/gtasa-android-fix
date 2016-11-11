require 'utils'

premake.modules.install_data = {}

-- Config variables
local BIN_DIR = "Bin"
local DATA_DIR = "Shared/Data"

newaction {
    trigger = "install_data",
    description = "Installs data files",

    execute = function()
        -- Make Bin directory if it doesn't exists
        os.mkdir(BIN_DIR)

        -- Copy data files
        os.copydir(DATA_DIR, BIN_DIR)
    end
}

return premake.modules.install_data
