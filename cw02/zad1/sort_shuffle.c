#include <time.h>
#include "sort_shuffle.h"


int main(int argc, char* argv[]){
    if (argc == 6) {
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
            printf("NIE ma takiej funkcji\n");
            exit(EXIT_FAILURE);
        }
    } else if (argc == 7){
        char* fileName1 = argv[3];
        char* fileName2 = argv[4];
        size_t recordsNumber = (size_t) atoi(argv[5]);
        size_t recordSize = (size_t) atoi(argv[6]);

        if(strcmp(argv[2], "copy") == 0){
            int res = copier(fileName1, fileName2, recordSize, recordsNumber, strcmp(argv[1], "sys") ? libCopier : sysCopier);
            if (res != 0){
                exit(EXIT_FAILURE);
            }
        }
        else {
            printf("NIE ma takiej funkcji\n");
            exit(EXIT_FAILURE);
        }

    } else {
        printf("Bledne argumenty\n./program sys shuffle datafile 100 512\n");
    }
    return EXIT_SUCCESS;
}

void sorter(const char *fileName, size_t recordSize, size_t recordsNumber, void (*f)(const char*, size_t, size_t)){
    double userStartTime, userEndTime,sysStartTime, sysEndTime;
    getTime(&userStartTime, &sysStartTime);
    (*f)(fileName ,recordSize, recordsNumber);
    getTime(&userEndTime, &sysEndTime);
    printf("Sorting : user time\t%fs , system time\t%fs\n",userEndTime-userStartTime,sysEndTime-sysStartTime);
}

void shuffler(const char* fileName, size_t recordSize,size_t recordsNumber, void (*f)(const char*, size_t, size_t)){
    double userStartTime, userEndTime,sysStartTime, sysEndTime;
    getTime(&userStartTime, &sysStartTime);
    (*f)(fileName ,recordSize, recordsNumber);
    getTime(&userEndTime, &sysEndTime);
    printf("Shuffling : user time\t%fs , system time\t%fs\n",userEndTime-userStartTime,sysEndTime-sysStartTime);
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
                if (feof(file)) {
                    printf ("END OF FILE\n");
                }
                perror("Reading file failed\n");
                exit(EXIT_FAILURE);
            }

            fseek(file,i*recordSize,SEEK_SET);
            if (fread(second,1,1,file) != 1){
                if (feof(file)) {
                    printf ("END OF FILE\n");
                }
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
        if (feof(file)) {
            printf ("END OF FILE\n");
        }
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, j*recordSize, SEEK_SET);
    if (fread(second,recordSize,1,file) != 1){
        if (feof(file)) {
            printf ("END OF FILE\n");
        }
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
    if ((size_t)read(file,first,recordSize) != recordSize){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    lseek(file, j*recordSize, SEEK_SET);
    if ((size_t)read(file,second,recordSize) != recordSize){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    lseek(file, j*recordSize, SEEK_SET);
    if ((size_t)write(file,first,recordSize) != recordSize){
        perror("Writing file failed\n");
        exit(EXIT_FAILURE);
    }

    lseek(file, i*recordSize, SEEK_SET);
    if ((size_t)write(file,second,recordSize) != recordSize){
        perror("Writing file failed\n");
        exit(EXIT_FAILURE);
    }

    free(first);
    free(second);
}

int copier(const char* fileNameOne, const char* fileNameTwo, size_t recordSize, size_t recordsNumber, int (*f)(const char*, const char*, size_t, size_t)){
    double userStartTime, userEndTime,sysStartTime, sysEndTime;
    getTime(&userStartTime, &sysStartTime);
    (*f)(fileNameOne , fileNameTwo, recordSize, recordsNumber);
    getTime(&userEndTime, &sysEndTime);
    printf("Copying : user time\t%fs , system time\t%fs\n",userEndTime-userStartTime,sysEndTime-sysStartTime);
}

int libCopier(const char* from, const char* to, size_t recordSize, size_t recordsNumber){
    FILE *source = fopen(from, "r");
    FILE *target = fopen(to, "w+");
    char *tmp = malloc(recordSize * sizeof(char));

    for (int i = 0; i < recordsNumber; i++){
        if(fread(tmp, sizeof(char), (size_t) (recordSize + 1), source) != (recordSize + 1)) {
            return 1;
        }

        if(fwrite(tmp, sizeof(char), (size_t) (recordSize + 1), target) != (recordSize + 1)) {
            return 1;
        }
    }
    fclose(source);
    fclose(target);
    free(tmp);
    return 0;

}

int sysCopier(const char* from, const char* to, size_t recordSize, size_t recordsNumber){
    int source = open(from, O_RDONLY);
    int target = open(to, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR); // create ifne, wr only, trunc to 0, args with OCR
    char *tmp = malloc(recordSize * sizeof(char));

    for (int i = 0; i < recordsNumber; i++){
        if(read(source, tmp, (size_t) (recordSize + 1) * sizeof(char)) != (recordSize + 1)) {
            return 1;
        }

        if(write(target, tmp, (size_t) (recordSize + 1) * sizeof(char)) != (recordSize + 1)) {
            return 1;
        }
    }
    close(source);
    close(target);
    free(tmp);
    return 0;
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
    } else if (!(readFile = fopen("/dev/urandom","r"))){
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

