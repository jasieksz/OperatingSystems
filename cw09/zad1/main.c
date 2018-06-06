#include "bounded.h"

FILE *in;
const char *path;
int searchValue;
int searchMode;
int outMode;
int runTime;
int myEOF;

/*
 * THREAD
 */

int producersNumber, consumersNumber;
pthread_t *PRODUCERS;
pthread_t *CONSUMERS;

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
pthread_cond_t canConsume = PTHREAD_COND_INITIALIZER;
pthread_cond_t canProduce = PTHREAD_COND_INITIALIZER;
int inPos, outPos, count;

int insertElement(char *);

int removeElement(char **, int *);

/*
 * HELPERS
 */

void initialize();

void cleanUp();

int getSearchMode(const char *);

int getOutMode(const char *);

void openFiles(const char *);

void terminate(char *);

//void longerSleep();

void removeEndLine(char *);

void sigintHandler(int sig);


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

    if (runTime != 0)
        sleep((unsigned int) runTime);
    else if (runTime == 0)
        while (myEOF != 1)
            sleep(1);
    /*
     * END
     */

    printf("THE END\n");
    endThreads(); // TODO : add cancelation point in funs()
    return 0;
}


/*
 * THREADS RELATED
 */

void *producerFun(void *param) {
    int arg = (int) (long long) param;
    printf("PRODUCER %d CREATED\n", arg);
    char *element = NULL;
    size_t len = 0;
    ssize_t read;


    while ((read = getline(&element, &len, in)) != -1) {
        removeEndLine(element);
        if (insertElement(element))
            terminate("Failed to insert element");
        else if (outMode == VERBOSE)
            printf("PRODUCER %d | %s\n", arg, element);

        sleep((unsigned int) (rand() % 3 + 1));
    }
    if (runTime == 0 && read == -1) { // EOF reached - time to finish
        myEOF = 1;
        pthread_cond_broadcast(&canConsume); // if any Consumer is stuck in removeElement, let him know
    }
    return NULL;
}

void *consumerFun(void *param) {
    int arg = (int) (long long) param;
    printf("CONSUMER %d CREATED\n", arg);
    char *element;
    int indx;
    while (1) {
        if (runTime == 0 && myEOF == 1) // PRODUCER reached EOF - time to finish
            break;
        if (removeElement(&element, &indx))
            terminate("Failed to remove element");
        else if (outMode == VERBOSE)
            printf("CONSUMER %d | %s\n", arg, element);
        if (searchMode == LS_MODE && strlen(element) < searchValue)
            printf("CONSUMER %d | FOUND %s AT %d\n", arg, element, indx);
        if (searchMode == EQ_MODE && strlen(element) == searchValue)
            printf("CONSUMER %d | FOUND %s AT %d\n", arg, element, indx);
        if (searchMode == GT_MODE && strlen(element) > searchValue)
            printf("CONSUMER %d | FOUND %s AT %d\n", arg, element, indx);

        sleep((unsigned int) (rand() % 3 + 1));
    }
    return NULL;
}

void createThreads() {
    for (int i = 0; i < producersNumber; i++) {
        if (pthread_create(&PRODUCERS[i], NULL, &producerFun, (void *) ((long long) i)) != 0)
            terminate("Producer thread create failed");
    }

    for (int i = 0; i < consumersNumber; i++) {
        if (pthread_create(&CONSUMERS[i], NULL, &consumerFun, (void *) ((long long) i)) != 0)
            terminate("Consumer thread create failed");
    }
}

void endThreads() {
   for (int i = 0; i < producersNumber; i++) {
       pthread_cancel(PRODUCERS[i]);
//       if (pthread_join(PRODUCERS[i], NULL) != 0)
//           terminate("Thread join failed");
   }

   for (int i = 0; i < consumersNumber; i++) {
       pthread_cancel(CONSUMERS[i]);
//       if (pthread_join(CONSUMERS[i], NULL) != 0)
//           terminate("Thread join failed");
   }
}

/*
 * BUFFER
 */

int insertElement(char *element) {
    int success;
    pthread_mutex_lock(&mutex);
    while (count == N) {
        pthread_cond_wait(&canProduce, &mutex);
    }

    if (count != N) {
        BOUNDED_BUFFER[inPos] = malloc(sizeof(char) * strlen(element));
        strcpy(BOUNDED_BUFFER[inPos], element);
        inPos = (inPos + 1) % N;
        count += 1;
        success = 0;
    } else
        success = -1;

    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&canConsume);
    return success;
}

int removeElement(char **element, int *i) {
    int success;
    pthread_mutex_lock(&mutex);
    while (count == 0) {
        pthread_cond_wait(&canConsume, &mutex);
    }

    if (count != N && count != 0) { // count != 0 might be used after myEOF = 1 and consumer was stuck on canConsume
        *element = malloc(sizeof(char) * strlen(BOUNDED_BUFFER[outPos]));
        strcpy(*element, BOUNDED_BUFFER[outPos]);
        free(BOUNDED_BUFFER[outPos]);
        *i = outPos;
        outPos = (outPos + 1) % N;
        count -= 1;
        success = 0;
    } else
        success = -1;

    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&canProduce);
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

    inPos = outPos = count = myEOF = 0;

    if (signal(SIGINT, sigintHandler) == SIG_ERR)
        terminate("Signal handler failed");

    if (atexit(cleanUp) != 0)
        terminate("Exit handler failed");

}

void cleanUp() {
    if (in != NULL && fclose(in) != 0)
        terminate("Closing IN failed");

    free(BOUNDED_BUFFER);

    pthread_cond_destroy(&canProduce);
    pthread_cond_destroy(&canConsume);
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

//void longerSleep() {
//    clock_t startTime = clock();
//    clock_t stopTime = clock();
//    double elapsed = (double) (stopTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
//    while (elapsed <= runTime) {
//        // DO NOTHING
//        stopTime = clock();
//        elapsed = (double) (stopTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
//    }
//}

void removeEndLine(char *string) {
    string[strlen(string) - 1] = '\0';
}

void sigintHandler(int sig) {
    if (sig == SIGINT && runTime == 0) {
        printf("\nReceived SIGINT - closing\n");
        exit(EXIT_SUCCESS);
    }
}