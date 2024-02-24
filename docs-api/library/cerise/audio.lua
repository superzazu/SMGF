--- @meta
--- @class smgf.audio
smgf.audio = {}

--- A sound that can be played.
--- @class SMGFSound
local Sound = {}

--- Starts playing a sound.
function Sound:play() end

--- Pauses the playback of a sound.
function Sound:pause() end

--- Returns the duration of a sound in milliseconds.
--- @return number Duration in milliseconds
function Sound:get_duration() end

--- Rewinds a sound.
function Sound:rewind() end

--- Modifies the playback position of a sound.
--- @param ms number The position in milliseconds
function Sound:seek(ms) end

--- Returns whether a sound is playing or not.
--- @return boolean playing
function Sound:is_playing() end

--- Returns whether a sound is predecoded or not.
--- @return boolean predecoded
function Sound:is_predecoded() end

--- Modifies the panning of a sound. Expects a number from "-1" (left) to "1"
--- (right). Default value is "0" (center).
--- @param pan number
function Sound:set_pan(pan) end

--- Returns the panning of a sound, which is a number from "-1" (left) to "1"
--- (right).
--- @return number pan
function Sound:get_pan() end

--- Modifies the gain ("volume") of a sound. Expects a number from "0" (silence)
--- to "2" (200%). Default value is "0" (100%).
--- @param gain number
function Sound:set_gain(gain) end

--- Returns the gain ("volume") of a sound, which is a number from "0" (silence)
--- to "2" (200%).
--- @return number gain
function Sound:get_gain() end

--- Sets whether a sound should be looped or not.
--- @param looped boolean
function Sound:set_loop(looped) end

--- Returns whether a sound is looped or not.
--- @return boolean looped
function Sound:get_loop() end

--- Creates a copy of a `SMGFSound`.
--- @return SMGFSound clone
function Sound:clone() end

-- @MARK: audio module

--- Sets the output gain ("volume"). Expects a value between "0" (silence) and
--- "2" (200%). Default value is "1" (100%).
--- @param gain number Gain (value between "0" (silence) and "2" (200%))
function smgf.audio.set_master_gain(gain) end

--- Returns the output gain ("volume"), which is a value between "0" (silence)
--- and "2" (200%).
--- @return number gain Gain (value between "0" (silence) and "2" (200%))
function smgf.audio.get_master_gain() end

--- Sets the master pause: when enabled, all sound will pause until re-enabled.
--- @param paused boolean
function smgf.audio.set_master_pause(paused) end

--- Returns the master pause.
--- @return boolean paused
function smgf.audio.get_master_pause() end

--- Loads a sound file and returns a `SMGFSound`. The second argument
--- controls whether the sound file should be predecoded in memory at load,
--- or decoded during playback ("streamed").
--- It is advised to predecode short sound files such as sound effects, and
--- stream longer sound files such as music.
--- SMGF only supports ogg vorbis and wav files by default (can be configured at build)
--- @param filename string File path
--- @param predecoded? boolean Whether the sound file should be predecoded in memory at load or decoded during playback ("streamed"), defaults to true
--- @return SMGFSound
function smgf.audio.new(filename, predecoded) end

--- Starts playing a sound.
--- @param sound SMGFSound Sound to play
function smgf.audio.play(sound) end

--- Pauses the playback of a sound.
--- @see smgf.audio.play to unpause the sound
--- @param sound SMGFSound Sound to pause
function smgf.audio.pause(sound) end

--- Returns the duration of a sound in milliseconds.
--- @param sound SMGFSound Sound to pause
--- @return number Duration in milliseconds
function smgf.audio.get_duration(sound) end

--- Rewinds a sound.
--- @param sound SMGFSound Sound to rewind
function smgf.audio.rewind(sound) end

--- Modifies the playback position of a sound.
--- @param sound SMGFSound
--- @param ms number The position in milliseconds
function smgf.audio.seek(sound, ms) end

--- Returns whether a sound is playing or not.
--- @param sound SMGFSound
--- @return boolean playing
function smgf.audio.is_playing(sound) end

--- Returns whether a sound is predecoded or not.
--- @param sound SMGFSound
--- @return boolean predecoded
function smgf.audio.is_predecoded(sound) end

--- Modifies the panning of a sound. Expects a number from "-1" (left) to "1"
--- (right). Default value is "0" (center).
--- @param sound SMGFSound
--- @param pan number
function smgf.audio.set_pan(sound, pan) end

--- Returns the panning of a sound, which is a number from "-1" (left) to "1"
--- (right).
--- @param sound SMGFSound
--- @return number pan
function smgf.audio.get_pan(sound) end

--- Modifies the gain ("volume") of a sound. Expects a number from "0" (silence)
--- to "2" (200%). Default value is "0" (100%).
--- @param sound SMGFSound
--- @param gain number
function smgf.audio.set_gain(sound, gain) end

--- Returns the gain ("volume") of a sound, which is a number from "0" (silence)
--- to "2" (200%).
--- @param sound SMGFSound
--- @return number gain
function smgf.audio.get_gain(sound) end

--- Sets whether a sound should be looped or not.
--- @param sound SMGFSound
--- @param looped boolean
function smgf.audio.set_loop(sound, looped) end

--- Returns whether a sound is looped or not.
--- @param sound SMGFSound
--- @return boolean looped
function smgf.audio.get_loop(sound) end

--- Creates a copy of a `SMGFSound`.
--- @param sound SMGFSound
--- @return SMGFSound clone
function smgf.audio.clone(sound) end
