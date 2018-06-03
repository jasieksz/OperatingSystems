#include "pbounded.h"

FILE *in;
const char *path;
int searchValue;
int searchMode;
int outMode;
int runTime;

/*
 * THREAD
 */

int producersNumber, consumersNumber;
pthread_t *PRODUCERS;
pthread_t *CONSUMERS;
int threadsReady = 0;

void *producerFun(void *);

void *consumerFun(void *);

void createThreads();

void endThreads();

/*
 *  BUFFER
 */

int N;
char **BOUNDED_BUFFER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty, full;
int inPos, outPos, count;

int insertElement(char *);

int removeElement(char **);

/*
 * HELPERS
 */

void initialize();

void cleanUp();

int getSearchMode(const char *);

int getOutMode(const char *);

void openFiles(const char *);

void terminate(char *);

void longerSleep();

void removeEndLine(char *);

int main(int argc, char const *argv[]) {

    if (argc != 9)
        terminate("Invalid arguments - ./program P K N path L [>, <, =] [v, s] nk");

    /*
     * INITIALIZE
     */

    producersNumber = (int) strtol(argv[1], NULL, 10);
    consumersNumber = (int) strtol(argv[2], NULL, 10);
    N = (int) strtol(argv[3], NULL, 10);
    path = argv[4];
    searchValue = (int) strtol(argv[5], NULL, 10);
    searchMode = getSearchMode(argv[6]);
    outMode = getOutMode(argv[7]);
    runTime = (int) strtol(argv[8], NULL, 10);

    initialize();
    createThreads();

    /*
     * WORK
     */

    if (runTime != 0) {
        sleep((unsigned int) runTime);
        //longerSleep();
    } else if (runTime == 0) {
        while (!feof(in)) {
            // DO NOTHING
        }
    }

    printf("END OF PROGRAM\n");

    /*
     * END
     */

//    endThreads();

    return 0;
}


/*
 * THREADS RELATED
 */

void *producerFun(void *param) {
    int arg = (int) param;
    printf("PRODUCER %d CREATED\n", arg);
    char *element = NULL;
    size_t len = 0;
    ssize_t read;


    while ((read = getline(&element, &len, in)) != -1) {
        removeEndLine(element);
        if (insertElement(element))
            terminate("Failed to insert element");
        else if (outMode == VERBOSE) {
            printf("PRODUCER %d | %s\n", arg, element);
        }

        sleep((unsigned int) (rand() % 3 + 1));
    }
    return NULL;
}

void *consumerFun(void *param) {
    int arg = (int) param;
    printf("CONSUMER %d CREATED\n", arg);
    char *element;
    int indx;
    while (1) {
        if (removeElement(&element))
            terminate("Failed to remove element");
        else if (outMode == VERBOSE) {
            printf("CONSUMER %d | %s\n", arg, element);
        }
        if (searchMode == LS_MODE && strlen(element) < searchValue)
            printf("CONSUMER %d | FOUND %s AT %d", arg, element, indx);
        if (searchMode == EQ_MODE && strlen(element) == searchValue)
            printf("CONSUMER %d | FOUND %s AT %d", arg, element, indx);
        if (searchMode == GT_MODE && strlen(element) > searchValue)
            printf("CONSUMER %d | FOUND %s AT %d", arg, element, indx);

        sleep((unsigned int) (rand() % 3 + 1));
    }
    return NULL;
}

void createThreads() {
    for (int i = 0; i < producersNumber; i++) {
        if (pthread_create(&PRODUCERS[i], NULL, &producerFun, (void *)(i)) != 0)
            terminate("Producer thread create failed");
    }

    for (int i = 0; i < consumersNumber; i++) {
        if (pthread_create(&CONSUMERS[i], NULL, &consumerFun, (void *)(i)) != 0)
            terminate("Consumer thread create failed");
    }

    printf("Threads ready\n");
    threadsReady = 1;
}

void endThreads() {
    for (int i = 0; i < producersNumber; i++) {
        if (pthread_join(PRODUCERS[i], NULL) != 0)
            terminate("Thread join failed");
    }

    for (int i = 0; i < consumersNumber; i++) {
        if (pthread_join(CONSUMERS[i], NULL) != 0)
            terminate("Thread join failed");
    }
}

/*
 * BUFFER
 */

int insertElement(char *element) {
    int success;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    if (count != N) {
        BOUNDED_BUFFER[inPos] = malloc(sizeof(char) * strlen(element));
        strcpy(BOUNDED_BUFFER[inPos], element);
        inPos = (inPos + 1) % N;
        count += 1;
        success = 0;
    } else
        success = -1;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return success;
}

int removeElement(char **element) {
    int success;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    if (count != N) {
        *element = malloc(sizeof(char) * strlen(BOUNDED_BUFFER[outPos]));
        strcpy(*element, BOUNDED_BUFFER[outPos]);
        free(BOUNDED_BUFFER[outPos]);
        outPos = (outPos + 1) % N;
        count -= 1;
        success = 0;
    } else
        success = -1;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return success;
}

/*
 * HELPERS
 */

void initialize() {
    PRODUCERS = malloc(sizeof(pthread_t) * producersNumber);
    CONSUMERS = malloc(sizeof(pthread_t) * consumersNumber);
    BOUNDED_BUFFER = malloc(sizeof(char *) * N);
    openFiles(path);

    sem_init(&empty, 0, (unsigned int) N); // All of buffer is empty
    sem_init(&full, 0, 0);
    inPos = outPos = count = 0;

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

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
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
        terminate("File open failed");
}

void terminate(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void longerSleep(){
    clock_t startTime = clock();
    clock_t stopTime = clock();
    double elapsed = (double) (stopTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
    while (elapsed <= runTime) {
        // DO NOTHING
        stopTime = clock();
        elapsed = (double) (stopTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
    }
}

void removeEndLine(char *string){
    string[strlen(string)-1] = '\0';
}