#ifndef CW03_INTERPRETER_LIMIT_H
#define CW03_INTERPRETER_LIMIT_H

#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 600
#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

  void parse(FILE *filePointer);
  void executeProg(char *line, int size);
  void enviromentVariable(char *line, int size);
  char **splitString(char *line, int *counter);
  float getTime();

#endif
