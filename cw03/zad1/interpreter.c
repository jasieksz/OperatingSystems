#include "interpreter.h"

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

  return 0;
}

void parse(FILE *filePointer){ //na koncu lini jest \n, puste linie czyta jako enter
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, filePointer)) != -1){
    int size = (int) read-1;
    if (line[0] == '#')
      enviromentVariable(line,size);
    else
      executeProg(line,size);
  }
  free(line);
}

void enviromentVariable(char *line, int size){
    line[size] = 0;
    line++;
    int counter = 0;

    char *args = splitString(line,&counter);

    if (counter == 1){
      if ((unsetenv(args[0])) != 0){
        perror("Removing env var failed");
        exit(EXIT_FAILURE);
      }
    } else if (counter == 2) {
      char *name = args[0];
      char *value = args[1];
      if ((setenv(name,value,1)) != 0){
        perror("Adding env var failed");
        exit(EXIT_FAILURE);
      }
    } else {
      perror("Too many arguments");
      exit(EXIT_FAILURE);
    }
}

void executeProg(char *line, int size){
  //fork exec wait
  //int execve(const char *filename, char *const argv[],char *const envp[]);
}

char *splitString(char *line, int *counter){
  char *args = calloc(5, sizeof(char));
  char *token;
  int i = 0;
  token = strtok(line," ");
  while (token && i < 5){
    args[i] = token;
    i++;
    token = strtok(NULL," ");
  }
  *counter = i;
  return args;
}
