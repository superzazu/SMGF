#include "../api.h"

int sf_gr_set_target(smgf* const c, stexture* const t) {
  stexture* const target = t == NULL ? c->screen_texture : t;
  int result = SDL_SetRenderTarget(c->renderer, target->tex);

  if (result == 0) {
    c->curstate->target = t;
  }

  return result;
}

// int sf_gr_get_target(smgf* const c, stexture** const t) {
//   if (c->curstate->target == NULL) {
//     *t = c->screen_texture;
//   } else {
//     *t = c->curstate->target;
//   }
//   return 0;
// }

int sf_gr_set_color(smgf* const c, SDL_Color* color) {
  c->curstate->r = color->r;
  c->curstate->g = color->g;
  c->curstate->b = color->b;
  c->curstate->a = color->a;
  return 0;
}

int sf_gr_get_color(smgf* const c, SDL_Color* color) {
  color->r = c->curstate->r;
  color->g = c->curstate->g;
  color->b = c->curstate->b;
  color->a = c->curstate->a;
  return 0;
}

int sf_gr_clear(smgf* const c, SDL_Color* color) {
  if (SDL_SetRenderDrawColor(
          c->renderer, color->r, color->g, color->b, color->a) != 0) {
    return -1;
  }

  if (SDL_RenderClear(c->renderer) != 0) {
    return -1;
  }

  return 0;
}

int sf_gr_set_blend_mode(smgf* const c, SDL_BlendMode b) {
  return SDL_SetRenderDrawBlendMode(c->renderer, b);
}

int sf_gr_get_blend_mode(smgf* const c, SDL_BlendMode* b) {
  return SDL_GetRenderDrawBlendMode(c->renderer, b);
}

static int sf_gr_reset_state(smgf* const c, smgf_graphic_state* const s) {
  if (s->target_luaref != 0) {
    luaL_unref(c->L, LUA_REGISTRYINDEX, s->target_luaref);
  }

  s->r = 255;
  s->g = 255;
  s->b = 255;
  s->a = 255;

  s->x = 0;
  s->y = 0;

  s->target = NULL;
  s->target_luaref = 0;
  return 0;
}

int sf_gr_push_state(smgf* const c) {
  if (c->gstates_ptr + 1 >= MAX_NB_GSTATES) {
    SDL_SetError(
        "error: cannot store more than %d graphic states", MAX_NB_GSTATES);
    return -1;
  }

  c->gstates_ptr += 1;
  c->curstate = &c->gstates[c->gstates_ptr];
  sf_gr_reset_state(c, c->curstate);
  sf_gr_set_target(c, NULL);

  return 0;
}

int sf_gr_pop_state(smgf* const c) {
  sf_gr_reset_state(c, c->curstate);
  c->gstates_ptr -= 1;
  if (c->gstates_ptr < 0) {
    c->gstates_ptr = 0;
  }
  c->curstate = &c->gstates[c->gstates_ptr];
  sf_gr_set_target(c, c->curstate->target);

  return 0;
}

int sf_gr_reset_graphics_stack(smgf* const c) {
  // popping all states, including the first one which has the result of
  // resetting it
  for (int i = c->gstates_ptr; i >= 0; i--) {
    sf_gr_pop_state(c);
  }
  return 0;
}

void sf_gr_set_translation(smgf* const c, int x, int y) {
  c->curstate->x = x;
  c->curstate->y = y;
}

void sf_gr_get_translation(smgf* const c, int* x, int* y) {
  *x = c->curstate->x;
  *y = c->curstate->y;
}

// warning: this is a very slow operation, it should only be used for testing
// purposes.
int sf_gr_get_point(smgf* const c, SDL_Color* color, int x, int y) {
  SDL_Rect pixel_rect = {c->curstate->x + x, c->curstate->y + y, 1, 1};
  return SDL_RenderReadPixels(
      c->renderer, &pixel_rect, SDL_PIXELFORMAT_RGB24, color, 32);
}

int sf_gr_draw_point(smgf* const c, float x, float y) {
  if (SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return -1;
  }

  return SDL_RenderDrawPointF(
      c->renderer, c->curstate->x + x, c->curstate->y + y);
}

int sf_gr_draw_line(smgf* const c, float x1, float y1, float x2, float y2) {
  if (SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return -1;
  }

  return SDL_RenderDrawLineF(
      c->renderer, c->curstate->x + x1, c->curstate->y + y1,
      c->curstate->x + x2, c->curstate->y + y2);
}

int sf_gr_draw_rect(smgf* const c, float x, float y, float w, float h) {
  if (SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return -1;
  }

  SDL_FRect r = {c->curstate->x + x, c->curstate->y + y, w, h};
  return SDL_RenderDrawRectF(c->renderer, &r);
}

int sf_gr_draw_rectfill(smgf* const c, float x, float y, float w, float h) {
  if (SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return -1;
  }

  SDL_FRect r = {c->curstate->x + x, c->curstate->y + y, w, h};
  return SDL_RenderFillRectF(c->renderer, &r);
}

/* int sf_gr_draw_geometry(
    smgf* const c, stexture* const t, SDL_Vertex* vertices, int nb_vertices,
    const int* indices, int nb_indices) {
  return SDL_RenderGeometry(
      c->renderer, t ? t->tex : NULL, vertices, nb_vertices, indices,
      nb_indices);
} */

int sf_gr_print_color(
    smgf* const c, int x, int y, Uint8 color, const char* str) {
  return DBGP_ColorPrint(&c->font, c->renderer, x, y, color, str);
}

int sf_gr_print(
    smgf* const c, int x, int y, const char* str, SDL_Color bg_color) {
  SDL_Color fg_color = {0, 0, 0, 255};
  sf_gr_get_color(c, &fg_color);
  return DBGP_Print(&c->font, c->renderer, x, y, bg_color, fg_color, str);
}

int sf_gr_texture_new(smgf* const c, stexture* const t, const char* filename) {
  SDL_RWops* texture_file = PHYSFSRWOPS_openRead(filename);
  if (texture_file == NULL) {
    return -1;
  }
  t->tex = STBIMG_LoadTexture_RW(c->renderer, texture_file, 0);
  SDL_RWclose(texture_file);

  t->width = 0;
  t->height = 0;
  t->format = 0;

  if (t->tex == NULL) {
    return -1;
  }

  sf_gr_texture_set_blend_mode(t, SDL_BLENDMODE_BLEND);

  return SDL_QueryTexture(t->tex, &t->format, NULL, &t->width, &t->height);
}

int sf_gr_texture_new_empty(smgf* const c, stexture* const t, int w, int h) {
  t->tex = SDL_CreateTexture(
      c->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
  t->width = w;
  t->height = h;
  t->format = 0;

  if (t->tex == NULL) {
    return -1;
  }

  sf_gr_texture_set_blend_mode(t, SDL_BLENDMODE_BLEND);
  return 0;
}

void sf_gr_texture_del(stexture* const t) {
  if (t->tex != NULL) {
    SDL_DestroyTexture(t->tex);
    t->tex = NULL;
  }
}

int sf_gr_texture_draw(
    smgf* const c, stexture* const t, float x, float y, int qx, int qy, int qw,
    int qh, float sx, float sy, double r, float ox, float oy, int flip) {
  if (!qx && !qy && !qw && !qh) {
    qw = t->width;
    qh = t->height;
  }

  SDL_Rect srcrect = {qx, qy, qw, qh};
  SDL_FRect dstrect = {
      c->curstate->x + x, c->curstate->y + y, qw * sx, qh * sy};
  SDL_FPoint center = {ox, oy};

  SDL_SetTextureColorMod(
      t->tex, c->curstate->r, c->curstate->g, c->curstate->b);
  SDL_SetTextureAlphaMod(t->tex, c->curstate->a);

  return SDL_RenderCopyExF(
      c->renderer, t->tex, &srcrect, &dstrect, r, &center, flip);
}

int sf_gr_texture_save(smgf* const c, stexture* const t, const char* filename) {
  Uint32 format = 0;
  int depth = 0, width = 0, height = 0;
  if (SDL_QueryTexture(t->tex, &format, NULL, &width, &height) != 0) {
    return 1;
  }
  depth = SDL_BITSPERPIXEL(format);

  SDL_Surface* s =
      SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);
  if (s == NULL) {
    return 1;
  }

  // copying from renderer to surface
  SDL_SetRenderTarget(c->renderer, t->tex);

  if (SDL_RenderReadPixels(
          c->renderer, NULL, s->format->format, s->pixels, s->pitch) != 0) {
    // reading pixels is not supported
    return 1;
  }

  if (c->curstate->target == NULL) {
    SDL_SetRenderTarget(c->renderer, c->screen_texture->tex);
  } else {
    SDL_SetRenderTarget(c->renderer, c->curstate->target->tex);
  }

  // writing to file
  SDL_RWops* f = PHYSFSRWOPS_openWrite(filename);
  if (f == NULL) {
    return 1;
  }
  SDL_SaveBMP_RW(s, f, 0);
  SDL_FreeSurface(s);
  SDL_RWclose(f);

  return 0;
}

int sf_gr_texture_get_dimensions(stexture* const t, int* w, int* h) {
  *w = t->width;
  *h = t->height;
  return 0;
}

int sf_gr_texture_set_blend_mode(stexture* const t, SDL_BlendMode b) {
  return SDL_SetTextureBlendMode(t->tex, b);
}

int sf_gr_texture_get_blend_mode(stexture* const t, SDL_BlendMode* b) {
  return SDL_GetTextureBlendMode(t->tex, b);
}
