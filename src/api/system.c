#include "../api.h"

void sf_sy_get_window_size(smgf* const c, int* w, int* h) {
  SDL_GetWindowSize(c->window, w, h);
  *w /= c->zoom;
  *h /= c->zoom;
}

void sf_sy_set_window_size(smgf* const c, int w, int h) {
  SDL_SetWindowMinimumSize(c->window, w, h);
  SDL_RenderSetLogicalSize(c->renderer, w, h);
  SDL_SetWindowSize(c->window, w * c->zoom, h * c->zoom);
}

int sf_sy_get_fps(smgf* const c) {
  return c->fps;
}

void sf_sy_set_fps(smgf* const c, int fps) {
  c->fps = fps;
}

int sf_sy_get_zoom(smgf* const c) {
  return c->zoom;
}

void sf_sy_set_zoom(smgf* const c, int zoom) {
  if (zoom == c->zoom) {
    return;
  }

  int w = 0;
  int h = 0;
  sf_sy_get_window_size(c, &w, &h);

  c->zoom = zoom;
  sf_sy_set_window_size(c, w, h);
}

bool sf_sy_get_cursor_visible(smgf* const c) {
  return SDL_ShowCursor(SDL_QUERY);
}

void sf_sy_set_cursor_visible(smgf* const c, bool visible) {
  SDL_ShowCursor(visible);
}

const char* sf_sy_get_window_title(smgf* const c) {
  return SDL_GetWindowTitle(c->window);
}

void sf_sy_set_window_title(smgf* const c, const char* title) {
  SDL_SetWindowTitle(c->window, title);
}

void sf_sy_get_fullscreen(smgf* const c, bool* fullscreen) {
  // @TODO: implement SDL_WINDOW_FULLSCREEN_DESKTOP mode?
  *fullscreen = SDL_GetWindowFlags(c->window) & SDL_WINDOW_FULLSCREEN;
}

int sf_sy_set_fullscreen(smgf* const c, bool fullscreen) {
  return SDL_SetWindowFullscreen(c->window, fullscreen * SDL_WINDOW_FULLSCREEN);
}

void sf_sy_quit(smgf* const c) {
  c->should_quit = true;
}

void sf_sy_get_platform(smgf* const c, char const** platform) {
  *platform = SDL_GetPlatform();
}

void sf_sy_sleep(smgf* const c, float seconds) {
  Uint32 ms = seconds * 1000.f;
  SDL_Delay(ms);
}

void sf_sy_get_dt(smgf* const c, float* dt) {
  *dt = c->dt;
}

int sf_sy_open_url(smgf* const c, const char* url) {
  return SDL_OpenURL(url);
}

// void sf_sy_log(smgf* const c, const char* str) {
//     SDL_Log("%s", str);
// }

int sf_sy_set_clipboard(smgf* const c, const char* text) {
  return SDL_SetClipboardText(text);
}

int sf_sy_get_clipboard(smgf* const c, char** text) {
  *text = SDL_GetClipboardText();
  return *text != NULL;
}

int sf_sy_set_identity(smgf* const c, const char* org, const char* app) {
  // unmount current write dir if exists
  const char* current_pref_dir =
      PHYSFS_getPrefDir(c->organisation, c->application);
  if (current_pref_dir != NULL) {
    if (PHYSFS_setWriteDir(NULL) == 0) {
      SDL_SetError(
          "error unmounting write directory (%s)",
          PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return -1;
    };
    PHYSFS_unmount(current_pref_dir);
  }

  if (c->organisation) {
    SDL_free(c->organisation);
    c->organisation = NULL;
  }
  if (c->application) {
    SDL_free(c->application);
    c->application = NULL;
  }

  if (org == NULL || app == NULL) {
    c->organisation = NULL;
    c->application = NULL;
    return 0;
  }

  // try to mount the pref path and set it as write directory
  const char* pref_dir = PHYSFS_getPrefDir(org, app);
  if (pref_dir == NULL) {
    SDL_SetError(
        "error: could not retrieve the write directory (%s)",
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return -1;
  }

  // prepend the pref dir to search path
  if (PHYSFS_mount(pref_dir, "/", 0) == 0) {
    SDL_SetError(
        "error: could not mount %s (%s)", pref_dir,
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return -1;
  }
  PHYSFS_setWriteDir(pref_dir);

  SDL_Log("mounted %s to / (as write directory)", pref_dir);

  c->organisation = org;
  c->application = app;

  return 0;
}

int sf_sy_get_identity(smgf* const c, const char** org, const char** app) {
  *org = c->organisation;
  *app = c->application;
  return 0;
}

const char* sf_sy_get_write_dir(smgf* const c) {
  return PHYSFS_getWriteDir();
}

int sf_sy_show_message(
    smgf* const c, Uint32 flags, const char* title, const char* message) {
  SDL_Window* window = c->window;
  return SDL_ShowSimpleMessageBox(flags, title, message, c->window);
}

char* sf_sy_iconv(
    smgf* const c, const char* to, const char* from, const char* str) {
  return SDL_iconv_string(to, from, str, SDL_strlen(str) + 1);
}

SDL_Locale* sf_sy_get_preferred_locales(smgf* const c) {
  return SDL_GetPreferredLocales();
}

char* sf_sy_get_version(smgf* const c) {
  return SMGF_VERSION;
}
