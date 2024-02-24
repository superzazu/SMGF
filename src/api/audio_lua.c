#include "../smgf.h"
#include "../api_lua.h"

static int l_set_master_gain(lua_State* L) {
  float gain = luaL_checknumber(L, 1);
  luaL_argcheck(L, gain >= 0 && gain <= 2, 1, "must be between 0 and 2");
  sf_au_set_master_gain(gain);
  return 0;
}

static int l_get_master_gain(lua_State* L) {
  lua_pushnumber(L, sf_au_get_master_gain());
  return 1;
}

static int l_set_master_pause(lua_State* L) {
  bool paused = lua_toboolean(L, 1);
  sf_au_set_master_pause(paused);
  return 0;
}

static int l_sound_new(lua_State* L) {
  const char* filename = luaL_checkstring(L, 1);
  int preloaded = 1; // defaults to preloaded (static)
  if (lua_gettop(L) > 1) {
    preloaded = lua_toboolean(L, 2);
  }

  ssound* s = (ssound*) lua_newuserdata(L, sizeof(ssound));
  if (sf_au_sound_new(s, filename, preloaded)) {
    return luaL_error(
        L, "unable to open sound file '%s' (%s)", filename, SDL_GetError());
  }

  luaL_getmetatable(L, SMGF_TYPE_SOUND);
  lua_setmetatable(L, -2);

  return 1;
}

static int l_sound_del(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  sf_au_sound_del(s);
  return 0;
}

static int l_sound_play(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  if (sf_au_sound_is_playing(s)) {
    return 0;
  }

  if (sf_au_sound_play(s)) {
    return luaL_error(L, "unable to play sound file (%s)", SDL_GetError());
  }

  return 0;
}

static int l_sound_get_duration(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  int duration = sf_au_sound_get_duration(s);
  if (duration == -1) {
    return luaL_error(
        L, "unable to get duration of sound file: %s", SDL_GetError());
  }

  lua_pushinteger(L, duration);

  return 1;
}

static int l_sound_pause(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  sf_au_sound_pause(s);
  return 0;
}

static int l_sound_rewind(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  if (sf_au_sound_rewind(s) != 0) {
    return luaL_error(L, "unable to rewind sound file (%s)", SDL_GetError());
  }
  return 0;
}

static int l_sound_seek(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);

  int ms = luaL_checknumber(L, 2);
  if (sf_au_sound_seek(s, ms) != 0) {
    return luaL_error(L, "unable to seek sound file (%s)", SDL_GetError());
  }

  return 0;
}

static int l_sound_is_playing(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  lua_pushboolean(L, sf_au_sound_is_playing(s));
  return 1;
}

static int l_sound_is_predecoded(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  lua_pushboolean(L, sf_au_sound_is_predecoded(s));
  return 1;
}

static int l_sound_get_pan(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  lua_pushnumber(L, sf_au_sound_get_pan(s));
  return 1;
}

static int l_sound_set_pan(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);

  float pan = luaL_checknumber(L, 2);
  luaL_argcheck(L, pan >= -1 && pan <= 1, 2, "must be between -1 and 1");
  sf_au_sound_set_pan(s, pan);
  return 0;
}

static int l_sound_get_gain(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  lua_pushinteger(L, sf_au_sound_get_gain(s));
  return 1;
}

static int l_sound_set_gain(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);

  float gain = luaL_checknumber(L, 2);
  luaL_argcheck(L, gain >= 0 && gain <= 2, 2, "must be between 0 and 2");
  sf_au_sound_set_gain(s, gain);
  return 0;
}

static int l_sound_get_loop(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);
  lua_pushnumber(L, sf_au_sound_get_loop(s));
  return 1;
}

static int l_sound_set_loop(lua_State* L) {
  ssound* s = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);

  bool loop = lua_toboolean(L, 2);
  sf_au_sound_set_loop(s, loop);
  return 0;
}

static int l_sound_clone(lua_State* L) {
  ssound* origin = (ssound*) luaL_checkudata(L, 1, SMGF_TYPE_SOUND);

  ssound* clone = (ssound*) lua_newuserdata(L, sizeof(ssound));
  if (sf_au_sound_new(clone, origin->filename, origin->snd->predecoded)) {
    return luaL_error(L, "unable to clone sound file (%s)", SDL_GetError());
  }

  luaL_getmetatable(L, SMGF_TYPE_SOUND);
  lua_setmetatable(L, -2);

  return 1;
}

static const struct luaL_Reg smgf_audio[] = {
    // static const struct luaL_Reg smgf_audio[] = {
    {"set_master_gain", l_set_master_gain},
    {"get_master_gain", l_get_master_gain},
    {"set_master_pause", l_set_master_pause},
    {"new", l_sound_new},
    // {"del_sound", l_del_sound}, // see #25
    {"play", l_sound_play},
    {"pause", l_sound_pause},
    {"get_duration", l_sound_get_duration},
    {"rewind", l_sound_rewind},
    {"seek", l_sound_seek},
    {"is_playing", l_sound_is_playing},
    {"is_predecoded", l_sound_is_predecoded},
    {"get_pan", l_sound_get_pan},
    {"set_pan", l_sound_set_pan},
    {"get_gain", l_sound_get_gain},
    {"set_gain", l_sound_set_gain},
    {"get_loop", l_sound_get_loop},
    {"set_loop", l_sound_set_loop},
    {"clone", l_sound_clone},
    {NULL, NULL}};

static const struct luaL_Reg sound_func[] = {
    // static const struct luaL_Reg sound_func[] = {
    {"play", l_sound_play},
    {"pause", l_sound_pause},
    {"get_duration", l_sound_get_duration},
    {"rewind", l_sound_rewind},
    {"seek", l_sound_seek},
    {"is_playing", l_sound_is_playing},
    {"is_predecoded", l_sound_is_predecoded},
    {"get_pan", l_sound_get_pan},
    {"set_pan", l_sound_set_pan},
    {"get_gain", l_sound_get_gain},
    {"set_gain", l_sound_set_gain},
    {"get_loop", l_sound_get_loop},
    {"set_loop", l_sound_set_loop},
    {"clone", l_sound_clone},
    {NULL, NULL}};

void init_audio(lua_State* L) {
  size_t n = SDL_arraysize(smgf_audio);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_audio, 0);
  lua_setfield(L, -2, "audio");

  // add sound type
  luaL_newmetatable(L, SMGF_TYPE_SOUND);
  lua_pushcfunction(L, l_sound_del);
  lua_setfield(L, -2, "__gc");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, sound_func, 0);
  lua_pop(L, 1);
}
