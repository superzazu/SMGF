---
title: Introduction
sidebar_position: 1
---

# SMGF

SMGF (Superzazu's Mini Game Framework) is a small framework to make 2D games in Lua for the web, macOS,
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

## examples

Displaying a red rectangle of 10x10 pixels:

```lua
function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.set_color(255, 0, 0)
  smgf.graphics.draw_rectfill(10, 10)
end
```

Displaying an image at a given position:

```lua
function smgf.init()
  player = smgf.graphics.new("player.png")
end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.draw(player, 50, 50)
end
```

Playing a sound on key press:

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

Check out the [Getting started](./getting-started.md) to learn more.

## license

SMGF is distributed under the zlib license, allowing you to modify the
source code, build it, redistribute it, and even use it in a commercial
product.

```txt
zlib License

(C) 2018-2020 Nicolas ALLEMAND

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
```

## libraries used

Internally, SMGF uses the following libraries:

- [SDL3](https://www.libsdl.org/)
- [SDL3_image](https://www.libsdl.org/)
- [SDL3_mixer](https://www.libsdl.org/)
- [Lua](http://www.lua.org)
- [physfs](https://www.icculus.org/physfs/)
