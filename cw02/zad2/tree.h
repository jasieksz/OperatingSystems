#ifndef CW02_TREE_H
#define CW02_TREE_H
#define _XOPEN_SOURCE 500

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ftw.h>
#include <time.h>
#include <limits.h>

int printDirectory(char *currentPath);
int conditionChecker(const char *fpath, const struct stat *file, int tflag, struct FTW *ftwbuf);
void printFileInfo(const struct stat *file, const char *newPath);
char *getPermissions(struct stat file);



#endif
