#include <stdio.h>
#include <stdlib.h>
// #include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SDL_STBIMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "SDL_stbimage.h"

#include "SDL_nmix.h"
#include <physfs.h>

#include "smgf.h"

#include "SDL_DBGP_unscii16.h"

#if !SDL_VERSION_ATLEAST(2, 0, 18)
#error SMGF requires SDL 2.0.18 or later.
#endif

// main loop variables
static int start_time = 0;
static int end_time = 0;
static int dt = 0;
static SDL_Rect dst_rect = {0};
static smgf c;

static inline void mainloop(void) {
  start_time = SDL_GetTicks();
  dt = start_time - end_time;

  SDL_Event e;

  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT: c.should_quit = true; break;

    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        c.focused = true;
        smgf_lfocus(&c, true);
      } else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        c.focused = false;
        smgf_lfocus(&c, false);
      }
      break;

    // keyboard
    case SDL_KEYDOWN:
      // ignore key repeats
      if (e.key.repeat != 0) {
        break;
      }
      smgf_lkey_down(&c, &e.key.keysym);
      break;

    case SDL_KEYUP: smgf_lkey_up(&c, &e.key.keysym); break;

    case SDL_TEXTINPUT: smgf_ltext_input(&c, e.text.text); break;

    // case SDL_TEXTEDITING:
    //     break;

    // mouse
    case SDL_MOUSEBUTTONDOWN:
      smgf_lmouse_down(&c, e.button.x, e.button.y, e.button.button);
      break;

    case SDL_MOUSEBUTTONUP:
      smgf_lmouse_up(&c, e.button.x, e.button.y, e.button.button);
      break;

    case SDL_MOUSEMOTION:
      smgf_lmouse_moved(
          &c, e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
      break;

    case SDL_MOUSEWHEEL:
      smgf_lmouse_wheel(&c, e.wheel.x, e.wheel.y, e.wheel.direction);
      break;

      // game controllers
      // note: "[SDL_JoystickID] is a unique ID for a joystick for
      // the time it is connected to the system, and is never reused
      // for the lifetime of the application. If the joystick
      // is disconnected and reconnected, it will get a new ID."
      // note2: this problem is fixed in SDL3
      // see:
      // https://github.com/libsdl-org/SDL/commit/46e1377d49ddd2d25b3c2b2c23b91788093f2392

    case SDL_CONTROLLERDEVICEADDED:
      smgf_lgamepad_added(&c, e.cdevice.which);
      break;

    case SDL_CONTROLLERDEVICEREMOVED:
      smgf_lgamepad_removed(&c, e.cdevice.which);
      break;

    case SDL_CONTROLLERDEVICEREMAPPED: break;

    case SDL_CONTROLLERBUTTONDOWN:
      smgf_lgamepad_down(&c, e.cbutton.which, e.cbutton.button);
      break;

    case SDL_CONTROLLERBUTTONUP:
      smgf_lgamepad_up(&c, e.cbutton.which, e.cbutton.button);
      break;

    case SDL_CONTROLLERAXISMOTION:
      smgf_lgamepad_axismotion(&c, e.caxis.which, e.caxis.axis, e.caxis.value);
      break;

    case SDL_RENDER_TARGETS_RESET: {
      // the render targets have been reset and their contents need to be
      // updated (>= SDL 2.0.2)
      DBGP_CloseFont(&c.font);
      if (DBGP_OpenFont(
              &c.font, c.renderer, DBGP_UNSCII16, sizeof(DBGP_UNSCII16),
              DBGP_UNSCII16_WIDTH, DBGP_UNSCII16_HEIGHT) != 0) {
        SDL_Log("Unable to initialise DBGP: %s", SDL_GetError());
      }

      smgf_lrender_targets_reset(&c);
    } break;

    case SDL_RENDER_DEVICE_RESET: {
      // the device has been reset and all textures need to be recreated (>=
      // SDL 2.0.4)
      DBGP_CloseFont(&c.font);
      if (DBGP_OpenFont(
              &c.font, c.renderer, DBGP_UNSCII16, sizeof(DBGP_UNSCII16),
              DBGP_UNSCII16_WIDTH, DBGP_UNSCII16_HEIGHT) != 0) {
        SDL_Log("Unable to initialise DBGP: %s", SDL_GetError());
      }

      smgf_ldevice_reset(&c);
    } break;
    }
  }

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
  SDL_RenderCopy(c.renderer, c.screen_texture->tex, NULL, &dst_rect);
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
}

/** returns file path if a file has been dropped, or NULL. Used for startup.
 * Returned value must be freed by user.
 */
static char* startup_get_dropped_file(void) {
  SDL_Event e;
  if (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_DROPFILE, SDL_DROPFILE) > 0) {
    return e.drop.file;
  }

  return NULL;
}

/* static const char* SDL_priority_prefixes[SDL_NUM_LOG_PRIORITIES] = {
    NULL, "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"};

static FILE* log_file = NULL;

static void custom_logger(
    void* userdata, int category, SDL_LogPriority priority,
    const char* message) {
  // FILE* log_file = fopen("/tmp/smgf.log", "a");
  if (log_file != NULL) {
    fprintf(log_file, "%s: %s\n", SDL_priority_prefixes[priority], message);
    // fclose(log_file);
  }

  fprintf(stderr, "%s: %s\n", SDL_priority_prefixes[priority], message);
} */

int main(int argc, char** argv) {
  /* log_file = fopen("/tmp/smgf.log", "wt");
  SDL_LogSetOutputFunction(custom_logger, NULL); */
  SDL_Log("SMGF v%s (%s)", SMGF_VERSION, SDL_GetPlatform());

  // SDL init
  Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER;
  if (SDL_Init(flags) != 0) {
    SDL_Log("Unable to initialise SDL2: %s", SDL_GetError());
    return 1;
  }
  // @TODO: calling SDL_PumpEvents() *once* does not work
  // for files dropped onto macOS app
  SDL_PumpEvents();

  // show info on SDL version used
  SDL_version compiled, linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  SDL_Log(
      "using SDL2 v%d.%d.%d (compiled with v%d.%d.%d)", linked.major,
      linked.minor, linked.patch, compiled.major, compiled.minor,
      compiled.patch);

  if (SDL_VERSIONNUM(linked.major, linked.minor, linked.patch) <
      SDL_VERSIONNUM(2, 0, 9)) {
    SDL_SetError("SMGF requires SDL 2.0.9 or later.");
    return -1;
  }

  SDL_Log("using %s", LUA_RELEASE);

  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  // note: on macOS, needs to save in legacy format to be recognised by
  // Apple apps, including Preview and Finder.
  if (SDL_strcmp(SDL_GetPlatform(), "Mac OS X") == 0) {
    SDL_SetHint(SDL_HINT_BMP_SAVE_LEGACY_FORMAT, "1");
  }

  // needed for PS5 rumble over Bluetooth
  if (SDL_strcmp(SDL_GetPlatform(), "Windows") != 0) {
    // according to SDL docs, this breaks DirectInput handling for applications
    // that don't use SDL
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
  }

// only activate debug/verbose log messages for debug builds:
#ifndef NDEBUG
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
#else
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
#endif

  if (Sound_Init() == 0) {
    SDL_Log("Unable to initialise SDL_sound: %s", Sound_GetError());
    return 1;
  }

  if (NMIX_OpenAudio(NULL, 44100, 4096) != 0) {
    SDL_Log("Unable to initialise audio: %s", SDL_GetError());
    return 1;
  }

  SDL_AudioSpec* mixer = NMIX_GetAudioSpec();

  char* format_name = "";
  switch (mixer->format) {
  case AUDIO_U8: format_name = "AUDIO_U8"; break;
  case AUDIO_S8: format_name = "AUDIO_S8"; break;
  case AUDIO_U16LSB: format_name = "AUDIO_U16LSB"; break;
  case AUDIO_S16LSB: format_name = "AUDIO_S16LSB"; break;
  case AUDIO_U16MSB: format_name = "AUDIO_U16MSB"; break;
  case AUDIO_S16MSB: format_name = "AUDIO_S16MSB"; break;
  case AUDIO_S32LSB: format_name = "AUDIO_S32LSB"; break;
  case AUDIO_S32MSB: format_name = "AUDIO_S32MSB"; break;
  case AUDIO_F32LSB: format_name = "AUDIO_F32LSB"; break;
  case AUDIO_F32MSB: format_name = "AUDIO_F32MSB"; break;
  default: format_name = "UNKNOWN"; break;
  }

  SDL_Log(
      "opening SDL_nmix audio device: "
      "%dHz, %d channels, %s (%d), %d samples",
      mixer->freq, mixer->channels, format_name, mixer->format, mixer->samples);

  // if a file "game.smgf" is accessible, smgf will auto-start this
  // game. This behaviour allows users to easily package games without
  // recompiling smgf. We call this file the "bundled game".
  char* base_path = SDL_GetBasePath();
  char* bundled_game_path = NULL;
  int bundled_game_path_len =
      SDL_strlen(base_path) + SDL_strlen(SMGF_AUTOLOAD_FILE) + 1;
  bundled_game_path = SDL_calloc(bundled_game_path_len, sizeof(char));
  if (bundled_game_path == NULL) {
    return SDL_OutOfMemory();
  }
  SDL_snprintf(
      bundled_game_path, bundled_game_path_len, "%s%s", base_path,
      SMGF_AUTOLOAD_FILE);
  SDL_free(base_path);

  char* arg_path = NULL;
  for (int i = 1; i < argc; i++) {
    // we ignore "-psn" arguments from macOS Finder
    // https://github.com/libsdl-org/SDL/blob/9130f7c377c34cc4a2742202bb42d9332b7d8d7e/test/testdropfile.c#L47
    if (SDL_strcmp(SDL_GetPlatform(), "Mac OS X") == 0 &&
        SDL_strncmp(argv[i], "-psn", 4) == 0) {
      continue;
    }

    arg_path = argv[i];
    break;
  }

  // handle files dropped on app
  SDL_PumpEvents();
  char* dropped_path = startup_get_dropped_file();

  char* game_path = NULL;
  if (arg_path) {
    game_path = arg_path;
  } else if (dropped_path) {
    game_path = dropped_path;
  } else {
    game_path = bundled_game_path;
  }

  if (PHYSFS_init(argv[0]) == 0) {
    SDL_Log(
        "Unable to initialise physfs: %s",
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return 1;
  }

  // smgf init (opens conf.lua file)
  if (smgf_init(&c, game_path) != 0) {
    return 1;
  }

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.w = c.width;
  dst_rect.h = c.height;

  // print info on renderer:
  SDL_RendererInfo renderer_info;
  SDL_GetRendererInfo(c.renderer, &renderer_info);
  bool flag_software = renderer_info.flags & SDL_RENDERER_SOFTWARE;
  bool flag_accelerated = renderer_info.flags & SDL_RENDERER_ACCELERATED;
  bool flag_presentvsync = renderer_info.flags & SDL_RENDERER_PRESENTVSYNC;
  bool flag_targettexture = renderer_info.flags & SDL_RENDERER_TARGETTEXTURE;
  SDL_Log(
      "using %s video renderer ("
      "software fallback: %s, "
      "hardware acceleration: %s, "
      "present vsync: %s, "
      "target texture: %s)",
      renderer_info.name, flag_software ? "yes" : "no",
      flag_accelerated ? "yes" : "no", flag_presentvsync ? "yes" : "no",
      flag_targettexture ? "yes" : "no");

  smgf_linit(&c);
  SDL_RaiseWindow(c.window);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainloop, 0, 1);
#else
  while (!c.should_quit) {
    mainloop();
    // printf(">> %s\n", lua_type(c.L, 1) == LUA_TNONE ? "none" : "something");
    // printf(">> %d elements on the stack\n", lua_gettop(c.L));
  }
#endif

  smgf_quit(&c);
  PHYSFS_deinit();
  SDL_free(bundled_game_path);
  if (dropped_path) {
    SDL_free(dropped_path);
  }
  NMIX_CloseAudio();
  Sound_Quit();
  SDL_Quit();

  return 0;
}
