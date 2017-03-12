-- Premake5 script
-- Author(s):       iFarbod <ifarbod@outlook.com>
--
-- Copyright (c) 2015-2017 CtNorth Team
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

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

        -- Copy data files
        os.copydir(DATA_DIR, BIN_DIR)

        -- Copy cef3 data files
        os.copydir(CEF_DIR .. "/Release", BIN_DIR, "*.dll")
        os.copydir(CEF_DIR .. "/Release", BIN_DIR, "*.bin")
        os.copydir(CEF_DIR .. "/Resources", BIN_DIR, "icudtl.dat")
        os.copydir(CEF_DIR .. "/Resources", BIN_DIR .. "/cef", "*.pak")
        os.copydir(CEF_DIR .. "/Resources/locales", BIN_DIR .. "/cef/locales", "en-US.pak")
    end
}

return premake.modules.install_data
