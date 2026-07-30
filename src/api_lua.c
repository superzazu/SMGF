#include "smgf.h"
#include "api_lua.h"

static bool readable(const char* filename) {
  PHYSFS_Stat st;
  return PHYSFS_stat(filename, &st) != 0 &&
         st.filetype == PHYSFS_FILETYPE_REGULAR;
}

// searches for a module name in a search path (usually package.path), through
// physfs. On success, pushes the file name on the Lua stack and returns it. On
// failure, pushes the "no file 'a'\n\tno file 'b'" message Lua would give and
// returns NULL.
static const char*
searchpath(lua_State* L, const char* name, const char* path) {
  // "a.b" -> "a/b", then "?.lua;?/init.lua" -> "a/b.lua;a/b/init.lua"
  const char* name_withdirsep = luaL_gsub(L, name, ".", "/");
  path = luaL_gsub(L, path, LUA_PATH_MARK, name_withdirsep);
  lua_remove(L, -2); // remove the string pushed by the first gsub

  // iterate over all templates (which are separated by a ";")
  const char* pos = path;
  while (*pos != '\0') {
    const char* sep = SDL_strchr(pos, *LUA_PATH_SEP);
    const size_t len = (sep != NULL) ? (size_t) (sep - pos) : SDL_strlen(pos);

    // note: physfs refuses paths with a "." component, so "./?.lua" templates
    // have to be trimmed
    const char* filename = lua_pushlstring(L, pos, len);
    if (filename[0] == '.' && filename[1] == '/') {
      filename += 2;
    }

    if (readable(filename)) {
      lua_remove(L, -2); // remove the path, keep the file name
      return filename;
    }

    lua_pop(L, 1);
    pos += len + (sep != NULL); // skip the ";", if there is one
  }

  lua_pushfstring(
      L, "no file '%s'", luaL_gsub(L, path, LUA_PATH_SEP, "'\n\tno file '"));
  lua_remove(L, -2); // remove the string pushed by gsub
  lua_remove(L, -2); // remove the path
  return NULL;
}

// a Lua chunk being streamed from physfs
typedef struct schunk {
  PHYSFS_File* file;
  char buff[BUFSIZ];
} schunk;

// lua_Reader over a physfs file (equivalent of "getF" in lauxlib.c): lua_load
// calls it back until it returns NULL, and we hand it the next block of file
static const char* smgf_getf(lua_State* L, void* ud, size_t* size) {
  schunk* const c = (schunk*) ud;

  const PHYSFS_sint64 nb_read =
      PHYSFS_readBytes(c->file, c->buff, sizeof(c->buff));
  if (nb_read <= 0) { // end of file, or read error (-1)
    *size = 0;
    return NULL;
  }

  *size = (size_t) nb_read;
  return c->buff;
}

// custom smgf package.searcher that loads modules through physfs (equivalent
// of "searcher_Lua" in loadlib.c). The "package" table is upvalue 1.
int l_smgf_searcher(lua_State* L) {
  const char* mod_name = luaL_checkstring(L, 1);

  if (lua_getfield(L, lua_upvalueindex(1), "path") != LUA_TSTRING) {
    return luaL_error(L, "'package.path' must be a string");
  }
  const char* packagepath = lua_tostring(L, -1);

  const char* file_name = searchpath(L, mod_name, packagepath);
  if (file_name == NULL) {
    return 1; // searchpath pushed the error message
  }

  schunk c;
  c.file = PHYSFS_openRead(file_name);
  if (c.file == NULL) {
    return luaL_error(
        L, "error loading module '%s' from file '%s':\n\t%s", mod_name,
        file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  // the "@" prefix tells Lua the chunk comes from a file, so that errors read
  // "mymodule.lua:12: ..." instead of "[string \"mymodule.lua\"]:12: ..."
  lua_pushfstring(L, "@%s", file_name);
  const int status = lua_load(L, smgf_getf, &c, lua_tostring(L, -1), NULL);
  PHYSFS_close(c.file);

  if (status != LUA_OK) {
    return luaL_error(
        L, "error loading module '%s' from file '%s':\n\t%s", mod_name,
        file_name, lua_tostring(L, -1));
  }

  lua_pushstring(L, file_name); // will be 2nd argument to module

  return 2;
}

// initialises a Lua state for smgf use
void lua_api_init(smgf* const c) {
  // store a pointer to smgf struct in Lua registry
  lua_pushstring(c->L, SMGF_PTRNAME);
  lua_pushlightuserdata(c->L, (void*) c);
  lua_settable(c->L, LUA_REGISTRYINDEX);

  // open standard libs
  luaL_openlibs(c->L);

  // remove almost entire io module, except: io.flush, io.write, io.stderr,
  // io.stdout
  lua_getglobal(c->L, "io");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "close");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "input");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "lines");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "open");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "output");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "popen");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "read");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "stdin");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "tmpfile");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "type");
  lua_pop(c->L, 1);

  // remove some functions from os module
  lua_getglobal(c->L, "os");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "execute");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "exit");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "getenv");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "remove");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "rename");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "setlocale");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "tmpname");
  lua_pop(c->L, 1);

  // remove dofile & loadfile
  lua_pushnil(c->L);
  lua_setglobal(c->L, "dofile");
  lua_pushnil(c->L);
  lua_setglobal(c->L, "loadfile");

  // set package.path + cpath
  lua_getglobal(c->L, "package");
  lua_pushstring(c->L, "./?.lua;./?/init.lua");
  lua_setfield(c->L, -2, "path");
  lua_pushstring(c->L, "");
  lua_setfield(c->L, -2, "cpath");
  lua_pop(c->L, 1);

  // we have to setup a custom package loader into package.searchers
  // so that modules are loaded through physfs
  // We overwrite package.searcher n°2 (the one using package.path). See:
  // https://www.lua.org/manual/5.5/manual.html#pdf-package.searchers
  lua_getglobal(c->L, "package");
  lua_getfield(c->L, -1, "searchers");
  // like Lua own "searchers", l_smgf_searcher takes "package" as upvalue
  // reading "package.path" from there means a game cannot break "require" by
  // overwriting the "package" global
  lua_pushvalue(c->L, -2);
  lua_pushcclosure(c->L, l_smgf_searcher, 1);
  lua_rawseti(c->L, -2, 2);
  // removing 3st & 4th package searchers that allow module loading
  // from C libraries:
  lua_pushnil(c->L);
  lua_rawseti(c->L, -2, 3);
  lua_pushnil(c->L);
  lua_rawseti(c->L, -2, 4);
  lua_pop(c->L, 1);
  // removing package.searchpath & package.loadlib
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "searchpath");
  lua_pushnil(c->L);
  lua_setfield(c->L, -2, "loadlib");
  lua_pop(c->L, 1);

  // add smgf table
  lua_newtable(c->L); // create smgf table

  // add modules to smgf table
  init_audio(c->L);
  init_graphics(c->L);
  init_input(c->L);
  init_io(c->L);
  init_system(c->L);

  // set smgf table global
  lua_setglobal(c->L, "smgf");
}

double lua_get_memory_kb(lua_State* L) {
  int count_k = lua_gc(L, LUA_GCCOUNT, 0);
  int count_b = lua_gc(L, LUA_GCCOUNTB, 0);
  return count_k + (count_b / 1024.0);
}
