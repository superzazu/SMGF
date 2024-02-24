--- @meta
--- version: 1.0
--- @class smgf
smgf = {}

--- A table containing game configuration (this should be the value returned
--- by `conf.lua` file)
--- @class smgf.conf
--- @field width number? Window width in pixels
--- @field height number? Window height in pixels
--- @field window_title string? Window title name
--- @field fps number? FPS limiting of the game (set to 0 to disable FPS limiting)
--- @field zoom number? Zoom of the game
--- @field cursor_visible boolean? Whether mouse cursor is visible when hovering game window
--- @field organisation string? Your organisation name
--- @field application string? Your application/game name
