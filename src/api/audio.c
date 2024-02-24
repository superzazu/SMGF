#include "../api.h"

void sf_au_set_master_gain(float vol) {
  NMIX_SetMasterGain(vol);
}

float sf_au_get_master_gain(void) {
  return NMIX_GetMasterGain();
}

void sf_au_set_master_pause(int paused) {
  NMIX_PausePlayback((SDL_bool) paused);
}

int sf_au_sound_new(ssound* const s, const char* filename, int preloaded) {
  s->filename = filename;
  s->rw = NULL;
  s->snd = NULL;

  s->rw = PHYSFSRWOPS_openRead(filename);
  if (s->rw == NULL) {
    return -1;
  }

  const char* ext = SDL_strrchr(filename, '.');
  if (ext != NULL) {
    ext += 1; // we remove the '.' character
  }

  s->snd = NMIX_NewFileSource(s->rw, ext, preloaded);
  if (s->snd == NULL) {
    return -1;
  }

  return 0;
}

void sf_au_sound_del(ssound* const s) {
  if (s->snd != NULL) {
    NMIX_Pause(s->snd->source);
    // note: the RWops is automatically closed by SDL_nmix
    NMIX_FreeFileSource(s->snd);
    s->snd = NULL;
  }
}

int sf_au_sound_get_duration(ssound* const s) {
  return NMIX_GetDuration(s->snd);
}

int sf_au_sound_play(ssound* const s) {
  return NMIX_Play(s->snd->source);
}

void sf_au_sound_pause(ssound* const s) {
  NMIX_Pause(s->snd->source);
}

int sf_au_sound_rewind(ssound* const s) {
  return NMIX_Rewind(s->snd);
}

int sf_au_sound_seek(ssound* const s, int ms) {
  return NMIX_Seek(s->snd, ms);
}

bool sf_au_sound_is_playing(ssound* const s) {
  return NMIX_IsPlaying(s->snd->source);
}

bool sf_au_sound_is_predecoded(ssound* const s) {
  return s->snd->predecoded;
}

float sf_au_sound_get_pan(ssound* const s) {
  return NMIX_GetPan(s->snd->source);
}

void sf_au_sound_set_pan(ssound* const s, float pan) {
  NMIX_SetPan(s->snd->source, pan);
}

float sf_au_sound_get_gain(ssound* const s) {
  return NMIX_GetGain(s->snd->source);
}

void sf_au_sound_set_gain(ssound* const s, float vol) {
  NMIX_SetGain(s->snd->source, vol);
}

bool sf_au_sound_get_loop(ssound* const s) {
  return NMIX_GetLoop(s->snd);
}

void sf_au_sound_set_loop(ssound* const s, bool loop) {
  NMIX_SetLoop(s->snd, loop);
}
