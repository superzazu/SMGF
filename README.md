# SMGF

SMGF is a small framework to make 2D games in Lua for the web, macOS,
Linux and Windows.

Features:

- minimal Lua (5.5) API, with exports for the web, macOS, Linux and Windows
- LSP annotations available for the whole API
- a simple hardware-accelerated 2D API powered by the SDL3
- streamed and static audio sources from multiple sound formats (ogg and wav by default, others formats such as mp3, flac, and tracker formats can be configured on build)
- inputs from mouse, keyboard and game controllers
- sandboxed IO environment to save/load data on the player computer, which
  allows for game modding out of the box
- open source under the zlib license, which means you can use it and
  distribute your games freely and free of charge

[Get started here!](https://superzazu.github.io/SMGF/docs/getting-started/)

## examples

Display an image on screen:

```lua
function smgf.init()
  my_texture = smgf.graphics.new("my_image.png")
end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.draw(my_texture, 5, 5)
end
```

Play a sound on key press:

```lua
function smgf.init()
  sound = smgf.audio.new("sfx.wav")
end

function smgf.key_down(key)
  if key == "return" then
    sound:play()
  end
end
```
