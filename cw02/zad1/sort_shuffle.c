#include "sort_shuffle.h"


int main(int argc, char* argv[]){
    if (argc != 6)
        printf("Bledna ilosc arguemntow\n");
    else { //./program sys shuffle datafile 100 512
        char* fileName = argv[3];
        size_t recordsNumber = (size_t) atoi(argv[4]);
        size_t recordSize = (size_t) atoi(argv[5]);

        if (strcmp(argv[2], "sort") == 0)
            sorter(fileName,recordSize,recordsNumber,strcmp(argv[1], "sys") ? libSorter : sysSorter);
        else if(strcmp(argv[2], "shuffle") == 0)
            shuffler(fileName,recordSize,recordsNumber,strcmp(argv[1], "sys") ? libShuffler : sysShuffler);
        else if(strcmp(argv[2], "generate") == 0)
            generate(fileName,recordSize,recordsNumber);
        else {
            printf("Nie ma takiej funkcji\n");
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}

void sorter(const char *fileName, size_t recordSize, size_t recordsNumber, void (*f)(const char*, size_t, size_t)){
    double userStartTime, userEndTime,sysStartTime, sysEndTime;
    getTime(&userStartTime, &sysStartTime);
    (*f)(fileName ,recordSize, recordsNumber);
    getTime(&userEndTime, &sysEndTime);
    printf("Sorting : user time\t%fs , system time\t%fs",userEndTime-userStartTime,sysEndTime-sysStartTime);
}

void shuffler(const char* fileName, size_t recordSize,size_t recordsNumber, void (*f)(const char*, size_t, size_t)){
    double userStartTime, userEndTime,sysStartTime, sysEndTime;
    getTime(&userStartTime, &sysStartTime);
    (*f)(fileName ,recordSize, recordsNumber);
    getTime(&userEndTime, &sysEndTime);
    printf("Shuffling : user time\t%fs , system time\t%fs",userEndTime-userStartTime,sysEndTime-sysStartTime);
}

void libSorter(const char *fileName, size_t recordSize, size_t recordsNumber){
    FILE* file;
    if(!(file = fopen(fileName,"r+"))){
        perror("Opening file failed\n");
        exit(EXIT_FAILURE);
    }
    int swapped = 1;
    while (swapped != 0){
        swapped = 0;
        for (size_t i = 1; i < recordsNumber; i++){
            char* first = malloc(recordSize); //TODO : malloc record_size czy char?
            char* second = malloc(recordSize);

            fseek(file, (i-1)*recordSize, SEEK_SET);
            if (fread(first,1,1,file) != 1){
                perror("Reading file failed\n");
                exit(EXIT_FAILURE);
            }

            fseek(file,i*recordSize,SEEK_SET);
            if (fread(second,1,1,file) != 1){
                perror("Reading file failed\n");
                exit(EXIT_FAILURE);
            }

            if((*first)>(*second)){
                libSwap(file,i-1,i,recordSize);
                swapped = 1;
            }
            free(first);
            free(second);
        }
    }
    fclose(file);
}

void sysSorter(const char* fileName, size_t recordSize, size_t recordsNumber){
    int file = open(fileName, O_RDWR); //open read write
    if(file == 0){
        perror("Opening file failed\n");
        exit(EXIT_FAILURE);
    }

    int swapped = 1;
    while (swapped != 0){
        swapped = 0;
        for (size_t i = 1; i < recordsNumber; i++){
            char *first = malloc(recordSize);
            char *second = malloc(recordSize);

            lseek(file, (i-1)*recordSize, SEEK_SET);
            if (read(file,first,1) != 1){
                perror("Reading file failed\n");
                exit(EXIT_FAILURE);
            }

            lseek(file,i*recordSize,SEEK_SET);
            if (read(file,second,1) != 1){
                perror("Reading file failed\n");
                exit(EXIT_FAILURE);
            }

            if((*first)>(*second)){
                sysSwap(file,i-1,i,recordSize);
                swapped = 1;
            }
        }
    }
}

void sysShuffler(const char* fileName,size_t recordSize, size_t recordsNumber){
    srand((unsigned int) time(NULL));
    int file = open(fileName, O_RDWR);
    if(file == 0){
        perror("Opening file failed\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < recordsNumber; i++) {
        size_t j = rand() % recordsNumber;
        sysSwap(file, i, j, recordSize);
    }
    close(file);
}

void libShuffler(const char* fileName, size_t recordSize, size_t recordsNumber){
    srand((unsigned int) time(NULL));
    FILE* file;
    if(!(file = fopen(fileName,"r+"))){
        perror("Opening file failed\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < recordsNumber; ++i) {
        size_t j = rand() % recordsNumber;
        libSwap(file,i,j,recordSize);
    }
    fclose(file);
}

void libSwap(FILE* file, size_t i, size_t j, size_t recordSize){
    void* first = malloc(recordSize);
    void* second = malloc(recordSize);

    fseek(file, i*recordSize, SEEK_SET);
    if (fread(first,recordSize,1,file) != 1){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, j*recordSize, SEEK_SET);
    if (fread(second,recordSize,1,file) != 1){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, i*recordSize, SEEK_SET);
    if (fwrite(second, recordSize, 1, file) != 1){
        perror("Writing file failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, j*recordSize, SEEK_SET);
    if (fwrite(first, recordSize, 1, file) != 1){
        perror("Writing file failed\n");
        exit(EXIT_FAILURE);
    }

    free(first);
    free(second);
}

void sysSwap(int file, size_t i, size_t j, size_t recordSize){
    void* first = malloc(recordSize);
    void* second = malloc(recordSize);

    lseek(file, i*recordSize, SEEK_SET);
    if (read(file,first,recordSize) != recordSize){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    lseek(file, j*recordSize, SEEK_SET);
    if (read(file,second,recordSize) != recordSize){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    lseek(file, j*recordSize, SEEK_SET);
    if (write(file,first,recordSize) != recordSize){
        perror("Writing file failed\n");
        exit(EXIT_FAILURE);
    }

    lseek(file, i*recordSize, SEEK_SET);
    if (write(file,second,recordSize) != recordSize){
        perror("Writing file failed\n");
        exit(EXIT_FAILURE);
    }

    free(first);
    free(second);
}

void getTime(double *user, double *sys){
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
    *user = rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / (double) 1e6;
    *sys = rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / (double) 1e6;
}

void generate(char* fileName, size_t recordSize, size_t recordsNumber){
    FILE *writeFile;
    FILE *readFile;

    if(!(writeFile = fopen(fileName, "w"))) {
        perror("Creating file failed\n");
        exit(EXIT_FAILURE);
    } else if (!(readFile = fopen("/dev/random","r"))){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < recordsNumber; i++){
        void *buffer = malloc(recordSize);
        if (!(fread(buffer,recordSize,1,readFile))){
            perror("Reading file failed\n");
            exit(EXIT_FAILURE);
        }
        fwrite(buffer,recordSize,1,writeFile);
        free(buffer);
    }

    fclose(readFile);
    fclose(writeFile);
}
