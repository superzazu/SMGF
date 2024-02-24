local GameOfLife = require "gol"

---@class GameOfLife
local gol
local timer = 0
local pause = false
local smgf_str = "SMGF " .. smgf.system.get_version()
local winw, winh = smgf.system.get_dimensions()

---@type smgf.init
function smgf.init()
  local width, height = smgf.system.get_dimensions()
  gol = GameOfLife:create():init(width, height)
end

---@type smgf.update
function smgf.update(dt)
  if smgf.mouse.is_down(1) then
    local x, y = smgf.mouse.get_pos()
    gol.cells[math.floor(y / gol.size) * gol.width + math.floor(x / gol.size)] =
        1
  end
  if smgf.mouse.is_down(3) then
    local x, y = smgf.mouse.get_pos()
    gol.cells[math.floor(y / gol.size) * gol.width + math.floor(x / gol.size)] =
        0
  end

  if not pause then
    timer = timer + dt
    if timer > 0.2 then
      timer = 0

      gol:step()
    end
  end
end

---@type smgf.draw
function smgf.draw()
  gol:draw()
  -- smgf.graphics.print_color(0, 0, 0x25, gol.nb_steps)

  smgf.graphics.set_color(0x5c, 0xab, 0x5e, 0xff)
  smgf.graphics.print(winw / 2 - (10 * 8) / 2, 16 * 3, smgf_str,
      {0x9a, 0xe2, 0x9b})
end

---@type smgf.key_down
function smgf.key_down(key)
  if key == 'r' then
    gol:randomize_cells()
  end
  if key == 'c' then
    gol:clear_cells()
  end
  if key == 'space' then
    pause = not pause
  end
end
