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
  fclose(filePointer);

  return 0;
}

void parse(FILE *filePointer){ //na koncu lini jest \n, puste linie czyta jako enter
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, filePointer)) != -1){ //-1 <=> EOF
    int size = (int) read-1;
    if (line[0] == '#')
      enviromentVariable(line,size);
    else
      executeProg(line,size);
  }
  free(line);
}

void enviromentVariable(char *line, int size){
    if (line[size] == '\n')
      line[size] = 0; // usun # i \n z line
    line++;
    int counter = 0;

    char **args = splitString(line,&counter);
    if (counter == 1){
      if ((unsetenv(args[0])) != 0){
        perror("Removing env var failed");
        exit(EXIT_FAILURE);
      } else printf("Removing %s succesful\n",args[0]);
    } else if (counter == 2) {
      if ((setenv(args[0],args[1],1)) != 0){
        perror("Adding env var failed");
        exit(EXIT_FAILURE);
      } else printf("Adding %s succesful\n",args[0]);
    } else {
      perror("Too many arguments");
      exit(EXIT_FAILURE);
    }
}

void executeProg(char *line, int size){
  if (line[size] == '\n')
    line[size] = 0; // usun \n z line
  int counter = 0;
  char **args = splitString(line,&counter);
  char *program = args[0];

  int status;
  pid_t pid = fork();

  if (pid == 0){ //TODO : przekazywanie arguemtow nie dziala :/
    if (execv(program,args) == -1 && execvp(program,args) == -1){
      perror("Runing program failed");
      exit(EXIT_FAILURE);
    }
  } else if (pid > 0){
    wait(&status);
    if(WIFEXITED(status) && !WEXITSTATUS(status))
      printf("SUCCESFUL ==> PID: %d PROGRAM: %s EXITCODE: %d\n",pid,program,WEXITSTATUS(status));
    else{
      printf("FAILED ==> PID: %d PROGRAM: %s\n",pid,program);
      /*
      if (WIFSIGNALED(status))
        printf("If signaled ==> Exit signal %s\n",WTERMSIG(status));
        */
      exit(EXIT_FAILURE);
    }
  } else{
    perror("Creating process failed");
    exit(EXIT_FAILURE);
  }
}



// maksymalnie 5 argumentow
char **splitString(char *line, int *counter){
  char **args = malloc(5*sizeof(char));
  char *token;
  int i = 0;
  token = strtok(line," \n");
  while (token && i < 5){
    args[i] = malloc((strlen(token)+1)*sizeof(char));
    strcpy(args[i],token);
    i++;
    token = strtok(NULL," \n");
  }
  args[i] = NULL;
  *counter = i;
  return args;
}
