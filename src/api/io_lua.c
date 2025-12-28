#include "../smgf.h"
#include "../api_lua.h"

static int l_open(lua_State* L) {
  smgf* const c = get_smgf(L);
  const char* filename = luaL_checkstring(L, 1);
  const char* mode = luaL_checkstring(L, 2);

  sfile* f = (sfile*) lua_newuserdata(L, sizeof(sfile));
  sf_io_new(f);
  if (sf_io_open(f, filename, mode[0]) != 0) {
    return luaL_error(L, "unable to open file: %s", SDL_GetError());
  }

  luaL_getmetatable(L, SMGF_TYPE_FILE);
  lua_setmetatable(L, -2);

  return 1;
}

static int l_mkdir(lua_State* L) {
  // smgf* const c = get_smgf(L);
  const char* dirname = luaL_checkstring(L, 1);
  if (sf_io_mkdir(dirname) != 0) {
    return luaL_error(L, "unable to create directory: %s", SDL_GetError());
  }
  return 0;
}

static int l_delete(lua_State* L) {
  // smgf* const c = get_smgf(L);
  const char* filename = luaL_checkstring(L, 1);
  if (sf_io_delete(filename) != 0) {
    return luaL_error(L, "unable to delete file: %s", SDL_GetError());
  }
  return 0;
}

static int l_get_type(lua_State* L) {
  // smgf* const c = get_smgf(L);
  const char* filename = luaL_checkstring(L, 1);

  if (!sf_io_exists(filename)) {
    return luaL_error(
        L, "unable to get type of file '%s': file does not exist", filename);
  }

  PHYSFS_FileType ft = sf_io_get_filetype(filename);
  if (ft == -1) {
    return luaL_error(
        L, "unable to get type of file '%s': stat error", filename);
  }
  const char* ft_str = "";
  switch (ft) {
  case PHYSFS_FILETYPE_REGULAR: ft_str = "file"; break;
  case PHYSFS_FILETYPE_DIRECTORY: ft_str = "directory"; break;
  case PHYSFS_FILETYPE_SYMLINK: ft_str = "symlink"; break;
  case PHYSFS_FILETYPE_OTHER:
  default: ft_str = "other"; break;
  }

  lua_pushstring(L, ft_str);
  return 1;
}

static int l_exists(lua_State* L) {
  // smgf* const c = get_smgf(L);
  const char* filename = luaL_checkstring(L, 1);
  lua_pushboolean(L, sf_io_exists(filename));
  return 1;
}

static int l_close(lua_State* L) {
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);

  if (sf_io_close(f) != 0) {
    return luaL_error(L, "unable to close file: %s", SDL_GetError());
  }

  return 0;
}

static int l_size(lua_State* L) {
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);

  Sint64 size = sf_io_size(f);
  if (size == -1) {
    return luaL_error(L, "unable to get size of file: %s", SDL_GetError());
  }
  lua_pushinteger(L, size);

  return 1;
}

static int l_seek(lua_State* L) {
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);
  Sint64 offset = luaL_checknumber(L, 2);

  static const int mode[] = {SDL_IO_SEEK_SET, SDL_IO_SEEK_CUR, SDL_IO_SEEK_END};
  static const char* const mode_names[] = {"set", "cur", "end", NULL};
  int op = luaL_checkoption(L, 3, "cur", mode_names);

  Sint64 final_offset = sf_io_seek(f, offset, mode[op]);
  if (final_offset == -1) {
    return luaL_error(L, "unable to seek file: %s", SDL_GetError());
  }

  lua_pushinteger(L, final_offset);
  return 1;
}

static int l_rewind(lua_State* L) {
  smgf* const c = get_smgf(L);

  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);
  sf_io_rewind(f);

  return 0;
}

static int l_tell(lua_State* L) {
  smgf* const c = get_smgf(L);

  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);
  Sint64 offset = sf_io_tell(f);
  lua_pushinteger(L, offset);

  return 1;
}

static int l_read(lua_State* L) {
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);

  if (f->mode != 'r') {
    return luaL_error(
        L, "unable to read from file: PhysicsFS error: file open for writing");
  }

  Sint64 bytes = 0;

  int n = lua_gettop(L);
  if (n == 1) {
    luaL_error(L, "missing argument ('all', 'line', or a number)");
  } else if (n >= 2) {
    // if the argument is a number, we read N characters
    if (lua_type(L, 2) == LUA_TNUMBER) {
      bytes = lua_tointeger(L, 2);
    } else if (lua_type(L, 2) == LUA_TSTRING) {
      const char* mode = lua_tostring(L, 2);
      if (SDL_strcmp(mode, "all") == 0) {
        // read the rest of the file starting at cursor
        bytes = sf_io_size(f) - sf_io_tell(f);
      } else if (SDL_strcmp(mode, "line") == 0) {
        // read only a line
        Sint64 pos = 0;
        sf_io_strchr(f, &pos, '\n');

        bytes = pos - sf_io_tell(f);
      } else {
        luaL_error(L, "unknown read mode '%s'", mode);
      }
    } else {
      luaL_error(L, "invalid call to read()");
    }
  }

  if (bytes == 0) {
    return 0;
  }

  void* data = SDL_malloc(bytes);
  if (data == NULL) {
    return luaL_error(L, "error allocating buffer for file reading");
  }

  if (sf_io_read(f, data, bytes) <= 0) {
    if (sf_io_tell(f) >= sf_io_size(f)) {
      return luaL_error(L, "unable to read outside of file");
    }

    return luaL_error(L, "unable to read from file: %s", SDL_GetError());
  }

  lua_pushlstring(L, data, bytes);
  SDL_free(data);

  return 1;
}

static int l_write(lua_State* L) {
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);

  size_t len;
  const char* str = luaL_checklstring(L, 2, &len);
  if (sf_io_write(f, (void*) str, len) <= 0) {
    return luaL_error(L, "unable to write to file: %s", SDL_GetError());
  }

  return 0;
}

static int l_file_del(lua_State* L) { // function on garbage collection
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);

  sf_io_del(f);
  return 0;
}

static int l_flush(lua_State* L) {
  smgf* const c = get_smgf(L);
  sfile* f = (sfile*) luaL_checkudata(L, 1, SMGF_TYPE_FILE);

  if (!sf_io_flush(f)) {
    return luaL_error(L, "unable to flush file: %s", SDL_GetError());
  }

  return 0;
}

static const struct luaL_Reg smgf_io[] = {
    {"open", l_open},     {"mkdir", l_mkdir},   {"delete", l_delete},
    {"type", l_get_type}, {"exists", l_exists},

    {"close", l_close},   {"size", l_size},     {"seek", l_seek},
    {"rewind", l_rewind}, {"tell", l_tell},     {"read", l_read},
    {"write", l_write},   {"flush", l_flush},   {NULL, NULL}};

static const struct luaL_Reg file_func[] = {
    {"close", l_close},   {"size", l_size},   {"seek", l_seek},
    {"rewind", l_rewind}, {"tell", l_tell},   {"read", l_read},
    {"write", l_write},   {"flush", l_flush}, {NULL, NULL}};

void init_io(lua_State* L) {
  size_t n = SDL_arraysize(smgf_io);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_io, 0);
  lua_setfield(L, -2, "io");

  // add file type
  luaL_newmetatable(L, SMGF_TYPE_FILE);
  lua_pushcfunction(L, l_file_del);
  lua_setfield(L, -2, "__gc");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, file_func, 0);
  lua_pop(L, 1);
}
