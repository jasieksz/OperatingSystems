#ifndef SERVER_H
#define SERVER_H

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


#define MMSGLEN 1000
/*
1 - login
2 - echo
3 - time
4 - vers
5 - term
*/
typedef struct message {
  long mtype;
  char mtext[MMSGLEN];
  pid_t sender;
} message;

typedef struct client {
    int key;
    pid_t pid;
} client;



#define MSG_SIZE sizeof(message) - sizeof(long)

#endif
