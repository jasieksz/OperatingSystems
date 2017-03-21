#ifndef CW02_LOCK_H
#define CW02_LOCK_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 2

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void setReadLock (int openDesc,int offset);
void setWriteLock (int openDesc,int offset);
void removeLock(int openDesc,int offset);
void printLocks(int openDesc,int offset);
void readChar(int openDesc,int offset);
void writeChar(int openDesc,int offset);
int executeLock(int openDesc, int f, int type, int offset);
void checkError(int result);
int get_option();

#endif
