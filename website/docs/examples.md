---
title: Examples
sidebar_position: 3
---

# Examples

## drawing basic shapes

```lua
function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.set_color(255, 0, 0)
  smgf.graphics.draw_rect(5, 5, 10, 10)
  smgf.graphics.set_color(0, 255, 0)
  smgf.graphics.draw_rectfill(50, 50, 10, 10)
end
```

## displaying a texture

```lua
function smgf.init()
  my_texture = smgf.graphics.new("my_image.png")
end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.draw(my_texture, 5, 5)
end
```

Note that "my_image.png" needs to be inside your game folder (alongside "main.lua").

## responding to user input

```lua
local x, y = 10, 10

function smgf.init()

end

function smgf.update(dt)

end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.set_color(255, 0, 0)
  smgf.graphics.draw_rect(5, 5, 10, 10)
  smgf.graphics.set_color(0, 255, 0)
  smgf.graphics.draw_rect(x, y, 10, 10)
end

function smgf.key_down(key)
  if key == "up" then
    y = y - 1
  elseif key == "down" then
    y = y + 1
  elseif key == "left" then
    x = x - 1
  elseif key == "right" then
    x = x + 1
  end
end
```

## drawing onto a texture (offscreen rendering)

```lua
texture = nil

function smgf.init()
  -- creates a texture of 100x100 pixels
  texture = smgf.graphics.new(100, 100)
  -- all draw calls after this will be done on texture and not on screen
  smgf.graphics.set_target(texture)
  smgf.graphics.draw_rect(10, 10, 10, 10)
  -- ...
  smgf.graphics.set_target(nil)
end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.draw(texture, 0, 0)
  smgf.graphics.draw_rect(20, 20, 20, 20)
end
```

## playing music and sound effects

By default, smgf only supports `.ogg` and `.wav` files (this can be configured on build).

```lua
function smgf.init()
  -- the sound effect is small, we want to predecode it all in memory:
  sound = smgf.audio.new("sfx.wav", true)
  -- the music sound file is bigger, we may want to stream it instead:
  music = smgf.audio.new("music.ogg", false)

  music:set_gain(0.5) -- between 0 (silence) and 2 (200%)
  music:set_loop(true)
  music:play()
end

function smgf.key_down(key)
  if key == "return" then
    sound:play()
  end
end
```

## save and load data from user computer

You can save files on the user's computer. Depending on the user's OS, files will be stored here:

- macOS: `~/Library/Application Support/<your-application-name>`
- Linux: `~/.local/share/<your-application-name>`
- Windows: `C:\Users\...\AppData\Roaming\<your-organisation-name>\<your-application-name>`

The "application name" and "organisation name" can be set in `conf.lua` (preferred way) or directly in code by using `smgf.system.set_identity()`.

```lua
function smgf.init()
  smgf.system.set_identity("my-super-organisation", "my-super-game")

  local filename = "savestate.txt"

  if smgf.io.exists(filename) then
    local file = smgf.io.open(filename, "w")
    file:write("hello world")
    file:close()
  else
    local file = smgf.io.open(filename, "r")
    local file_contents = file:read("all")
    print(file_contents) -- hello world
  end
end
```
