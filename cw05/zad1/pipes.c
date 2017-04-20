#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE 600
#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


void parse(FILE *filePointer);
char **splitString(char *line, int *counter);

int main(int argc, char *argv[]){

  if (argc != 2){
    printf("Bledna ilosc argumentow\n");
    exit(EXIT_FAILURE);
  }
  char *fileName;
  fileName = argv[1];
  FILE *filePointer;
  if(!(filePointer = fopen(fileName,"r"))){
      perror("Opening file failed\n");
      exit(EXIT_FAILURE);
  }
  parse(filePointer);
  fclose(filePointer);
  return 0;
}

void parse(FILE *filePointer){
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, filePointer)) != -1){ // -1 <=> EOF
    int counter = 0;
    char **args = splitString(line,&counter); // args=[prog,arg1,arg2,|,prog2,arg1]
    char **progArgs = malloc(5 * 12 * sizeof(char));
    int i = 0;
    while (i < counter){
      int j = 0;
      while (i < counter && strcmp(args[i],"|") != 0){
        progArgs[j] = args[i];
        i++; j++;
      }
      for (int c=0;c<j;c++)
        printf("%s ",progArgs[c]);
      printf("\n");
      progArgs[j] = NULL;
      i++;
      printf("EXECUTE\n\n"); //TODO execute this program
    }
  }
}

char **splitString(char *line, int *counter){
  char **args = malloc(80*sizeof(char *)); //20 pipeow po 3 argumenty
  char *token;
  int i = 0;
  token = strtok(line," \n");
  while (token && i < 80){
    args[i] = malloc((strlen(token)+1)*sizeof(char));
    strcpy(args[i],token);
    i++;
    token = strtok(NULL," \n");
  }
  *counter = i;
  return args;
}
