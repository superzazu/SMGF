#ifndef SMGF_API_H
#define SMGF_API_H

#include <physfssdl3.h>
#include "SDL_DBGP.h"

#include "smgf.h"

// Functions in API are named this way: `sf.<module (2 letters)>.<function>`:
// - sf_gr = SmgF GRaphics
// - sf_sy = SmgF SYstem
// - sf_kb = SmgF KeyBoard
// - sf_ms = SmgF MouSe
// - sf_au = SmgF AUdio
// - sf_io = SmgF I/O
// - sf_gp = SmgF GamePad

// graphics
bool sf_gr_set_target(smgf* const c, stexture* const t);
// int sf_gr_get_target(smgf* const c, stexture** const t);
bool sf_gr_set_color(smgf* const c, SDL_Color* color);
bool sf_gr_get_color(smgf* const c, SDL_Color* color);
bool sf_gr_clear(smgf* const c, SDL_Color* color);
bool sf_gr_set_blend_mode(smgf* const c, SDL_BlendMode b);
bool sf_gr_get_blend_mode(smgf* const c, SDL_BlendMode* b);
int sf_gr_push_state(smgf* const c);
int sf_gr_pop_state(smgf* const c);
int sf_gr_reset_graphics_stack(smgf* const c);
void sf_gr_set_translation(smgf* const c, int x, int y);
void sf_gr_get_translation(smgf* const c, int* x, int* y);
int sf_gr_clip(smgf* const c, int x, int y, int w, int h);

int sf_gr_get_point(smgf* const c, SDL_Color* color, int x, int y);
bool sf_gr_draw_point(smgf* const c, float x, float y);
bool sf_gr_draw_line(smgf* const c, float x1, float y1, float x2, float y2);
bool sf_gr_draw_rect(smgf* const c, float x, float y, float w, float h);
bool sf_gr_draw_rectfill(smgf* const c, float x, float y, float w, float h);
/* int sf_gr_draw_geometry(
    smgf* const c, stexture* const t, SDL_Vertex* vertices, int nb_vertices,
    const int* indices, int nb_indices); */
int sf_gr_print_color(
    smgf* const c, int x, int y, Uint8 color, const char* str);
int sf_gr_print(
    smgf* const c, int x, int y, const char* str, SDL_Color bg_color);

// texture functions
int sf_gr_texture_new(smgf* const c, stexture* const t, const char* filename);
int sf_gr_texture_new_empty(smgf* const c, stexture* const t, int w, int h);
void sf_gr_texture_del(stexture* const t);
bool sf_gr_texture_draw(
    smgf* const c, stexture* const t, float x, float y, int qx, int qy, int qw,
    int qh, float sx, float sy, double r, float ox, float oy, int flip);
int sf_gr_texture_get_dimensions(stexture* const t, int* w, int* h);
bool sf_gr_texture_set_blend_mode(stexture* const t, SDL_BlendMode b);
bool sf_gr_texture_get_blend_mode(stexture* const t, SDL_BlendMode* b);
int sf_gr_texture_save(smgf* const c, stexture* const t, const char* filename);

// system
void sf_sy_quit(smgf* const c);
void sf_sy_get_platform(smgf* const c, char const** platform);
void sf_sy_sleep(smgf* const c, float seconds);
void sf_sy_get_dt(smgf* const c, float* dt);
void sf_sy_get_fullscreen(smgf* const c, bool* fullscreen);
int sf_sy_set_fullscreen(smgf* const c, bool fullscreen);
int sf_sy_open_url(smgf* const c, const char* url);
// void sf_sy_log(smgf* const c, const char* str);
int sf_sy_set_clipboard(smgf* const c, const char* text);
int sf_sy_get_clipboard(smgf* const c, char** text);
int sf_sy_set_identity(smgf* const c, const char* org, const char* app);
int sf_sy_get_identity(smgf* const c, const char** org, const char** app);
const char* sf_sy_get_write_dir(smgf* const c);
void sf_sy_get_window_size(smgf* const c, int* w, int* h);
void sf_sy_set_window_size(smgf* const c, int w, int h);
int sf_sy_get_fps(smgf* const c);
void sf_sy_set_fps(smgf* const c, int fps);
int sf_sy_get_zoom(smgf* const c);
void sf_sy_set_zoom(smgf* const c, int zoom);
bool sf_sy_get_cursor_visible(smgf* const c);
void sf_sy_set_cursor_visible(smgf* const c, bool visible);
const char* sf_sy_get_window_title(smgf* const c);
void sf_sy_set_window_title(smgf* const c, const char* title);
int sf_sy_show_message(
    smgf* const c, Uint32 flags, const char* title, const char* message);
char* sf_sy_iconv(
    smgf* const c, const char* to, const char* from, const char* str);
SDL_Locale** sf_sy_get_preferred_locales(smgf* const c);
char* sf_sy_get_version(smgf* const c);

// keyboard
int sf_kb_is_down(smgf* const c, const char* key, bool* is_down);
int sf_kb_is_up(smgf* const c, const char* key, bool* is_up);
void sf_kb_set_textinput(smgf* const c, bool enable);
bool sf_kb_get_textinput(smgf* const c);

// mouse
void sf_ms_is_down(int button, bool* is_down);
void sf_ms_get_pos(smgf* const c, float* x, float* y);

// audio
void sf_au_set_master_gain(smgf* const c, float vol);
float sf_au_get_master_gain(smgf* const c);
void sf_au_set_master_pause(smgf* const c, int paused);

int sf_au_sound_new(
    smgf* const c, ssound* const s, const char* filename, int predecoded);
void sf_au_sound_del(smgf* const c, ssound* const s);
int sf_au_sound_get_duration(smgf* const c, ssound* const s);
int sf_au_sound_play(smgf* const c, ssound* const s, bool loop);
void sf_au_sound_pause(smgf* const c, ssound* const s);
void sf_au_sound_stop(smgf* const c, ssound* const s, Sint64 fade_out_frames);
bool sf_au_sound_rewind(smgf* const c, ssound* const s);
bool sf_au_sound_seek(smgf* const c, ssound* const s, int ms);
bool sf_au_sound_is_playing(smgf* const c, ssound* const s);
bool sf_au_sound_is_predecoded(smgf* const c, ssound* const s);
bool sf_au_sound_get_pan(smgf* const c, ssound* const s, float* pan);
bool sf_au_sound_set_pan(smgf* const c, ssound* const s, float pan);
float sf_au_sound_get_gain(smgf* const c, ssound* const s);
bool sf_au_sound_set_gain(smgf* const c, ssound* const s, float gain);
bool sf_au_sound_get_loop(smgf* const c, ssound* const s);

// io
int sf_io_new(sfile* const f);
int sf_io_del(sfile* const f);
int sf_io_open(sfile* const f, const char* filename, char mode);
int sf_io_close(sfile* const f);
Sint64 sf_io_seek(sfile* const f, Sint64 offset, int whence);
Sint64 sf_io_tell(sfile* const f);
Sint64 sf_io_rewind(sfile* const f);
Sint64 sf_io_size(sfile* const f);
int sf_io_read(sfile* const f, void* ptr, size_t size);
int sf_io_write(sfile* const f, const void* ptr, size_t size);
int sf_io_strchr(sfile* const f, Sint64* pos, char needle);
int sf_io_mkdir(const char* dirname);
int sf_io_delete(const char* filename);
PHYSFS_FileType sf_io_get_filetype(const char* filename);
bool sf_io_exists(const char* filename);
bool sf_io_flush(sfile* const f);

// gamepad
bool sf_gp_is_open(int player_index);
bool sf_gp_is_down(int player_index, SDL_GamepadButton button);
float sf_gp_get_axis(int player_index, SDL_GamepadAxis axis);
int sf_gp_rumble(
    int player_index, float duration_s, float lintensity, float rintensity);
const char* sf_gp_get_name(int player_index);

#endif // SMGF_API_H
