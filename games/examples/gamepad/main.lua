-- buttons from @DavitMasia (https://kronbits.itch.io/matriax-free-assets)
local GAMEPAD = -1
---@type SMGFTexture
local IMG = nil

---@type smgf.init
function smgf.init()
  IMG = smgf.graphics.new("xbox_buttons.png")
end

---@type smgf.update
function smgf.update(dt)
end

---@type smgf.draw
function smgf.draw()
  smgf.graphics.clear()

  -- DPAD
  smgf.graphics.translate(10, 40)
  smgf.graphics.draw(IMG, {241 + 100, 71, 17, 17}, 0, 0)
  if smgf.gamepad.is_down(GAMEPAD, "dpup") then
    smgf.graphics.draw(IMG, {241, 71, 17, 17}, 0, 0)
  end
  if smgf.gamepad.is_down(GAMEPAD, "dpright") then
    smgf.graphics.draw(IMG, {241 + 25, 71, 17, 17}, 0, 0)
  end
  if smgf.gamepad.is_down(GAMEPAD, "dpdown") then
    smgf.graphics.draw(IMG, {241 + 49, 71, 17, 17}, 0, 0)
  end
  if smgf.gamepad.is_down(GAMEPAD, "dpleft") then
    smgf.graphics.draw(IMG, {241 + 75, 71, 17, 17}, 0, 0)
  end

  -- lb/rb
  smgf.graphics.translate(17 + 2, -20)
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "leftshoulder") then
    dy = 22
  end
  smgf.graphics.draw(IMG, {52, 224 - dy, 17, 9}, 0, 0) -- lb
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "rightshoulder") then
    dy = 22
  end
  smgf.graphics.draw(IMG, {71, 224 - dy, 17, 9}, 17 + 2 + 10 + 2, 0) -- rb

  smgf.graphics.translate(0, 10)
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "back") then
    dy = 11
  end
  smgf.graphics.draw(IMG, {52, 257 - dy, 17, 9}, 0, 0) -- back
  local dx = 0
  if smgf.gamepad.is_down(GAMEPAD, "guide") then
    dx = 44
  end
  smgf.graphics.draw(IMG, {462 + dx, 279, 10, 8}, 17 + 2, 0) -- guide
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "start") then
    dy = 11
  end
  smgf.graphics.draw(IMG, {71, 257 - dy, 17, 9}, 17 + 2 + 10 + 2, 0) -- start

  smgf.graphics.translate(50, 0)
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "y") then
    dy = 11
  end
  smgf.graphics.draw(IMG, {21 + (10 + 2) * 0, 66 - dy, 10, 9}, 10, 9 * 0) -- y
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "x") then
    dy = 11
  end
  smgf.graphics.draw(IMG, {21 + (10 + 2) * 1, 66 - dy, 10, 9}, 0, 9 * 1) -- x
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "a") then
    dy = 11
  end
  smgf.graphics.draw(IMG, {21 + (10 + 2) * 2, 66 - dy, 10, 9}, 10, 9 * 2) -- a
  local dy = 0
  if smgf.gamepad.is_down(GAMEPAD, "b") then
    dy = 11
  end
  smgf.graphics.draw(IMG, {21 + (10 + 2) * 3, 66 - dy, 10, 9}, 20, 9 * 1) -- b
end

---@type smgf.gamepad_added
function smgf.gamepad_added(id)
  print(string.format("gamepad %d added", id))
  GAMEPAD = id
end

---@type smgf.gamepad_removed
function smgf.gamepad_removed(id)
  print(string.format("gamepad %d removed", id))
  if GAMEPAD == id then
    GAMEPAD = -1
  end
end

---@type smgf.gamepad_down
function smgf.gamepad_down(id, button_no)
  print(string.format("gamepad %d DOWN: %s", id, button_no))
end

---@type smgf.gamepad_up
function smgf.gamepad_up(id, button_no)
  print(string.format("gamepad %d UP: %s", id, button_no))
end

---@type smgf.gamepad_axismotion
function smgf.gamepad_axismotion(id, axis, value)
  print(string.format("gamepad %d AXIS: %s %f", id, axis, value))
end
