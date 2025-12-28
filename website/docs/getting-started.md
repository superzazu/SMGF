---
title: Getting started
sidebar_position: 2
---

# Getting started

## installation

Download compiled binaries from GitHub [here](https://github.com/superzazu/SMGF/releases/latest) for your platform and unzip the file on your computer.

:::note

You can also [build from source](./getting-started.md#building-smgf-yourself) if your platform is not available or if you want to customise build options. SMGF should be compatible with every platform that SDL3 supports.

:::

## how to make a game

Create a folder "my_game" anywhere with a file "main.lua" inside it:

```lua
function smgf.init()

end

function smgf.update(dt)

end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.set_color(255, 0, 0)
  smgf.graphics.draw_rect(5, 5, 10, 10)
end
```

## how to run games

SMGF can run a game from either a folder or a `.smgf` file (which is a zip file of the contents of your game folder, with the `.smgf` extension).

On Mac or Windows, just drag'n'drop your folder "my_game" onto the `SMGF` executable to run it. You should see a window with a red rectangle in it.

From a terminal, you can also run:

```sh
./SMGF path/to/my_game
```

### the game loop

`smgf.init`, `smgf.update` and `smgf.draw` are the three pillars of every SMGF game, those functions will be called by SMGF at different times during execution:

- `smgf.init` is only called once at game start
- `smgf.update` is called once for every frame of the game (typically 60 times per second). The argument `dt` contains the time in seconds since last call. This is where all game updates should be done.
- `smgf.draw` is called once for every frame _after_ `smgf.update`. This function is where all draw calls (eg `smgf.graphics.draw`) should be made.

There are other callbacks, such as `smgf.key_down()` which is called when a key on the keyboard is pressed. Go to [API reference](./api.md) and search for "callback" to see them all.

### the configuration file

A configuration file named `conf.lua` can also be added next to `main.lua` to customise options such as window size, window title etc. This file is read automatically by SMGF before starting the game.

Here's an example `conf.lua` file:

```lua
local conf = {}
conf.width = 640 -- in pixels
conf.height = 480 -- in pixels
conf.window_title = "my super game"
conf.fps = 0 -- to limit FPS (set to 0 to disable FPS limiting)
conf.zoom = 1 -- a zoom
conf.cursor_visible = true -- whether to show the mouse cursor when inside window
conf.application = 'my-super-game' -- unique identifier of your game (see game identity in docs)
conf.organisation = 'my-super-organisation' -- unique identifier of your organisation (see game identity in docs)
return conf
```

## how to distribute a game

To easily share your games with people, the simple way is to ZIP all files inside your game folder, then rename that file so it has the extension `.smgf`. You can share this file with people who already have SMGF installed.

When SMGF starts, if a file named `game.smgf` exists, SMGF automatically starts this game. You can bundle your `game.smgf` file with the SMGF executable and libs to distribute to people who do not have SMGF installed on their machine.

:::tip

On macOS, put your `game.smgf` file inside `SMGF.app`: right click on the app bundle and click "Show Package Contents", then put your file in `Contents/Resources/` folder.

:::

## how to run your game on the web

You need to install [emscripten](https://emscripten.org/docs/getting_started/downloads.html).
Then, download [smgf-web](https://github.com/superzazu/SMGF/releases/latest) from GitHub.

Run the following command to run SMGF in your browser:

```sh
emrun smgf.html
```

To run your game instead of the default one, you need to package your game files a bit differently:

```sh
python3 $EMSDK_PATH/upstream/emscripten/tools/file_packager.py game.data --preload $PATH_TO_MY_GAME_FOLDER@/game
```

This will create a `game.data` file that you can use with SMGF.

## building SMGF yourself

If you want to customise build options or if there is no official release for your platform, you need to build SMGF yourself. SMGF can be compiled on macOS, Linux and Windows.

CMake is used to build the project. You can build SMGF for your platform
with those commands:

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug # or Release or RelWithDebInfo
cmake --build .
```

This should get you the `smgf` executable needed for development.

:::tip

On macOS, to compile for both Intel and Apple Silicon, add `-DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"` to the first CMake command.

:::

### cross-compiling for the web

SMGF can be cross-compiled for the web thanks to emscripten. You need
to install the [emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html#download-and-install).

You will have to use the `emcmake cmake ..` command:

```shell
mkdir build-web && cd build-web
# you have to set the variable "GAME_PATH" to specify the game you
# want to bundle with smgf:
emcmake cmake .. -DGAME_PATH=$HOME/mysupergame
cmake --build .
```

You can then upload the smgf.html/js/wasm/data files on a web server.

### cross-compiling to Windows

:::warning

This part about cross-compiling to Windows was not updated for SDL3.
You can use pre-compiled Windows binaries from GitHub instead.

:::

To cross-compile to Windows from macOS or Linux, you have to install MinGW and the [SDL2 MinGW development libraries](https://github.com/libsdl-org/SDL/releases/download/release-2.26.1/SDL2-devel-2.26.1-mingw.tar.gz).

On a Mac, you can install MinGW with Homebrew `brew install mingw-w64` and the SDL development librairies using `make install-package arch=x86_64-w64-mingw32 prefix=$(brew --prefix mingw-w64)/toolchain-x86_64/x86_64-w64-mingw32`.

When everything is set up, run:

```shell
mkdir build-win && cd build-win
cmake -DCMAKE_TOOLCHAIN_FILE=../scripts/cmake/windows_toolchain.cmake -DMINGW_PREFIX=$(brew --prefix mingw-w64)/toolchain-x86_64 ..
cmake --build .
```

DLL files are automatically copied to the folder where smgf.exe is.

## LSP annotations

Annotations for [lua-language-server](https://github.com/LuaLS/lua-language-server) (Lua LSP server) are available for the whole SMGF API. These should be compatible with every editor that supports LSP.
Here is what your `.luarc.json` file should look like:

```json
{
  "$schema": "https://raw.githubusercontent.com/LuaLS/vscode-lua/master/setting/schema.json",
  "runtime.version": "Lua 5.4",
  "workspace.library": ["/<path-to-SMGF>/docs-api"]
}
```
