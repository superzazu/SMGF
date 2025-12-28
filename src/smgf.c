#include "SDL_DBGP_unscii16.h"

#include "smgf.h"
#include "api.h"

// returns smgf instance from the user data stored in Lua registry
smgf* get_smgf(lua_State* L) {
  lua_pushstring(L, SMGF_PTRNAME);
  lua_gettable(L, LUA_REGISTRYINDEX);
  smgf* const f = lua_touserdata(L, -1);
  lua_pop(L, 1);
  return f;
}

// custom Lua error handler that pushes the traceback of the error
// on top of the stack
static int smgf_error_handler(lua_State* L) {
  const char* err = lua_tostring(L, -1);
  luaL_traceback(L, L, err, 1);
  const char* trace_back = lua_tostring(L, -1);
  return 1;
}

// from Lua 5.4 source (lua.c, function "docall"):
int smgf_pcall(lua_State* L, int narg, int nres) {
  int base = lua_gettop(L) - narg;
  lua_pushcfunction(L, smgf_error_handler);
  lua_insert(L, base);
  int status = lua_pcall(L, narg, nres, base);
  lua_remove(L, base);

  // if an error happened, we raise the error to smgf
  if (status != LUA_OK) {
    smgf* const c = get_smgf(L);
    smgf_set_error(c, "%s", lua_tostring(L, -1));
  }

  return status;
}

// reads a whole PHYSFS file into a buffer. Caller must call "free" on the
// pointer.
static inline char* PHYSFS_load(PHYSFS_File* handle) {
  if (handle == NULL) {
    return NULL;
  }

  int len = PHYSFS_fileLength(handle);
  char* contents = SDL_calloc(len + 1, sizeof(char));
  if (contents == NULL) {
    return NULL;
  }

  if (PHYSFS_readBytes(handle, contents, len) != len) {
    SDL_free(contents);
    return NULL;
  }

  return contents;
}

// opens a file and copies all its contents into a buffer.
static inline char* PHYSFS_readToBuffer(const char* filename) {
  PHYSFS_file* file = PHYSFS_openRead(filename);
  if (file == NULL) {
    int error_code = PHYSFS_getLastErrorCode();
    SDL_LogErrorC(
        "error opening %s: %s (%d)", filename,
        PHYSFS_getErrorByCode(error_code), error_code);
    return NULL;
  }

  char* file_contents = PHYSFS_load(file);
  if (file_contents == NULL) {
    PHYSFS_close(file);
    int error_code = PHYSFS_getLastErrorCode();
    SDL_LogErrorC(
        "error reading %s: %s (%d)", filename,
        PHYSFS_getErrorByCode(error_code), error_code);
    return NULL;
  }

  PHYSFS_close(file);
  return file_contents;
}

// Copies a string. The pointer returned must be freed by user.
const char* smgf_strcpy(const char* str) {
  int len = SDL_strlen(str) + 1;
  char* copy = SDL_malloc(len);
  if (!copy) {
    return NULL;
  }
  SDL_memcpy(copy, str, len);
  return copy;
}

// loads a smgf config file in a separated Lua state. If the file does not
// exists, sets the values to smgf defaults
static int load_config(smgf* c, const char* conf_file_name) {
  c->conf.window_title = WINDOW_TITLE_DEFAULT;
  c->conf.application = NULL;
  c->conf.organisation = NULL;
  c->conf.width = WIDTH_DEFAULT;
  c->conf.height = HEIGHT_DEFAULT;
  c->conf.fps = FPS_DEFAULT;
  c->conf.zoom = ZOOM_DEFAULT;
  c->conf.cursor_visible = CURSOR_VISIBLE_DEFAULT;

  if (!PHYSFS_exists(conf_file_name)) {
    SDL_LogInfoC("cannot find %s, skipping...", conf_file_name);
    return 0;
  }

  lua_State* L = luaL_newstate();

  char* buffer = PHYSFS_readToBuffer(conf_file_name);
  if (buffer == NULL) {
    return 1;
  }

  if (luaC_loadstring(L, buffer, conf_file_name)) {
    smgf_set_error(c, "%s", lua_tostring(L, -1));
    SDL_free(buffer);
    return 1;
  }
  SDL_free(buffer);

  if (smgf_pcall(L, 0, LUA_MULTRET)) {
    return 1;
  }

  // loading up the conf
  if (lua_gettop(L) != 1 || !lua_istable(L, 1)) {
    smgf_set_error(c, "conf.lua should return a table");
    return 1;
  }

  if (lua_getfield(L, -1, "width") == LUA_TNUMBER) {
    c->conf.width = lua_tonumber(L, -1);
    if (c->conf.width <= 0) {
      smgf_set_error(c, "width in conf.lua must be > 0");
      return 1;
    }
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "height") == LUA_TNUMBER) {
    c->conf.height = lua_tonumber(L, -1);
    if (c->conf.height <= 0) {
      smgf_set_error(c, "height in conf.lua must be > 0");
      return 1;
    }
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "fps") == LUA_TNUMBER) {
    c->conf.fps = lua_tonumber(L, -1);
    if (c->conf.fps < 0) {
      smgf_set_error(c, "fps in conf.lua must be >= 0");
      return 1;
    }
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "zoom") == LUA_TNUMBER) {
    c->conf.zoom = lua_tonumber(L, -1);
    if (c->conf.zoom < 1) {
      smgf_set_error(c, "zoom in conf.lua must be >= 1");
      return 1;
    }
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "cursor_visible") == LUA_TBOOLEAN) {
    c->conf.cursor_visible = lua_toboolean(L, -1);
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "window_title") == LUA_TSTRING) {
    const char* str = lua_tostring(L, -1);
    c->conf.window_title = smgf_strcpy(str);
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "organisation") == LUA_TSTRING) {
    const char* str = lua_tostring(L, -1);
    c->conf.organisation = smgf_strcpy(str);
  }
  lua_pop(L, 1);

  if (lua_getfield(L, -1, "application") == LUA_TSTRING) {
    const char* str = lua_tostring(L, -1);
    c->conf.application = smgf_strcpy(str);
  }
  lua_pop(L, 1);

  lua_close(L);

  return 0;
}

int smgf_init(smgf* const c, const char* game_folder) {
  c->should_quit = false;

  SDL_AudioSpec spec = {
      .format = SDL_AUDIO_F32,
      .channels = 2,
      .freq = 44100,
  };
  MIX_Mixer* mixer =
      MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
  if (mixer == NULL) {
    smgf_set_error(c, "Unable to create mixer device: %s", SDL_GetError());
    return 1;
  }
  c->mixer = mixer;

  // mount the game folder in physfs
  if (PHYSFS_mount(game_folder, "/", 1) == 0) {
    int error_code = PHYSFS_getLastErrorCode();
    smgf_set_error(
        c, "Error mounting %s: %s (%d)", game_folder,
        PHYSFS_getErrorByCode(error_code), error_code);
    return 1;
  }
  SDL_Log("mounted %s to %s", game_folder, PHYSFS_getMountPoint(game_folder));

  // load conf or populate with default values if needed
  if (load_config(c, CONF_FILE_NAME) != 0) {
    return 1;
  }
  c->width = c->conf.width;
  c->height = c->conf.height;
  c->fps = c->conf.fps;
  c->zoom = c->conf.zoom;

  // opening Lua env
  c->L = luaL_newstate();
  lua_api_init(c); // init Lua state + register smgf functions

  // sets the identity of the game, which has the effect of mounting pref path
  // if application+organisation are valid values
  const char* org =
      c->conf.organisation ? smgf_strcpy(c->conf.organisation) : NULL;
  const char* app =
      c->conf.application ? smgf_strcpy(c->conf.application) : NULL;
  sf_sy_set_identity(c, org, app);

  SDL_memset(c->controllers, -1, sizeof(SDL_JoystickID) * 4);

  // setting up SDL window
  c->window = SDL_CreateWindow(
      c->conf.window_title, c->width * c->zoom, c->height * c->zoom,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (c->window == NULL) {
    SDL_Log("unable to create window: %s", SDL_GetError());
    return 1;
  }
  SDL_SetWindowMinimumSize(c->window, c->width, c->height);

  // create SDL renderer
  c->renderer = SDL_CreateRenderer(c->window, NULL);
  if (c->renderer == NULL) {
    SDL_Log("unable to create renderer: %s", SDL_GetError());
    return 1;
  }
  SDL_SetRenderVSync(c->renderer, true);

  SDL_SetRenderLogicalPresentation(
      c->renderer, c->width, c->height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
  SDL_SetRenderDrawBlendMode(c->renderer, SDL_BLENDMODE_BLEND);

  if (!DBGP_CreateFont(
          &c->font, c->renderer, DBGP_UNSCII16, sizeof(DBGP_UNSCII16),
          DBGP_UNSCII16_HEIGHT)) {
    SDL_Log("Unable to initialise DBGP: %s", SDL_GetError());
    return 1;
  }

  // setting up graphics state + screen texture
  sf_gr_set_blend_mode(c, SDL_BLENDMODE_BLEND);
  c->screen_texture = SDL_malloc(sizeof(stexture));
  if (c->screen_texture == NULL) {
    smgf_set_error(c, "error allocating memory for screen texure");
    return 1;
  }
  sf_gr_texture_new_empty(c, c->screen_texture, c->width, c->height);

  c->gstates = SDL_calloc(MAX_NB_GSTATES, sizeof(smgf_graphic_state));
  c->gstates_ptr = 0;
  c->curstate = &c->gstates[c->gstates_ptr];
  sf_gr_reset_graphics_stack(c);

  // clearing screen texture once
  SDL_SetRenderTarget(c->renderer, c->screen_texture->tex);
  SDL_SetRenderDrawColor(c->renderer, 0, 0, 0, 255);
  SDL_RenderClear(c->renderer);

  // loading up main.lua
  char* buffer = PHYSFS_readToBuffer(MAIN_FILE_NAME);
  if (buffer == NULL) {
    smgf_set_error(c, "File %s not found from game directory.", MAIN_FILE_NAME);
    return 1;
  }

  if (luaC_loadstring(c->L, buffer, MAIN_FILE_NAME)) {
    smgf_set_error(c, "%s", lua_tostring(c->L, -1));
    SDL_free(buffer);
    return 1;
  }
  SDL_free(buffer);

  if (smgf_pcall(c->L, 0, LUA_MULTRET)) {
    return 1;
  }

  c->dt = 0;
  c->keyboard_state = SDL_GetKeyboardState(NULL);
  sf_kb_set_textinput(c, false);
  sf_sy_set_cursor_visible(c, c->conf.cursor_visible);
  c->focused = true;

  return 0;
}

int smgf_quit(smgf* const c) {
  if (c->L) {
    lua_close(c->L);
  }
  if (c->screen_texture != NULL) {
    sf_gr_texture_del(c->screen_texture);
  }
  if (c->screen_texture != NULL) {
    SDL_free(c->screen_texture);
  }
  c->curstate = NULL;
  if (c->gstates != NULL) {
    SDL_free(c->gstates);
  }
  DBGP_DestroyFont(&c->font);
  sf_sy_set_identity(c, NULL, NULL);
  if (c->conf.application) {
    SDL_free(c->conf.application);
  }
  if (c->conf.organisation) {
    SDL_free(c->conf.organisation);
  }
  if (c->renderer != NULL) {
    SDL_DestroyRenderer(c->renderer);
  }
  if (c->window != NULL) {
    SDL_DestroyWindow(c->window);
  }
  if (c->mixer != NULL) {
    MIX_DestroyMixer(c->mixer);
  }
  return 0;
}

// prints the error in logs + on a SDL message box, and sets smgf to a
// state where it can cleanly quit.
int smgf_set_error(smgf* const c, const char* fmt, ...) {
  SDL_Window* window = NULL;
  if (c != NULL) {
    window = c->window;

    if (c->should_quit) {
      return 1;
    }
    c->should_quit = true;
  }

  va_list args, args_copy;
  va_start(args, fmt);
  va_copy(args_copy, args);

  // note: SDL_Log**** functions does not accept va_list
  SDL_LogMessageV(
      SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, args);

  // note: we need to make a copy of "args" to avoid segfault
  char error_message[4096];
  SDL_vsnprintf(error_message, 4096, fmt, args_copy);
  SDL_ShowSimpleMessageBox(
      SDL_MESSAGEBOX_ERROR, "SMGF error", error_message, window);

  va_end(args);
  va_end(args_copy);

  return 1;
}

// Adds "smgf" global on the top of the stack.
// Returns 0 if OK, 1 if smgf table not found
int lua_getsmgf(smgf* const c) {
  if (lua_getglobal(c->L, "smgf") != LUA_TTABLE) {
    return smgf_set_error(c, "%s", "smgf table does not exist");
  }

  return 0;
}

// Adds a "smgf" function on the top of the stack.
// returns 0 if OK, 1 if smgf table not found, 2 if func not found
int lua_getsmgffunc(smgf* const c, const char* fname) {
  if (lua_getsmgf(c) != 0) {
    return 1;
  }

  if (lua_getfield(c->L, -1, fname) != LUA_TFUNCTION) {
    lua_pop(c->L, 2);
    return 2;
  }
  lua_remove(c->L, -2); // remove smgf table from stack

  return 0;
}

SDL_Scancode smgf_get_scancode_from_name(const char* name) {
  return SDL_GetScancodeFromName(name);
}

const char* smgf_get_name_from_scancode(SDL_Scancode scancode) {
  return SDL_GetScancodeName(scancode);
}
