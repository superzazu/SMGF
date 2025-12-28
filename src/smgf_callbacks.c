#include "smgf.h"
#include "api.h"

static inline void l_pushtotable(lua_State* L, int key, char* val) {
  lua_pushstring(L, val);
  lua_rawseti(L, -2, key);
}

static inline void l_pushkeymodtable(lua_State* L, SDL_Keymod mod) {
  lua_newtable(L);

  int i = 0;
  if (mod & SDL_KMOD_LSHIFT)
    l_pushtotable(L, ++i, "lshift");
  if (mod & SDL_KMOD_RSHIFT)
    l_pushtotable(L, ++i, "rshift");
  if (mod & SDL_KMOD_LCTRL)
    l_pushtotable(L, ++i, "lctrl");
  if (mod & SDL_KMOD_RCTRL)
    l_pushtotable(L, ++i, "rctrl");
  if (mod & SDL_KMOD_LALT)
    l_pushtotable(L, ++i, "lalt");
  if (mod & SDL_KMOD_RALT)
    l_pushtotable(L, ++i, "ralt");
  if (mod & SDL_KMOD_LGUI)
    l_pushtotable(L, ++i, "lgui");
  if (mod & SDL_KMOD_RGUI)
    l_pushtotable(L, ++i, "rgui");
  if (mod & SDL_KMOD_NUM)
    l_pushtotable(L, ++i, "num");
  if (mod & SDL_KMOD_CAPS)
    l_pushtotable(L, ++i, "caps");
  if (mod & SDL_KMOD_MODE)
    l_pushtotable(L, ++i, "mode");
}

int smgf_linit(smgf* const c) {
  if (lua_getsmgffunc(c, "init") != 0) {
    return 1;
  }

  smgf_pcall(c->L, 0, 0);

  return 0;
}

int smgf_lupdate(smgf* const c) {
  if (lua_getsmgffunc(c, "update") != 0) {
    return 1;
  }

  lua_pushnumber(c->L, c->dt);
  smgf_pcall(c->L, 1, 0);

  return 0;
}

int smgf_ldraw(smgf* const c) {
  sf_gr_reset_graphics_stack(c);

  if (lua_getsmgffunc(c, "draw") != 0) {
    return 1;
  }

  smgf_pcall(c->L, 0, 0);

  return 0;
}

int smgf_lfocus(smgf* const c, bool is_focused) {
  if (lua_getsmgffunc(c, "focus") != 0) {
    return 1;
  }

  lua_pushboolean(c->L, is_focused);
  smgf_pcall(c->L, 1, 0);

  return 0;
}

int smgf_lkey_down(smgf* const c, SDL_KeyboardEvent* ev) {
  const char* key_name = smgf_get_name_from_scancode(ev->scancode);
  if (key_name == NULL) {
    return 1;
  }

  if (lua_getsmgffunc(c, "key_down") != 0) {
    return 1;
  }

  lua_pushstring(c->L, key_name);
  l_pushkeymodtable(c->L, ev->mod);

  smgf_pcall(c->L, 2, 0);

  return 0;
}

int smgf_lkey_up(smgf* const c, SDL_KeyboardEvent* ev) {
  const char* key_name = smgf_get_name_from_scancode(ev->scancode);
  if (key_name == NULL) {
    return 1;
  }

  if (lua_getsmgffunc(c, "key_up") != 0) {
    return 1;
  }

  lua_pushstring(c->L, key_name);
  l_pushkeymodtable(c->L, ev->mod);

  smgf_pcall(c->L, 2, 0);

  return 0;
}

int smgf_ltext_input(smgf* const c, const char* text) {
  if (lua_getsmgffunc(c, "text_input") != 0) {
    return 1;
  }

  lua_pushstring(c->L, text);
  smgf_pcall(c->L, 1, 0);

  return 0;
}

int smgf_lmouse_down(smgf* const c, int x, int y, int button) {
  // ignore events in case of clicks outside the game screen:
  if (x < 0 || y < 0 || x >= c->width || y >= c->height) {
    return 1;
  }

  if (lua_getsmgffunc(c, "mouse_down") != 0) {
    return 1;
  }

  lua_pushinteger(c->L, x);
  lua_pushinteger(c->L, y);
  lua_pushinteger(c->L, button);
  smgf_pcall(c->L, 3, 0);

  return 0;
}

int smgf_lmouse_up(smgf* const c, int x, int y, int button) {
  // ignore events in case of clicks outside the game screen:
  if (x < 0 || y < 0 || x >= c->width || y >= c->height) {
    return 1;
  }

  if (lua_getsmgffunc(c, "mouse_up") != 0) {
    return 1;
  }

  lua_pushinteger(c->L, x);
  lua_pushinteger(c->L, y);
  lua_pushinteger(c->L, button);
  smgf_pcall(c->L, 3, 0);

  return 0;
}

int smgf_lmouse_moved(smgf* const c, int x, int y, int xrel, int yrel) {
  // ignore events in case of clicks outside the game screen:
  if (x < 0 || y < 0 || x >= c->width || y >= c->height) {
    return 1;
  }

  if (lua_getsmgffunc(c, "mouse_moved") != 0) {
    return 1;
  }

  lua_pushinteger(c->L, x);
  lua_pushinteger(c->L, y);
  lua_pushinteger(c->L, xrel);
  lua_pushinteger(c->L, yrel);
  smgf_pcall(c->L, 4, 0);

  return 0;
}

int smgf_lmouse_wheel(smgf* const c, int x, int y, int direction) {
  // "If direction is SDL_MOUSEWHEEL_FLIPPED the values in x and y
  // will be opposite. Multiply by -1 to change them back."
  // see https://wiki.libsdl.org/SDL_MouseWheelEvent#Remarks
  if (direction == SDL_MOUSEWHEEL_FLIPPED) {
    x *= -1;
    y *= -1;
  }

  if (lua_getsmgffunc(c, "mouse_wheel") != 0) {
    return 1;
  }

  lua_pushinteger(c->L, x);
  lua_pushinteger(c->L, y);
  smgf_pcall(c->L, 2, 0);

  return 0;
}

// gamepad
int smgf_lgamepad_added(smgf* const c, int joystick_index) {
  // note: on "SDL_CONTROLLERDEVICEADDED" event only,
  // the "which" attribute corresponds to the "joystick device index",
  // and *not* the "instance id" like for "SDL_CONTROLLERDEVICEREMOVED"
  // and "SDL_CONTROLLERDEVICEREMAPPED"
  SDL_Gamepad* pad = SDL_OpenGamepad(joystick_index);
  if (pad == NULL) {
    return 1;
  }

  int player_index = SDL_GetGamepadPlayerIndex(pad);

  if (player_index == -1) {
    SDL_Log("Unable to open game controller: could not retrieve player index");
    return 1;
  }

  SDL_PropertiesID props = SDL_GetGamepadProperties(pad);

  SDL_Log(
      "Opened gamepad %d (%s), RUMBLE: %s, LED: %s, ACCEL: %s, GYRO: %s, "
      "TOUCHPADS: %i",
      player_index, SDL_GetGamepadName(pad),
      SDL_GetBooleanProperty(props, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false)
          ? "YES"
          : "NO",
      SDL_GetBooleanProperty(props, SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN, false)
          ? "YES"
          : "NO",
      SDL_GamepadHasSensor(pad, SDL_SENSOR_ACCEL) ? "YES" : "NO",
      SDL_GamepadHasSensor(pad, SDL_SENSOR_GYRO) ? "YES" : "NO",
      SDL_GetNumGamepadTouchpads(pad));

  // we need to store instance ids for when the controller is removed
  SDL_Joystick* joy = SDL_GetGamepadJoystick(pad);
  c->controllers[player_index] = SDL_GetJoystickID(joy);

  if (lua_getsmgffunc(c, "gamepad_added") != 0) {
    return 1;
  }
  lua_pushinteger(c->L, player_index);
  smgf_pcall(c->L, 1, 0);

  return 0;
}

int smgf_lgamepad_removed(smgf* const c, SDL_JoystickID id) {
  SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
  if (pad == NULL) {
    return 1;
  }

  // SDL_GameControllerGetPlayerIndex() does not work when the GameController
  // has been removed: so we need to store "joystick id" in memory so that
  // we can tell the user *which* gamepad has been removed. If we don't do
  // that, we can only tell the user "a gamepad has been plugged out"
  int player_index = -1;
  for (int i = 0; i < 4; i++) {
    if (c->controllers[i] == id) {
      player_index = i;
      c->controllers[i] = -1;
      break;
    }
  }

  SDL_Log("Closed gamepad %d (%s)", player_index, SDL_GetGamepadName(pad));
  SDL_CloseGamepad(pad);

  if (lua_getsmgffunc(c, "gamepad_removed") != 0) {
    return 1;
  }

  lua_pushinteger(c->L, player_index);
  smgf_pcall(c->L, 1, 0);

  return 1;
}

int smgf_lgamepad_down(
    smgf* const c, SDL_JoystickID id, SDL_GamepadButton button) {
  SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
  if (pad == NULL) {
    return 1;
  }

  int player_index = SDL_GetGamepadPlayerIndex(pad);
  if (player_index == -1) {
    return 1;
  }

  const char* button_str = SDL_GetGamepadStringForButton(button);
  if (button_str == NULL) {
    return 1;
  }

  if (lua_getsmgffunc(c, "gamepad_down") != 0) {
    return 1;
  }
  lua_pushinteger(c->L, player_index);
  lua_pushstring(c->L, button_str);
  smgf_pcall(c->L, 2, 0);

  return 0;
}

int smgf_lgamepad_up(
    smgf* const c, SDL_JoystickID id, SDL_GamepadButton button) {
  SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
  if (pad == NULL) {
    return 1;
  }

  int player_index = SDL_GetGamepadPlayerIndex(pad);
  if (player_index == -1) {
    return 1;
  }

  const char* button_str = SDL_GetGamepadStringForButton(button);
  if (button_str == NULL) {
    return 1;
  }

  if (lua_getsmgffunc(c, "gamepad_up") != 0) {
    return 1;
  }
  lua_pushinteger(c->L, player_index);
  lua_pushstring(c->L, button_str);
  smgf_pcall(c->L, 2, 0);

  return 0;
}

int smgf_lgamepad_axismotion(
    smgf* const c, SDL_JoystickID id, Uint8 axis, Sint16 value) {
  SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
  if (pad == NULL) {
    return 1;
  }

  int player_index = SDL_GetGamepadPlayerIndex(pad);
  if (player_index == -1) {
    return 1;
  }

  const char* axis_str = SDL_GetGamepadStringForAxis(axis);
  if (axis_str == NULL) {
    return 1;
  }

  if (lua_getsmgffunc(c, "gamepad_axismotion") != 0) {
    return 1;
  }

  float valuef = (float) value / SDL_MAX_SINT16; // in range [-1, 1]

  lua_pushinteger(c->L, player_index);
  lua_pushstring(c->L, axis_str);
  lua_pushnumber(c->L, valuef);
  smgf_pcall(c->L, 3, 0);

  return 0;
}

int smgf_lrender_targets_reset(smgf* const c) {
  if (lua_getsmgffunc(c, "targets_reset") != 0) {
    return 1;
  }

  smgf_pcall(c->L, 0, 0);

  return 0;
}

int smgf_ldevice_reset(smgf* const c) {
  if (lua_getsmgffunc(c, "device_reset") != 0) {
    return 1;
  }

  smgf_pcall(c->L, 0, 0);

  return 0;
}
