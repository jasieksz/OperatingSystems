#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE_

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void childHandler(int sig, siginfo_t *info, void *context);

int main(){
  time_t startTime,endTime;
  printf("CHILD PID: %d\n",getpid());
  //srand((unsigned int) getpid());
  sleep(3);
  //sleep((unsigned int) rand()%10);

  printf("SENDING SIGUSR FROM: %d\n",getpid());
  kill(getppid(), SIGUSR1);

  signal(SIGUSR1, childHandler);
  signal(SIGINT, childHandler);

  time(&startTime);
  pause();
  time(&endTime);

  printf("UNPAUSED PID: %d\n",getpid());
  int diffTime = (int) difftime(endTime,startTime);
  printf("TIME DIFF: %d\n",diffTime);
  exit(diffTime);
}

void childHandler(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1)
    kill(getppid(), SIGRTMIN + (rand() % (SIGRTMAX - SIGRTMIN))); //+1?
  else if (sig == SIGINT)
    exit(1);
}
