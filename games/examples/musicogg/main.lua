---@type smgf.init
function smgf.init()
  local music = smgf.audio.new("music.ogg", false)
  music:set_loop(true)
  music:play()
end
