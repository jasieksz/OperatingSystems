#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <sys/wait.h>

void childAction();
void parentAction(pid_t child);
void childHandler(int sig);


int L,type,counter;

int main(int argc, char *argv[]){

  if (argc != 3){
    perror("Invalid arguments");
    exit(EXIT_FAILURE);
  }

  L = atoi(argv[1]);
  type = atoi(argv[2]);

  pid_t pid = fork();
  if (pid == 0)
    childAction();
  else
    parentAction(pid);
}

void childAction(){
  singal(SIGUSR1, childHandler);
  singal(SIGUSR2, childHandler);
  singal(SIGINT, childHandler);
  singal(SIGRTMIN, childHandler);
  singal(SIGRTMAX, childHandler);

  pause();
}

void childHandler(int sig){
  if (sig == SIGUSR1){
    counter += 1;
    kill(getppid(),SIGUSR1);
  } else if (sig == SIGUSR2){
    printf("CAUGHT SIGUSR2, COUNTER : %d\n",counter);
    exit(0);
  } else if (sig == SIGRTMIN){
    counter++;
    kill(getppid(),SIGUSR1);
  } else if (sig == SIGRTMAX){
    printf("CAUGHT SIGRTMAX, COUNTER : %d\n",counter);
    exit(0);
  } else if (sig == SIGINT)
    exit(1);
}

void parentAction(pid_t child){
  signal(SIGUSR1, parentHandler);
  signal(SIGINT, parentHandler);

  union sigval myVal;

  for (int i=0;i < L;i++){
    switch(type) {
      case (1):
        kill(child,SIGUSR1);
        break;
      case (2):
        sigqueue(child, SIGUSR1, myVal);
        break;
      case (3):
        kill(child,SIGRTMIN);
        break;
    }
  }



}

















a
