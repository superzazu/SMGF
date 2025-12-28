---
title: API reference
sidebar_position: 5
---

## smgf



```lua
smgf
```


---
## smgf.audio


### smgf.audio.clone {#smgf.audio.clone}

```lua
function smgf.audio.clone(sound: SMGFSound)
  -> clone: SMGFSound
```

Creates a copy of a `SMGFSound`.

### smgf.audio.get_duration {#smgf.audio.get_duration}

```lua
function smgf.audio.get_duration(sound: SMGFSound)
  -> Duration: number
```

Returns the duration of a sound in milliseconds.

@*param* `sound` — Sound to pause

@*return* `Duration` — in milliseconds

### smgf.audio.get_gain {#smgf.audio.get_gain}

```lua
function smgf.audio.get_gain(sound: SMGFSound)
  -> gain: number
```

Returns the gain ("volume") of a sound, which is a number from "0" (silence)
 to "2" (200%).

### smgf.audio.get_loop {#smgf.audio.get_loop}

```lua
function smgf.audio.get_loop(sound: SMGFSound)
  -> looped: boolean
```

Returns whether a sound is looped or not.

### smgf.audio.get_master_gain {#smgf.audio.get_master_gain}

```lua
function smgf.audio.get_master_gain()
  -> gain: number
```

Returns the output gain ("volume"), which is a value between "0" (silence)
 and "2" (200%).

@*return* `gain` — Gain (value between "0" (silence) and "2" (200%))

### smgf.audio.get_master_pause {#smgf.audio.get_master_pause}

```lua
function smgf.audio.get_master_pause()
  -> paused: boolean
```

Returns the master pause.

### smgf.audio.get_pan {#smgf.audio.get_pan}

```lua
function smgf.audio.get_pan(sound: SMGFSound)
  -> pan: number
```

Returns the panning of a sound, which is a number from "-1" (left) to "1"
 (right).

### smgf.audio.is_playing {#smgf.audio.is_playing}

```lua
function smgf.audio.is_playing(sound: SMGFSound)
  -> playing: boolean
```

Returns whether a sound is playing or not.

### smgf.audio.is_predecoded {#smgf.audio.is_predecoded}

```lua
function smgf.audio.is_predecoded(sound: SMGFSound)
  -> predecoded: boolean
```

Returns whether a sound is predecoded or not.

### smgf.audio.new {#smgf.audio.new}

```lua
function smgf.audio.new(filename: string, predecoded?: boolean)
  -> SMGFSound
```

Loads a sound file and returns a `SMGFSound`. The second argument
 controls whether the sound file should be predecoded in memory at load,
 or decoded during playback ("streamed").
 It is advised to predecode short sound files such as sound effects, and
 stream longer sound files such as music.
 SMGF only supports ogg vorbis and wav files by default (can be configured at build)

@*param* `filename` — File path

@*param* `predecoded` — Whether the sound file should be predecoded in memory at load or decoded during playback ("streamed"), defaults to true

### smgf.audio.pause {#smgf.audio.pause}

```lua
function smgf.audio.pause(sound: SMGFSound)
```

Pauses the playback of a sound.

@*param* `sound` — Sound to pause

See: [smgf.audio.play](#smgf.audio.play) to unpause the sound

### smgf.audio.play {#smgf.audio.play}

```lua
function smgf.audio.play(sound: SMGFSound, looped?: boolean)
```

Starts playing a sound.

@*param* `sound` — Sound to play

### smgf.audio.rewind {#smgf.audio.rewind}

```lua
function smgf.audio.rewind(sound: SMGFSound)
```

Rewinds a sound.

@*param* `sound` — Sound to rewind

### smgf.audio.seek {#smgf.audio.seek}

```lua
function smgf.audio.seek(sound: SMGFSound, ms: number)
```

Modifies the playback position of a sound.

@*param* `ms` — The position in milliseconds

### smgf.audio.set_gain {#smgf.audio.set_gain}

```lua
function smgf.audio.set_gain(sound: SMGFSound, gain: number)
```

Modifies the gain ("volume") of a sound. Expects a number from "0" (silence)
 to "2" (200%). Default value is "0" (100%).

### smgf.audio.set_master_gain {#smgf.audio.set_master_gain}

```lua
function smgf.audio.set_master_gain(gain: number)
```

Sets the output gain ("volume"). Expects a value between "0" (silence) and
 "2" (200%). Default value is "1" (100%).

@*param* `gain` — Gain (value between "0" (silence) and "2" (200%))

### smgf.audio.set_master_pause {#smgf.audio.set_master_pause}

```lua
function smgf.audio.set_master_pause(paused: boolean)
```

Sets the master pause: when enabled, all sound will pause until re-enabled.

### smgf.audio.set_pan {#smgf.audio.set_pan}

```lua
function smgf.audio.set_pan(sound: SMGFSound, pan: number)
```

Modifies the panning of a sound. Expects a number from "-1" (left) to "1"
 (right). Default value is "0" (center).

### smgf.audio.stop {#smgf.audio.stop}

```lua
function smgf.audio.stop(sound: SMGFSound, fade_out?: number)
```

Stops the playback of a sound (with optional fade out)

@*param* `sound` — Sound to pause

@*param* `fade_out` — Number of sample frames for fade out (defaults to 0)


---
## smgf.conf


### smgf.conf.application {#smgf.conf.application}

```lua
string?
```

Your application/game name

### smgf.conf.cursor_visible {#smgf.conf.cursor_visible}

```lua
boolean?
```

Whether mouse cursor is visible when hovering game window

### smgf.conf.fps {#smgf.conf.fps}

```lua
number?
```

FPS limiting of the game (set to 0 to disable FPS limiting)

### smgf.conf.height {#smgf.conf.height}

```lua
number?
```

Window height in pixels

### smgf.conf.organisation {#smgf.conf.organisation}

```lua
string?
```

Your organisation name

### smgf.conf.width {#smgf.conf.width}

```lua
number?
```

Window width in pixels

### smgf.conf.window_title {#smgf.conf.window_title}

```lua
string?
```

Window title name

### smgf.conf.zoom {#smgf.conf.zoom}

```lua
number?
```

Zoom of the game


---
## smgf.device_reset



---
## smgf.draw



---
## smgf.focus



---
## smgf.gamepad


### smgf.gamepad.get_axis {#smgf.gamepad.get_axis}

```lua
function smgf.gamepad.get_axis(player_index: SMGFPlayerIndex, axis: SMGFGamepadAxis)
  -> axis_value: number
```

Returns the current state of a given axis (value goes from -1 to 1).

```lua
axis:
    | "leftx"
    | "lefty"
    | "rightx"
    | "righty"
    | "lefttrigger"
    | "righttrigger"
```

### smgf.gamepad.get_name {#smgf.gamepad.get_name}

```lua
function smgf.gamepad.get_name(player_index: SMGFPlayerIndex)
  -> name: string
```

Returns the name of a gamepad.

### smgf.gamepad.is_down {#smgf.gamepad.is_down}

```lua
function smgf.gamepad.is_down(player_index: SMGFPlayerIndex, button_no: SMGFGamepadButton)
  -> is_down: boolean
```

Returns whether a button is pressed for a given gamepad.

```lua
button_no:
    | "a"
    | "b"
    | "x"
    | "y"
    | "back"
    | "guide"
    | "start"
    | "leftstick"
    | "rightstick"
    | "leftshoulder"
    | "rightshoulder"
    | "dpup"
    | "dpdown"
    | "dpleft"
    | "dpright"
    | "misc1"
    | "paddle1"
    | "paddle2"
    | "paddle3"
    | "paddle4"
    | "touchpad"
```

### smgf.gamepad.is_open {#smgf.gamepad.is_open}

```lua
function smgf.gamepad.is_open(player_index: SMGFPlayerIndex)
  -> is_open: boolean
```

Returns whether a given gamepad is opened and available to use.

### smgf.gamepad.rumble {#smgf.gamepad.rumble}

```lua
function smgf.gamepad.rumble(player_index: SMGFPlayerIndex, duration_sec: number, lintensity: number, rintensity?: number)
  -> result: number
```

Rumbles the given gamepad.

@*param* `duration_sec` — The duration of the rumble in seconds

@*param* `lintensity` — The left intensity (value between 0 and 1)

@*param* `rintensity` — The right intensity (value between 0 and 1), defaults is the `lintensity`

@*return* `result` — 0 if everything OK, -1 if rumble not supported for this gamepad, 1 if gamepad not found


---
## smgf.gamepad_added



---
## smgf.gamepad_axismotion



---
## smgf.gamepad_down



---
## smgf.gamepad_removed



---
## smgf.gamepad_up



---
## smgf.graphics


### smgf.graphics.clear {#smgf.graphics.clear}

```lua
function smgf.graphics.clear(r: number, g: number, b: number, a?: number)
```

Clears the current target with a given color.

@*param* `r` — Red component (0 - 255)

@*param* `g` — Green component (0 - 255)

@*param* `b` — Blue component (0 - 255)

@*param* `a` — Alpha component (0 - 255)

### smgf.graphics.draw {#smgf.graphics.draw}

```lua
function smgf.graphics.draw(texture: SMGFTexture, x?: number, y?: number, scale_x?: number, scale_y?: number, rotation?: number, origin_x?: number, origin_y?: number, flip?: SMGFFlip)
```

Draws a texture at position `(x, y)`, scaled to `(scale_x, scale_y)`,
 rotated of `rotation` degrees around `(origin_x, origin_y)`
 and optionally flipped ("none", "horizontal" or "vertical"). To only display
 a portion of the texture, pass a `SMGFQuad` (table with four numbers) as second parameter.

---

@*param* `texture` — The texture to draw

@*param* `x` — The position to draw to (X), defaults to 0

@*param* `y` — The position to draw to (Y), defaults to 0

@*param* `scale_x` — Scale value (X), defaults to 1

@*param* `scale_y` — Scale value (Y), defaults to 1

@*param* `rotation` — The rotation in degrees, defaults to 0

@*param* `origin_x` — The origin of the rotation (X), defaults to 0

@*param* `origin_y` — The origin of the rotation (Y), defaults to 0

@*param* `flip` — To flip the texture when drawing, defaults to "none"

```lua
flip:
    | "none"
    | "horizontal"
    | "vertical"
```

### smgf.graphics.draw_line {#smgf.graphics.draw_line}

```lua
function smgf.graphics.draw_line(x1: number, y1: number, x2: number, y2: number)
```

Draws a line from `(x1, y1)` to `(x2, y2)`.

@*param* `x1` — The position to draw the line from (X)

@*param* `y1` — The position to draw the line from (Y)

@*param* `x2` — The position to draw the line to (X)

@*param* `y2` — The position to draw the line to (Y)

### smgf.graphics.draw_point {#smgf.graphics.draw_point}

```lua
function smgf.graphics.draw_point(x: number, y: number)
```

Draws a point at `(x, y)`.

@*param* `x` — The position to draw to (X)

@*param* `y` — The position to draw to (Y)

### smgf.graphics.draw_rect {#smgf.graphics.draw_rect}

```lua
function smgf.graphics.draw_rect(x: number, y: number, width: number, height: number)
```

Draws a rectangle at `(x, y)` of size `(width, height)`.

@*param* `x` — The position to draw to (X)

@*param* `y` — The position to draw to (Y)

@*param* `width` — The width of the rectangle

@*param* `height` — The height of the rectangle

### smgf.graphics.draw_rectfill {#smgf.graphics.draw_rectfill}

```lua
function smgf.graphics.draw_rectfill(x: number, y: number, width: number, height: number)
```

Draws a filled rectangle at `(x, y)` of size `(width, height)`.

@*param* `x` — The position to draw to (X)

@*param* `y` — The position to draw to (Y)

@*param* `width` — The width of the rectangle

@*param* `height` — The height of the rectangle

### smgf.graphics.get_blend_mode {#smgf.graphics.get_blend_mode}

```lua
function smgf.graphics.get_blend_mode()
  -> mode: SMGFBlendMode
```

Returns the blend mode used for drawing operations (points, lines, rects).
 By default, the blend mode is set to "blend".

```lua
--  Blend mode.
mode:
    | "none"
    | "blend"
    | "add"
    | "mod"
    | "mul"
```

See: [SMGFTexture.get_blend_mode](#SMGFTexture.get_blend_mode)

### smgf.graphics.get_color {#smgf.graphics.get_color}

```lua
function smgf.graphics.get_color()
  -> r: number
  2. g: number
  3. b: number
  4. a: number
```

Returns the current color.

@*return* `r` — Red component (0 - 255)

@*return* `g` — Green component (0 - 255)

@*return* `b` — Blue component (0 - 255)

@*return* `a` — Alpha component (0 - 255)

### smgf.graphics.get_point {#smgf.graphics.get_point}

```lua
function smgf.graphics.get_point(x: number, y: number)
  -> r: number
  2. g: number
  3. b: number
```

Returns the color a single point on texture (or screen).
 **WARNING**: for testing uses only, do not use this function.

@*return* `r` — Red component (0 - 255)

@*return* `g` — Green component (0 - 255)

@*return* `b` — Blue component (0 - 255)

### smgf.graphics.get_target {#smgf.graphics.get_target}

```lua
function smgf.graphics.get_target()
  -> texture: SMGFTexture|nil
```

Returns the current target, which can either be a texture or "nil" (the screen).

@*return* `texture` — Current target

### smgf.graphics.get_translation {#smgf.graphics.get_translation}

```lua
function smgf.graphics.get_translation()
  -> x: number
  2. y: number
```

Returns the current drawing origin.

### smgf.graphics.new {#smgf.graphics.new}

```lua
function smgf.graphics.new(filename: string)
  -> SMGFTexture
```

Loads an image into memory, and returns a texture. Note that smgf
 only supports PNG files by default (can be configured at build)

@*param* `filename` — File path

### smgf.graphics.new {#smgf.graphics.new}

```lua
function smgf.graphics.new(width: number, height: number)
  -> SMGFTexture
```

Creates a new empty texture which can be drawn upon.

@*param* `width` — Width of texture to create

@*param* `height` — Height of texture to create

See: [smgf.graphics.set_target](#smgf.graphics.set_target)

### smgf.graphics.pop_state {#smgf.graphics.pop_state}

```lua
function smgf.graphics.pop_state()
```

Destroys the current graphic state and activates the previous one. If no
 previous state exists, smgf automatically creates a new "blank" state.

### smgf.graphics.print {#smgf.graphics.print}

```lua
function smgf.graphics.print(x: number, y: number, text: string, bg_color?: number[])
```

Draws text using an internal debug font, using SMGF current color.
 Meant as a quick way to display debug info on screen.

@*param* `x` — The position to draw to (X)

@*param* `y` — The position to draw to (Y)

@*param* `text` — The text to draw (encoded in ISO-8859-1)

@*param* `bg_color` — Background color (R, G, B, A components)

### smgf.graphics.print_color {#smgf.graphics.print_color}

```lua
function smgf.graphics.print_color(x: number, y: number, color: number, text: string)
```

Draws text using an internal debug font.
 Meant as a quick way to display debug info on screen.

@*param* `x` — The position to draw to (X)

@*param* `y` — The position to draw to (Y)

@*param* `color` — Expects a number between 0-15 (CGA 16-color palette). Pass `0x0F` to draw text in white with transparent background.

@*param* `text` — The text to draw (encoded in ISO-8859-1)

### smgf.graphics.push_state {#smgf.graphics.push_state}

```lua
function smgf.graphics.push_state()
```

Creates a new graphic state and activates it.
 Use "smgf.graphics.pop_state" to re-activate the previous state.

### smgf.graphics.reset_state {#smgf.graphics.reset_state}

```lua
function smgf.graphics.reset_state()
```

Resets the current graphic state (color, drawing origin, current target).

### smgf.graphics.screenshot {#smgf.graphics.screenshot}

```lua
function smgf.graphics.screenshot(filename: string)
```

Takes a screenshot of the screen and saves it as a BMP. Filename must end
 with ".bmp".

@*param* `filename` — The filename (must end with ".bmp")

### smgf.graphics.set_blend_mode {#smgf.graphics.set_blend_mode}

```lua
function smgf.graphics.set_blend_mode(mode?: SMGFBlendMode)
```

Sets the blend mode used for drawing of points, lines and rects.
 By default, the blend mode is set to "blend".

@*param* `mode` — Defaults to "blend" if nil

```lua
--  Blend mode.
mode:
    | "none"
    | "blend"
    | "add"
    | "mod"
    | "mul"
```

See: [SMGFTexture.set_blend_mode](#SMGFTexture.set_blend_mode)

### smgf.graphics.set_color {#smgf.graphics.set_color}

```lua
function smgf.graphics.set_color(r: number, g: number, b: number, a?: number)
```

Sets the current color. If no parameter is passed, color is set to opaque white (all 255).

@*param* `r` — Red component (0 - 255)

@*param* `g` — Green component (0 - 255)

@*param* `b` — Blue component (0 - 255)

@*param* `a` — Alpha component (0 - 255), defaults to 255

### smgf.graphics.set_target {#smgf.graphics.set_target}

```lua
function smgf.graphics.set_target(texture: SMGFTexture|nil)
```

Sets the current target to a texture. All future draws will be executed
 on this texture. Pass "nil" to draw directly to the screen.

@*param* `texture` — The texture to set as target or nil to draw directly to the screen

### smgf.graphics.set_translation {#smgf.graphics.set_translation}

```lua
function smgf.graphics.set_translation(x: number, y: number)
```

Sets the current drawing origin.

### smgf.graphics.translate {#smgf.graphics.translate}

```lua
function smgf.graphics.translate(x: number, y: number)
```

Adds "x" and "y" to the drawing origin: all future drawing
 operations will be translated.


---
## smgf.init



---
## smgf.io


### smgf.io.delete {#smgf.io.delete}

```lua
function smgf.io.delete(filename: string)
```

Deletes a file.

@*param* `filename` — The filename to delete

### smgf.io.exists {#smgf.io.exists}

```lua
function smgf.io.exists(filename: string)
  -> exists: boolean
```

Returns whether a file exists.

@*param* `filename` — The filename to query

### smgf.io.mkdir {#smgf.io.mkdir}

```lua
function smgf.io.mkdir(dirname: string)
```

Creates a directory.

@*param* `dirname` — The directory to create

### smgf.io.open {#smgf.io.open}

```lua
function smgf.io.open(filename: string, mode: "a"|"r"|"w")
  -> SMGFFile
```

Opens a file for use.

@*param* `filename` — The filename to open

@*param* `mode` — Whether to open the file in read (r), write (w) mode, or append (a) mode.

```lua
mode:
    | "r"
    | "w"
    | "a"
```

### smgf.io.type {#smgf.io.type}

```lua
function smgf.io.type(filename: string)
  -> type: "directory"|"file"|"other"|"symlink"
```

Returns a file type.

@*param* `filename` — The filename to query

@*return* `type` — The file type

```lua
type:
    | "file"
    | "directory"
    | "symlink"
    | "other"
```


---
## smgf.key_down



---
## smgf.key_up



---
## smgf.keyboard


### smgf.keyboard.get_textinput {#smgf.keyboard.get_textinput}

```lua
function smgf.keyboard.get_textinput()
  -> enabled: boolean
```

Returns whether Unicode text input events are enabled. This is
 disabled by default.

### smgf.keyboard.is_down {#smgf.keyboard.is_down}

```lua
function smgf.keyboard.is_down(key: string)
  -> is_down: boolean
```

Returns whether a key is down (pressed).

### smgf.keyboard.is_up {#smgf.keyboard.is_up}

```lua
function smgf.keyboard.is_up(key: string)
  -> is_up: boolean
```

Returns whether a key is up (not pressed).

### smgf.keyboard.set_textinput {#smgf.keyboard.set_textinput}

```lua
function smgf.keyboard.set_textinput(enabled: boolean)
```

Enables or disables Unicode text input events. This function controls
 whether `smgf.text_input` callbacks are sent or not. This is
 disabled by default.


---
## smgf.mouse


### smgf.mouse.get_pos {#smgf.mouse.get_pos}

```lua
function smgf.mouse.get_pos()
  -> x: number
  2. y: number
```

Returns the mouse position (2 values). Note that the coordinates start from
 the top left corner of the screen.

### smgf.mouse.get_x {#smgf.mouse.get_x}

```lua
function smgf.mouse.get_x()
  -> x: number
```

Returns the mouse position in X. Note that the coordinates start from
 the top left corner of the screen.

### smgf.mouse.get_y {#smgf.mouse.get_y}

```lua
function smgf.mouse.get_y()
  -> y: number
```

Returns the mouse position in Y. Note that the coordinates start from
 the top left corner of the screen.

### smgf.mouse.is_down {#smgf.mouse.is_down}

```lua
function smgf.mouse.is_down(button_no: number)
  -> is_down: boolean
```

Returns whether a key is pressed. Expects a number: 1 is left button, 2 is
 middle button, 3 is right button.


---
## smgf.mouse_down



---
## smgf.mouse_moved



---
## smgf.mouse_up



---
## smgf.mouse_wheel



---
## smgf.system


### smgf.system.get_clipboard {#smgf.system.get_clipboard}

```lua
function smgf.system.get_clipboard()
  -> clipboard: string
```

Returns the value of the user clipboard.

@*return* `clipboard` — The contents of clipboard

### smgf.system.get_cursor_visible {#smgf.system.get_cursor_visible}

```lua
function smgf.system.get_cursor_visible()
  -> visible: boolean
```

Returns whether the cursor is visible when mouse hovers over game window.

@*return* `visible` — Whether the cursor is visible

### smgf.system.get_dimensions {#smgf.system.get_dimensions}

```lua
function smgf.system.get_dimensions()
  -> width: number
  2. height: number
```

Returns the dimensions of the screen.

@*return* `width` — The width of the screen

@*return* `height` — The height of the screen

### smgf.system.get_dt {#smgf.system.get_dt}

```lua
function smgf.system.get_dt()
  -> dt: number
```

Returns the delta time sent to the most recent call of `smgf.update`.

@*return* `dt` — Delta time in seconds

### smgf.system.get_fps {#smgf.system.get_fps}

```lua
function smgf.system.get_fps()
  -> fps: number
```

Returns the FPS (frames per second) limit of the game.

@*return* `fps` — FPS limit of the game (0 if FPS limiting is disabled)

### smgf.system.get_fullscreen {#smgf.system.get_fullscreen}

```lua
function smgf.system.get_fullscreen()
  -> fullscreen: boolean
```

Returns whether the full screen mode is enabled.

@*return* `fullscreen` — Delta Whether fullscreen is enabled or not

### smgf.system.get_height {#smgf.system.get_height}

```lua
function smgf.system.get_height()
  -> height: number
```

Returns the height of the screen.

@*return* `height` — The height of the screen

### smgf.system.get_identity {#smgf.system.get_identity}

```lua
function smgf.system.get_identity()
  -> organisation: string
  2. application: string
```

Returns the identity for the game.

@*return* `organisation` — The name of the organisation

@*return* `application` — The name of the application

### smgf.system.get_platform {#smgf.system.get_platform}

```lua
function smgf.system.get_platform()
  -> platform: string
```

Returns the platform on which the game is running on: "Linux", "macOS", "Windows" or "Emscripten".

@*return* `platform` — The platform on which the game is running on

### smgf.system.get_preferred_locales {#smgf.system.get_preferred_locales}

```lua
function smgf.system.get_preferred_locales()
  -> locales: string[]|nil
```

Returns a list of user preferred locales (or nil if it is impossible
 to retrieve). Only languages codes (ISO-639) are returned.

### smgf.system.get_version {#smgf.system.get_version}

```lua
function smgf.system.get_version()
  -> smgf: string
```

Returns smgf version (in form "1.0.0")

@*return* `smgf` — version

### smgf.system.get_width {#smgf.system.get_width}

```lua
function smgf.system.get_width()
  -> width: number
```

Returns the width of the screen.

@*return* `width` — The width of the screen

### smgf.system.get_window_title {#smgf.system.get_window_title}

```lua
function smgf.system.get_window_title()
  -> title: string
```

Returns the window's title.

@*return* `title` — The window title

### smgf.system.get_write_dir {#smgf.system.get_write_dir}

```lua
function smgf.system.get_write_dir()
  -> path: string|nil
```

Returns the path to the writeable directory, or nil if the identity of the
 game has not been set.

@*return* `path` — The path to the writeable directory

See: [smgf.system.set_identity](#smgf.system.set_identity)

### smgf.system.get_zoom {#smgf.system.get_zoom}

```lua
function smgf.system.get_zoom()
  -> zoom: number
```

Returns the zoom of the game.

@*return* `zoom` — Zoom of the game

### smgf.system.iconv {#smgf.system.iconv}

```lua
function smgf.system.iconv(to: string, from: string, str: string)
  -> converted: string
```

Converts a string between encodings.

@*param* `to` — The source encoding

@*param* `from` — The target encoding

@*param* `str` — The string to convert

@*return* `converted` — The converted string

### smgf.system.log {#smgf.system.log}

```lua
function smgf.system.log(str: string)
```

Logs a string

@*param* `str` — String to log

### smgf.system.open_url {#smgf.system.open_url}

```lua
function smgf.system.open_url(url: string)
```

Opens a URL. Can be either a local file path or a HTTPS URL.

@*param* `url` — The URL to open

### smgf.system.quit {#smgf.system.quit}

```lua
function smgf.system.quit()
```

Exits smgf gracefully.

### smgf.system.set_clipboard {#smgf.system.set_clipboard}

```lua
function smgf.system.set_clipboard(contents: string)
```

Sets the user clipboard to some value.

@*param* `contents` — The string to put in clipboard

### smgf.system.set_cursor_visible {#smgf.system.set_cursor_visible}

```lua
function smgf.system.set_cursor_visible(visible: boolean)
```

Toggles cursor visibility when mouse hovers over game window.

@*param* `visible` — Whether the cursor should be visible

### smgf.system.set_dimensions {#smgf.system.set_dimensions}

```lua
function smgf.system.set_dimensions(width: number, height: number)
```

Sets the dimensions of the screen.

@*param* `width` — The width of the screen

@*param* `height` — The height of the screen

### smgf.system.set_fps {#smgf.system.set_fps}

```lua
function smgf.system.set_fps(fps: number|nil)
```

Sets the FPS (frames per second) limit.

@*param* `fps` — The fps of the screen (or nil to reset limit)

### smgf.system.set_fullscreen {#smgf.system.set_fullscreen}

```lua
function smgf.system.set_fullscreen(fullscreen: boolean)
```

Enables or disables the full screen mode.

@*param* `fullscreen` — Whether to enable fullscreen or not

### smgf.system.set_identity {#smgf.system.set_identity}

```lua
function smgf.system.set_identity(organisation: string, application: string)
```

Sets the identity for the game. Expects two strings: the name of your
 organisation and your application name.
 These two strings are used to open a writeable directory on the user
 computer. For maximum compatibility, try to avoid special characters.
 It is recommended to set identity using `conf.lua` file instead of calling
 this function.

@*param* `organisation` — The name of your organisation

@*param* `application` — The name of your application

### smgf.system.set_window_title {#smgf.system.set_window_title}

```lua
function smgf.system.set_window_title(title: string)
```

Sets the window title.

@*param* `title` — The window title

### smgf.system.set_zoom {#smgf.system.set_zoom}

```lua
function smgf.system.set_zoom(zoom: number|nil)
```

Sets the zoom.

@*param* `zoom` — The zoom of the screen (or nil to reset zoom)

### smgf.system.show_message {#smgf.system.show_message}

```lua
function smgf.system.show_message(title: string, message: string, level?: "error"|"info"|"warning")
```

Opens a dialog to the user, with a given title and message.

@*param* `level` — Defaults to "info"

```lua
level:
    | "info"
    | "warning"
    | "error"
```

### smgf.system.sleep {#smgf.system.sleep}

```lua
function smgf.system.sleep(seconds: number)
```

Waits for a given time (in seconds) before returning to the program.

@*param* `seconds` — Sleep time in seconds


---
## smgf.targets_reset



---
## smgf.text_input



---
## smgf.update



---
## SMGFBlendMode



---
## SMGFFile


### SMGFFile.close {#SMGFFile.close}

```lua
(method) SMGFFile:close()
```

Closes a file. Beware that the file object will not be usable after calling this function.

### SMGFFile.flush {#SMGFFile.flush}

```lua
(method) SMGFFile:flush()
  -> offset: number
```

Flushs a buffered file.

### SMGFFile.read {#SMGFFile.read}

```lua
(method) SMGFFile:read(mode: "all"|"line")
  -> string
```

Reads from a file. Pass "all" to read all file at once, "line" to read
 the current line, or a number to read an arbitrary number of bytes from file.
 Make sure the file was opened with "r" flag or it will trigger an error.

---

@*param* `mode` — Whether to read the current line in file, or the whole file.

```lua
mode:
    | "all"
    | "line"
```

### SMGFFile.rewind {#SMGFFile.rewind}

```lua
(method) SMGFFile:rewind()
```

Sets the file cursor position to the beginning of file.

### SMGFFile.seek {#SMGFFile.seek}

```lua
(method) SMGFFile:seek(offset: number, mode?: "cur"|"end"|"set")
  -> final_offset: number
```

Sets the file cursor position.

@*param* `offset` — The offset in bytes

@*param* `mode` — `cur` = seek relative to current read point, `set` = seek from the beginning of data, `end` = seek relative to the end of data, defaults to `cur`

@*return* `final_offset` — The offset after operation

```lua
mode:
    | "set"
    | "cur"
    | "end"
```

### SMGFFile.size {#SMGFFile.size}

```lua
(method) SMGFFile:size()
  -> size: number
```

Returns file size in bytes.

@*return* `size` — Size in bytes

### SMGFFile.tell {#SMGFFile.tell}

```lua
(method) SMGFFile:tell()
  -> offset: number
```

Returns current cursor position.

### SMGFFile.write {#SMGFFile.write}

```lua
(method) SMGFFile:write(data: string)
```

Writes a string to a file.
 Make sure the file was opened with "w" flag or it will trigger an error.

@*param* `data` — The data to write to file


---
## SMGFFlip



---
## SMGFGamepadAxis



---
## SMGFGamepadButton



---
## SMGFKeyMod



---
## SMGFPlayerIndex



---
## SMGFQuad



---
## SMGFSound


### SMGFSound.clone {#SMGFSound.clone}

```lua
(method) SMGFSound:clone()
  -> clone: SMGFSound
```

Creates a copy of a `SMGFSound`.

### SMGFSound.get_duration {#SMGFSound.get_duration}

```lua
(method) SMGFSound:get_duration()
  -> Duration: number
```

Returns the duration of a sound in milliseconds.

@*return* `Duration` — in milliseconds

### SMGFSound.get_gain {#SMGFSound.get_gain}

```lua
(method) SMGFSound:get_gain()
  -> gain: number
```

Returns the gain ("volume") of a sound, which is a number from "0" (silence)
 to "2" (200%).

### SMGFSound.get_loop {#SMGFSound.get_loop}

```lua
(method) SMGFSound:get_loop()
  -> looped: boolean
```

Returns whether a sound is looped or not.

### SMGFSound.get_pan {#SMGFSound.get_pan}

```lua
(method) SMGFSound:get_pan()
  -> pan: number
```

Returns the panning of a sound, which is a number from "-1" (left) to "1"
 (right).

### SMGFSound.is_playing {#SMGFSound.is_playing}

```lua
(method) SMGFSound:is_playing()
  -> playing: boolean
```

Returns whether a sound is playing or not.

### SMGFSound.is_predecoded {#SMGFSound.is_predecoded}

```lua
(method) SMGFSound:is_predecoded()
  -> predecoded: boolean
```

Returns whether a sound is predecoded or not.

### SMGFSound.pause {#SMGFSound.pause}

```lua
(method) SMGFSound:pause()
```

Pauses the playback of a sound.

### SMGFSound.play {#SMGFSound.play}

```lua
(method) SMGFSound:play(looped?: boolean)
```

Starts playing a sound.

### SMGFSound.rewind {#SMGFSound.rewind}

```lua
(method) SMGFSound:rewind()
```

Rewinds a sound.

### SMGFSound.seek {#SMGFSound.seek}

```lua
(method) SMGFSound:seek(ms: number)
```

Modifies the playback position of a sound.

@*param* `ms` — The position in milliseconds

### SMGFSound.set_gain {#SMGFSound.set_gain}

```lua
(method) SMGFSound:set_gain(gain: number)
```

Modifies the gain ("volume") of a sound. Expects a number from "0" (silence)
 to "2" (200%). Default value is "0" (100%).

### SMGFSound.set_pan {#SMGFSound.set_pan}

```lua
(method) SMGFSound:set_pan(pan: number)
```

Modifies the panning of a sound. Expects a number from "-1" (left) to "1"
 (right). Default value is "0" (center).

### SMGFSound.stop {#SMGFSound.stop}

```lua
(method) SMGFSound:stop(fade_out?: number)
```

Stops the playback of a sound (with optional fade out)

@*param* `fade_out` — Number of sample frames for fade out (defaults to 0)


---
## SMGFTexture


### SMGFTexture.draw {#SMGFTexture.draw}

```lua
(method) SMGFTexture:draw(x?: number, y?: number, scale_x?: number, scale_y?: number, rotation?: number, origin_x?: number, origin_y?: number, flip?: SMGFFlip)
```

Draws the texture.

---

@*param* `x` — The position to draw to (X), defaults to 0

@*param* `y` — The position to draw to (Y), defaults to 0

@*param* `scale_x` — Scale value (X), defaults to 1

@*param* `scale_y` — Scale value (Y), defaults to 1

@*param* `rotation` — The rotation in degrees, defaults to 0

@*param* `origin_x` — The origin of the rotation (X), defaults to 0

@*param* `origin_y` — The origin of the rotation (Y), defaults to 0

@*param* `flip` — To flip the texture when drawing, defaults to "none"

```lua
flip:
    | "none"
    | "horizontal"
    | "vertical"
```

See: [smgf.graphics.draw](#smgf.graphics.draw) For the list of all parameters

### SMGFTexture.get_blend_mode {#SMGFTexture.get_blend_mode}

```lua
(method) SMGFTexture:get_blend_mode()
  -> mode: SMGFBlendMode
```

Returns the blend mode used when drawing the texture ("blend" by default).

```lua
--  Blend mode.
mode:
    | "none"
    | "blend"
    | "add"
    | "mod"
    | "mul"
```

### SMGFTexture.get_dimensions {#SMGFTexture.get_dimensions}

```lua
(method) SMGFTexture:get_dimensions()
  -> width: number
  2. height: number
```

Returns the dimensions (width and height) of a texture.

@*return* `width` — Width of texture

@*return* `height` — Height of texture

### SMGFTexture.get_height {#SMGFTexture.get_height}

```lua
(method) SMGFTexture:get_height()
  -> height: number
```

Returns the height of a texture.

@*return* `height` — Height of texture

### SMGFTexture.get_width {#SMGFTexture.get_width}

```lua
(method) SMGFTexture:get_width()
  -> width: number
```

Returns the width of a texture.

@*return* `width` — Width of texture

### SMGFTexture.save {#SMGFTexture.save}

```lua
(method) SMGFTexture:save(filename: string)
```

Saves the texture as a BMP file. Filename must end with ".bmp".

@*param* `filename` — The filename (must end with ".bmp")

### SMGFTexture.set_blend_mode {#SMGFTexture.set_blend_mode}

```lua
(method) SMGFTexture:set_blend_mode(mode?: SMGFBlendMode)
```

Sets the blend mode used when drawing the texture.

@*param* `mode` — Defaults to "blend" if nil

```lua
--  Blend mode.
mode:
    | "none"
    | "blend"
    | "add"
    | "mod"
    | "mul"
```


---
