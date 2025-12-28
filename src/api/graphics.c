// #include <SDL3_image/SDL_image.h>
#include "../api.h"

bool sf_gr_set_target(smgf* const c, stexture* const t) {
  stexture* const target = t == NULL ? c->screen_texture : t;
  bool result = SDL_SetRenderTarget(c->renderer, target->tex);

  if (result) {
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

bool sf_gr_set_color(smgf* const c, SDL_Color* color) {
  c->curstate->r = color->r;
  c->curstate->g = color->g;
  c->curstate->b = color->b;
  c->curstate->a = color->a;
  return true;
}

bool sf_gr_get_color(smgf* const c, SDL_Color* color) {
  color->r = c->curstate->r;
  color->g = c->curstate->g;
  color->b = c->curstate->b;
  color->a = c->curstate->a;
  return true;
}

bool sf_gr_clear(smgf* const c, SDL_Color* color) {
  if (!SDL_SetRenderDrawColor(
          c->renderer, color->r, color->g, color->b, color->a)) {
    return false;
  }

  return SDL_RenderClear(c->renderer);
}

bool sf_gr_set_blend_mode(smgf* const c, SDL_BlendMode b) {
  return SDL_SetRenderDrawBlendMode(c->renderer, b);
}

bool sf_gr_get_blend_mode(smgf* const c, SDL_BlendMode* b) {
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

  // can't read outside viewport, but do not raise an error:
  SDL_Rect real_rect;
  if (!SDL_GetRenderViewport(c->renderer, &real_rect)) {
    return -1;
  }
  if (!SDL_GetRectIntersection(&pixel_rect, &real_rect, &real_rect)) {
    color->r = 0;
    color->g = 0;
    color->b = 0;
    color->a = 0;
    return 0;
  }

  SDL_Surface* s = SDL_RenderReadPixels(c->renderer, &pixel_rect);

  if (s == NULL) {
    return -1;
  }

  SDL_Surface* converted = SDL_ConvertSurface(s, SDL_PIXELFORMAT_RGBA32);
  SDL_DestroySurface(s);
  if (converted == NULL) {
    SDL_Log("SDL_ConvertSurfaceFormat failed: %s", SDL_GetError());
    return -1;
  }

  Uint8* px = (Uint8*) converted->pixels;
  color->r = px[0];
  color->g = px[1];
  color->b = px[2];
  color->a = px[3];

  SDL_DestroySurface(converted);
  return 0;
}

bool sf_gr_draw_point(smgf* const c, float x, float y) {
  if (!SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return false;
  }

  return SDL_RenderPoint(c->renderer, c->curstate->x + x, c->curstate->y + y);
}

bool sf_gr_draw_line(smgf* const c, float x1, float y1, float x2, float y2) {
  if (!SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return false;
  }

  return SDL_RenderLine(
      c->renderer, c->curstate->x + x1, c->curstate->y + y1,
      c->curstate->x + x2, c->curstate->y + y2);
}

bool sf_gr_draw_rect(smgf* const c, float x, float y, float w, float h) {
  if (!SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return false;
  }

  SDL_FRect r = {c->curstate->x + x, c->curstate->y + y, w, h};
  return SDL_RenderRect(c->renderer, &r);
}

bool sf_gr_draw_rectfill(smgf* const c, float x, float y, float w, float h) {
  if (!SDL_SetRenderDrawColor(
          c->renderer, c->curstate->r, c->curstate->g, c->curstate->b,
          c->curstate->a) != 0) {
    return false;
  }

  SDL_FRect r = {c->curstate->x + x, c->curstate->y + y, w, h};
  return SDL_RenderFillRect(c->renderer, &r);
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
  SDL_IOStream* texture_file = PHYSFSSDL3_openRead(filename);
  if (texture_file == NULL) {
    return -1;
  }
  t->tex = IMG_LoadTexture_IO(c->renderer, texture_file, false);
  SDL_CloseIO(texture_file);

  t->width = 0;
  t->height = 0;
  t->format = 0;

  if (t->tex == NULL) {
    return -1;
  }

  sf_gr_texture_set_blend_mode(t, SDL_BLENDMODE_BLEND);

  SDL_PropertiesID props = SDL_GetTextureProperties(t->tex);
  if (props == 0) {
    return 1;
  }
  t->width = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
  t->height = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);
  t->format = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, 0);

  return 0;
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
  SDL_SetTextureScaleMode(t->tex, SDL_SCALEMODE_NEAREST);
  return 0;
}

void sf_gr_texture_del(stexture* const t) {
  if (t->tex != NULL) {
    SDL_DestroyTexture(t->tex);
    t->tex = NULL;
  }
}

bool sf_gr_texture_draw(
    smgf* const c, stexture* const t, float x, float y, int qx, int qy, int qw,
    int qh, float sx, float sy, double r, float ox, float oy, int flip) {
  if (!qx && !qy && !qw && !qh) {
    qw = t->width;
    qh = t->height;
  }

  SDL_FRect srcrect = {qx, qy, qw, qh};
  SDL_FRect dstrect = {
      c->curstate->x + x, c->curstate->y + y, qw * sx, qh * sy};
  SDL_FPoint center = {ox, oy};

  SDL_SetTextureColorMod(
      t->tex, c->curstate->r, c->curstate->g, c->curstate->b);
  SDL_SetTextureAlphaMod(t->tex, c->curstate->a);

  return SDL_RenderTextureRotated(
      c->renderer, t->tex, &srcrect, &dstrect, r, &center, flip);
}

int sf_gr_texture_save(smgf* const c, stexture* const t, const char* filename) {
  // Uint32 format = 0;
  // int width = 0, height = 0;

  // SDL_PropertiesID props = SDL_GetTextureProperties(t->tex);
  // if (props == 0) {
  //   return 1;
  // }
  // width = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
  // height = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);
  // format = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, 0);

  // copying from renderer to surface
  SDL_SetRenderTarget(c->renderer, t->tex);

  // @TODO: LockTexture and copy pixels directly instead of calling
  // RenderReadPixels?
  SDL_Surface* s = SDL_RenderReadPixels(c->renderer, NULL);

  if (c->curstate->target == NULL) {
    SDL_SetRenderTarget(c->renderer, c->screen_texture->tex);
  } else {
    SDL_SetRenderTarget(c->renderer, c->curstate->target->tex);
  }

  if (s == NULL) {
    // reading pixels is not supported
    return 1;
  }

  // writing to file
  SDL_IOStream* f = PHYSFSSDL3_openWrite(filename);
  if (f == NULL) {
    SDL_DestroySurface(s);
    return 1;
  }
  if (!IMG_SavePNG_IO(s, f, false) /* SDL_SaveBMP_IO(s, f, false) */) {
    SDL_DestroySurface(s);
    SDL_CloseIO(f);
    return 1;
  }
  SDL_DestroySurface(s);
  SDL_CloseIO(f);

  return 0;
}

int sf_gr_texture_get_dimensions(stexture* const t, int* w, int* h) {
  *w = t->width;
  *h = t->height;
  return 0;
}

bool sf_gr_texture_set_blend_mode(stexture* const t, SDL_BlendMode b) {
  return SDL_SetTextureBlendMode(t->tex, b);
}

bool sf_gr_texture_get_blend_mode(stexture* const t, SDL_BlendMode* b) {
  return SDL_GetTextureBlendMode(t->tex, b);
}
