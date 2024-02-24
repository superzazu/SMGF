-- tries to load the same resource 20k times
-- used to check whether "l_texture_del"-type functions are called correctly
local TIMES = 20000

local resource = nil

---@type smgf.init
function smgf.init()
  for i = 1, TIMES do
    resource = smgf.graphics.new("monster1.png")
    resource = nil
  end
  collectgarbage("collect")
  print("done")
end
