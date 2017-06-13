#define _GNU_SOURCE
#define RECORD_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char* argv[]) { // ./generator in.txt 10
    if(argc != 3) {
        perror("Wrong args : ./generator in 10");
        exit(1);
    }

    char* file_name = argv[1];
    int record_number = atoi(argv[2]);
    srand(time(NULL));

    FILE* file;
    if((file = fopen(file_name, "w+")) == NULL) {
        perror("fopen failed");
        exit(1);
    }

    char stream[RECORD_SIZE];
    int i = 0;
    int j = 0;
    while (i++ < record_number) {
        j = 0;
        while (j < RECORD_SIZE) {
            stream[j] = (char) ((rand() % 26) + 97);
            j++;
        }
        stream[j-1]='\0';
        stream[j-2]='\n';
        if (i == 2 || i == 7 || i == 16){
          stream[32]='c'; stream[33]='a'; stream[34]='t'; stream[35]='s';
        }
        if (fwrite(stream, sizeof(char), RECORD_SIZE, file) != RECORD_SIZE) {
            perror("fwrite failed");
            exit(1);
        }
    }

    fclose(file);
    return 0;
}
