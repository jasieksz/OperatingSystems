#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void generate(char* fileName, int recordsNumber, size_t recordSize);
//"Usage: ./generate [path] [number of records] [record size]
int main(int argc, char *argv[]){
    if (argc != 4){
        printf("bledna liczba argumentow\n");
        return EXIT_FAILURE;
    }
    //TODO : po co rzutowanie (size_t) atoi(argv[3])
    generate(argv[1],atoi(argv[2]),(size_t) atoi(argv[3]));
    return EXIT_SUCCESS;
}

void generate(char* fileName, int recordsNumber, size_t recordSize){
    FILE* writeFile,readFile;

    if(!(writeFile = fopen(fileName, "w"))) {
        perror("Creating file failed\n");
        exit(EXIT_FAILURE);
    } else if (!(readFile = fopen("/dev/random","r"))){
        perror("Reading file failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < recordsNumber, i++){
        void *buffer = malloc(recordSize);
        if (!(fread(buffer,recordSize,1,readFile))){
            perror("Reading file failed\n");
            exit(EXIT_FAILURE);
        }
        fwrite(buffer,recordSize,1,writeFile);
        free(buffer);
    }
    fclose(writeFile);
    fclose(readFile);
}
//size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
