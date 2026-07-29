# SMGF games — notes for AI agents

> Copy this file into a game project as its `CLAUDE.md`. It describes how to
> **write games for SMGF**, not how to work on the SMGF engine itself.

## What SMGF is

A small 2D game framework: an SDL3 application that runs games written in
**Lua 5.5**. You write Lua; SMGF provides graphics, audio, input, and a
sandboxed filesystem. There is no scene graph, no ECS, no physics engine —
it is an immediate-mode drawing API and a callback loop.

## Anatomy of a game

A game is a folder with two files at its root:

- **`conf.lua`** — *returns a table* of configuration. Optional; defaults apply.
- **`main.lua`** — defines callbacks on the global `smgf` table.

```lua
-- conf.lua
return {
  window_title = "My Game",
  width = 384, height = 216,
  zoom = 3,
  organisation = "acme",   -- required to write files (see "Saving files")
  application  = "my-game",
}
```

```lua
-- main.lua
function smgf.init() end          -- once, at startup
function smgf.update(dt) end      -- every frame; dt in seconds
function smgf.draw() end          -- every frame, after update
```

Other callbacks: `focus`, `key_down`/`key_up` (`key`, `mod`), `text_input`,
`mouse_down`/`mouse_up`/`mouse_moved`/`mouse_wheel`,
`gamepad_added`/`gamepad_removed`/`gamepad_down`/`gamepad_up`/`gamepad_axismotion`,
`targets_reset`, `device_reset`. All are optional. Full signatures are in
`docs-api/library/callbacks.lua`.

## Documentation — read this before writing code

- **`website/docs/api.md`** — the complete API reference. **This is the most
  important file**: it lists every function with parameters and return values.
  Consult it rather than guessing at function names.
- `website/docs/getting-started.md`, `tutorials.md`, `examples.md` — prose docs.
- **`docs-api/`** — [LuaLS](https://github.com/LuaLS/lua-language-server)
  definitions, usable for autocomplete and type-checking. Useful for exact
  field lists that read awkwardly in prose — e.g. `docs-api/library/smgf.lua`
  holds every `conf.lua` field (`update_rate`, `vsync`, `zoom`, …) with its
  default.
- `games/` in the SMGF repo — working example games.

## Running a game

```sh
<path-to-SMGF-binary> <path-to-game-folder>
```

CLI flags (from `--help`):

| flag | effect |
|---|---|
| `--help` | print usage and exit |
| `--mute` | silence output (audio is still loaded and decoded) |
| `--hidden` | run without showing the window |
| `--max-updates=N` | quit after N calls to `smgf.update()` |

`--hidden`, `--mute` and `--max-updates=N` together give a deterministic,
non-interactive run — the right way to script a test or a screenshot capture
instead of launching the game and killing it after a `sleep`.

## Timing: `update_rate` and `vsync`

Both are `conf.lua` fields, both are **developer** decisions, not player settings.

- `update_rate = -1` (default) — **variable timestep**: `smgf.update(dt)` is
  called once per frame with the real elapsed time.
- `update_rate = 30` / `60` — **fixed timestep**: `dt` is always exactly
  `1/update_rate`. Use this when simulation must be reproducible.
- `vsync = true` (default) — leave it on. It paces the loop and prevents
  tearing. Under a **fixed** timestep, disabling it does *not* change how often
  `smgf.update` runs — the accumulator is driven by real elapsed time — but
  `smgf.draw` and the present then run unthrottled, thousands of times a
  second, re-presenting identical frames and burning CPU/GPU for nothing.
  Under a **variable** timestep, disabling it uncaps both. There is no reason
  to turn it off in a shipped game.

## The filesystem is sandboxed — this trips people up

SMGF runs Lua through PhysicsFS, and standard Lua file access is **removed**:

- `dofile` and `loadfile` are `nil`.
- `io.open`, `io.read`, `io.lines`, `io.close`, `io.input`, `io.output`,
  `io.popen`, `io.type`, `io.stdin`, `io.tmpfile` are all `nil`.
  (`io.write`, `io.flush`, `io.stdout`, `io.stderr` survive.)
- `os.execute`, `os.exit`, `os.getenv`, `os.remove`, `os.rename`,
  `os.setlocale`, `os.tmpname` are `nil`.
- `package.cpath` is empty and the C-library loaders are removed — no native
  modules.

**Use `smgf.io.*` for all file access** (`open`, `exists`, `mkdir`, `delete`,
`type`). Paths are virtual and relative to the game root — `"assets/hero.png"`,
never an absolute host path.

`require` **does** work (`require "src.world"`): SMGF installs a PhysicsFS
searcher, with `package.path = "./?.lua;./?/init.lua"` resolved from the game
root. Prefer it for splitting a game into modules.

## Saving files ("pref path")

To write anything, `conf.lua` must set **both** `organisation` and
`application`. SMGF then creates a per-user directory (via
`PHYSFS_getPrefDir`), mounts it, and makes it the **write directory**.
`smgf.system.get_write_dir()` returns its host path, or `nil` if identity was
never set — so writes silently have nowhere to go without those two fields.

Two consequences worth knowing:

- The pref path is **prepended** to the search path, so it takes priority over
  the game folder. A file written to the pref dir **shadows** a same-named file
  shipped with the game — which is exactly what you want for save files.
- Therefore `smgf.io.exists("assets/font.png")` is true for shipped assets
  *and* `smgf.io.exists("save.dat")` is true for written files; reads search
  both locations, writes always land in the pref dir.

## Asset formats

- **Images: PNG only** by default. Prefer PNG with transparency.
- **Audio: Ogg Vorbis and WAV** only by default. Predecode short sounds
  (default), stream long music: `smgf.audio.new(file, false)`.

## Gotchas

- **Texture draws are modulated by the current draw colour.** Every texture
  draw applies `SDL_SetTextureColorMod`/`AlphaMod` from the current graphics
  state. Leave the colour at something dark or transparent and your sprites
  render dark or invisible. Call `smgf.graphics.set_color(255, 255, 255, 255)`
  before drawing sprites. (The same mechanism cheaply tints a white bitmap
  font.)
- **Key names come from SDL scancode names** — `"Left Shift"`, `"Space"`,
  `"Escape"`, `"A"`. An unknown name **raises a Lua error**, it does not
  return false. This is deliberate: it surfaces typos immediately.
- **Looping is an argument to `play`**, not a setter: `sound:play(true)`.
  There is no `set_loop`.
- **`smgf.graphics.screenshot(name)` writes into the write directory**, so it
  needs `organisation` + `application` set in `conf.lua` like any other write.
