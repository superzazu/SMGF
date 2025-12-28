---@type smgf.init
function smgf.init()
  local music = smgf.audio.new("music.ogg", false)
  music:play(true)
end
