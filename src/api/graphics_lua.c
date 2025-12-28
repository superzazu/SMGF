
#include "../smgf.h"
#include "../api_lua.h"

// gets color from the top of Lua stack
static inline int lua_get_color(lua_State* L, int narg, SDL_Color* c) {
  int r = 0, g = 0, b = 0, a = 0;
  if (lua_istable(L, narg)) {
    int n = luaL_len(L, narg);
    if (n < 3 || n > 4) {
      return luaL_argerror(L, 2, "invalid color (must have 3 or 4 components)");
    }

    lua_geti(L, narg, 1);
    lua_geti(L, narg, 2);
    lua_geti(L, narg, 3);
    lua_geti(L, narg, 4);

    r = luaL_checknumber(L, -4);
    g = luaL_checknumber(L, -3);
    b = luaL_checknumber(L, -2);
    a = luaL_optnumber(L, -1, 255);

    lua_pop(L, 4);
  } else {
    r = luaL_optnumber(L, narg + 0, 0);
    g = luaL_optnumber(L, narg + 1, 0);
    b = luaL_optnumber(L, narg + 2, 0);
    a = luaL_optnumber(L, narg + 3, 255);
  }

  luaL_argcheck(
      L, r >= 0 && r <= 255, narg + 0, "RGBA values must be between 0 and 255");
  luaL_argcheck(
      L, g >= 0 && g <= 255, narg + 1, "RGBA values must be between 0 and 255");
  luaL_argcheck(
      L, b >= 0 && b <= 255, narg + 2, "RGBA values must be between 0 and 255");
  luaL_argcheck(
      L, a >= 0 && a <= 255, narg + 3, "RGBA values must be between 0 and 255");

  c->r = r;
  c->g = g;
  c->b = b;
  c->a = a;

  return 0;
}

static int l_clear(lua_State* L) {
  smgf* const c = get_smgf(L);

  SDL_Color color = {.r = 0, .g = 0, .b = 0, .a = 255};
  lua_get_color(L, 1, &color);

  if (!sf_gr_clear(c, &color)) {
    return luaL_error(L, "cannot clear (%s)", SDL_GetError());
  }
  return 0;
}

static int l_set_color(lua_State* L) {
  smgf* const c = get_smgf(L);

  // if no arg, set color to opaque white
  if (lua_gettop(L) == 0) {
    SDL_Color color = {.r = 255, .g = 255, .b = 255, .a = 255};
    if (!sf_gr_set_color(c, &color)) {
      return luaL_error(L, "cannot set color (%s)", SDL_GetError());
    }
    return 0;
  }

  SDL_Color color = {.r = 0, .g = 0, .b = 0, .a = 255};
  lua_get_color(L, 1, &color);

  if (!sf_gr_set_color(c, &color)) {
    return luaL_error(L, "cannot set color (%s)", SDL_GetError());
  }
  return 0;
}

static int l_get_color(lua_State* L) {
  smgf* const c = get_smgf(L);
  SDL_Color color = {0};
  if (!sf_gr_get_color(c, &color)) {
    return luaL_error(L, "cannot get color (%s)", SDL_GetError());
  }
  lua_pushnumber(L, color.r);
  lua_pushnumber(L, color.g);
  lua_pushnumber(L, color.b);
  lua_pushnumber(L, color.a);
  return 4;
}

static int l_get_blend_mode(lua_State* L) {
  smgf* const c = get_smgf(L);

  SDL_BlendMode b;
  if (!sf_gr_get_blend_mode(c, &b)) {
    return luaL_error(L, "cannot get blend mode (%s)", SDL_GetError());
  }

  switch (b) {
  case SDL_BLENDMODE_NONE: lua_pushstring(L, "none"); break;
  case SDL_BLENDMODE_BLEND: lua_pushstring(L, "blend"); break;
  case SDL_BLENDMODE_ADD: lua_pushstring(L, "add"); break;
  case SDL_BLENDMODE_MOD: lua_pushstring(L, "mod"); break;
  case SDL_BLENDMODE_INVALID: lua_pushstring(L, "invalid"); break;
  case SDL_BLENDMODE_MUL: lua_pushstring(L, "mul"); break;
  default: return luaL_error(L, "unknown blend mode (%s)", SDL_GetError());
  }

  return 1;
}

static int l_set_blend_mode(lua_State* L) {
  smgf* const c = get_smgf(L);

  static const int blend[] = {
      SDL_BLENDMODE_NONE, SDL_BLENDMODE_BLEND, SDL_BLENDMODE_ADD,
      SDL_BLENDMODE_MOD, SDL_BLENDMODE_MUL};
  static const char* const blend_names[] = {"none", "blend", "add",
                                            "mod",  "mul",   NULL};
  int op = luaL_checkoption(L, 1, "blend", blend_names);

  if (!sf_gr_set_blend_mode(c, blend[op])) {
    return luaL_error(L, "cannot set blend mode (%s)", SDL_GetError());
  }

  return 0;
}

static int l_get_point(lua_State* L) {
  smgf* const c = get_smgf(L);

  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);

  SDL_Color color = {0};
  if (sf_gr_get_point(c, &color, x, y)) {
    return luaL_error(L, "cannot get point (%s)", SDL_GetError());
  }

  lua_pushnumber(L, color.r);
  lua_pushnumber(L, color.g);
  lua_pushnumber(L, color.b);
  return 3;
}

static int l_draw_point(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);

  if (!sf_gr_draw_point(c, x, y)) {
    return luaL_error(L, "cannot draw point (%s)", SDL_GetError());
  }

  return 0;
}

static int l_draw_line(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x1 = luaL_checknumber(L, 1);
  float y1 = luaL_checknumber(L, 2);
  float x2 = luaL_checknumber(L, 3);
  float y2 = luaL_checknumber(L, 4);

  if (!sf_gr_draw_line(c, x1, y1, x2, y2)) {
    return luaL_error(L, "cannot draw line (%s)", SDL_GetError());
  }

  return 0;
}

static int l_draw_rect(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  float w = luaL_checknumber(L, 3);
  float h = luaL_checknumber(L, 4);

  if (!sf_gr_draw_rect(c, x, y, w, h)) {
    return luaL_error(L, "cannot draw rect (%s)", SDL_GetError());
  }

  return 0;
}

static int l_draw_rectfill(lua_State* L) {
  smgf* const c = get_smgf(L);

  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  float w = luaL_checknumber(L, 3);
  float h = luaL_checknumber(L, 4);

  if (!sf_gr_draw_rectfill(c, x, y, w, h)) {
    return luaL_error(L, "cannot draw rectfill (%s)", SDL_GetError());
  }

  return 0;
}

/* static int l_draw_geometry(lua_State* L) {
  smgf* const c = get_smgf(L);

  stexture* t = (stexture*) luaL_testudata(L, 1, SMGF_TYPE_TEXTURE);

  if (!lua_istable(L, 2)) {
    return luaL_argerror(L, 2, "expected vertices table");
  }

  int nb_vertices = luaL_len(L, 2);
  if (nb_vertices % 3 != 0 || nb_vertices <= 0) {
    return luaL_argerror(
        L, 2,
        "invalid vertices table: length must be divisible by 3 (and non-zero)");
  }

  // @TODO: test code, move memory allocation outside of this function
  SDL_Vertex* vertices = SDL_malloc(sizeof(SDL_Vertex) * nb_vertices);
  if (vertices == NULL) {
    return luaL_argerror(L, 2, "memory error");
  }

  // iter over all vertices
  int i = 0;
  lua_pushnil(L);
  while (lua_next(L, 2) != 0) {
    if (!lua_istable(L, -1)) {
      return luaL_argerror(
          L, 2, "expected vertices table to contain vertices objects");
    }

    SDL_FPoint pos = {0};
    SDL_Color color = {0};
    SDL_FPoint tex_coords = {0};

    if (lua_getfield(L, -1, "position") == LUA_TTABLE) {
      if (luaL_len(L, -1) != 2) {
        return luaL_argerror(
            L, 2,
            "invalid vertice's \"position\": must contain 2 components (XY)");
      }

      lua_geti(L, -1, 1);
      pos.x = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
      lua_geti(L, -1, 2);
      pos.y = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
    } else {
      return luaL_argerror(
          L, 2, "invalid vertice's \"position\": must be a table");
    }
    lua_pop(L, 1);

    int color_type = lua_getfield(L, -1, "color");
    if (color_type == LUA_TTABLE) {
      int len = luaL_len(L, -1);
      if (len < 3 || len > 4) {
        return luaL_argerror(
            L, 2,
            "invalid vertice's \"color\": must contain 3 or 4 components "
            "(RGBA)");
      }

      lua_geti(L, -1, 1);
      color.r = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
      lua_geti(L, -1, 2);
      color.g = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
      lua_geti(L, -1, 3);
      color.b = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
      lua_geti(L, -1, 4);
      color.a = luaL_optnumber(L, -1, 255);
      lua_pop(L, 1);
    } else if (color_type == LUA_TNIL) {
      // if color not specified by user, we take the current color
      if (get_color(c, &color) != 0) {
        return luaL_error(L, "cannot get color (%s)", SDL_GetError());
      }
    } else {
      return luaL_argerror(
          L, 2, "invalid vertice's \"color\": must be a table");
    }
    lua_pop(L, 1);

    int tex_coords_type = lua_getfield(L, -1, "tex_coord");
    if (tex_coords_type == LUA_TTABLE) {
      int len = luaL_len(L, -1);
      if (len != 2) {
        return luaL_argerror(
            L, 2,
            "invalid vertice's \"tex_coord\": must contain 2 components (XY)");
      }

      lua_geti(L, -1, 1);
      tex_coords.x = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
      lua_geti(L, -1, 2);
      tex_coords.y = luaL_optnumber(L, -1, 0);
      lua_pop(L, 1);
    } else if (tex_coords_type != LUA_TNIL) {
      return luaL_argerror(
          L, 2, "invalid vertice's \"tex_coord\": must be a table");
    }
    lua_pop(L, 1);

    SDL_Vertex* v = &vertices[i++];
    v->position = pos;
    v->color = color;
    v->tex_coord = tex_coords;

    lua_pop(L, 1);
  }

  // indices
  int nb_indices = 0;
  int* indices = NULL;

  if (lua_istable(L, 3)) {
    nb_indices = luaL_len(L, 3);
    if (nb_indices % 3 != 0 || nb_indices <= 0) {
      return luaL_argerror(
          L, 2,
          "invalid indices table: length must be divisible by 3 (and "
          "non-zero)");
    }

    // @TODO: test code, move memory allocation outside of this function
    indices = SDL_malloc(sizeof(int) * nb_indices);
    if (indices == NULL) {
      return luaL_argerror(L, 3, "memory error");
    }

    // iter over all vertices
    int i = 0;
    lua_pushnil(L);
    while (lua_next(L, 3) != 0) {
      if (!lua_isinteger(L, -1)) {
        return luaL_argerror(L, 3, "indices table must to contain integers");
      }

      indices[i++] = (int) lua_tointeger(L, -1);

      lua_pop(L, 1);
    }
  }

  if (draw_geometry(c, t, vertices, nb_vertices, indices, nb_indices)) {
    return luaL_error(L, "cannot draw geometry (%s)", SDL_GetError());
  }

  if (vertices != NULL) {
    SDL_free(vertices);
  }
  if (indices != NULL) {
    SDL_free(indices);
  }

  return 0;
} */

static int l_push_state(lua_State* L) {
  smgf* const c = get_smgf(L);

  if (sf_gr_push_state(c)) {
    return luaL_error(L, "cannot push state (%s)", SDL_GetError());
  }

  return 0;
}

static int l_pop_state(lua_State* L) {
  smgf* const c = get_smgf(L);

  if (sf_gr_pop_state(c)) {
    return luaL_error(L, "cannot pop state (%s)", SDL_GetError());
  }

  return 0;
}

static int l_reset_state(lua_State* L) {
  smgf* const c = get_smgf(L);

  if (sf_gr_reset_graphics_stack(c)) {
    return luaL_error(L, "cannot reset state (%s)", SDL_GetError());
  }

  return 0;
}

static int l_texture_get_dimensions(lua_State* L) {
  smgf* const c = get_smgf(L);
  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);

  int w = 0, h = 0;
  if (sf_gr_texture_get_dimensions(t, &w, &h)) {
    return luaL_error(L, "cannot get dimensions (%s)", SDL_GetError());
  }

  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  return 2;
}

static int l_texture_get_width(lua_State* L) {
  smgf* const c = get_smgf(L);
  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);

  int w = 0, h = 0;
  if (sf_gr_texture_get_dimensions(t, &w, &h)) {
    return luaL_error(L, "cannot get dimensions (%s)", SDL_GetError());
  }

  lua_pushinteger(L, w);
  return 1;
}

static int l_texture_get_height(lua_State* L) {
  smgf* const c = get_smgf(L);
  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);

  int w = 0, h = 0;
  if (sf_gr_texture_get_dimensions(t, &w, &h)) {
    return luaL_error(L, "cannot get dimensions (%s)", SDL_GetError());
  }

  lua_pushinteger(L, h);
  return 1;
}

static int l_texture_set_blend_mode(lua_State* L) {
  smgf* const c = get_smgf(L);
  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);

  static const int blend[] = {
      SDL_BLENDMODE_NONE, SDL_BLENDMODE_BLEND, SDL_BLENDMODE_ADD,
      SDL_BLENDMODE_MOD, SDL_BLENDMODE_MUL};
  static const char* const blend_names[] = {"none", "blend", "add",
                                            "mod",  "mul",   NULL};
  int op = luaL_checkoption(L, 2, "blend", blend_names);

  if (!sf_gr_texture_set_blend_mode(t, blend[op])) {
    return luaL_error(L, "cannot set blend mode (%s)", SDL_GetError());
  }

  return 0;
}

static int l_texture_get_blend_mode(lua_State* L) {
  smgf* const c = get_smgf(L);
  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);
  SDL_BlendMode b;
  if (!sf_gr_texture_get_blend_mode(t, &b)) {
    return luaL_error(L, "cannot get blend mode (%s)", SDL_GetError());
  }

  switch (b) {
  case SDL_BLENDMODE_NONE: lua_pushstring(L, "none"); break;
  case SDL_BLENDMODE_BLEND: lua_pushstring(L, "blend"); break;
  case SDL_BLENDMODE_ADD: lua_pushstring(L, "add"); break;
  case SDL_BLENDMODE_MOD: lua_pushstring(L, "mod"); break;
  case SDL_BLENDMODE_INVALID: lua_pushstring(L, "invalid"); break;
  case SDL_BLENDMODE_MUL: lua_pushstring(L, "mul"); break;
  default: return luaL_error(L, "unknown blend mode (%s)", SDL_GetError());
  }

  return 1;
}

static int l_texture_new(lua_State* L) {
  smgf* const c = get_smgf(L);

  if (lua_type(L, 1) == LUA_TSTRING) {
    // loading a texture from a file
    const char* filename = luaL_checkstring(L, 1);

    stexture* t = (stexture*) lua_newuserdata(L, sizeof(stexture));
    if (sf_gr_texture_new(c, t, filename)) {
      return luaL_error(
          L, "unable to open file %s (%s)", filename, SDL_GetError());
    }
  } else {
    // creating an empty texture
    int w = luaL_checknumber(L, 1);
    int h = luaL_checknumber(L, 2);

    stexture* t = (stexture*) lua_newuserdata(L, sizeof(stexture));
    if (sf_gr_texture_new_empty(c, t, w, h)) {
      return luaL_error(L, "unable to create texture (%s)", SDL_GetError());
    }
  }

  luaL_getmetatable(L, SMGF_TYPE_TEXTURE);
  lua_setmetatable(L, -2);

  return 1;
}

static int l_texture_del(lua_State* L) {
  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);
  sf_gr_texture_del(t);
  return 0;
}

static int l_texture_draw(lua_State* L) {
  smgf* const c = get_smgf(L);

  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);

  int qx = 0;
  int qy = 0;
  int qw = 0;
  int qh = 0;
  float x = 0;
  float y = 0;
  float sx = 1;
  float sy = sx;
  double r = 0;
  float ox = 0;
  float oy = 0;
  int flip = 0;

  // if second argument is table, we only draw part of the texture
  if (lua_istable(L, 2)) {
    if (luaL_len(L, 2) != 4) {
      return luaL_argerror(
          L, 2, "invalid quad (must have 4 components) (XYWH)");
    }
    lua_geti(L, 2, 1);
    lua_geti(L, 2, 2);
    lua_geti(L, 2, 3);
    lua_geti(L, 2, 4);
    qx = luaL_checknumber(L, -4);
    qy = luaL_checknumber(L, -3);
    qw = luaL_checknumber(L, -2);
    qh = luaL_checknumber(L, -1);
    lua_pop(L, 4);

    luaL_argcheck(L, qx >= 0, 2, "x component of quad must be positive");
    luaL_argcheck(L, qy >= 0, 2, "y component of quad must be positive");
    luaL_argcheck(L, qw >= 0, 2, "w component of quad must be positive");
    luaL_argcheck(L, qh >= 0, 2, "h component of quad must be positive");

    x = luaL_optnumber(L, 3, 0);
    y = luaL_optnumber(L, 4, 0);
    sx = luaL_optnumber(L, 5, 1);
    sy = luaL_optnumber(L, 6, sx);
    r = luaL_optnumber(L, 7, 0);
    ox = luaL_optnumber(L, 8, 0);
    oy = luaL_optnumber(L, 9, 0);
    // flip = luaL_optnumber(L, 10, 0);
    static const int flips[] = {
        SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL};
    static const char* const flip_names[] = {
        "none", "horizontal", "vertical", NULL};
    int op = luaL_checkoption(L, 10, "none", flip_names);
    flip = flips[op];
  } else {
    x = luaL_optnumber(L, 2, 0);
    y = luaL_optnumber(L, 3, 0);
    sx = luaL_optnumber(L, 4, 1);
    sy = luaL_optnumber(L, 5, sx);
    r = luaL_optnumber(L, 6, 0);
    ox = luaL_optnumber(L, 7, 0);
    oy = luaL_optnumber(L, 8, 0);
    // flip = luaL_optnumber(L, 9, 0);
    static const int flips[] = {
        SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL};
    static const char* const flip_names[] = {
        "none", "horizontal", "vertical", NULL};
    int op = luaL_checkoption(L, 9, "none", flip_names);
    flip = flips[op];
  }

  if (!sf_gr_texture_draw(
          c, t, x, y, qx, qy, qw, qh, sx, sy, r, ox, oy, flip)) {
    return luaL_error(c->L, "cannot draw texture (%s)", SDL_GetError());
  }

  return 0;
}

static int l_set_target(lua_State* L) {
  smgf* const c = get_smgf(L);

  // unref'ing the current target if ref'ed
  if (c->curstate->target_luaref != 0) {
    luaL_unref(L, LUA_REGISTRYINDEX, c->curstate->target_luaref);
    c->curstate->target_luaref = 0;
  }

  stexture* t = (stexture*) luaL_testudata(L, 1, SMGF_TYPE_TEXTURE);

  if (sf_gr_set_target(c, t)) {
    if (t != NULL) {
      // we keep a reference to this texture so that we can return it when
      // user calls "get_target"
      c->curstate->target_luaref = luaL_ref(L, LUA_REGISTRYINDEX);
    }
  } else {
    return luaL_error(L, "cannot set target (%s)", SDL_GetError());
  }

  return 0;
}

static int l_get_target(lua_State* L) {
  smgf* const c = get_smgf(L);

  if (c->curstate->target_luaref != 0) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, c->curstate->target_luaref);
  } else {
    lua_pushnil(L);
  }

  return 1;
}

static int l_texture_save(lua_State* L) {
  smgf* const c = get_smgf(L);

  stexture* t = (stexture*) luaL_checkudata(L, 1, SMGF_TYPE_TEXTURE);
  const char* filename = luaL_checkstring(L, 2);

  if (sf_gr_texture_save(c, t, filename)) {
    return luaL_error(L, "cannot save texture (%s)", SDL_GetError());
  }

  return 0;
}

static int l_translate(lua_State* L) {
  smgf* const c = get_smgf(L);

  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);

  int tx = 0, ty = 0;
  sf_gr_get_translation(c, &tx, &ty);

  sf_gr_set_translation(c, tx + x, ty + y);

  return 0;
}

static int l_get_translation(lua_State* L) {
  smgf* const c = get_smgf(L);

  int tx = 0, ty = 0;
  sf_gr_get_translation(c, &tx, &ty);

  lua_pushinteger(L, tx);
  lua_pushinteger(L, ty);
  return 2;
}

static int l_set_translation(lua_State* L) {
  smgf* const c = get_smgf(L);

  int tx = luaL_checknumber(L, 1);
  int ty = luaL_checknumber(L, 2);

  sf_gr_set_translation(c, tx, ty);

  return 0;
}

/* static int l_rotate(lua_State* L) {
    smgf* const c = get_smgf(L);

    // reset rotation if no argument
    if (lua_gettop(L) == 0) {
        reset_rotation(c);
        return 0;
    }

    float r = luaL_optnumber(L, 1, 0);
    rotate(c, r);

    return 0;
} */

/* static int l_clip(lua_State* L) {
  smgf* const c = get_smgf(L);

  // reset clipping if no arg
  if (lua_gettop(L) == 0) {
    clip(c, 0, 0, 0, 0);
    return 0;
  }

  int x = luaL_optnumber(L, 1, 0);
  int y = luaL_optnumber(L, 2, 0);
  int w = luaL_optnumber(L, 3, 0);
  int h = luaL_optnumber(L, 4, 0);

  if (clip(c, x, y, w, h)) {
    return luaL_error(L, "cannot clip (%s)", SDL_GetError());
  }
  return 0;
} */

static int l_print_color(lua_State* L) {
  smgf* const c = get_smgf(L);

  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  int color = luaL_checknumber(L, 3);
  luaL_argcheck(
      L, color >= 0x00 && color <= 0xFF, 3, "Color must be between 0 and 255");
  const char* str = luaL_checkstring(L, 4);

  sf_gr_print_color(c, x, y, (Uint8) color, str);
  return 0;
}

static int l_print(lua_State* L) {
  smgf* const c = get_smgf(L);

  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  const char* str = luaL_checkstring(L, 3);
  SDL_Color bg_color = {.r = 0, .g = 0, .b = 0, .a = 0};
  if (lua_istable(L, 4)) {
    if (lua_get_color(L, 4, &bg_color) != 0) {
      return 0;
    }
  } else {
    // no background color specified = transparent background
    bg_color.r = 0;
    bg_color.g = 0;
    bg_color.b = 0;
    bg_color.a = 0;
  }

  sf_gr_print(c, x, y, str, bg_color);
  return 0;
}

static int l_screenshot(lua_State* L) {
  smgf* const c = get_smgf(L);

  const char* filename = luaL_checkstring(L, 1);

  if (sf_gr_texture_save(c, c->screen_texture, filename)) {
    return luaL_error(L, "cannot screenshot (%s)", SDL_GetError());
  }

  return 0;
}

static const struct luaL_Reg smgf_graphics[] = {
    {"set_target", l_set_target},
    {"get_target", l_get_target},
    {"set_color", l_set_color},
    {"get_color", l_get_color},
    {"clear", l_clear},
    {"set_blend_mode", l_set_blend_mode},
    {"get_blend_mode", l_get_blend_mode},
    {"push_state", l_push_state},
    {"pop_state", l_pop_state},
    {"reset_state", l_reset_state},
    {"translate", l_translate},
    {"set_translation", l_set_translation},
    {"get_translation", l_get_translation},
    // {"clip", l_clip},
    {"screenshot", l_screenshot},

    {"get_point", l_get_point}, // for now, only used for test cases.
    {"draw_point", l_draw_point},
    {"draw_line", l_draw_line},
    {"draw_rect", l_draw_rect},
    {"draw_rectfill", l_draw_rectfill},
    // {"draw_geometry", l_draw_geometry},
    {"print_color", l_print_color},
    {"print", l_print},

    // texture
    {"new", l_texture_new},
    // {"texture_del", l_texture_del}, // see #25
    // {"texture_draw", l_texture_draw},
    {"draw", l_texture_draw},

    {NULL, NULL}};

static const struct luaL_Reg texture_func[] = {
    {"get_dimensions", l_texture_get_dimensions},
    {"get_width", l_texture_get_width},
    {"get_height", l_texture_get_height},
    {"set_blend_mode", l_texture_set_blend_mode},
    {"get_blend_mode", l_texture_get_blend_mode},
    {"save", l_texture_save},
    {"draw", l_texture_draw},
    {NULL, NULL}};

void init_graphics(lua_State* L) {
  // @NOTE: we specify the number of functions of each module, so that
  // Lua can preallocate memory (see lua_createtable docs)
  size_t n = SDL_arraysize(smgf_graphics);
  lua_createtable(L, 0, n);
  luaL_setfuncs(L, smgf_graphics, 0);
  lua_setfield(L, -2, "graphics");

  // add texture type
  luaL_newmetatable(L, SMGF_TYPE_TEXTURE);
  lua_pushcfunction(L, l_texture_del);
  lua_setfield(L, -2, "__gc");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, texture_func, 0);
  lua_pop(L, 1);
}
