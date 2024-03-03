--- @meta
--- @class smgf.system
smgf.system = {}

--- Exits smgf gracefully.
function smgf.system.quit() end

--- Returns the platform on which the game is running on: "Linux", "Mac OS X", "Windows" or "Emscripten".
--- @return string platform The platform on which the game is running on
function smgf.system.get_platform() end

--- Waits for a given time (in seconds) before returning to the program.
--- @param seconds number Sleep time in seconds
function smgf.system.sleep(seconds) end

--- Returns the delta time sent to the most recent call of `smgf.update`.
--- @return number dt Delta time in seconds
function smgf.system.get_dt() end

--- Returns whether the full screen mode is enabled.
--- @return boolean fullscreen Delta Whether fullscreen is enabled or not
function smgf.system.get_fullscreen() end

--- Sets the user clipboard to some value.
--- @param contents string The string to put in clipboard
function smgf.system.set_clipboard(contents) end

--- Returns the value of the user clipboard.
--- @return string clipboard The contents of clipboard
function smgf.system.get_clipboard() end

--- Sets the identity for the game. Expects two strings: the name of your
--- organisation and your application name.
--- These two strings are used to open a writeable directory on the user
--- computer. For maximum compatibility, try to avoid special characters.
--- It is recommended to set identity using `conf.lua` file instead of calling
--- this function.
--- @param organisation string The name of your organisation
--- @param application string The name of your application
--- @overload fun() Calling this function with no parameter resets org/app values.
function smgf.system.set_identity(organisation, application) end

--- Returns the identity for the game.
--- @return string organisation The name of the organisation
--- @return string application The name of the application
function smgf.system.get_identity() end

--- Returns the path to the writeable directory, or nil if the identity of the
--- game has not been set.
--- @return string|nil path The path to the writeable directory
--- @see smgf.system.set_identity
function smgf.system.get_write_dir() end

--- Returns the dimensions of the screen.
--- @return number width The width of the screen
--- @return number height The height of the screen
function smgf.system.get_dimensions() end

--- Sets the dimensions of the screen.
--- @param width number The width of the screen
--- @param height number The height of the screen
function smgf.system.set_dimensions(width, height) end

--- Returns the width of the screen.
--- @return number width The width of the screen
function smgf.system.get_width() end

--- Returns the height of the screen.
--- @return number height The height of the screen
function smgf.system.get_height() end

--- Returns the FPS (frames per second) limit of the game.
--- @return number fps FPS limit of the game (0 if FPS limiting is disabled)
function smgf.system.get_fps() end

--- Sets the FPS (frames per second) limit.
--- @param fps number|nil The fps of the screen (or nil to reset limit)
function smgf.system.set_fps(fps) end

--- Returns the zoom of the game.
--- @return number zoom Zoom of the game
function smgf.system.get_zoom() end

--- Sets the zoom.
--- @param zoom number|nil The zoom of the screen (or nil to reset zoom)
function smgf.system.set_zoom(zoom) end

--- Enables or disables the full screen mode.
--- @param fullscreen boolean Whether to enable fullscreen or not
function smgf.system.set_fullscreen(fullscreen) end

--- Toggles cursor visibility when mouse hovers over game window.
--- @param visible boolean Whether the cursor should be visible
function smgf.system.set_cursor_visible(visible) end

--- Returns whether the cursor is visible when mouse hovers over game window.
--- @return boolean visible Whether the cursor is visible
function smgf.system.get_cursor_visible() end

--- Sets the window title.
--- @param title string The window title
function smgf.system.set_window_title(title) end

--- Returns the window's title.
--- @return string title The window title
function smgf.system.get_window_title() end

--- Opens a URL. Can be either a local file path or a HTTPS URL.
--- @param url string The URL to open
function smgf.system.open_url(url) end

--- Converts a string between encodings.
--- @param to string The source encoding
--- @param from string The target encoding
--- @param str string The string to convert
--- @return string converted The converted string
function smgf.system.iconv(to, from, str) end

--- Opens a dialog to the user, with a given title and message.
--- @param title string
--- @param message string
--- @param level? "info" | "warning" | "error" Defaults to "info"
function smgf.system.show_message(title, message, level) end

--- Returns a list of user preferred locales (or nil if it is impossible
--- to retrieve). Only languages codes (ISO-639) are returned.
--- @return string[] | nil locales
function smgf.system.get_preferred_locales() end

--- Returns smgf version (in form "1.0.0")
--- @return string smgf version
function smgf.system.get_version() end

--- Logs a string
--- @param str string String to log
function smgf.system.log(str) end
