#define _GNU_SOURCE

#define RECORD_SIZE 1024
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [file_name] [records_number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* file_name = argv[1];
    int record_number;
    if((record_number = atoi(argv[2])) < 1) {
        fprintf(stderr, "record number must be positive!\n");
        exit(EXIT_FAILURE);
    }

    FILE* file;
    if((file = fopen(file_name, "w+")) == NULL) {
        perror("fopen failure");
        exit(EXIT_FAILURE);
    }

    char stream[RECORD_SIZE];
    char buf[RECORD_SIZE];
    int i = 0;
    int j = 0;
    while (i++ < record_number) {
        j = 0;
        while (j < 1020) {
            stream[j] = (char) ((rand() % 26) + 97);
            if(!(rand()%15)) {
                stream[j] = ' ';
            }
            j++;
        }
        stream[j-1]='\0';
        stream[j-2]='\n';
        sprintf(buf, "%4d%s", i, stream);
        if (fwrite(buf, sizeof(char), RECORD_SIZE, file) != RECORD_SIZE) {
            perror("fwrite failure");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    return 0;

}
