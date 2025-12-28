local uunit = require "uunit"
local assert_true = uunit.assertTrue
local assert_false = uunit.assertFalse
local assert_equal = uunit.assertEq
local assert_not_equal = uunit.assertNotEq
local assert_raises = uunit.assertRaises
local assert_str_in = uunit.assertStrIn
local assert_nil = uunit.assertNil
local assert_not_nil = uunit.assertNotNil
local assert_type = uunit.assertType
local assert_almost_eq = uunit.assertAlmostEq

local tests = {}

local function start_up()
  smgf.system.set_identity()
  smgf.graphics.reset_state()
  smgf.graphics.clear(0, 0, 0)
  smgf.graphics.set_blend_mode("blend")
end

local function tear_down()
end

--
-- GENERAL
--

tests.general = uunit.newSuite("general")

tests.general:test("package searchers", function()
  assert_equal(package.path, "./?.lua;./?/init.lua")
  assert_equal(package.cpath, "")

  assert_equal(#package.searchers, 2)
end)

tests.general:test("can load modules", function()
  local mod1 = require "testfiles.testmodule1"
  assert_equal(mod1.testmoduleno, 1)
  local mod2 = require "testfiles.testmodule2"
  assert_equal(mod2.testmoduleno, 2)

  -- trying to load a module from the same directory:
  local mod3 = require "uunit"
end)

tests.general:test("some functions from std lib are not available", function()
  -- most of these functions/librairies are removed for security
  assert_equal(io.close, nil)
  assert_equal(io.input, nil)
  assert_equal(io.lines, nil)
  assert_equal(io.open, nil)
  assert_equal(io.output, nil)
  assert_equal(io.popen, nil)
  assert_equal(io.read, nil)
  assert_equal(io.stdin, nil)
  assert_equal(io.tmpfile, nil)
  assert_equal(io.type, nil)

  assert_equal(os.execute, nil)
  assert_equal(os.exit, nil)
  assert_equal(os.getenv, nil)
  assert_equal(os.remove, nil)
  assert_equal(os.rename, nil)
  assert_equal(os.setlocale, nil)
  assert_equal(os.tmpname, nil)
  assert_equal(package.searchpath, nil)
  assert_equal(package.loadlib, nil)
  assert_equal(dofile, nil)
  assert_equal(loadfile, nil)
end)

--
-- GRAPHICS
--
-- note: the test suite suppose that get_point works correctly.

tests.graphics = uunit.newSuite("graphics module")

tests.graphics.beforeEach = start_up
tests.graphics.afterEach = tear_down

tests.graphics:test("default color is white", function()
  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 255)
  assert_equal(g, 255)
  assert_equal(b, 255)
  assert_equal(a, 255)
end)

tests.graphics:test("can set color", function()
  smgf.graphics.set_color(255, 0, 140, 30)
  local r, g, b, a = smgf.graphics.get_color()

  assert_equal(r, 255)
  assert_equal(g, 0)
  assert_equal(b, 140)
  assert_equal(a, 30)
end)

tests.graphics:test("alpha is automatically set to 255 if omitted", function()
  smgf.graphics.set_color(0, 0, 0, 30)
  smgf.graphics.set_color(0, 0, 0)
  local r, g, b, a = smgf.graphics.get_color()

  assert_equal(a, 255)
end)

tests.graphics:test("set color requires an integer from 0 to 255", function()
  assert_raises(function()
    smgf.graphics.set_color(-1, 2, 3)
  end, "RGBA values must be between 0 and 255")
  assert_raises(function()
    --- @diagnostic disable-next-line: param-type-mismatch
    smgf.graphics.set_color("A", 2, 3)
  end, "bad argument #1 to 'set_color' (number expected, got string)")
  assert_raises(function()
    smgf.graphics.set_color(256, 2, 3)
  end, "RGBA values must be between 0 and 255")
end)

tests.graphics:test("set color accepts tables", function()
  smgf.graphics.set_color({1, 2, 3, 4})
  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 1)
  assert_equal(g, 2)
  assert_equal(b, 3)
  assert_equal(a, 4)
end)

tests.graphics:test("set color accepts tables with no alpha component",
  function()
    smgf.graphics.set_color({1, 2, 3})
    local r, g, b, a = smgf.graphics.get_color()
    assert_equal(r, 1)
    assert_equal(g, 2)
    assert_equal(b, 3)
    assert_equal(a, 255) -- defaults to 255
  end)

tests.graphics:test("set color with table and wrong nb of arguments", function()
  assert_raises(function()
    smgf.graphics.set_color({0})
  end, "invalid color (must have 3 or 4 components)")
  assert_raises(function()
    smgf.graphics.set_color({0, 1, 2, 3, 1})
  end, "invalid color (must have 3 or 4 components)")
end)

tests.graphics:test("set color with no arguments set color to opaque white",
  function()
    smgf.graphics.set_color(140, 140, 140, 140)
    smgf.graphics.set_color()

    local r, g, b, a = smgf.graphics.get_color()
    assert_equal(r, 255)
    assert_equal(g, 255)
    assert_equal(b, 255)
    assert_equal(a, 255)
  end)

tests.graphics:test("can clear screen", function()
  smgf.graphics.clear()

  -- note: this work in theory, but it is way too slow, hence the "return"
  for x = 0, smgf.system.get_width() - 1 do
    for y = 0, smgf.system.get_height() - 1 do
      local r, g, b = smgf.graphics.get_point(x, y)
      assert_equal(r, 0)
      assert_equal(g, 0)
      assert_equal(b, 0)
      return
    end
  end
end)

tests.graphics:test("can pass clear color as argument of clear", function()
  smgf.graphics.clear(255, 200, 10)

  -- note: this work in theory, but it is way too slow, hence the "return"
  for x = 0, smgf.system.get_width() - 1 do
    for y = 0, smgf.system.get_height() - 1 do
      local r, g, b = smgf.graphics.get_point(x, y)
      assert_equal(r, 255)
      assert_equal(g, 200)
      assert_equal(b, 10)
      return
    end
  end
end)

tests.graphics:test("can pass alpha color to clear", function()
  smgf.graphics.clear(0, 0, 0, 0)
end)

tests.graphics:test("calling clear does not change current color", function()
  smgf.graphics.set_color(1, 2, 3, 4)
  smgf.graphics.clear(255, 255, 255)
  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 1)
  assert_equal(g, 2)
  assert_equal(b, 3)
  assert_equal(a, 4)
end)

tests.graphics:test("can pass table as argument of clear", function()
  smgf.graphics.clear({255, 255, 255})

  assert_raises(function()
    smgf.graphics.clear({0})
  end, "invalid color (must have 3 or 4 components)")
  assert_raises(function()
    smgf.graphics.clear({0, 1, 2, 3, 1})
  end, "invalid color (must have 3 or 4 components)")
end)

tests.graphics:test("can get blend mode", function()
  assert_equal(type(smgf.graphics.get_blend_mode()), "string")
end)

tests.graphics:test("default blend mode is blend", function()
  assert_equal(smgf.graphics.get_blend_mode(), "blend")
end)

tests.graphics:test("can set blend mode", function()
  smgf.graphics.set_blend_mode("blend")
  assert_equal(smgf.graphics.get_blend_mode(), "blend")
  smgf.graphics.set_blend_mode("add")
  assert_equal(smgf.graphics.get_blend_mode(), "add")
  smgf.graphics.set_blend_mode("mod")
  assert_equal(smgf.graphics.get_blend_mode(), "mod")
  smgf.graphics.set_blend_mode("mul")
  assert_equal(smgf.graphics.get_blend_mode(), "mul")
  smgf.graphics.set_blend_mode("none")
  assert_equal(smgf.graphics.get_blend_mode(), "none")
end)

tests.graphics:test("invalid blend mode value", function()
  assert_raises(function()
    --- @diagnostic disable-next-line: param-type-mismatch
    smgf.graphics.set_blend_mode("a")
  end, "bad argument #1 to 'set_blend_mode' (invalid option 'a')")
end)

tests.graphics:test("set blend mode expects string", function()
  assert_raises(function()
    --- @diagnostic disable-next-line: param-type-mismatch
    smgf.graphics.set_blend_mode(1)
  end, "bad argument #1 to 'set_blend_mode' (invalid option '1')")
end)

tests.graphics:test("set_blend_mode with no parameters defaults to blend",
  function()
    smgf.graphics.set_blend_mode("none")
    smgf.graphics.set_blend_mode()
    assert_equal(smgf.graphics.get_blend_mode(), "blend")
  end)

tests.graphics:test("default translation is 0 0", function()
  local tx, ty = smgf.graphics.get_translation()
  assert_equal(tx, 0)
  assert_equal(ty, 0)
end)

tests.graphics:test("can set translation using set_translation()", function()
  smgf.graphics.set_translation(3, 4)
  local tx, ty = smgf.graphics.get_translation()
  assert_equal(tx, 3)
  assert_equal(ty, 4)

  -- check that set_translation() does *not* add to current translation:
  smgf.graphics.set_translation(10, 10)
  tx, ty = smgf.graphics.get_translation()
  assert_equal(tx, 10)
  assert_equal(ty, 10)

  smgf.graphics.set_translation(0, 0)
  tx, ty = smgf.graphics.get_translation()
  assert_equal(tx, 0)
  assert_equal(ty, 0)
end)

tests.graphics:test("can set translation using translate()", function()
  smgf.graphics.translate(1, 2)
  local tx, ty = smgf.graphics.get_translation()
  assert_equal(tx, 1)
  assert_equal(ty, 2)

  -- check that translate() does add to current translation:
  smgf.graphics.translate(10, 20)
  tx, ty = smgf.graphics.get_translation()
  assert_equal(tx, 11)
  assert_equal(ty, 22)
end)

tests.graphics:test("can get point", function()
  local r, g, b = smgf.graphics.get_point(0, 0)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 0)
end)

tests.graphics:test("get point outside texture returns 0", function()
  local r, g, b = smgf.graphics.get_point(-10, -30)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 0)
end)

tests.graphics:test("can draw point", function()
  smgf.graphics.set_color(230, 0, 231)
  smgf.graphics.draw_point(1, 1)
  local r, g, b = smgf.graphics.get_point(1, 1)
  assert_equal(r, 230)
  assert_equal(g, 0)
  assert_equal(b, 231)
end)

tests.graphics:test("can draw line", function()
  smgf.graphics.set_color(10, 20, 30)
  smgf.graphics.draw_line(0, 0, 20, 0)

  local r, g, b = 0, 0, 0
  for i = 0, 20 - 1 do
    r, g, b = smgf.graphics.get_point(i, 0)
    assert_equal(r, 10)
    assert_equal(g, 20)
    assert_equal(b, 30)
    r, g, b = 0, 0, 0
  end
end)

tests.graphics:test("can draw rect", function()
  smgf.graphics.set_color(10, 20, 30)
  smgf.graphics.draw_rect(0, 0, 20, 20)

  local r, g, b = smgf.graphics.get_point(0, 0)
  assert_equal(r, 10)
  assert_equal(g, 20)
  assert_equal(b, 30)

  -- rect is not filled:
  r, g, b = smgf.graphics.get_point(1, 1)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 0)
end)

tests.graphics:test("can draw rectfill", function()
  smgf.graphics.set_color(10, 20, 30)
  smgf.graphics.draw_rectfill(0, 0, 20, 20)

  local r, g, b = smgf.graphics.get_point(0, 0)
  assert_equal(r, 10)
  assert_equal(g, 20)
  assert_equal(b, 30)

  -- rect is filled:
  r, g, b = smgf.graphics.get_point(1, 1)
  assert_equal(r, 10)
  assert_equal(g, 20)
  assert_equal(b, 30)
end)

tests.graphics:test("can create empty texture", function()
  local t = smgf.graphics.new(20, 30)
  assert_not_nil(t)
  assert_equal(type(t), "userdata")
  assert_str_in("smgf.texture", tostring(t))
end)

tests.graphics:test("check dimensions are set on new texture", function()
  local t = smgf.graphics.new(20, 30)

  assert_equal(t:get_width(), 20)
  assert_equal(t:get_height(), 30)

  local w, h = t:get_dimensions()
  assert_equal(w, 20)
  assert_equal(h, 30)
end)

tests.graphics:test("error message on new with bad arguments", function()
  assert_raises(function()
    smgf.graphics.new(-1, 0)
  end, "unable to create texture (Texture dimensions can't be 0)")
end)

tests.graphics:test("can load texture from png file", function()
  local t = smgf.graphics.new("test.png")
  assert_not_nil(t)
  assert_equal(type(t), "userdata")
  assert_str_in("smgf.texture", tostring(t))
end)

tests.graphics:test("supported formats", function()
  -- only png are supported
  local img1 = smgf.graphics.new("test.png")

  -- bmp (for example) should not be supported and raise an error
  assert_raises(function()
      smgf.graphics.new("test.bmp")
    end,
    "unable to open file test.bmp (Unsupported image format)")
end)

tests.graphics:test("check dimensions are set on new texture", function()
  local t = smgf.graphics.new("test.png")
  assert_equal(t:get_width(), 384)
  assert_equal(t:get_height(), 384)

  local w, h = t:get_dimensions()
  assert_equal(w, 384)
  assert_equal(h, 384)
end)

tests.graphics:test("new texture raises if file does not exist", function()
  assert_raises(function()
    local t = smgf.graphics.new("inexistent_file.png")
  end, "unable to open file inexistent_file.png (PhysicsFS error: not found)")
end)

tests.graphics:test("can draw whole texture", function()
  local t = smgf.graphics.new("test.png")
  smgf.graphics.draw(t)

  local r, g, b = smgf.graphics.get_point(0, 0)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 2)

  r, g, b = smgf.graphics.get_point(24, 0)
  assert_equal(r, 29)
  assert_equal(g, 43)
  assert_equal(b, 83)
end)

tests.graphics:test("can draw by calling method on texture directly", function()
  local t = smgf.graphics.new("test.png")
  t:draw()
end)

tests.graphics:test("can specify position to draw", function()
  local t = smgf.graphics.new("test.png")
  smgf.graphics.draw(t, 10, 0)

  local r, g, b = smgf.graphics.get_point(10, 0)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 2)

  r, g, b = smgf.graphics.get_point(10 + 24, 0)
  assert_equal(r, 29)
  assert_equal(g, 43)
  assert_equal(b, 83)

  -- nothing is drawn at 0,0:
  r, g, b = smgf.graphics.get_point(0, 0)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 0)
end)

tests.graphics:test("can send floats as position", function()
  local t = smgf.graphics.new("test.png")
  smgf.graphics.draw(t, 10.0, 3.3)
end)

tests.graphics:test("can draw part of texture", function()
  local t = smgf.graphics.new("test.png")
  -- only draw orange square:
  smgf.graphics.draw(t, {24, 24 * 2, 24, 24}, 0, 0)

  local r, g, b = smgf.graphics.get_point(0, 0)
  assert_equal(r, 255)
  assert_equal(g, 163)
  assert_equal(b, 0)

  r, g, b = smgf.graphics.get_point(24, 24)
  assert_equal(r, 0)
  assert_equal(g, 0)
  assert_equal(b, 0)
end)

tests.graphics:test("default target is nil (= screen)", function()
  assert_nil(smgf.graphics.get_target())
end)

tests.graphics:test("can set target", function()
  local t = smgf.graphics.new(20, 30)
  smgf.graphics.set_target(t)
  assert_equal(smgf.graphics.get_target(), t)
end)

tests.graphics:test("can reset target", function()
  smgf.graphics.set_target(nil)
  assert_nil(smgf.graphics.get_target())
  smgf.graphics.set_target()
  assert_nil(smgf.graphics.get_target())
end)

tests.graphics:test("cannot set target from a loaded texture", function()
  local t = smgf.graphics.new("test.png")

  -- @TODO: cleaner error message than this:
  assert_raises(function()
    smgf.graphics.set_target(t)
  end, "Texture not created with SDL_TEXTUREACCESS_TARGET")
end)

-- methods on smgf.texture:
tests.graphics:test("texture get_width/height", function()
  local t = smgf.graphics.new(20, 30)
  assert_equal(t:get_width(), 20)
  assert_equal(t:get_height(), 30)
end)

tests.graphics:test("texture get dimensions", function()
  local t = smgf.graphics.new(20, 30)
  local w, h = t:get_dimensions()
  assert_equal(w, 20)
  assert_equal(h, 30)
end)

tests.graphics:test("texture get_blend_mode", function()
  local t = smgf.graphics.new(20, 30)
  assert_equal(type(t:get_blend_mode()), "string")
end)

tests.graphics:test("texture default blend_mode", function()
  local t = smgf.graphics.new(20, 30)
  assert_equal(t:get_blend_mode(), "blend")
end)

tests.graphics:test("texture set_blend_mode", function()
  local t = smgf.graphics.new(20, 30)
  t:set_blend_mode("blend")
  assert_equal(t:get_blend_mode(), "blend")
  t:set_blend_mode("add")
  assert_equal(t:get_blend_mode(), "add")
  t:set_blend_mode("mod")
  assert_equal(t:get_blend_mode(), "mod")
  t:set_blend_mode("mul")
  assert_equal(t:get_blend_mode(), "mul")
  t:set_blend_mode("none")
  assert_equal(t:get_blend_mode(), "none")
end)

tests.graphics:test("texture set_blend_mode expects string", function()
  local t = smgf.graphics.new(20, 30)
  assert_raises(function()
    --- @diagnostic disable-next-line: param-type-mismatch
    t:set_blend_mode(1)
  end, "bad argument #1 to 'set_blend_mode' (invalid option '1')")
end)

tests.graphics:test("texture set blend mode with no parameters", function()
  local t = smgf.graphics.new(20, 30)
  t:set_blend_mode("none")
  t:set_blend_mode()
  assert_equal(t:get_blend_mode(), "blend")
end)

tests.graphics:test("can push graphics state", function()
  local tex = smgf.graphics.new(10, 10)
  smgf.graphics.set_color(1, 2, 3, 4)
  smgf.graphics.translate(5, 6)
  smgf.graphics.set_target(tex)

  smgf.graphics.push_state()
  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 255)
  assert_equal(g, 255)
  assert_equal(b, 255)
  assert_equal(a, 255)
  local x, y = smgf.graphics.get_translation()
  assert_equal(x, 0)
  assert_equal(y, 0)
  assert_nil(smgf.graphics.get_target())

  local tex2 = smgf.graphics.new(10, 10)
  smgf.graphics.set_target(tex2)

  smgf.graphics.pop_state()

  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 1)
  assert_equal(g, 2)
  assert_equal(b, 3)
  assert_equal(a, 4)
  local x, y = smgf.graphics.get_translation()
  assert_equal(x, 5)
  assert_equal(y, 6)
  assert_equal(smgf.graphics.get_target(), tex)
end)

tests.graphics:test("can only push 64 graphic states", function()
  local MAX_GSTATES = 64
  for i = 1, MAX_GSTATES - 1 do
    smgf.graphics.push_state()
  end
  assert_raises(function()
    smgf.graphics.push_state()
  end, "error: cannot store more than 64 graphic states")
end)

tests.graphics:test("multiple pop_state when no state has been pushed",
  function()
    smgf.graphics.pop_state()
    smgf.graphics.pop_state()
    smgf.graphics.pop_state()
  end)

tests.graphics:test("can reset the state", function()
  smgf.graphics.set_color(1, 2, 3, 4)

  smgf.graphics.reset_state()

  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 255)
  assert_equal(g, 255)
  assert_equal(b, 255)
  assert_equal(a, 255)
end)

tests.graphics:test("reset state values", function()
  smgf.graphics.reset_state()

  local r, g, b, a = smgf.graphics.get_color()
  assert_equal(r, 255)
  assert_equal(g, 255)
  assert_equal(b, 255)
  assert_equal(a, 255)

  local x, y = smgf.graphics.get_translation()
  assert_equal(x, 0)
  assert_equal(y, 0)

  assert_equal(smgf.graphics.get_target(), nil)
end)

tests.graphics:test("can save texture as png", function()
  smgf.system.set_identity("smgf", "smgftestgame")
  local t = smgf.graphics.new(20, 30)
  t:save("screenshot.png")
end)

-- hard to test, at least it tests that API function can be called without error
tests.graphics:test("can call print & print_color", function()
  smgf.graphics.print(0, 0, "Hello", {0x0f, 0x0f, 0xf0})
  smgf.graphics.print_color(0, 0, 0xf0, "Hello")
end)

--
-- SYSTEM
--

tests.system = uunit.newSuite("system module")
tests.system.afterEach = function()
  smgf.system.set_identity()
  smgf.system.set_cursor_visible(true)
  smgf.system.set_window_title("SMGF v2.0.0")
  smgf.system.set_dimensions(256, 256)
  smgf.system.set_zoom(1)
  smgf.system.set_fps(nil)
end

tests.system:test("can get window width+height", function()
  assert_equal(smgf.system.get_width(), 256)
  assert_equal(smgf.system.get_height(), 256)

  local w, h = smgf.system.get_dimensions()
  assert_equal(w, 256)
  assert_equal(h, 256)
end)

tests.system:test("can set window width+height", function()
  smgf.system.set_dimensions(300, 310)
  assert_equal(smgf.system.get_width(), 300)
  assert_equal(smgf.system.get_height(), 310)
end)

tests.system:test("can get fps limit (defaults to nil)", function()
  assert_equal(smgf.system.get_fps(), nil)
end)

tests.system:test("can set fps limit", function()
  smgf.system.set_fps(60)
  assert_equal(smgf.system.get_fps(), 60)
  smgf.system.set_fps()
  assert_equal(smgf.system.get_fps(), nil)
end)

tests.system:test("can get zoom (defaults to 1)", function()
  assert_equal(smgf.system.get_zoom(), 1)
end)

tests.system:test("can set zoom", function()
  smgf.system.set_zoom(2)
  assert_equal(smgf.system.get_zoom(), 2)
  smgf.system.set_zoom()
  assert_equal(smgf.system.get_zoom(), 1)
end)

tests.system:test("zooming does not change the width/height", function()
  smgf.system.set_zoom()
  assert_equal(smgf.system.get_width(), 256)
  assert_equal(smgf.system.get_height(), 256)
  smgf.system.set_zoom(2)
  assert_equal(smgf.system.get_width(), 256)
  assert_equal(smgf.system.get_height(), 256)
end)

tests.system:test("quit function exists", function()
  assert_type(smgf.system.quit, "function")
end)

tests.system:test("get_platform returns a string", function()
  assert_type(smgf.system.get_platform(), "string")
end)

tests.system:test("sleep", function()
  assert_equal(type(smgf.system.sleep), "function")
  smgf.system.sleep(0.001)
end)

tests.system:test("can get delta time", function()
  assert_type(smgf.system.get_dt(), "number")
end)

tests.system:test("can get and set fullscreen mode", function()
  assert_equal(smgf.system.get_fullscreen(), false)
end)

tests.system:skip("set_fullscreen", function()
  smgf.system.set_fullscreen(true)
  assert_equal(smgf.system.get_fullscreen(), true)
  smgf.system.set_fullscreen(false)
end)

tests.system:skip("can log", function()
  smgf.system.log("hello")
end)

tests.system:test("rand", function()
  math.randomseed(42)
  assert_almost_eq(math.random(), 0.93081217803957)
  assert_almost_eq(math.random(), 0.45178389935924)
  assert_almost_eq(math.random(), 0.54688311243421)
  assert_almost_eq(math.random(), 0.79358935263827)
  assert_almost_eq(math.random(), 0.61731763595847)
end)

tests.system:test("can get user preferred languages", function()
  assert_type(smgf.system.get_preferred_locales(), "table")
end)

tests.system:test("can get and set clipboard contents", function()
  local text = "some text to copy to clipboard"
  smgf.system.set_clipboard(text)
  assert_equal(smgf.system.get_clipboard(), text)
end)

tests.system:test("can get and set cursor visibility", function()
  assert_equal(smgf.system.get_cursor_visible(), true)
  smgf.system.set_cursor_visible(false)
  assert_equal(smgf.system.get_cursor_visible(), false)
end)

tests.system:test("can get game identity", function()
  local org, app = smgf.system.get_identity()
  assert_equal(org, "")
  assert_equal(app, "")
end)

tests.system:test("can set game identity", function()
  smgf.system.set_identity("smgf", "smgftestgame")
  local org, app = smgf.system.get_identity()
  assert_equal(org, "smgf")
  assert_equal(app, "smgftestgame")
end)

tests.system:test("can set game identity (bug 1)", function()
  -- app/org strings must be copied internally by smgf
  local testFunc = function()
    smgf.system.set_identity("smgf", "smgftestgame")
  end
  testFunc()
  collectgarbage()

  local org, app = smgf.system.get_identity()
  assert_equal(org, "smgf")
  assert_equal(app, "smgftestgame")
end)

tests.system:test("can use iconv", function()
  assert_equal(smgf.system.iconv("ISO-8859-1", "UTF-8", "Ébène sud"),
    "\xC9\x62\xE8\x6E\x65 sud")

  -- cannot convert to/from unknown encodings
  assert_raises(function()
    smgf.system.iconv("NOT EXISTING", "UTF-8", "Ébène")
  end, "unable to convert string")
  assert_raises(function()
    smgf.system.iconv("UTF-8", "NOT EXISTING", "Ébène")
  end, "unable to convert string")
end)

tests.system:test("can set window title", function()
  assert_equal(smgf.system.get_window_title(), "SMGF v2.0.0")
  smgf.system.set_window_title("new window title")
  assert_equal(smgf.system.get_window_title(), "new window title")
end)

tests.system:test("can get smgf version", function()
  local v = smgf.system.get_version()
  assert_equal(type(v), "string")
  -- assert_equal(v, "0.1.0")
end)

--
-- AUDIO
--

tests.audio = uunit.newSuite("audio module")

tests.audio.beforeEach = function()
  smgf.audio.set_master_gain(1)
end

tests.audio.afterEach = function()
  smgf.audio.set_master_gain(0)
end

tests.audio:test("can get master gain", function()
  assert_equal(smgf.audio.get_master_gain(), 1)
end)

tests.audio:todo("default gain is 1", function()
  assert_equal(smgf.audio.get_master_gain(), 1)
end)

tests.audio:test("can set master gain", function()
  smgf.audio.set_master_gain(.5)
  assert_equal(smgf.audio.get_master_gain(), .5)
end)

tests.audio:test("can set master pause", function()
  smgf.audio.set_master_pause(true)
  smgf.audio.set_master_pause(false)
end)

tests.audio:test("master gain must be between 0 and 2", function()
  assert_raises(function()
    smgf.audio.set_master_gain(-10)
  end, "(must be between 0 and 2)")
  assert_raises(function()
    smgf.audio.set_master_gain(2.1)
  end, "(must be between 0 and 2)")
end)

tests.audio:test("can create new sound", function()
  local s = smgf.audio.new("0.wav")
  assert_equal(type(s), "userdata")
  assert_not_nil(tostring(s):find("smgf.sound"))
end)

tests.audio:test("new_sound predecodes whole file by default", function()
  local s = smgf.audio.new("0.wav")
  assert_equal(smgf.audio.is_predecoded(s), true)
  assert_equal(s:is_predecoded(), true)
end)

tests.audio:test("can create streamed sound", function()
  local s = smgf.audio.new("0.wav", false)
  assert_equal(smgf.audio.is_predecoded(s), false)
  assert_equal(s:is_predecoded(), false)
end)

tests.audio:test("new sound raises if file does not exist", function()
  assert_raises(function()
      local t = smgf.audio.new("inexistent_file.wav")
    end,
    "unable to open sound file 'inexistent_file.wav' (PhysicsFS error: not found)")
end)

tests.audio:test("sound get_gain", function()
  local s = smgf.audio.new("0.wav")
  assert_equal(s:get_gain(), 1) -- default is 1
end)

tests.audio:test("sound set_gain", function()
  local s = smgf.audio.new("0.wav")
  s:set_gain(2)
  assert_equal(s:get_gain(), 2)
end)

tests.audio:test("sound gain must be between 0 and 2", function()
  local s = smgf.audio.new("0.wav")
  assert_raises(function()
    s:set_gain(-10)
  end, "(must be between 0 and 2)")
  assert_raises(function()
    s:set_gain(130)
  end, "(must be between 0 and 2)")
end)

tests.audio:test("sound get_loop is false when not playing", function()
  local s = smgf.audio.new("0.wav")
  assert_equal(s:get_loop(), false)
end)

tests.audio:test("sound loop", function()
  local s = smgf.audio.new("0.wav")
  smgf.audio.play(s, true)
  assert_equal(s:get_loop(), true)
  assert_equal(smgf.audio.get_loop(s), true)

  -- even paused, still counts as looped
  smgf.audio.pause(s)
  assert_equal(s:get_loop(), true)

  smgf.audio.stop(s)
  assert_equal(s:get_loop(), false)
end)

tests.audio:test("supported formats", function()
  -- ogg and wav should be supported
  smgf.audio.new("0.wav")
  smgf.audio.new("0.ogg")

  -- mp3 should not be supported
  assert_raises(function()
      smgf.audio.new("0.mp3")
    end,
    "unable to open sound file '0.mp3' (Audio data is in unknown/unsupported/corrupt format)")
end)

tests.audio:test("can play sound", function()
  smgf.audio.set_master_gain(0)

  local s = smgf.audio.new("0.wav")
  smgf.audio.play(s)
  s:play()
end)

tests.audio:test("sound is_playing", function()
  smgf.audio.set_master_gain(0)

  local s = smgf.audio.new("0.ogg")
  assert_false(s:is_playing())
  assert_false(smgf.audio.is_playing(s))
  s:play()
  assert_true(s:is_playing())
  assert_true(smgf.audio.is_playing(s))
end)

tests.audio:test("can pause sound", function()
  local s = smgf.audio.new("0.ogg")
  smgf.audio.pause(s)
  s:pause()
end)

tests.audio:test("sound is_playing", function()
  smgf.audio.set_master_gain(0)

  local s = smgf.audio.new("0.ogg")
  assert_false(s:is_playing())
  assert_false(smgf.audio.is_playing(s))
  s:play()
  assert_true(s:is_playing())
  assert_true(smgf.audio.is_playing(s))
  s:pause()
  assert_false(s:is_playing())
  assert_false(smgf.audio.is_playing(s))
end)

tests.audio:test("can clone sound", function()
  smgf.audio.set_master_gain(0)

  local s = smgf.audio.new("0.ogg")
  local s2 = smgf.audio.clone(s)
  local s3 = s:clone()

  -- we free the first sound to check that cloned sources are completely
  -- independent of the original
  --- @diagnostic disable-next-line: cast-local-type
  s = nil
  collectgarbage()

  smgf.audio.play(s2)
end)

tests.audio:test("sound panning", function()
  local s = smgf.audio.new("0.wav")
  assert_equal(s:get_pan(), 0) -- defaults to 0

  s:set_pan(-1)
  assert_equal(s:get_pan(), -1)
  s:set_pan(1)
  assert_equal(s:get_pan(), 1)
end)


--
-- INPUT
--

tests.input = uunit.newSuite("input module")

tests.input.afterEach = function()
  smgf.keyboard.set_textinput(false)
end

tests.input:test("get_textinput is false by default", function()
  assert_equal(smgf.keyboard.get_textinput(), false)
end)

tests.input:test("can set textinput", function()
  smgf.keyboard.set_textinput(true)
  assert_equal(smgf.keyboard.get_textinput(), true)
  smgf.keyboard.set_textinput(false)
  assert_equal(smgf.keyboard.get_textinput(), false)
end)

tests.input:test("can call is_down", function()
  assert_equal(smgf.keyboard.is_down("up"), false)
end)

tests.input:test("can call is_up", function()
  assert_equal(smgf.keyboard.is_up("up"), true)
end)

tests.input:test("is_down/up on invalid key name", function()
  assert_raises(function()
    smgf.keyboard.is_down("invalid_key")
  end, "key 'invalid_key' does not exist")
  assert_raises(function()
    smgf.keyboard.is_up("invalid_key")
  end, "key 'invalid_key' does not exist")
end)

tests.input:test("mouse get_x/y", function()
  assert_not_equal(smgf.mouse.get_x(), nil)
  assert_not_equal(smgf.mouse.get_y(), nil)
  assert_equal(type(smgf.mouse.get_x()), "number")
  assert_equal(type(smgf.mouse.get_y()), "number")

  local x, y = smgf.mouse.get_pos()
  assert_equal(type(x), "number")
  assert_equal(type(y), "number")
end)

tests.input:test("mouse is_down", function()
  assert_equal(smgf.mouse.is_down(1), false)
  assert_equal(smgf.mouse.is_down(2), false)
  assert_equal(smgf.mouse.is_down(3), false)
end)

--
-- FILE IO
--

tests.io = uunit.newSuite("file i/o module")

tests.io.afterEach = function()
  collectgarbage()
  smgf.system.set_identity()
end

local function create_test_file()
  smgf.system.set_identity("smgf", "smgftestgame")

  local file = smgf.io.open("file.txt", "w")
  file:write("hello world\nthis is smgf")
  file:close()
end

tests.io:test("write directory is not mounted by default", function()
  assert_equal(smgf.system.get_write_dir(), nil)
end)

-- @TODO: test that conf auto-mounts writedir?
tests.io:test("writedir can be mounted by calling set identity", function()
  smgf.system.set_identity("smgf", "smgftestgame")

  local org, app = smgf.system.get_identity()
  assert_equal(org, "smgf")
  assert_equal(app, "smgftestgame")

  assert_not_equal(smgf.system.get_write_dir(), "")
  local write_dir = smgf.system.get_write_dir()
  if write_dir == nil then
    error(string.format("`%s` should not be nil", write_dir))
  end
  assert_str_in("smgftestgame", write_dir)
end)

tests.io:test("writedir can be unmounted by calling set identity with no arg",
  function()
    smgf.system.set_identity("smgf", "smgftestgame")
    local write_dir = smgf.system.get_write_dir()
    if write_dir == nil then
      error(string.format("`%s` should not be nil", write_dir))
    end
    assert_str_in("smgftestgame", write_dir)

    smgf.system.set_identity()
    assert_equal(smgf.system.get_write_dir(), nil)
  end)

tests.io:test("cannot open non-existent file", function()
  assert_raises(function()
    local file = smgf.io.open("nonexistentfile.txt", "r")
  end, "error: not found")
end)

tests.io:test("cannot open file in write mode if writedir hasn't been set",
  function()
    assert_raises(function()
      local file = smgf.io.open("file.txt", "w")
    end, "write directory is not set")
  end)

tests.io:test("can write and read to writedir", function()
  smgf.system.set_identity("smgf", "smgftestgame")

  local file = smgf.io.open("file.txt", "w")
  file:write("hello world")
  file:close()

  local file = smgf.io.open("file.txt", "r")
  local file_contents = file:read("all")
  assert_equal(file_contents, "hello world")
  file:close()
end)

tests.io:test("can only open using a/w/r modes", function()
  smgf.system.set_identity("smgf", "smgftestgame")

  local file1 = smgf.io.open("file.txt", "w")
  local file2 = smgf.io.open("file.txt", "a")
  local file3 = smgf.io.open("file.txt", "r")

  assert_raises(function()
    --- @diagnostic disable-next-line: param-type-mismatch
    smgf.io.open("file.txt", "c")
  end, "unknown open mode 'c'")
end)

tests.io:test("can get size of file", function()
  create_test_file()

  local file = smgf.io.open("file.txt", "r")
  assert_equal(file:size(), 24)
end)

tests.io:test("can call tell", function()
  create_test_file()

  local file = smgf.io.open("file.txt", "r")
  assert_equal(file:tell(), 0)
end)

tests.io:test("can seek in file", function()
  create_test_file()

  local file = smgf.io.open("file.txt", "r")
  assert_equal(file:tell(), 0)
  file:seek(5)
  assert_equal(file:tell(), 5)
end)

tests.io:test("can rewind file", function()
  create_test_file()

  local file = smgf.io.open("file.txt", "r")
  file:seek(5)
  assert_equal(file:tell(), 5)
  file:rewind()
  assert_equal(file:tell(), 0)
end)

tests.io:test("cannot read outside of file", function()
  create_test_file()

  local file = smgf.io.open("file.txt", "r")

  file:seek(file:size() + 10)
  assert_raises(function()
    file:read(1)
  end, "unable to read outside of file")
end)

tests.io:test("file read with no argument raises an error", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "r")

  assert_raises(function()
    --- @diagnostic disable-next-line: missing-parameter
    file:read()
  end, "missing argument ('all', 'line', or a number)")
end)

tests.io:test("can read a arbitrary number of characters", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "r")

  assert_equal(file:read(1), "h")
  assert_equal(file:read(2), "el")
  assert_equal(file:read(8), "lo world")
end)

tests.io:test("can read a whole file", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "r")

  assert_equal(file:read("all"), "hello world\nthis is smgf")
end)

tests.io:test("can read a file line by line", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "r")

  assert_equal(file:read("line"), "hello world\n")
  assert_equal(file:read("line"), "this is smgf")
end)

tests.io:test("test file opened in read mode cannot be written to", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "r")

  assert_raises(function()
    file:write("test")
  end, "unable to write to file: PhysicsFS error: file open for reading")
end)

tests.io:test("test file opened in write mode cannot be read from", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "w")

  assert_raises(function()
    file:read(1)
  end, "unable to read from file: PhysicsFS error: file open for writing")
end)

tests.io:test("test can check if file exists", function()
  create_test_file()
  assert_equal(smgf.io.exists("file.txt"), true)
  assert_equal(smgf.io.exists("nonexistentfile.txt"), false)
end)

tests.io:test("test type check on non-existent file raises error", function()
  assert_raises(function()
    smgf.io.type("nonexistentfile.txt")
  end, "unable to get type of file 'nonexistentfile.txt': file does not exist")
end)

tests.io:test("test can call mkdir", function()
  smgf.system.set_identity("smgf", "smgftestgame")
  smgf.io.mkdir("test-directory")
  assert_equal(smgf.io.exists("test-directory"), true)
end)

tests.io:test("test can check file type", function()
  create_test_file()
  assert_equal(smgf.io.type("file.txt"), "file")

  smgf.io.mkdir("test-directory")
  assert_equal(smgf.io.type("test-directory"), "directory")

  assert_raises(function()
    smgf.io.type("nonexistent.file")
  end, "unable to get type of file")
end)

tests.io:test("test can delete file", function()
  create_test_file()
  assert_equal(smgf.io.exists("file.txt"), true)
  smgf.io.delete("file.txt")
  assert_equal(smgf.io.exists("file.txt"), false)
end)

tests.io:test("test deleting non existing file raises error", function()
  assert_raises(function()
    smgf.io.delete("nonexistentfile.txt")
  end, "unable to delete file")
end)

tests.io:test("test can call flush on files", function()
  create_test_file()
  local file = smgf.io.open("file.txt", "w")
  file:flush()
  file:close()

  -- should be a safe no-op for file reads:
  local file2 = smgf.io.open("file.txt", "r")
  file2:flush()
  file2:close()
end)

return tests
