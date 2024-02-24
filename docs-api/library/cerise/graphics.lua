--- @meta
--- @class smgf.graphics
smgf.graphics = {}

--- A texture that can be draw on screen (or on an offscreen target)
--- @class SMGFTexture
local Texture = {}

--- Returns the dimensions (width and height) of a texture.
--- @return number width Width of texture
--- @return number height Height of texture
function Texture:get_dimensions() end

--- Returns the width of a texture.
--- @return number width Width of texture
function Texture:get_width() end

--- Returns the height of a texture.
--- @return number height Height of texture
function Texture:get_height() end

--- Blend mode.
--- @alias SMGFBlendMode
--- | "none"
--- | "blend"
--- | "add"
--- | "mod"
--- | "mul"

--- Sets the blend mode used when drawing the texture.
--- @param mode? SMGFBlendMode Defaults to "blend" if nil
function Texture:set_blend_mode(mode) end

--- Returns the blend mode used when drawing the texture ("blend" by default).
--- @return SMGFBlendMode mode
function Texture:get_blend_mode() end

--- Draws the texture.
--- @see smgf.graphics.draw For the list of all parameters
--- @overload fun(self: SMGFTexture, quad: SMGFQuad, x?: number, y?: number, scale_x?: number, scale_y?: number, rotation?: number, origin_x?: number, origin_y?: number, flip?: SMGFFlip)
--- @param x? number The position to draw to (X), defaults to 0
--- @param y? number The position to draw to (Y), defaults to 0
--- @param scale_x? number Scale value (X), defaults to 1
--- @param scale_y? number Scale value (Y), defaults to 1
--- @param rotation? number The rotation in degrees, defaults to 0
--- @param origin_x? number The origin of the rotation (X), defaults to 0
--- @param origin_y? number The origin of the rotation (Y), defaults to 0
--- @param flip? SMGFFlip To flip the texture when drawing, defaults to "none"
function Texture:draw(x, y, scale_x, scale_y, rotation, origin_x, origin_y, flip)
end

--- Saves the texture as a BMP file. Filename must end with ".bmp".
--- @param filename string The filename (must end with ".bmp")
function Texture:save(filename) end

-- @MARK: graphics module

--- Loads an image into memory, and returns a texture. Note that smgf
--- only supports PNG files by default (can be configured at build)
--- @param filename string File path
--- @return SMGFTexture
function smgf.graphics.new(filename) end

--- Creates a new empty texture which can be drawn upon.
--- @see smgf.graphics.set_target
--- @param width number Width of texture to create
--- @param height number Height of texture to create
--- @return SMGFTexture
function smgf.graphics.new(width, height) end

--- Sets the current target to a texture. All future draws will be executed
--- on this texture. Pass "nil" to draw directly to the screen.
--- @param texture SMGFTexture | nil The texture to set as target or nil to draw directly to the screen
function smgf.graphics.set_target(texture) end

--- Returns the current target, which can either be a texture or "nil" (the screen).
--- @return SMGFTexture | nil texture Current target
function smgf.graphics.get_target() end

--- Sets the current color. If no parameter is passed, color is set to opaque white (all 255).
--- @param r number Red component (0 - 255)
--- @param g number Green component (0 - 255)
--- @param b number Blue component (0 - 255)
--- @param a? number Alpha component (0 - 255), defaults to 255
--- @overload fun(rgba: number[])
--- @overload fun() Resets color to opaque white
function smgf.graphics.set_color(r, g, b, a) end

--- Returns the current color.
--- @return number r Red component (0 - 255)
--- @return number g Green component (0 - 255)
--- @return number b Blue component (0 - 255)
--- @return number a Alpha component (0 - 255)
function smgf.graphics.get_color() end

--- Clears the current target with a given color.
--- @param r number Red component (0 - 255)
--- @param g number Green component (0 - 255)
--- @param b number Blue component (0 - 255)
--- @param a? number Alpha component (0 - 255)
--- @overload fun(rgba: number[])
--- @overload fun() Clears the screen in black
function smgf.graphics.clear(r, g, b, a) end

--- Sets the blend mode used for drawing of points, lines and rects.
--- By default, the blend mode is set to "blend".
--- @see SMGFTexture.set_blend_mode
--- @param mode? SMGFBlendMode Defaults to "blend" if nil
function smgf.graphics.set_blend_mode(mode) end

--- Returns the blend mode used for drawing operations (points, lines, rects).
--- By default, the blend mode is set to "blend".
--- @see SMGFTexture.get_blend_mode
--- @return SMGFBlendMode mode
function smgf.graphics.get_blend_mode() end

--- Creates a new graphic state and activates it.
--- Use "smgf.graphics.pop_state" to re-activate the previous state.
function smgf.graphics.push_state() end

--- Destroys the current graphic state and activates the previous one. If no
--- previous state exists, smgf automatically creates a new "blank" state.
function smgf.graphics.pop_state() end

--- Resets the current graphic state (color, drawing origin, current target).
function smgf.graphics.reset_state() end

--- Adds "x" and "y" to the drawing origin: all future drawing
--- operations will be translated.
--- @param x number
--- @param y number
function smgf.graphics.translate(x, y) end

--- Returns the current drawing origin.
--- @return number x
--- @return number y
function smgf.graphics.get_translation() end

--- Sets the current drawing origin.
--- @param x number
--- @param y number
function smgf.graphics.set_translation(x, y) end

--- Takes a screenshot of the screen and saves it as a BMP. Filename must end
--- with ".bmp".
--- @param filename string The filename (must end with ".bmp")
function smgf.graphics.screenshot(filename) end

--- @alias SMGFFlip
--- | "none"
--- | "horizontal"
--- | "vertical"

--- A quadrilateral, used to work on a portion of a texture.
--- Requires 4 elements: x, y, width and height.
--- @alias SMGFQuad number[]

--- Draws a texture at position `(x, y)`, scaled to `(scale_x, scale_y)`,
--- rotated of `rotation` degrees around `(origin_x, origin_y)`
--- and optionally flipped ("none", "horizontal" or "vertical"). To only display
--- a portion of the texture, pass a `SMGFQuad` (table with four numbers) as second parameter.
--- @overload fun(texture: SMGFTexture, quad: SMGFQuad, x?: number, y?: number, scale_x?: number, scale_y?: number, rotation?: number, origin_x?: number, origin_y?: number, flip?: SMGFFlip)
--- @param texture SMGFTexture The texture to draw
--- @param x? number The position to draw to (X), defaults to 0
--- @param y? number The position to draw to (Y), defaults to 0
--- @param scale_x? number Scale value (X), defaults to 1
--- @param scale_y? number Scale value (Y), defaults to 1
--- @param rotation? number The rotation in degrees, defaults to 0
--- @param origin_x? number The origin of the rotation (X), defaults to 0
--- @param origin_y? number The origin of the rotation (Y), defaults to 0
--- @param flip? SMGFFlip To flip the texture when drawing, defaults to "none"
function smgf.graphics.draw(texture, x, y, scale_x, scale_y, rotation,
    origin_x, origin_y, flip) end

--- Returns the color a single point on texture (or screen).
--- **WARNING**: for testing uses only, do not use this function.
--- @param x number
--- @param y number
--- @return number r Red component (0 - 255)
--- @return number g Green component (0 - 255)
--- @return number b Blue component (0 - 255)
function smgf.graphics.get_point(x, y) end

--- Draws a point at `(x, y)`.
--- @param x number The position to draw to (X)
--- @param y number The position to draw to (Y)
function smgf.graphics.draw_point(x, y) end

--- Draws a line from `(x1, y1)` to `(x2, y2)`.
--- @param x1 number The position to draw the line from (X)
--- @param y1 number The position to draw the line from (Y)
--- @param x2 number The position to draw the line to (X)
--- @param y2 number The position to draw the line to (Y)
function smgf.graphics.draw_line(x1, y1, x2, y2) end

--- Draws a rectangle at `(x, y)` of size `(width, height)`.
--- @param x number The position to draw to (X)
--- @param y number The position to draw to (Y)
--- @param width number The width of the rectangle
--- @param height number The height of the rectangle
function smgf.graphics.draw_rect(x, y, width, height) end

--- Draws a filled rectangle at `(x, y)` of size `(width, height)`.
--- @param x number The position to draw to (X)
--- @param y number The position to draw to (Y)
--- @param width number The width of the rectangle
--- @param height number The height of the rectangle
function smgf.graphics.draw_rectfill(x, y, width, height) end

--- Draws text using an internal debug font.
--- Meant as a quick way to display debug info on screen.
--- @param x number The position to draw to (X)
--- @param y number The position to draw to (Y)
--- @param color number Expects a number between 0-15 (CGA 16-color palette). Pass `0x0F` to draw text in white with transparent background.
--- @param text string The text to draw (encoded in ISO-8859-1)
function smgf.graphics.print_color(x, y, color, text) end

--- Draws text using an internal debug font, using SMGF current color.
--- Meant as a quick way to display debug info on screen.
--- @param x number The position to draw to (X)
--- @param y number The position to draw to (Y)
--- @param text string The text to draw (encoded in ISO-8859-1)
--- @param bg_color? number[] Background color (R, G, B, A components)
function smgf.graphics.print(x, y, text, bg_color) end
