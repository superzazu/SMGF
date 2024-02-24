local rotation = 0
---@type SMGFTexture
local monster = nil

---@type smgf.init
function smgf.init()
  monster = smgf.graphics.new("monster1.png")
end

---@type smgf.update
function smgf.update(dt)
  rotation = rotation + 100 * dt
end

---@type smgf.draw
function smgf.draw()
  smgf.graphics.clear()

  -- red rectangle
  smgf.graphics.set_color(255, 0, 0, 255)
  smgf.graphics.draw_rectfill(5, 5, 50, 50)

  -- drawing a blue rectangle, semi-transparent
  smgf.graphics.set_color(0, 0, 255, 255 / 2)
  smgf.graphics.draw_rectfill(30, 30, 50, 50)

  -- drawing 4 points (should be outside the rectangles)
  smgf.graphics.set_color(0, 255, 0, 255)
  smgf.graphics.translate(5 + 50 + 5, 5)
  smgf.graphics.draw_point(0, 0)
  smgf.graphics.translate(50, 0)
  smgf.graphics.draw_point(0, 0)
  smgf.graphics.translate(0, 50)
  smgf.graphics.draw_point(0, 0)
  smgf.graphics.translate(-50, 0)
  smgf.graphics.draw_point(0, 0)

  smgf.graphics.set_translation(0, 0)
  smgf.graphics.set_color(0, 255, 255, 100)
  smgf.graphics.draw_line(0, 0, 90, 90)

  -- drawing sprites
  smgf.graphics.set_color(255, 255, 255, 255)
  smgf.graphics.translate(80, 5)
  smgf.graphics.draw(monster, 0, 25 * 0)
  smgf.graphics.draw(monster, 0, 25 * 1)
  smgf.graphics.draw(monster, 0, 25 * 2)
  smgf.graphics.draw(monster, 0, 25 * 3)

  -- semi transparent red sprites
  smgf.graphics.set_color(255, 0, 255, 255 / 2)
  smgf.graphics.translate(1, 1)
  smgf.graphics.draw(monster, 0, 25 * 0)
  smgf.graphics.draw(monster, 0, 25 * 1)
  smgf.graphics.draw(monster, 0, 25 * 2)
  smgf.graphics.draw(monster, 0, 25 * 3)

  -- rotation + flipping
  smgf.graphics.set_translation(0, 0)
  smgf.graphics.translate(100, 5)
  smgf.graphics.set_color(255, 255, 255, 255)
  smgf.graphics.draw(monster, 0, 0, 2, 1.5, 0, 0, 0, "vertical")

  -- scaling + flipping
  smgf.graphics.translate(10, 45)
  smgf.graphics.draw(monster, 0, 0, 1, 1, rotation, 16, 16, "horizontal")
end
