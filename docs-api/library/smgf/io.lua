--- @meta
--- @class smgf.io
smgf.io = {}

--- A file that can be read and/or written to.
--- @class SMGFFile
local File = {}

--- Closes a file. Beware that the file object will not be usable after calling this function.
function File:close() end

--- Returns file size in bytes.
--- @return number size Size in bytes
function File:size() end

--- Sets the file cursor position.
--- @param offset number The offset in bytes
--- @param mode? "set" | "cur" | "end" `cur` = seek relative to current read point, `set` = seek from the beginning of data, `end` = seek relative to the end of data, defaults to `cur`
--- @return number final_offset The offset after operation
function File:seek(offset, mode) end

--- Sets the file cursor position to the beginning of file.
function File:rewind() end

--- Returns current cursor position.
--- @return number offset
function File:tell() end

--- Reads from a file. Pass "all" to read all file at once, "line" to read
--- the current line, or a number to read an arbitrary number of bytes from file.
--- Make sure the file was opened with "r" flag or it will trigger an error.
--- @overload fun(file: SMGFFile, nb_bytes: number)
--- @param mode "all" | "line" Whether to read the current line in file, or the whole file.
--- @return string
function File:read(mode) end

--- Writes a string to a file.
--- Make sure the file was opened with "w" flag or it will trigger an error.
--- @param data string The data to write to file
function File:write(data) end

--- Flushs a buffered file.
--- @return number offset
function File:flush() end

-- @MARK: io module

--- Opens a file for use.
--- @param filename string The filename to open
--- @param mode "r" | "w" | "a" Whether to open the file in read (r), write (w) mode, or append (a) mode.
--- @return SMGFFile
function smgf.io.open(filename, mode) end

--- Creates a directory.
--- @param dirname string The directory to create
function smgf.io.mkdir(dirname) end

--- Deletes a file.
--- @param filename string The filename to delete
function smgf.io.delete(filename) end

--- Returns a file type.
--- @param filename string The filename to query
--- @return "file" | "directory" | "symlink" | "other" type The file type
function smgf.io.type(filename) end

--- Returns whether a file exists.
--- @param filename string The filename to query
--- @return boolean exists
function smgf.io.exists(filename) end
