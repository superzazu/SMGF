--- @meta
--- version: 1.0
--- @class smgf
smgf = {}

--- A table containing game configuration (this should be the value returned
--- by `conf.lua` file)
--- @class smgf.conf
--- @field width number? Window width in pixels (default: 256)
--- @field height number? Window height in pixels (default: 256)
--- @field window_title string? Window title name
--- @field update_rate number? Represnts how often should smgf.update() be called. Can be set to a FPS value (eg 30, 60) for a fixed timestep or -1 for a variable timestep. (default: -1)
--- @field vsync boolean? Vertical synchronisation (default: true)
--- @field zoom number? Zoom of the game (float) (default: 1)
--- @field cursor_visible boolean? Whether mouse cursor is visible when hovering game window (default: true)
--- @field organisation string? Your organisation name. Required to be able to mount the "pref path" and write files to player device.
--- @field application string? Your application/game name. Required to be able to mount the "pref path" and write files to player device.
