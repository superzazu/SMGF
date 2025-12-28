#include <stdio.h>
#include <stdlib.h>
// #include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_revision.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <physfs.h>

#include "smgf.h"

#include "SDL_DBGP_unscii16.h"

#if !SDL_VERSION_ATLEAST(3, 1, 5)
#error SMGF requires SDL 3.1.5 or later.
#endif

// main loop variables
static int start_time = 0;
static int end_time = 0;
static int dt = 0;
static SDL_FRect dst_rect = {0};
static smgf c;
static char* bundled_game_path = NULL;
static const char* dropped_path = NULL;
static SDL_IOStream* log_file = NULL;

/** returns file path if a file has been dropped, or NULL. Used for startup.
 * Returned value must be freed by user.
 */
static const char* startup_get_dropped_file(void) {
  SDL_Event e;
  if (SDL_PeepEvents(
          &e, 1, SDL_GETEVENT, SDL_EVENT_DROP_FILE, SDL_EVENT_DROP_FILE) > 0) {
    return e.drop.data;
  }

  return NULL;
}

static const char* SDL_priority_prefixes[SDL_LOG_PRIORITY_COUNT] = {
    "INVALID", "TRACE", "VERBOSE", "DEBUG",
    "INFO",    "WARN",  "ERROR",   "CRITICAL"};

static char custom_logger_datetime_str[30] = {0};

static void custom_logger(
    void* userdata, int category, SDL_LogPriority priority,
    const char* message) {
  SDL_Time utc_time = 0;
  SDL_GetCurrentTime(&utc_time);
  SDL_DateTime date_time = {0};
  SDL_TimeToDateTime(utc_time, &date_time, false);
  SDL_snprintf(
      custom_logger_datetime_str, 30, "%04d-%02d-%02d %02d:%02d:%02d.%09d",
      date_time.year, date_time.month, date_time.day, date_time.hour,
      date_time.minute, date_time.second, date_time.nanosecond);

  if (userdata != NULL) {
    SDL_IOStream* log_file = (SDL_IOStream*) userdata;
    SDL_IOprintf(
        log_file, "%s [SMGF] %s: %s\n", custom_logger_datetime_str,
        SDL_priority_prefixes[priority], message);
  }

  fprintf(
      stdout, "%s [SMGF] %s: %s\n", custom_logger_datetime_str,
      SDL_priority_prefixes[priority], message);
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  // log_file = SDL_IOFromFile("/tmp/smgf.log", "a");
  // SDL_SetLogOutputFunction(custom_logger, log_file);
  SDL_SetLogOutputFunction(custom_logger, NULL);

  SDL_Log("SMGF v%s (%s)", SMGF_VERSION, SDL_GetPlatform());

  // SDL init
  Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD;
  if (!SDL_Init(flags)) {
    SDL_LogErrorC("Unable to initialise SDL3: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  // @TODO: calling SDL_PumpEvents() *once* does not work
  // for files dropped onto macOS app
  SDL_PumpEvents();

  // show info on SDL version used
  int compiled = SDL_VERSION;
  int linked = SDL_GetVersion();

  SDL_Log(
      "using SDL v%d.%d.%d (compiled with v%d.%d.%d), revision %s",
      SDL_VERSIONNUM_MAJOR(linked), SDL_VERSIONNUM_MINOR(linked),
      SDL_VERSIONNUM_MICRO(linked), SDL_VERSIONNUM_MAJOR(compiled),
      SDL_VERSIONNUM_MINOR(compiled), SDL_VERSIONNUM_MICRO(compiled),
      SDL_REVISION);

  if (linked < SDL_VERSIONNUM(3, 1, 5)) {
    SDL_LogErrorC("SMGF requires SDL 3.1.5 or later.");
    return SDL_APP_FAILURE;
  }

  SDL_Log("using %s", LUA_RELEASE);

  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
  // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "gpu");

  if (SDL_strcmp(SDL_GetPlatform(), "macOS") == 0) {
    // note: on macOS, needs to save in legacy format to be recognised by
    // Apple apps, including Preview and Finder.
    SDL_SetHint(SDL_HINT_BMP_SAVE_LEGACY_FORMAT, "1");

    // avoid renaming some scancodes on macOS
    SDL_SetScancodeName(SDL_SCANCODE_LALT, "Left Alt");
    SDL_SetScancodeName(SDL_SCANCODE_LGUI, "Left GUI");
    SDL_SetScancodeName(SDL_SCANCODE_RALT, "Right Alt");
    SDL_SetScancodeName(SDL_SCANCODE_RGUI, "Right GUI");
  }

  // needed for PS5 rumble over Bluetooth
  if (SDL_strcmp(SDL_GetPlatform(), "Windows") != 0) {
    // according to SDL docs, this breaks DirectInput handling for applications
    // that don't use SDL
    SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "1");
  }

// only activate debug/verbose log messages for debug builds:
#ifndef NDEBUG
  SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
#else
  SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
#endif

  if (!MIX_Init()) {
    SDL_LogErrorC("Unable to initialise SDL_mixer: %s", SDL_GetError());
  }

  // if a file "game.smgf" is accessible, smgf will auto-start this
  // game. This behaviour allows users to easily package games without
  // recompiling smgf. We call this file the "bundled game".
  const char* base_path = SDL_GetBasePath();
  bundled_game_path = NULL;
  int bundled_game_path_len =
      SDL_strlen(base_path) + SDL_strlen(SMGF_AUTOLOAD_FILE) + 1;
  bundled_game_path = SDL_calloc(bundled_game_path_len, sizeof(char));
  if (bundled_game_path == NULL) {
    SDL_OutOfMemory();
    return SDL_APP_FAILURE;
  }
  SDL_snprintf(
      bundled_game_path, bundled_game_path_len, "%s%s", base_path,
      SMGF_AUTOLOAD_FILE);

  char* arg_path = NULL;
  for (int i = 1; i < argc; i++) {
    // we ignore "-psn" arguments from macOS Finder
    // https://github.com/libsdl-org/SDL/blob/9130f7c377c34cc4a2742202bb42d9332b7d8d7e/test/testdropfile.c#L47
    if (SDL_strcmp(SDL_GetPlatform(), "macOS") == 0 &&
        SDL_strncmp(argv[i], "-psn", 4) == 0) {
      continue;
    }

    arg_path = argv[i];
    break;
  }

  // handle files dropped on app on launch
  SDL_PumpEvents();
  dropped_path = startup_get_dropped_file();

  const char* game_path = NULL;
  if (arg_path) {
    game_path = arg_path;
  } else if (dropped_path) {
    game_path = dropped_path;
  } else {
    game_path = bundled_game_path;
  }

  if (PHYSFS_init(argv[0]) == 0) {
    SDL_LogErrorC(
        "Unable to initialise physfs: %s",
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return SDL_APP_FAILURE;
  }

  // smgf init (opens conf.lua file)
  if (smgf_init(&c, game_path) != 0) {
    return SDL_APP_FAILURE;
  }

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.w = c.width;
  dst_rect.h = c.height;

  // print info on renderer:
  const char* renderer_name = SDL_GetRendererName(c.renderer);
  int vsync = 0;
  SDL_GetRenderVSync(c.renderer, &vsync);
  SDL_Log("using \"%s\" video renderer (vsync: %d)", renderer_name, vsync);

  smgf_linit(&c);
  SDL_RaiseWindow(c.window);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  start_time = SDL_GetTicks();
  dt = start_time - end_time;

  // update
  c.dt = dt / 1000.f;
  smgf_lupdate(&c);

  // draw
  SDL_SetRenderTarget(c.renderer, c.screen_texture->tex);
  smgf_ldraw(&c);

  // clearing the renderer
  SDL_SetRenderTarget(c.renderer, NULL);
  SDL_SetRenderDrawColor(c.renderer, 0, 0, 0, 255);
  SDL_RenderClear(c.renderer);

  // drawing texture on renderer
  SDL_RenderTexture(c.renderer, c.screen_texture->tex, NULL, &dst_rect);
  SDL_RenderPresent(c.renderer);

  // wait a little bit before next frame if needed
  if (c.fps > 0) {
    const int frame_rate = 1000.f / c.fps;
    int frame_time = SDL_GetTicks() - start_time;
    if (frame_time < frame_rate) {
      SDL_Delay(frame_rate - frame_time);
    }
  }

  end_time = start_time;

// see https://github.com/libsdl-org/SDL/issues/1871
#if TARGET_OS_MAC
  if (!c.focused) {
    SDL_Delay(16);
  }
#endif

  // printf(">> %s\n", lua_type(c.L, 1) == LUA_TNONE ? "none" : "something");
  // printf(">> %d elements on the stack\n", lua_gettop(c.L));

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* e) {
  switch (e->type) {

  case SDL_EVENT_QUIT: c.should_quit = true; break;

  case SDL_EVENT_WINDOW_FOCUS_GAINED:
    c.focused = true;
    smgf_lfocus(&c, true);
    break;

  case SDL_EVENT_WINDOW_FOCUS_LOST:
    c.focused = false;
    smgf_lfocus(&c, false);
    break;

  // keyboard
  case SDL_EVENT_KEY_DOWN:
    // ignore key repeats
    if (e->key.repeat != 0) {
      break;
    }
    smgf_lkey_down(&c, &e->key);
    break;

  case SDL_EVENT_KEY_UP: smgf_lkey_up(&c, &e->key); break;

  case SDL_EVENT_TEXT_INPUT: smgf_ltext_input(&c, e->text.text); break;

  // case SDL_TEXTEDITING:
  //     break;

  // mouse
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    smgf_lmouse_down(&c, e->button.x, e->button.y, e->button.button);
    break;

  case SDL_EVENT_MOUSE_BUTTON_UP:
    smgf_lmouse_up(&c, e->button.x, e->button.y, e->button.button);
    break;

  case SDL_EVENT_MOUSE_MOTION:
    smgf_lmouse_moved(
        &c, e->motion.x, e->motion.y, e->motion.xrel, e->motion.yrel);
    break;

  case SDL_EVENT_MOUSE_WHEEL:
    smgf_lmouse_wheel(&c, e->wheel.x, e->wheel.y, e->wheel.direction);
    break;

    // game controllers
    // note: "[SDL_JoystickID] is a unique ID for a joystick for
    // the time it is connected to the system, and is never reused
    // for the lifetime of the application. If the joystick
    // is disconnected and reconnected, it will get a new ID."
    // note2: this problem is fixed in SDL3
    // see:
    // https://github.com/libsdl-org/SDL/commit/46e1377d49ddd2d25b3c2b2c23b91788093f2392

  case SDL_EVENT_GAMEPAD_ADDED:
    smgf_lgamepad_added(&c, e->cdevice.which);
    break;

  case SDL_EVENT_GAMEPAD_REMOVED:
    smgf_lgamepad_removed(&c, e->cdevice.which);
    break;

  case SDL_EVENT_GAMEPAD_REMAPPED: break;

  case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    smgf_lgamepad_down(&c, e->gbutton.which, e->gbutton.button);
    break;

  case SDL_EVENT_GAMEPAD_BUTTON_UP:
    smgf_lgamepad_up(&c, e->gbutton.which, e->gbutton.button);
    break;

  case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    smgf_lgamepad_axismotion(&c, e->gaxis.which, e->gaxis.axis, e->gaxis.value);
    break;

  case SDL_EVENT_RENDER_TARGETS_RESET: {
    // the render targets have been reset and their contents need to be
    // updated (>= SDL 2.0.2)
    DBGP_DestroyFont(&c.font);
    if (!DBGP_CreateFont(
            &c.font, c.renderer, DBGP_UNSCII16, sizeof(DBGP_UNSCII16),
            DBGP_UNSCII16_HEIGHT)) {
      SDL_Log("Unable to initialise DBGP: %s", SDL_GetError());
    }

    smgf_lrender_targets_reset(&c);
  } break;

  case SDL_EVENT_RENDER_DEVICE_RESET: {
    // the device has been reset and all textures need to be recreated (>=
    // SDL 2.0.4)
    DBGP_DestroyFont(&c.font);
    if (!DBGP_CreateFont(
            &c.font, c.renderer, DBGP_UNSCII16, sizeof(DBGP_UNSCII16),
            DBGP_UNSCII16_HEIGHT)) {
      SDL_Log("Unable to initialise DBGP: %s", SDL_GetError());
    }

    smgf_ldevice_reset(&c);
  } break;
  }

  if (c.should_quit) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  smgf_quit(&c);
  PHYSFS_deinit();
  SDL_free(bundled_game_path);
  if (dropped_path) {
    SDL_free((char*) dropped_path);
  }
  if (log_file != NULL) {
    SDL_CloseIO(log_file);
  }
  // Mix_CloseAudio();
  MIX_Quit();
  SDL_Quit();
}
