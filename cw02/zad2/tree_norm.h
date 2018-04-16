#ifndef CW02_TREE_NORM_H
#define CW02_TREE_NORM_H
#define _XOPEN_SOURCE 500

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ftw.h>
#include <time.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>

void printDirectory(char *filePath);
void printFileInfo(const struct stat file, const char *newPath);
char *getPermissions(struct stat file);

#endif
