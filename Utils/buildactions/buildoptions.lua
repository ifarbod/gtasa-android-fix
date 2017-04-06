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

