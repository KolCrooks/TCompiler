// Copyright 2019 Justin Hu
//
// This file is part of the T Language Compiler.

// file manipulation for a POSIX based system

#ifndef TLC_UTIL_FILE_H_
#define TLC_UTIL_FILE_H_

#include <stdbool.h>
#include <stddef.h>

extern size_t const F_BUFFER_SIZE;

// low level file manipulation
typedef struct {
  char *buffer;
  int fd;
  size_t offset;     // offset from start of file of next character to be read
  size_t bufferMax;  // first offset from start of file that is invalid
  bool eof;          // eof flag - set when a read fails.
} File;

// opens a file. returns NULL if failed
File *fOpen(char const *fileName);

extern char const F_OK;
extern char const F_EOF;
extern char const F_ERR;
// gets a character from the file
// returns RF_EOF on end of file.
//         RF_ERR on an error.
// RF_ERR and RF_EOF are guarenteed not to infringe on ASCII text.
// returning RF_ERR indicates the stream may be in an inconsistent state, and
// can only be safely closed.
char fGet(File *);
// backs up a character.
// returns RF_ERR if there is an error, i.e. backing up before start of file
// returning RF_ERR indicates the stream may be in an inconsistent state, and
// can only be safely closed.
int fUnget(File *);

// closes a file. silently fails if unsuccessful; no recovery is possible
void fClose(File *);

#endif  // TLC_UTIL_FILESTREAM_H_