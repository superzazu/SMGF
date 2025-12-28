#include "../api.h"

static int mixer = 0;

void sf_au_set_master_gain(smgf* const c, float vol) {
  MIX_SetMasterGain(c->mixer, vol);
}

float sf_au_get_master_gain(smgf* const c) {
  return MIX_GetMasterGain(c->mixer);
}

void sf_au_set_master_pause(smgf* const c, int paused) {
  if (paused) {
    MIX_PauseAllTracks(c->mixer);
  } else {
    MIX_ResumeAllTracks(c->mixer);
  }
}

int sf_au_sound_new(
    smgf* const c, ssound* const s, const char* filename, int predecoded) {
  s->filename = filename;
  s->predecoded = predecoded;
  s->rw = NULL;
  s->snd = NULL;
  s->track = NULL;

  s->rw = PHYSFSSDL3_openRead(filename);
  if (s->rw == NULL) {
    return -1;
  }

  s->snd = MIX_LoadAudio_IO(c->mixer, s->rw, predecoded, false);
  if (s->snd == NULL) {
    return -1;
  }

  s->track = MIX_CreateTrack(c->mixer);
  if (s->track == NULL) {
    return -1;
  }

  if (!MIX_SetTrackAudio(s->track, s->snd)) {
    return -1;
  }

  return 0;
}

void sf_au_sound_del(smgf* const c, ssound* const s) {
  if (s->track != NULL) {
    MIX_DestroyTrack(s->track);
    s->track = NULL;
  }
  if (s->snd != NULL) {
    MIX_DestroyAudio(s->snd);
    SDL_CloseIO(s->rw);
    s->snd = NULL;
    s->rw = NULL;
  }
}

int sf_au_sound_get_duration(smgf* const c, ssound* const s) {
  Sint64 duration = MIX_GetAudioDuration(s->snd);
  return MIX_AudioFramesToMS(s->snd, duration);
}

int sf_au_sound_play(smgf* const c, ssound* const s, bool loop) {
  if (s->track == NULL) {
    return -1;
  }

  SDL_PropertiesID options = SDL_CreateProperties();
  if (loop) {
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
  }

  if (!MIX_PlayTrack(s->track, options)) {
    SDL_DestroyProperties(options);
    return -1;
  }

  SDL_DestroyProperties(options);

  return 0;
}

void sf_au_sound_pause(smgf* const c, ssound* const s) {
  MIX_PauseTrack(s->track);
}

void sf_au_sound_stop(smgf* const c, ssound* const s, Sint64 fade_out_frames) {
  MIX_StopTrack(s->track, fade_out_frames);
}

bool sf_au_sound_rewind(smgf* const c, ssound* const s) {
  return MIX_SetTrackPlaybackPosition(s->track, 0);
}

bool sf_au_sound_seek(smgf* const c, ssound* const s, int ms) {
  return MIX_SetTrackPlaybackPosition(
      s->track, MIX_TrackMSToFrames(s->track, ms));
}

bool sf_au_sound_is_playing(smgf* const c, ssound* const s) {
  return MIX_TrackPlaying(s->track);
}

bool sf_au_sound_is_predecoded(smgf* const c, ssound* const s) {
  return s->predecoded;
}

bool sf_au_sound_get_pan(smgf* const c, ssound* const s, float* pan) {
  MIX_Point3D pos = {0, 0, 0};
  if (!MIX_GetTrack3DPosition(s->track, &pos)) {
    return false;
  }
  *pan = pos.x;
  return true;
}

bool sf_au_sound_set_pan(smgf* const c, ssound* const s, float pan) {
  const MIX_Point3D pos = {pan, 0, 0};
  if (!MIX_SetTrack3DPosition(s->track, &pos)) {
    return false;
  }
  return true;
}

float sf_au_sound_get_gain(smgf* const c, ssound* const s) {
  return MIX_GetTrackGain(s->track);
}

bool sf_au_sound_set_gain(smgf* const c, ssound* const s, float vol) {
  return MIX_SetTrackGain(s->track, vol);
}

bool sf_au_sound_get_loop(smgf* const c, ssound* const s) {
  return MIX_TrackLooping(s->track);
}
