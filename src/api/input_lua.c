#include "../smgf.h"
#include "../api_lua.h"

// keyboard

static int l_keyboard_is_down(lua_State* L) {
  smgf* const c = get_smgf(L);

  const char* key = luaL_checkstring(L, 1);
  bool key_down = false;
  if (sf_kb_is_down(c, key, &key_down)) {
    return luaL_error(L, "key '%s' does not exist", key);
  }

  lua_pushboolean(L, key_down);
  return 1;
}

static int l_keyboard_is_up(lua_State* L) {
  smgf* const c = get_smgf(L);

  const char* key = luaL_checkstring(L, 1);
  bool key_up = true;
  if (sf_kb_is_up(c, key, &key_up)) {
    return luaL_error(L, "key '%s' does not exist", key);
  }

  lua_pushboolean(L, key_up);
  return 1;
}

static int l_set_textinput(lua_State* L) {
  smgf* const c = get_smgf(L);
  bool enable = lua_toboolean(L, 1);
  sf_kb_set_textinput(c, enable);
  return 0;
}

static int l_get_textinput(lua_State* L) {
  smgf* const c = get_smgf(L);
  lua_pushboolean(L, sf_kb_get_textinput(c));
  return 1;
}

// mouse

static int l_mouse_is_down(lua_State* L) {
  smgf* const c = get_smgf(L);

  int button = luaL_checknumber(L, 1);
  luaL_argcheck(L, button >= 1 && button <= 3, 1, "mouse button index must \
        be between 1 and 3 (1 for the left, 2 for middle, 3 for the right \
        button)");

  bool is_down = false;
  sf_ms_is_down(button, &is_down);

  lua_pushboolean(L, is_down);
  return 1;
}

static int l_mouse_get_pos(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x, y;
  sf_ms_get_pos(c, &x, &y);

  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

static int l_mouse_get_x(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x, y;
  sf_ms_get_pos(c, &x, &y);

  lua_pushnumber(L, x);
  return 1;
}

static int l_mouse_get_y(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x, y;
  sf_ms_get_pos(c, &x, &y);

  lua_pushnumber(L, y);
  return 1;
}

// gamepad

static int l_gamepad_is_open(lua_State* L) {
  smgf* const c = get_smgf(L);
  int player_index = luaL_checknumber(L, 1);
  // luaL_argcheck(L, player_index >= 0, 1, "player index must be positive");

  lua_pushboolean(L, sf_gp_is_open(player_index));
  return 1;
}

static int l_gamepad_is_down(lua_State* L) {
  smgf* const c = get_smgf(L);
  int player_index = luaL_checknumber(L, 1);
  // luaL_argcheck(L, player_index >= 0, 1, "player index must be positive");
  const char* button_name = luaL_checkstring(L, 2);
  int button_id = SDL_GetGamepadButtonFromString(button_name);

  luaL_argcheck(
      L, button_id != SDL_GAMEPAD_BUTTON_INVALID, 2,
      "expected a valid button name");

  lua_pushboolean(L, sf_gp_is_down(player_index, button_id));
  return 1;
}

static int l_gamepad_get_axis(lua_State* L) {
  smgf* const c = get_smgf(L);
  int player_index = luaL_checknumber(L, 1);
  // luaL_argcheck(L, player_index >= 0, 1, "player index must be positive");
  const char* axis_name = luaL_checkstring(L, 2);
  int axis_id = SDL_GetGamepadAxisFromString(axis_name);

  luaL_argcheck(
      L, axis_id != SDL_GAMEPAD_AXIS_INVALID, 2, "expected a valid axis name");

  float axis_value = sf_gp_get_axis(player_index, axis_id);
  lua_pushnumber(L, axis_value);
  return 1;
}

static int l_gamepad_rumble(lua_State* L) {
  smgf* const c = get_smgf(L);
  int player_index = luaL_checknumber(L, 1);
  float duration_s = luaL_checknumber(L, 2);
  float lintensity = luaL_checknumber(L, 3);
  float rintensity = luaL_optnumber(L, 4, lintensity);

  // luaL_argcheck(L, player_index >= 0, 1, "player index must be positive");
  luaL_argcheck(L, duration_s >= 0.f, 2, "duration must be positive");
  luaL_argcheck(
      L, lintensity >= 0.f && lintensity <= 1.f, 3,
      "intensity must be between 0 and 1");
  luaL_argcheck(
      L, rintensity >= 0.f && rintensity <= 1.f, 4,
      "intensity must be between 0 and 1");

  int r = sf_gp_rumble(player_index, duration_s, lintensity, rintensity);
  lua_pushinteger(L, r);

  return 1;
}

static int l_gamepad_get_name(lua_State* L) {
  smgf* const c = get_smgf(L);
  int player_index = luaL_checknumber(L, 1);

  // luaL_argcheck(L, player_index >= 0, 1, "player index must be positive");

  const char* name = sf_gp_get_name(player_index);
  lua_pushstring(L, name == NULL ? "" : name);
  return 1;
}

static const struct luaL_Reg smgf_keyboard[] = {
    {"is_down", l_keyboard_is_down},
    {"is_up", l_keyboard_is_up},
    {"set_textinput", l_set_textinput},
    {"get_textinput", l_get_textinput},
    {NULL, NULL}};

static const struct luaL_Reg smgf_mouse[] = {
    {"is_down", l_mouse_is_down},
    {"get_pos", l_mouse_get_pos},
    {"get_x", l_mouse_get_x},
    {"get_y", l_mouse_get_y},
    {NULL, NULL}};

static const struct luaL_Reg smgf_gamepad[] = {
    {"is_open", l_gamepad_is_open},   {"is_down", l_gamepad_is_down},
    {"get_axis", l_gamepad_get_axis}, {"rumble", l_gamepad_rumble},
    {"get_name", l_gamepad_get_name}, {NULL, NULL}};

void init_input(lua_State* L) {
  size_t n = 0;

  n = SDL_arraysize(smgf_keyboard);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_keyboard, 0);
  lua_setfield(L, -2, "keyboard");

  n = SDL_arraysize(smgf_mouse);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_mouse, 0);
  lua_setfield(L, -2, "mouse");

  n = SDL_arraysize(smgf_gamepad);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_gamepad, 0);
  lua_setfield(L, -2, "gamepad");
}
