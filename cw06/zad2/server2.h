#ifndef SERVER2_H
#define SERVER2_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>


#define MMSGLEN 1000
static const char *NAME = "/serv";

#define Mlogin 1
#define Mecho 2
#define Mtime 3
#define Mvers 4
#define Mterm 5

typedef struct message {
  long mtype;
  char mtext[MMSGLEN];
  pid_t sender;
} message;

typedef struct clientT {
    mqd_t key;
    pid_t pid;
} clientT;

#endif
