#ifndef SMGF_API_LUA_H
#define SMGF_API_LUA_H

#include "api.h"

#define SMGF_TYPE_TEXTURE "smgf.texture"
#define SMGF_TYPE_SOUND "smgf.sound"
#define SMGF_TYPE_FILE "smgf.file"

const char* searchpath(
    lua_State* L, const char* name, const char* path, const char* sep,
    const char* dirsep);
int l_smgf_searcher(lua_State* L);
// void luaapi_init(smgf* const c);

void init_audio(lua_State* L);
void init_graphics(lua_State* L);
void init_input(lua_State* L);
void init_io(lua_State* L);
void init_system(lua_State* L);

#endif // SMGF_API_LUA_H
