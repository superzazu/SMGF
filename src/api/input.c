
#include "../api.h"

int sf_kb_is_down(smgf* const c, const char* key, bool* is_down) {
  SDL_Scancode sc = smgf_get_scancode_from_name(key);

  if (sc == SDLK_UNKNOWN) {
    return -1;
  }

  *is_down = c->keyboard_state[sc] == true;

  return 0;
}

int sf_kb_is_up(smgf* const c, const char* key, bool* is_up) {
  SDL_Scancode sc = smgf_get_scancode_from_name(key);

  if (sc == SDLK_UNKNOWN) {
    return -1;
  }

  *is_up = c->keyboard_state[sc] == false;

  return 0;
}

void sf_kb_set_textinput(smgf* const c, bool enable) {
  if (enable) {
    SDL_StartTextInput(c->window);
  } else {
    SDL_StopTextInput(c->window);
  }
}

bool sf_kb_get_textinput(smgf* const c) {
  return SDL_TextInputActive(c->window);
}

// mouse
void sf_ms_is_down(int button, bool* is_down) {
  // "button" variable expects either:
  // - 1: Left mouse button
  // - 2: Middle mouse button
  // - 3: Right mouse button

  int buttons = SDL_GetMouseState(NULL, NULL);
  *is_down = buttons & SDL_BUTTON_MASK(button);
}

void sf_ms_get_pos(smgf* const c, float* x, float* y) {
  float _x = 0, _y = 0;
  SDL_GetMouseState(&_x, &_y);

  float lx = 0, ly = 0;
  SDL_RenderCoordinatesFromWindow(c->renderer, _x, _y, &lx, &ly);

  // cap the coordinates to the game screen, so that mouse_get_pos()
  // always returns a valid vector inside game screen
  if (lx < 0)
    lx = 0;
  if (lx >= c->width)
    lx = c->width - 1;
  if (ly < 0)
    ly = 0;
  if (ly >= c->height)
    ly = c->height - 1;

  *x = lx;
  *y = ly;
}

// gamepad
// returns true if gamepad is opened, false if not
bool sf_gp_is_open(int player_index) {
  SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(player_index);
  return SDL_GamepadConnected(pad);
}

// returns 0 if gamepad does not exist
bool sf_gp_is_down(int player_index, SDL_GamepadButton button) {
  SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(player_index);
  if (pad == NULL) {
    return 0;
  }

  return SDL_GetGamepadButton(pad, button);
}

// returns 0 if gamepad does not exist
float sf_gp_get_axis(int player_index, SDL_GamepadAxis axis) {
  SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(player_index);
  if (pad == NULL) {
    return 0;
  }

  return (float) SDL_GetGamepadAxis(pad, axis) / SDL_MAX_SINT16;
}

// returns 0 if OK, -1 if rumble not supported for this gamepad, 1 if not found
int sf_gp_rumble(
    int player_index, float duration_s, float lintensity, float rintensity) {
  SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(player_index);
  if (pad == NULL) {
    return 1;
  }

  lintensity = lintensity > 1 ? 1 : (lintensity < 0 ? 0 : lintensity);
  Uint16 lintensity_uint = lintensity * 0xFFFF;

  rintensity = rintensity > 1 ? 1 : (rintensity < 0 ? 0 : rintensity);
  Uint16 rintensity_uint = rintensity * 0xFFFF;

  return SDL_RumbleGamepad(
      pad, lintensity_uint, rintensity_uint, duration_s * 1000);
}

// returns NULL if gamepad is not known
const char* sf_gp_get_name(int player_index) {
  SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(player_index);
  if (pad == NULL) {
    return NULL;
  }

  return SDL_GetGamepadName(pad);
}
