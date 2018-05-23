#ifndef FFT_H
#define FFT_H


#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_N 256

typedef struct Area {
    int x0;
    int x1;

} Area;

#endif /* FFT_H */
