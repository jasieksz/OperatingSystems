#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 200809L
//#define _GNU_SOURCE_

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
pid_t child;

int main(int argc, char *argv[]){

  if (argc != 3){
    perror("Invalid arguments");
    exit(EXIT_FAILURE);
  }

  L = atoi(argv[1]);
  type = atoi(argv[2]);
  counter = 0;

  struct sigaction paction;
  paction.sa_sigaction = &parentHandler;
  paction.sa_flags = SA_RESTART;
  sigemptyset(&paction.sa_mask);

  sigaction(SIGUSR1, &paction, NULL);
  sigaction(SIGINT, &paction, NULL);


  pid_t pid = fork();

  if (pid == 0){
    struct sigaction saction;
    saction.sa_sigaction = &childHandler;
    saction.sa_flags = (SA_SIGINFO | SA_RESTART);
    sigemptyset(&saction.sa_mask);
    sigaddset(&saction.sa_mask, SIGRTMAX);

    sigaction(SIGUSR1, &saction, NULL);
    sigaction(SIGUSR2, &saction, NULL);
    sigaction(SIGINT, &saction, NULL);
    sigaction(SIGRTMIN, &saction, NULL);
    sigaction(SIGRTMAX, &saction, NULL);

    while(1)
      sigsuspend(0);

    printf("%s\n","KONIEC DIZECKA");
  } else{
    sleep(1);
    child = pid;
    union sigval myVal;
      for (int i=0;i < L;i++){

        switch(type) {
          case (1):
            kill(pid,SIGUSR1);
            break;
          case (2):
            sigqueue(pid, SIGUSR1, myVal);
            break;
          case (3):
            kill(pid,SIGRTMIN);
            break;
        }
      }
      sleep(1);
      switch (type) {
        case (1):
          kill(pid,SIGUSR2);
          break;
        case (2):
          sigqueue(pid, SIGUSR2, myVal);
          break;
        case (3):
          kill(pid,SIGRTMAX);
          break;
      }

  }
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

void parentHandler(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1)
    counter++;
  else if (sig == SIGINT){
    //kill(child, SIGINT);
    //exit(1);
    printf("%s\n","PARENT RECEIVED SIGINT" );
  }
}
