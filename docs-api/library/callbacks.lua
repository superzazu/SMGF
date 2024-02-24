--- @meta

--- Callback, called once at the start of the program.
--- @alias smgf.init fun()

--- Callback, called every frame before `smgf.draw`. "dt" represents the seconds since the last call to `smgf.update`. All game updates should be done there.
--- @alias smgf.update fun(dt: number)

--- Callback, called every frame after `smgf.update`. All drawing operations should be done here.
--- @alias smgf.draw fun()

--- Callback, called on mouse release. The button number can be either 1 (left button), 2 (middle button) or 3 (right button).
--- @see smgf.mouse_down
--- @alias smgf.mouse_up fun(x: number, y: number, button_no: number)

--- Callback, called on mouse press. The button number can be either 1 (left button), 2 (middle button) or 3 (right button).
--- @see smgf.mouse_up
--- @alias smgf.mouse_down fun(x: number, y: number, button_no: number)

--- Callback, called on mouse movement. The first two arguments represent the current
--- mouse position, and the two last represent the actual mouse movement.
--- @alias smgf.mouse_moved fun(x: number, y: number, xdiff: number, ydiff: number)

--- Callback, called on mouse wheel movement.
--- @alias smgf.mouse_wheel fun(x: number, y: number)

--- Callback, called when the smgf window loses or gains focus.
--- @alias smgf.focus fun(focused: boolean)

--- A key modifier (such as "shift", "control", "command", etc.)
--- @alias SMGFKeyMod
--- | "lshift"
--- | "rshift"
--- | "lctrl"
--- | "rctrl"
--- | "lalt"
--- | "ralt"
--- | "lgui"
--- | "rgui"
--- | "num"
--- | "caps"
--- | "mode"

--- Callback, called when a keyboard key is pressed. The "mod" parameter is a table and
--- represent the key modifiers currently pressed: "lshift", "rshift", "lctrl",
--- "rctrl", "lalt", "ralt", "lgui" (Windows key or Command ⌘ on Mac),
--- "rgui", "num", "caps", "mode".
--- @alias smgf.key_down fun(key: string, mod: SMGFKeyMod[])

--- Callback, called when a keyboard key is released. The "mod" parameter is a table and
--- represent the key modifiers currently pressed: "lshift", "rshift", "lctrl",
--- "rctrl", "lalt", "ralt", "lgui" (Windows key or Command ⌘ on Mac),
--- "rgui", "num", "caps", "mode".
--- @alias smgf.key_up fun(key: string, mod: SMGFKeyMod[])

--- Callback, called on text input. Needs to be enabled first with "smgf.keyboard.set_textinput".
--- @alias smgf.text_input fun(text: string)

--- A number between 1 and 4 to identify the player. The first gamepad plugged
--- will be "1", the second "2", the third "3" etc.
--- @alias SMGFPlayerIndex number

--- Callback, called when a gamepad has been detected. The "player_index" is a number
--- between 1 and 4 to identify the player.
--- @alias smgf.gamepad_added fun(player_index: SMGFPlayerIndex)

--- Callback, called when a gamepad has been removed. The "player_index" is a number
--- between 1 and 4 to identify the player.
--- @alias smgf.gamepad_removed fun(player_index: SMGFPlayerIndex)

--- @alias SMGFGamepadButton
--- | "a"
--- | "b"
--- | "x"
--- | "y"
--- | "back"
--- | "guide"
--- | "start"
--- | "leftstick"
--- | "rightstick"
--- | "leftshoulder"
--- | "rightshoulder"
--- | "dpup"
--- | "dpdown"
--- | "dpleft"
--- | "dpright"
--- | "misc1"
--- | "paddle1"
--- | "paddle2"
--- | "paddle3"
--- | "paddle4"
--- | "touchpad"

--- Callback, called when a button has been pressed on a gamepad. The "player_index" is a number
--- between 1 and 4 to identify the player.
--- @alias smgf.gamepad_down fun(player_index: SMGFPlayerIndex, button_no: SMGFGamepadButton)

--- Callback, called when a button has been released on a gamepad. The "player_index" is a number
--- between 1 and 4 to identify the player.
--- @alias smgf.gamepad_up fun(player_index: SMGFPlayerIndex, button_no: SMGFGamepadButton)

--- @alias SMGFGamepadAxis
--- | "leftx"
--- | "lefty"
--- | "rightx"
--- | "righty"
--- | "lefttrigger"
--- | "righttrigger"

--- Callback, called when there has been an axis motion on a gamepad. The "player_index" is a number
--- between 1 and 4 to identify the player. The "axis" parameter can either be
--- "leftx", "lefty", "rightx", "righty", "lefttrigger" or "righttrigger". The "value"
--- parameter is in the range [-1;1].
--- @alias smgf.gamepad_axismotion fun(player_index: SMGFPlayerIndex, axis: SMGFGamepadAxis, value: number)

--- Callback, called on SDL_RENDER_TARGETS_RESET event.
--- @alias smgf.targets_reset fun()

--- Callback, called on SDL_RENDER_DEVICE_RESET event.
--- @alias smgf.device_reset fun()
