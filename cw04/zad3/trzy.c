#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE_

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>


void childAction();
void parentAction(pid_t child);
void childHandler(int sig, siginfo_t *info, void *context);
void parentHandler(int sig, siginfo_t *info, void *context);


int L,type,counter,sent;
pid_t childPid;

int main(int argc, char *argv[]){

  if (argc != 3){
    perror("Invalid arguments");
    exit(EXIT_FAILURE);
  }

  L = atoi(argv[1]);
  type = atoi(argv[2]);
  counter = 0;
  pid_t pid = fork();
  if (pid == 0){
    childAction();
  } else{
    childPid = pid;
    parentAction(pid);
  }
}

void childAction(){
  struct sigaction saction;
  saction.sa_sigaction = &childHandler;
  saction.sa_handler = &childHandler;
  saction.sa_flags = SA_SIGINFO | SA_RESTART;
  sigemptyset(&saction.sa_mask);

  sigaction(SIGUSR1, &saction, NULL);
  sigaction(SIGUSR2, &saction, NULL);
  sigaction(SIGINT, &saction, NULL);
  sigaction(SIGRTMIN, &saction, NULL);
  sigaction(SIGRTMAX, &saction, NULL);
}

void childHandler(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1){
    counter++;
    kill(info->si_pid,SIGUSR1); //ppid
  } else if (sig == SIGUSR2){
    printf("CAUGHT SIGUSR2, COUNTER : %d\n",counter);
    exit(0);
  } else if (sig == SIGRTMIN){
    counter++;
    kill(info->si_pid,SIGUSR1);
  } else if (sig == SIGRTMAX){
    printf("CAUGHT SIGRTMAX, COUNTER : %d\n",counter);
    exit(0);
  } else if (sig == SIGINT)
    exit(1);
}

void parentAction(pid_t child){
  struct sigaction saction;
  saction.sa_sigaction = &parentHandler;
  saction.sa_handler = &parentHandler;
  saction.sa_flags = SA_RESTART;
  sigemptyset(&saction.sa_mask);

  sigaction(SIGUSR1, &saction, NULL);
  sigaction(SIGINT, &saction, NULL);

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


  switch (type) {
    case (1):
      kill(child,SIGUSR2);
      break;
    case (2):
      sigqueue(child, SIGUSR2, myVal);
      break;
    case (3):
      kill(child,SIGRTMAX);
      break;
  }

  while (wait(NULL)){
    if (errno == ECHILD)
      break;
  }

  printf("SENT SIGUSR1 : %d CAUGHT %d\n",L,counter);
}

void parentHandler(int sig, siginfo_t *info, void *context){
  printf("%s\n","PARENT HANDLER" );
  if (sig == SIGUSR1)
    counter++;
  else if (sig == SIGINT){
    kill(childPid, SIGINT);
    exit(1);
  }
}
