local NB_MONSTERS = 200

local monsters = {}
---@type SMGFTexture
local tex = nil

---@type smgf.init
function smgf.init()
  tex = smgf.graphics.new("monster1.png")

  for i = 1, NB_MONSTERS do
    monsters[i] = {}
    monsters[i].x = math.random(0, 800 - 32)
    monsters[i].y = math.random(0, 600 - 32)
    monsters[i].vx = (math.random() * 2 - 1) * 150
    monsters[i].vy = (math.random() * 2 - 1) * 150
  end
end

---@type smgf.update
function smgf.update(dt)
  for i = 1, NB_MONSTERS do
    monsters[i].x = monsters[i].x + monsters[i].vx * dt
    monsters[i].y = monsters[i].y + monsters[i].vy * dt

    if monsters[i].x < 0 or monsters[i].x > 800 - 32 then
      monsters[i].vx = -monsters[i].vx
    end
    if monsters[i].y < 0 or monsters[i].y > 600 - 32 then
      monsters[i].vy = -monsters[i].vy
    end
  end
end

---@type smgf.draw
function smgf.draw()
  smgf.graphics.clear()
  for i = 1, NB_MONSTERS do
    -- smgf.graphics.draw_rectfill(monsters[i].x, monsters[i].y, 16*2,16*2)
    smgf.graphics.draw(tex, monsters[i].x, monsters[i].y)
  end

  -- smgf.graphics.set_color(0, 0, 0, 140)
  -- smgf.graphics.draw_rectfill(640 - 35 - 96, 480 - 30, 35, 30)
  smgf.graphics.set_color(255, 255, 255, 255)
  smgf.graphics.print_color(0, 0, 0x1F, "NB MONSTERS: " .. NB_MONSTERS)
  smgf.graphics.print_color(0, 16, 0x1F, tostring(1 / smgf.system.get_dt()))
end

---@type smgf.key_down
function smgf.key_down(key, mod)
  if key == "s" then
    smgf.graphics.screenshot("screenshot.bmp")
  end
end

---@type smgf.mouse_down
function smgf.mouse_down(x, y, button_no)
  for i = NB_MONSTERS, NB_MONSTERS * 2 do
    monsters[i] = {}
    monsters[i].x = math.random(0, 800 - 32)
    monsters[i].y = math.random(0, 600 - 32)
    monsters[i].vx = (math.random() * 2 - 1) * 150
    monsters[i].vy = (math.random() * 2 - 1) * 150
  end
  NB_MONSTERS = NB_MONSTERS * 2
end
