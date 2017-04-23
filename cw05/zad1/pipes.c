#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CMDS 20
#define ARGS 5
#define PDEL "|"
#define WDEL " \n"
int lastCmd = 0;

void parse(char *line);
char **splitString(char *line, char *deli, int *counter);
void execute(char *cmd, int *fd, int in);

int main(int argc, char *argv[]){
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, stdin)) != -1)
    parse(line);

  return 0;
}

void parse(char *line){
  //printf("%s\n","TEST 2 - PARSE PARSE PARSE");
  int cmdCounter; cmdCounter = 0;
  char **cmds = splitString(line,PDEL,&cmdCounter);

  int fd[2]; // odczyt , zapis
  pipe(fd);
  int prevOut = STDIN_FILENO;

  for (int i=0; i < cmdCounter; i++){ //foreach cmd -> pipe
    pipe(fd);
    if (i == cmdCounter-1)
      lastCmd = 1;
    execute(cmds[i],fd,prevOut);
    prevOut = fd[0];
    close(fd[1]);
  }
  close(fd[0]);
  free(cmds);
}

void execute(char *cmd, int *fd, int in){
  int argCounter; argCounter = 0;
  char **args = splitString(cmd,WDEL,&argCounter);

  switch(fork()){
    case -1:
      perror("Fork failed");
      exit(1);
    case 0: // child
      if (in != -1)
        dup2(in,0);
      if (!lastCmd)
        dup2(fd[1],1);
      if (execvp(args[0],args) == -1){
        perror("Executing cmd failed");
        exit(1);
      }
    default: { //parent
      close(fd[1]);
      dup2(fd[0],0);
      wait(NULL);
      if (in != -1)
        close(in);
    }
  }
  free(args);
}

char **splitString(char *line, char *deli, int *counter){
  int number = (deli == PDEL) ? CMDS : ARGS;
  //printf("SPLIT STRING NUMBER : %d\n",number);
  char **args = malloc(number+1 * sizeof(char *));
  char *token;
  int i = 0;

  token = strtok(line,deli);
  while (token && i < number){
    args[i] = malloc((strlen(token)+1)*sizeof(char));
    strcpy(args[i],token);
    i++;
    token = strtok(NULL,deli);
  }
  args[i] = NULL;
  *counter = i;
  //printf("SPLIT STRING COUNTER : %d\n",i);
  return args;
}
