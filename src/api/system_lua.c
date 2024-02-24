#include "../smgf.h"
#include "../api_lua.h"

static int l_get_width(lua_State* L) {
  smgf* const c = get_smgf(L);
  int w = 0;
  int h = 0;
  sf_sy_get_window_size(c, &w, &h);
  lua_pushinteger(L, w);
  return 1;
}

static int l_get_height(lua_State* L) {
  smgf* const c = get_smgf(L);
  int w = 0;
  int h = 0;
  sf_sy_get_window_size(c, &w, &h);
  lua_pushinteger(L, h);
  return 1;
}

static int l_get_dimensions(lua_State* L) {
  smgf* const c = get_smgf(L);
  int w = 0;
  int h = 0;
  sf_sy_get_window_size(c, &w, &h);
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  return 2;
}

static int l_set_dimensions(lua_State* L) {
  smgf* const c = get_smgf(L);

  int width = luaL_checknumber(L, 1);
  luaL_argcheck(L, width > 0, 1, "must be positive and non-zero");
  int height = luaL_checknumber(L, 2);
  luaL_argcheck(L, height > 0, 2, "must be positive and non-zero");

  sf_sy_set_window_size(c, width, height);
  return 0;
}

static int l_get_fps(lua_State* L) {
  smgf* const c = get_smgf(L);
  int fps = sf_sy_get_fps(c);
  if (fps == 0) {
    lua_pushnil(L);
  } else {
    lua_pushinteger(L, fps);
  }
  return 1;
}

static int l_set_fps(lua_State* L) {
  smgf* const c = get_smgf(L);

  int fps = luaL_optnumber(L, 1, 0);
  luaL_argcheck(
      L, fps >= 0, 1, "must be positive (or nil to disable FPS limiting)");

  sf_sy_set_fps(c, fps);
  return 0;
}

static int l_get_zoom(lua_State* L) {
  smgf* const c = get_smgf(L);
  lua_pushinteger(L, sf_sy_get_zoom(c));
  return 1;
}

static int l_set_zoom(lua_State* L) {
  smgf* const c = get_smgf(L);

  int zoom = luaL_optnumber(L, 1, 1);
  luaL_argcheck(L, zoom >= 1, 1, "must be positive (or nil to disable zoom)");

  sf_sy_set_zoom(c, zoom);
  return 0;
}

static int l_get_cursor_visible(lua_State* L) {
  smgf* const c = get_smgf(L);
  lua_pushboolean(L, sf_sy_get_cursor_visible(c));
  return 1;
}

static int l_set_cursor_visible(lua_State* L) {
  smgf* const c = get_smgf(L);
  bool visible = lua_toboolean(L, 1);
  sf_sy_set_cursor_visible(c, visible);
  return 0;
}

static int l_get_window_title(lua_State* L) {
  smgf* const c = get_smgf(L);
  lua_pushstring(L, sf_sy_get_window_title(c));
  return 1;
}

static int l_set_window_title(lua_State* L) {
  smgf* const c = get_smgf(L);
  const char* title = luaL_checkstring(L, 1);
  sf_sy_set_window_title(c, title);
  return 0;
}

static int l_quit(lua_State* L) {
  smgf* const c = get_smgf(L);
  sf_sy_quit(c);
  return 0;
}

static int l_get_platform(lua_State* L) {
  smgf* const c = get_smgf(L);
  const char* platform = NULL;
  sf_sy_get_platform(c, &platform);
  lua_pushstring(L, platform);
  return 1;
}

static int l_sleep(lua_State* L) {
  smgf* const c = get_smgf(L);

  int seconds = luaL_checknumber(L, 1);
  luaL_argcheck(L, seconds >= 0, 1, "must be positive");

  sf_sy_sleep(c, seconds);

  return 0;
}

static int l_get_dt(lua_State* L) {
  smgf* const c = get_smgf(L);
  float dt;
  sf_sy_get_dt(c, &dt);
  lua_pushnumber(L, dt);
  return 1;
}

static int l_get_fullscreen(lua_State* L) {
  smgf* const c = get_smgf(L);

  bool fullscreen = false;
  sf_sy_get_fullscreen(c, &fullscreen);

  lua_pushboolean(L, fullscreen);
  return 1;
}

static int l_set_fullscreen(lua_State* L) {
  smgf* const c = get_smgf(L);

  bool fullscreen = lua_toboolean(L, 1);
  if (sf_sy_set_fullscreen(c, fullscreen)) {
    return luaL_error(L, "unable to change fullscreen state");
  }

  return 0;
}

static int l_open_url(lua_State* L) {
  smgf* const c = get_smgf(L);
  const char* url = luaL_checkstring(L, 1);
  lua_pushinteger(L, sf_sy_open_url(c, url));
  return 1;
}

static int l_get_clipboard(lua_State* L) {
  smgf* const c = get_smgf(L);
  char* text = NULL;
  sf_sy_get_clipboard(c, &text);
  lua_pushstring(L, text);
  if (text != NULL) {
    SDL_free(text);
  }
  return 1;
}

static int l_set_clipboard(lua_State* L) {
  smgf* const c = get_smgf(L);
  const char* text = luaL_checkstring(L, 1);
  sf_sy_set_clipboard(c, text);
  return 0;
}

static int l_get_identity(lua_State* L) {
  smgf* const c = get_smgf(L);

  const char* org = NULL;
  const char* app = NULL;
  sf_sy_get_identity(c, &org, &app);

  lua_pushstring(L, org ? org : "");
  lua_pushstring(L, app ? app : "");

  return 2;
}

static int l_set_identity(lua_State* L) {
  smgf* const c = get_smgf(L);

  int r = -1;

  if (lua_gettop(L) == 0) {
    r = sf_sy_set_identity(c, NULL, NULL);
  } else {
    const char* org = luaL_checkstring(L, 1);
    const char* app = luaL_checkstring(L, 2);

    const char* org_copy = smgf_strcpy(org);
    const char* app_copy = smgf_strcpy(app);

    r = sf_sy_set_identity(c, org_copy, app_copy);
  }

  if (r != 0) {
    return luaL_error(L, SDL_GetError());
  }

  return 0;
}

static int l_get_write_dir(lua_State* L) {
  smgf* const c = get_smgf(L);
  const char* write_dir = sf_sy_get_write_dir(c);
  if (write_dir == NULL) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, write_dir);
  }
  return 1;
}

/* static int l_log(lua_State* L) {
  smgf* const c = get_smgf(L);

  lua_Debug ar;
  lua_getstack(L, 1, &ar);
  lua_getinfo(L, "nSl", &ar);

  fprintf(stdout, "%s:%d: ", ar.short_src, ar.currentline);

  int n = lua_gettop(L);
  for (int i = 1; i <= n; i++) {
    fprintf(stdout, "%s ", lua_tostring(L, i));
  }
  lua_pop(L, n);

  fprintf(stdout, "\n");

  return 0;
} */

static int l_show_message(lua_State* L) {
  smgf* const c = get_smgf(L);

  const char* title = luaL_checkstring(L, 1);
  const char* message = luaL_checkstring(L, 2);

  static const SDL_MessageBoxFlags flags[] = {
      SDL_MESSAGEBOX_ERROR,
      SDL_MESSAGEBOX_WARNING,
      SDL_MESSAGEBOX_INFORMATION,
  };
  static const char* const flag_names[] = {"error", "warning", "info", NULL};
  int op = luaL_checkoption(L, 3, "info", flag_names);

  int r = sf_sy_show_message(c, flags[op], title, message);
  if (r != 0) {
    return luaL_error(L, SDL_GetError());
  }

  return 0;
}

static int l_iconv(lua_State* L) {
  smgf* const c = get_smgf(L);

  const char* to = luaL_checkstring(L, 1);
  const char* from = luaL_checkstring(L, 2);
  const char* str = luaL_checkstring(L, 3);

  char* str_converted = sf_sy_iconv(c, to, from, str);
  if (str_converted == NULL) {
    return luaL_error(L, "unable to convert string");
  }

  lua_pushstring(L, str_converted);
  SDL_free(str_converted);

  return 1;
}

static int l_get_preferred_locales(lua_State* L) {
  smgf* const c = get_smgf(L);

  SDL_Locale* locale_list = sf_sy_get_preferred_locales(c);
  if (locale_list == NULL) {
    // return luaL_error(L, "unable to retrieve user preferred locales");
    lua_pushnil(L);
    return 1;
  }

  lua_newtable(c->L);

  int nb = 0;
  for (SDL_Locale* locale = locale_list; locale->language; locale++) {
    nb += 1;
    lua_pushinteger(L, nb);
    lua_pushstring(L, locale->language);
    lua_settable(L, -3);
  }

  SDL_free(locale_list);

  return 1;
}

static int l_get_version(lua_State* L) {
  smgf* const c = get_smgf(L);
  lua_pushstring(L, sf_sy_get_version(c));
  return 1;
}

static const struct luaL_Reg smgf_system[] = {
    {"get_dimensions", l_get_dimensions},
    {"set_dimensions", l_set_dimensions},
    {"get_width", l_get_width},
    {"get_height", l_get_height},
    {"get_fps", l_get_fps},
    {"set_fps", l_set_fps},
    {"get_zoom", l_get_zoom},
    {"set_zoom", l_set_zoom},
    {"get_cursor_visible", l_get_cursor_visible},
    {"set_cursor_visible", l_set_cursor_visible},
    {"get_window_title", l_get_window_title},
    {"set_window_title", l_set_window_title},
    {"quit", l_quit},
    {"get_platform", l_get_platform},
    {"sleep", l_sleep},
    {"get_dt", l_get_dt},
    {"get_fullscreen", l_get_fullscreen},
    {"set_fullscreen", l_set_fullscreen},
    // {"log", l_log},
    {"open_url", l_open_url},
    {"get_clipboard", l_get_clipboard},
    {"set_clipboard", l_set_clipboard},
    {"get_identity", l_get_identity},
    {"set_identity", l_set_identity},
    {"get_write_dir", l_get_write_dir},
    {"show_message", l_show_message},
    {"iconv", l_iconv},
    {"get_preferred_locales", l_get_preferred_locales},
    {"get_version", l_get_version},
    {NULL, NULL}};

void init_system(lua_State* L) {
  size_t n = SDL_arraysize(smgf_system);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_system, 0);
  lua_setfield(L, -2, "system");
}
