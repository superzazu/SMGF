--- @meta
--- @class smgf.keyboard
smgf.keyboard = {}

--- Returns whether a key is down (pressed).
--- @param key string
--- @return boolean is_down
function smgf.keyboard.is_down(key) end

--- Returns whether a key is up (not pressed).
--- @param key string
--- @return boolean is_up
function smgf.keyboard.is_up(key) end

--- Enables or disables Unicode text input events. This function controls
--- whether `smgf.text_input` callbacks are sent or not. This is
--- disabled by default.
--- @param enabled boolean
function smgf.keyboard.set_textinput(enabled) end

--- Returns whether Unicode text input events are enabled. This is
--- disabled by default.
--- @return boolean enabled
function smgf.keyboard.get_textinput() end

--- @class smgf.mouse
smgf.mouse = {}

--- Returns whether a key is pressed. Expects a number: 1 is left button, 2 is
--- middle button, 3 is right button.
--- @param button_no number
--- @return boolean is_down
function smgf.mouse.is_down(button_no) end

--- Returns the mouse position (2 values). Note that the coordinates start from
--- the top left corner of the screen.
--- @return number x
--- @return number y
function smgf.mouse.get_pos() end

--- Returns the mouse position in X. Note that the coordinates start from
--- the top left corner of the screen.
--- @return number x
function smgf.mouse.get_x() end

--- Returns the mouse position in Y. Note that the coordinates start from
--- the top left corner of the screen.
--- @return number y
function smgf.mouse.get_y() end

--- @class smgf.gamepad
smgf.gamepad = {}

--- Returns whether a given gamepad is opened and available to use.
--- @param player_index SMGFPlayerIndex
--- @return boolean is_open
function smgf.gamepad.is_open(player_index) end

--- Returns whether a button is pressed for a given gamepad.
--- @param player_index SMGFPlayerIndex
--- @param button_no SMGFGamepadButton
--- @return boolean is_down
function smgf.gamepad.is_down(player_index, button_no) end

--- Returns the current state of a given axis (value goes from -1 to 1).
--- @param player_index SMGFPlayerIndex
--- @param axis SMGFGamepadAxis
--- @return number axis_value
function smgf.gamepad.get_axis(player_index, axis) end

--- Rumbles the given gamepad.
--- @param player_index SMGFPlayerIndex
--- @param duration_sec number The duration of the rumble in seconds
--- @param lintensity number The left intensity (value between 0 and 1)
--- @param rintensity? number The right intensity (value between 0 and 1), defaults is the `lintensity`
--- @return number result 0 if everything OK, -1 if rumble not supported for this gamepad, 1 if gamepad not found
function smgf.gamepad.rumble(player_index, duration_sec, lintensity,
    rintensity) end

--- Returns the name of a gamepad.
--- @param player_index SMGFPlayerIndex
--- @return string name
function smgf.gamepad.get_name(player_index) end
