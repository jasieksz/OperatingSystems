#include "fft.h"

int threadsNumber;
pthread_t *threads;
int threadsReady = 0;

FILE *in;
FILE *filter;
FILE *out;

int **IMAGE;
int **FILTER;
int **RESULT;
int imageSize;
int filterSize;

void terminate(char *);

void openFiles(const char *, const char *, const char *);

void createFilesMatrices(FILE *, int **, int *);

void allocateMemory();

/*
 *
 */

void *correlate(void *);

void kill_threads(pthread_t);

int main(int argc, char const *argv[]) {
    if (argc != 5)
        terminate("Invalid arguments - ./program N_Threads In Filter Out");

    /*
     * INITIALIZE
     */
    allocateMemory();
    openFiles(argv[2], argv[3], argv[4]);
    createFilesMatrices(in, IMAGE, &imageSize);
    createFilesMatrices(filter, FILTER, &filterSize);
//    printf("%d %d\n", imageSize, filterSize);

    /*
     * WORK
     */

    threadsNumber = atoi(argv[1]);

    int delta = imageSize / threadsNumber;
    int x = 0;

    threads = malloc(sizeof(pthread_t) * threadsNumber);
    for (int i = 0; i < threadsNumber; i++) {
        Area *area = malloc(sizeof(Area));
        area->x0 = x;
        area->x1 = x + delta;
        if (i == threadsNumber - 1)
            area->x1 = imageSize;
        if (pthread_create(&threads[i], NULL, &correlate, (void *) area) != 0)
            terminate("Thread create failed");
        x += delta;
    }

    printf("Threads ready\n");
    threadsReady = 1;

    for (int i = 0; i < threadsNumber; i++)
        if (pthread_join(threads[i], NULL) != 0)
            terminate("Thread join failed");

    /*
     * SAVE RESULTS
     */

    // TODO : Save RESULTS to out and free arrays
    for (int i = 0; i < imageSize; i++) {
        for (int j = 0; j < imageSize; j++)
            printf("%d ", RESULT[i][j]);
        printf("\n");
    }
    return 0;
}

/*
 * THREADS RELATED
 */

void *correlate(void *arg) {
    Area *area = (Area *) arg;
    printf("THREAD : area %d %d\n", area->x0, area->x1);
    for (int i = area->x0; i < area->x1; i++) {
        for (int j = 0; j < imageSize; j++) {
            RESULT[i][j] = i * j;
            // TODO : Calculate convolution here!
        }
    }
    pthread_exit(NULL);
}

void kill_threads(pthread_t tid) {
    for (int i = 0; i < threadsNumber; i++) {
        if (threads[i] != tid) {
            pthread_cancel(threads[i]);
        }
    }
    printf("%s\n", "Threads cancelled");
}

/*
 * HELPERS
 */

void allocateMemory() {
    IMAGE = malloc(sizeof(int *) * MAX_N);
    for (int i = 0; i < MAX_N; i++)
        IMAGE[i] = malloc(sizeof(int) * MAX_N);

    FILTER = malloc(sizeof(int *) * MAX_N);
    for (int i = 0; i < MAX_N; i++)
        FILTER[i] = malloc(sizeof(int) * MAX_N);

    RESULT = malloc(sizeof(int *) * MAX_N);
    for (int i = 0; i < MAX_N; i++)
        RESULT[i] = malloc(sizeof(int) * MAX_N);
}

void openFiles(const char *i, const char *f, const char *o) { // in filter out
    if (!(in = fopen(i, "r")))
        terminate("Image open failed");

    if (!(filter = fopen(f, "r")))
        terminate("Filter open failed");

    if (!(out = fopen(o, "w")))
        terminate("Image open failed");
}

void createFilesMatrices(FILE *file, int **tab, int *len) {
    char *line = NULL;
    size_t length = 0;
    ssize_t read;

    int i = 0;
    int j = 0;
    while ((read = getline(&line, &length, file)) != -1) {
        char *num = strtok(line, " ");
        while (num != NULL) {
            tab[i][j] = atoi(num);
            j += 1;
            num = strtok(NULL, " \n");
        }
        j = 0;
        i += 1;
    }
    *len = i;
}

void terminate(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}



