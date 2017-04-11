#define _XOPEN_SOURCE
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

int n,k,ktmp,index; //n k currentcounter counter of procq
int *procQ; //continue
int *children;

void parentAction();
void parentHandler(int sig, siginfo_t *info, void *context);

int main(int argc, char *argv[]){

  if (argc != 3){
    perror("Invalid arguments");
    exit(EXIT_FAILURE);
  }

  n = atoi(argv[1]);
  k = atoi(argv[2]);

  children = calloc((size_t) n, sizeof(int *));
  procQ = calloc((size_t) n, sizeof(int *));
  int pid = 1;

  for (int i = 0; i < n; i++){
    if (pid > 0){
      pid = fork();
      children[i] = pid;
    } else if (pid < 0){
      perror("Fork failed");
      exit(EXIT_FAILURE);
    }
  }
  if (pid == 0)
    execle("./child",NULL);
  else
	  parentAction();
  return 0;
}

void parentAction(){
  struct sigaction saction;
  saction.sa_sigaction = &parentHandler;
  saction.sa_flags = SA_SIGINFO;
  sigemptyset(&saction.sa_mask);

  sigaction(SIGUSR1, &saction, NULL);
  sigaction(SIGINT, &saction, NULL);
  sigaction(SIGCHLD, &saction, NULL);
  for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
    sigaction(i, &saction, NULL);

  while (wait(NULL))
    if (errno == ECHILD) //calling process doesnt hace any unwaited for children
      break;
}

int contains(int *A, int x){
  for (int i=0; i < sizeof(A)/sizeof(A[0]); i++)
    if (A[i] == x)
      return 1;
  return 0;
}

void parentHandler(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1){
    printf("CAUGHT SIGUSR1 FROM: %d\n",info->si_pid);
    if (!contains(procQ,info->si_pid))
      procQ[index++] = info->si_pid;

    if(ktmp == k)
      kill(info->si_pid, SIGUSR1);
    else {
      ktmp++;
      if (ktmp == k){
        for (int i=0; i < index; i++){
          kill(procQ[i], SIGUSR1);
          waitpid(procQ[i],NULL,0);
        }
      }
    }
  } else if (sig == SIGINT){
    for (int i=0; i < n; i++)
      kill(children[i],SIGINT);
    exit(1);
  } else if (sig == SIGCHLD){
    printf("PROC: %d ENDED WITH: %d\n",info->si_pid,info->si_status);
  } else if (sig >= SIGRTMIN && sig <= SIGRTMAX){
    printf("CAUGHT SIGNAL (SIGRT): %d\n",sig);
  }
}
