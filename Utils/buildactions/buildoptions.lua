-- CTNorth build configuration script
-- Author(s):       iFarbod <ifarbod@outlook.com>
--
-- Copyright (c) 2015-2017 CTNorth Team
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

-- Set CI Build global
local ci = os.getenv("CI")
if ci and ci:lower() == "true" then
    CI_BUILD = true
else
    CI_BUILD = false
end

---
-- Build options
---

-- Game
newoption {
    trigger = "game",
    value = "VALUE",
    description = "Specify the target game to build CTN for",
    allowed = {
        -- SA
        {"sa", "GTA: San Andreas"},
        {"north", "GTA: San Andreas"},
        {"gtasa", "GTA: San Andreas"},
        -- VC
        {"vc", "GTA: Vice City"},
        {"miami", "GTA: Vice City"},
        {"gtavc", "GTA: Vice City"},
        -- 3
        {"lc", "GTA 3"},
        {"3", "GTA 3"},
        {"iii", "GTA 3"},
        {"liberty", "GTA 3"},
        {"gta3", "GTA 3"},
        {"gtaiii", "GTA 3"}
    }
}

-- LuaJIT
newoption {
    trigger = "luajit",
    value = "VALUE",
    description = "Specify wheather to use or not use LuaJIT",
}

if not _OPTIONS["game"] then
    _OPTIONS["game"] = "north"
end

if not _OPTIONS["luajit"] then
    _OPTIONS["luajit"] = "false"
end

TARGET_GAME = _OPTIONS["game"]:togame()
USE_LUAJIT = _OPTIONS["luajit"]:tobool()
