-- test game that tests the SMGF Lua API
local uunit = require "uunit"
local test_suites = require "smgf_tests"

local success = true

local run_tests = function()
  local nbTests = 0
  local nbTestsFailed = 0

  for _, suite in pairs(test_suites) do
    local run = uunit.run(suite)
    if run.nbTestsFailed > 0 then
      success = false
    end
    nbTests = nbTests + run.nbTestsExecuted
    nbTestsFailed = nbTestsFailed + run.nbTestsFailed
    print()
  end

  local nbTestsPassed = nbTests - nbTestsFailed
  print("\nTESTS PASSED: " .. nbTestsPassed .. "/" .. nbTests)
end

---@type smgf.init
function smgf.init()
  run_tests()
end

---@type smgf.update
function smgf.update(dt)
end

---@type smgf.draw
function smgf.draw()
  smgf.graphics.set_color(255, 255, 255)
  smgf.graphics.clear(0x11, 0x11, 0x11)
  if success then
    smgf.graphics.print_color(8, 8, 0x2f, "SMGF TEST SUITE: SUCCESS")
  else
    smgf.graphics.print_color(8, 8, 0x4f, "SMGF TEST SUITE: FAIL")
  end
end

---@type smgf.key_down
function smgf.key_down(key, mod)
  if key == "return" or key == "escape" then
    smgf.system.quit()
  end
  if key == "r" then
    run_tests()
  end
end
