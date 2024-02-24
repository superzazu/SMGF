#ifndef SMGF_SMGF_H
#define SMGF_SMGF_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <physfs.h>
#include "SDL_nmix_file.h"
#include "SDL_DBGP.h"

#ifndef SMGF_VERSION
#define SMGF_VERSION "0.0.1"
#endif

#define CONF_FILE_NAME "conf.lua"
#define MAIN_FILE_NAME "main.lua"
#define SMGF_PTRNAME "smgf_ptr"
#ifdef __EMSCRIPTEN__
#define SMGF_AUTOLOAD_FILE "game/"
#else
#define SMGF_AUTOLOAD_FILE "game.smgf"
#endif
// configuration defaults:
#define WIDTH_DEFAULT 256
#define HEIGHT_DEFAULT 256
#define FPS_DEFAULT 0
#define ZOOM_DEFAULT 1
#define WINDOW_TITLE_DEFAULT "SMGF v" SMGF_VERSION
#define CURSOR_VISIBLE_DEFAULT true

#define MAX_NB_GSTATES 64

#define SDL_LogErrorC(...) \
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define SDL_LogWarnC(...) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define SDL_LogInfoC(...) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define SDL_LogDebugC(...) \
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define luaC_loadstring(L, s, n) luaL_loadbuffer(L, s, strlen(s), n)

// definition of smgf types
typedef struct stexture {
  SDL_Texture* tex;
  int width, height;
  Uint32 format;
} stexture;

typedef struct ssound {
  const char* filename;
  SDL_RWops* rw;
  NMIX_FileSource* snd;
} ssound;

typedef struct sfile {
  SDL_RWops* file;
  PHYSFS_Stat stat;
  char mode;
} sfile;

typedef struct smgf_config {
  const char* window_title;
  const char* application; // game identity
  const char* organisation; // game identity
  int width, height; // of game screen
  int fps; // fps capping for update (defaults to 0 = disabled)
  float zoom; // zoom at startup
  bool cursor_visible;
} smgf_config;

typedef struct smgf_graphic_state {
  uint8_t r, g, b, a; // current color
  int x, y; // point of origin (modified by translations)
  stexture* target;
  int target_luaref;
} smgf_graphic_state;

// smgf machine
typedef struct smgf {
  lua_State* L;
  SDL_Window* window;
  SDL_Renderer* renderer;
  stexture* screen_texture;
  int width, height;
  int fps;
  float zoom;
  const char* application;
  const char* organisation;

  smgf_config conf;
  bool should_quit;
  bool focused;

  smgf_graphic_state* gstates;
  int gstates_ptr;

  float dt; // last dt
  Uint8 const* keyboard_state;
  SDL_JoystickID controllers[4];
  DBGP_Font font;
} smgf;

int smgf_init(smgf* const c, const char* game_folder);
int smgf_quit(smgf* const c);

smgf* get_smgf(lua_State* L);
int smgf_set_error(smgf* const c, const char* fmt, ...);
int smgf_pcall(lua_State* L, int narg, int nres);
int lua_getsmgf(smgf* const c);
int lua_getsmgffunc(smgf* const c, const char* fname);
void lua_api_init(smgf* const c); // initialises a Lua state for smgf use

const char* smgf_strcpy(const char* str);

// Lua callbacks:
int smgf_linit(smgf* const c);
int smgf_lupdate(smgf* const c);
int smgf_ldraw(smgf* const c);
int smgf_lfocus(smgf* const c, bool is_focused);
int smgf_lkey_down(smgf* const c, SDL_Keysym* key);
int smgf_lkey_up(smgf* const c, SDL_Keysym* key);
int smgf_ltext_input(smgf* const c, const char* text);
int smgf_lmouse_down(smgf* const c, int x, int y, int button);
int smgf_lmouse_up(smgf* const c, int x, int y, int button);
int smgf_lmouse_moved(smgf* const c, int x, int y, int xrel, int yrel);
int smgf_lmouse_wheel(smgf* const c, int x, int y, int direction);
int smgf_lgamepad_added(smgf* const c, int joystick_index);
int smgf_lgamepad_removed(smgf* const c, SDL_JoystickID id);
int smgf_lgamepad_down(
    smgf* const c, SDL_JoystickID id, SDL_GameControllerButton button);
int smgf_lgamepad_up(
    smgf* const c, SDL_JoystickID id, SDL_GameControllerButton button);
int smgf_lgamepad_axismotion(
    smgf* const c, SDL_JoystickID id, Uint8 axis, Sint16 value);
int smgf_lrender_targets_reset(smgf* const c);
int smgf_ldevice_reset(smgf* const c);

SDL_Scancode smgf_get_scancode_from_name(const char* name);
const char* smgf_get_name_from_scancode(SDL_Scancode scancode);

#endif // SMGF_SMGF_H
