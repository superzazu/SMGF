# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

> This file is about **working on the SMGF engine**. `CLAUDE.game-template.md` is a
> separate document meant to be copied into a *game* project — it describes how to
> write games against the SMGF Lua API, not how to hack on the C code.

## What this repo is

SMGF is a small 2D game framework: a single C99 SDL3 application that runs games
written in Lua 5.5. There is no library to link against — the engine *is* the
executable, and a "game" is a folder (or a zip renamed `.smgf`) it mounts and
runs.

Dependencies: SDL3 + SDL3_image + SDL3_mixer (fetched via CMake `FetchContent`,
or found on the system for SDL3 itself), Lua 5.5 (vendored source in
`deps/lua-5.5.0`, compiled as a static lib by our own CMakeLists), PhysicsFS and
SDL_DBGP (git submodules in `deps/`). **`git clone --recursive`, or
`git submodule update --init`, is required** or the CMake configure fails.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build
```

On Mac, the local `build/` uses the **Xcode** generator, so the binary is at
`build/Debug/SMGF.app/Contents/MacOS/SMGF` and `--config Debug|Release` must be
passed to `cmake --build`.
With a Makefile/Ninja generator it lands in `build/`
directly. First configure is slow: it clones SDL3/SDL3_image/SDL3_mixer.

`-DGAME_PATH=<folder>` (default `games/default-game`) selects the game zipped
into `game.smgf` and installed alongside the binary — it is the game the
executable auto-runs when given no argument. Only that bundling is affected;
a path argument at runtime still overrides it.

Web (emscripten) and Windows cross-compile invocations are documented in
[website/docs/getting-started.md](website/docs/getting-started.md); the Windows
toolchain file lives at [scripts/cmake/windows_toolchain.cmake](scripts/cmake/windows_toolchain.cmake)
and is stale (for SDL2). CI ([.github/workflows/build.yml](.github/workflows/build.yml))
builds macOS, Windows and web on pushes to `master` and tags releases on `v*`.

Android is built separately through Gradle in `android-project/` (currently WIP, see [TODO.md](TODO.md)).

## Tests

The test suite is itself a game: [games/test-game](games/test-game) drives the whole Lua API
through `uunit.lua` (a vendored micro test library) from `smgf.init`. Run it
headless:

```bash
./build/Debug/SMGF.app/Contents/MacOS/SMGF games/test-game --hidden --max-updates=1
```

Results are printed to stdout, one summary per suite. Two caveats:

- **The process always exits 0** — a failing test does not change the exit
  status. Grep the output for `FAILURES` / non-zero `failures` or last line `TESTS PASSED : N/total`.
- **Do not pass `--mute`** when running the tests: it sets the master gain to 0
  and the audio-gain assertions fail spuriously.

To run a single suite, edit `run_tests()` in [games/test-game/main.lua](games/test-game/main.lua) — there is
no filter flag. Suites live in [games/test-game/smgf_tests.lua](games/test-game/smgf_tests.lua) as
`tests.<name> = uunit.newSuite(...)`.

Other games in `games/` are manual probes rather than assertions: `test-perf`,
`test-mem`, `test-resource-free`, `graphics-api`, `empty`, plus `examples/`.

## Lint & format

- `.clang-format` (LLVM base, 2-space, left pointers, no include sorting) —
  run `clang-format -i` on touched files; the codebase is fully formatted.
- clang-tidy: configure with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`, then build
  the `tidy` target (`cmake --build build --target tidy`), which runs
  `run-clang-tidy` over `src/`.
- Warnings are `-Wall -Wextra -pedantic` (`/W4` on MSVC) and the target is
  **C99** — no C11+ constructs.

## Architecture

Everything hangs off one `smgf` struct (a single file-scope instance `c` in
[src/main.c](src/main.c)) holding the `lua_State`, SDL window/renderer/mixer, config, and the
graphics-state stack. It is defined in [src/smgf.h](src/smgf.h).

**Entry point.** [src/main.c](src/main.c) uses SDL3's `SDL_MAIN_USE_CALLBACKS`, so there is no
`main()` — `SDL_AppInit` / `SDL_AppIterate` / `SDL_AppEvent` / `SDL_AppQuit`.
`SDL_AppInit` parses argv, resolves the game path (argv → dropped file →
bundled `game.smgf` next to the executable), inits PhysicsFS and calls
`smgf_init`. `SDL_AppEvent` translates SDL events into `smgf_l*` Lua callbacks.

**Two-layer API — this is the central convention.** Every engine feature is
written twice:

- `src/api/<module>.c` — pure C, SDL-facing, no Lua. Functions are named
  `sf_<2-letter module>_<verb>`: `sf_gr_` graphics, `sf_sy_` system, `sf_kb_`
  keyboard, `sf_ms_` mouse, `sf_au_` audio, `sf_io_` I/O, `sf_gp_` gamepad. All
  declared in [src/api.h](src/api.h).
- `src/api/<module>_lua.c` — the Lua binding: `static int l_<name>(lua_State*)`
  functions that unpack arguments with `luaL_check*`, call the `sf_*` layer, and
  turn failures into `luaL_error`. Each file ends with a `luaL_Reg` table and an
  `init_<module>(lua_State*)` that installs it as a field of the `smgf` table.

Adding a function means touching both layers, registering it in the `luaL_Reg`
table, **and** adding its LuaLS annotation in `docs-api/` (see below).

Object types (`SMGF_TYPE_TEXTURE`, `SMGF_TYPE_SOUND`, `SMGF_TYPE_FILE` in
[src/api_lua.h](src/api_lua.h)) are userdata with a metatable whose `__index` is itself and
whose `__gc` frees the resource — which is why most methods are registered
twice, once in the module table and once in the type's method table.

**Lua state setup** is [src/api_lua.c](src/api_lua.c) `lua_api_init`: it stashes the `smgf*` as
light userdata in the registry under `SMGF_PTRNAME` (every binding recovers it
via `get_smgf(L)`), opens the standard libs, then **deliberately removes** most
of `io`, part of `os`, `dofile`/`loadfile`, `package.searchpath`/`loadlib`, the
C-loader searchers and `package.cpath`. It replaces searcher #2 with
`l_smgf_searcher`, which resolves `require` through PhysicsFS with
`package.path = "./?.lua;./?/init.lua"`. **The test suite asserts this exact
sandbox surface** — changing what is removed will fail `smgf_tests.lua`, so keep
the two in sync intentionally.

**Filesystem.** All game file access goes through PhysicsFS: the game folder (or
`.smgf` zip) is mounted at `/`, and if `conf.lua` supplies both `organisation`
and `application`, `sf_sy_set_identity` mounts the per-user pref dir *in front*
of it and makes it the write directory (so saved files shadow shipped ones).
[src/smgf.c](src/smgf.c) reads files with PhysicsFS directly; `smgf.io.*` goes through
`physfssdl3.c` (from PhysicsFS `extras/`) to get `SDL_IOStream` handles.

**Frame loop** (`SDL_AppIterate`). `update_rate == -1` means a variable timestep
(`dt` = real elapsed). Any other value is a fixed timestep driven by an
accumulator, capped at `MAX_STEPS_PER_ITERATE` steps to avoid a death spiral
after a stall. Drawing always renders into `c.screen_texture` (a render target
sized `width` × `height`), which is then blitted to the window — that indirection
is what makes `zoom` and the letterboxed logical presentation work.

**Error handling.** Lua is always called through `smgf_pcall`, which installs a
traceback handler and routes failures to `smgf_set_error` — that logs and sets
`should_quit`, so a Lua error terminates the app cleanly rather than being
swallowed. C-layer failures inside a binding should become `luaL_error` instead.

## Documentation pipeline

The API reference is **generated, not written**. [docs-api/library/](docs-api/library/) holds LuaLS
(`---@meta`) annotation files — `smgf.lua` (`conf` fields), `callbacks.lua`,
and `smgf/{audio,graphics,input,io,system}.lua`. [scripts/build_docs.ts](scripts/build_docs.ts) runs
`lua-language-server --doc` over that folder and formats the JSON into
[website/docs/api.md](website/docs/api.md):

```bash
deno run --allow-run=lua-language-server --allow-read --allow-write scripts/build_docs.ts
```

Never hand-edit `website/docs/api.md` — edit `docs-api/` and regenerate.
The same folder is what game projects add to `workspace.library` in
`.luarc.json` for autocomplete, so an annotation change ships to users as
tooling, not just prose.

The site is Docusaurus in `website/` (`npm start`, `npm run build`, `npm run
typecheck`). Prose docs are the other files in `website/docs/`.

## Conventions

- Use the SDL allocators and string functions (`SDL_calloc`/`SDL_free`,
  `SDL_strlen`, `SDL_snprintf`) rather than libc ones, and the `SDL_Log*C`
  macros from [src/smgf.h](src/smgf.h) for application-category logging.
- Configuration defaults live as `*_DEFAULT` macros in [src/smgf.h](src/smgf.h); a new
  `conf.lua` field means a macro, a `smgf_config` member, a parse block in
  `load_config` ([src/smgf.c](src/smgf.c)), and a `@field` in `docs-api/library/smgf.lua`.
