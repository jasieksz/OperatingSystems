#include "bounded.h"



int threadsReady = 0;
pthread_t *PRODUCENTS;
pthread_t *CONSUMERS;
char **BOUNDED_BUFFER;
FILE *in;

int producentsNumber, consumentsNumber;
int N;
const char *path;
int searchValue;
int searchMode;
int outMode;
int runTime;

/*
 * THREAD
 */

void *fun(void *);

/*
 * HELPERS
 */

void initialize();

void cleanUp();

int getSearchMode(const char *);

int getOutMode(const char *);

void openFiles(const char *);

void terminate(char *);


int max(int, int);


int main(int argc, char const *argv[]) {

    if (argc != 9)
        terminate("Invalid arguments - ./program P K N path L [>, <, =] [v, s] nk");

    /*
     * INITIALIZE
     */

    producentsNumber = (int) strtol(argv[1], NULL, 10);
    consumentsNumber = (int) strtol(argv[2], NULL, 10);
    N = (int) strtol(argv[3], NULL, 10);
    path = argv[4];
    searchValue = (int) strtol(argv[5], NULL, 10);
    searchMode = getSearchMode(argv[6]);
    outMode = getOutMode(argv[7]);
    runTime = (int) strtol(argv[8], NULL, 10);

    initialize();

    /*
     * WORK
     */


    if (pthread_create(0, NULL, &fun, NULL) != 0)
        terminate("Thread create failed");


    printf("Threads ready\n");
    threadsReady = 1;

    /*
     * END
     */

    if (pthread_join(0, NULL) != 0)
        terminate("Thread join failed");


    return 0;
}


/*
 * THREADS RELATED
 */

void *fun(void *arg) {
    return NULL;
}


/*
 * HELPERS
 */

void initialize() {
    BOUNDED_BUFFER = malloc(sizeof(char *) * N);
    for (int i = 0; i < N; i++) {
        BOUNDED_BUFFER[i] = malloc(sizeof(char) * MAX_LENGTH);

    }
    openFiles(path);

    if (atexit(cleanUp) != 0)
        terminate("Exit handler failed");

}

void cleanUp() {
    if (in != NULL && fclose(in) != 0)
        terminate("Closing IN failed");

    for (int i = 0; i < N; ++i) {
        free(BOUNDED_BUFFER[i]);
    }
    free(BOUNDED_BUFFER);
}

int getSearchMode(const char *string) {
    if (strcmp(string, ">") == 0)
        return LS_MODE; // strings shorter than searchValue
    else if (strcmp(string, "<") == 0)
        return GT_MODE;
    else
        return EQ_MODE; // default
}

int getOutMode(const char *string) {
    if (strcmp(string, "s") == 0)
        return SILENT; // strings shorter than searchValue
    else
        return VERBOSE; // default
}

void openFiles(const char *path) {
    if (!(in = fopen(path, "r")))
        terminate("Image open failed");
}

void terminate(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


int max(int a, int b) {
    return a > b ? a : b;
}


