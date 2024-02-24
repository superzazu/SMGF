#include "../api.h"

int sf_io_new(sfile* const f) {
  f->file = NULL;
  SDL_memset(&f->stat, 0, sizeof(f->stat));
  f->mode = 0;
  return 0;
}

int sf_io_del(sfile* const f) {
  sf_io_close(f);
  return 0;
}

int sf_io_open(sfile* const f, const char* filename, char mode) {
  switch (mode) {
  case 'r': f->file = PHYSFSRWOPS_openRead(filename); break;
  case 'w': f->file = PHYSFSRWOPS_openWrite(filename); break;
  case 'a': f->file = PHYSFSRWOPS_openAppend(filename); break;
  default: SDL_SetError("unknown open mode '%c'", mode); return 2;
  }

  f->mode = mode;

  if (f->file == NULL) {
    // PHYSFSRWOPS_open* already sets error message
    return 1;
  }

  // get stat on file:
  if (PHYSFS_stat(filename, &f->stat) == 0) {
    SDL_LogWarnC("unable to get stats of file '%s'", filename);
  }

  return 0;
}

int sf_io_close(sfile* const f) {
  if (f->file == NULL) {
    return 0;
  }

  int r = SDL_RWclose(f->file);
  if (r != 0) {
    SDL_SetError(
        "unable to close file (%s)",
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return r;
  }

  f->file = NULL;

  return 0;
}

Sint64 sf_io_seek(sfile* const f, Sint64 offset, int whence) {
  return SDL_RWseek(f->file, offset, whence);
}

Sint64 sf_io_tell(sfile* const f) {
  return SDL_RWtell(f->file);
}

Sint64 sf_io_rewind(sfile* const f) {
  return SDL_RWseek(f->file, 0, RW_SEEK_SET);
}

Sint64 sf_io_size(sfile* const f) {
  return SDL_RWsize(f->file);
}

// returns nb of objects read
int sf_io_read(sfile* const f, void* ptr, size_t size, size_t maxnum) {
  return SDL_RWread(f->file, ptr, size, maxnum);
}

// returns nb of objects written
int sf_io_write(sfile* const f, const void* ptr, size_t size, size_t num) {
  return SDL_RWwrite(f->file, ptr, size, num);
}

// get the position of the next occurence of character "c". If not found,
// returns the EOF offset.
int sf_io_strchr(sfile* const f, Sint64* pos, char needle) {
  Sint64 cur_pos = sf_io_tell(f);

  char c = 0;
  while (c != needle) {
    if (sf_io_read(f, &c, 1, 1) <= 0) {
      break; // eof of file or read error
    }
  }
  *pos = sf_io_tell(f);
  sf_io_seek(f, cur_pos, RW_SEEK_SET);

  return 0;
}

int sf_io_mkdir(const char* dirname) {
  return PHYSFS_mkdir(dirname) == 0;
}

int sf_io_delete(const char* filename) {
  return PHYSFS_delete(filename) == 0;
}

// Returns -1 on error
PHYSFS_FileType sf_io_get_filetype(const char* filename) {
  PHYSFS_Stat s = {0};
  if (PHYSFS_stat(filename, &s) == 0) {
    return -1;
  }
  return s.filetype;
}

bool sf_io_exists(const char* filename) {
  return PHYSFS_exists(filename) != 0;
}

int sf_io_flush(sfile* const f) {
  // @WARN: use of private API
  PHYSFS_File* handle = (PHYSFS_File*) f->file->hidden.unknown.data1;
  return PHYSFS_flush(handle) == 0;
}
