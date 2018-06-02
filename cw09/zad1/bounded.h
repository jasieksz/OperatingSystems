#ifndef BOUNDED_H
#define BOUNDED_H


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
#include <math.h>
#include <time.h>
#include <sys/resource.h>

#define MAX_THREADS 16
#define MAX_LENGTH 1024

#define LS_MODE (-1)
#define EQ_MODE 0
#define GT_MODE 1

#define SILENT 2
#define VERBOSE 3



typedef struct Area {
    int x0;
    int x1;

} Area;


#endif /* BOUNDED_H */
