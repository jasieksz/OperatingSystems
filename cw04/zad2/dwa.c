#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>

int n,k,ktmp;
pid_t *procQ;

void createProc();
void getRequest(int sig, siginfo_t *siginfo, void *context);
void getPerm(int sig);

int main(int argc, char *argv[]){

  if (argc != 3){
    perror("Invalid arguments");
    exit(EXIT_FAILURE);
  }

  n = atoi(argv[1]);
  k = atoi(argv[2]);
  ktmp = -k;

  procQ = malloc(sizeof(pid_t)*k);
  signal(SIGUSR2,getPerm);
  createProc();
}

void getPerm(int sig){
  if (sig == SIGUSR2)
      printf("PID: %d got permission\n",getpid());
}


void getRequest(int sig, siginfo_t *siginfo, void *context){
  printf("%d got request from %d\n",getpid(),siginfo->si_pid);

  procQ[ktmp+k] = siginfo->si_pid;
  ktmp += 1;
  if (ktmp == 0)
    for (int i = 0; i < k; i++)
      kill(procQ[i],SIGUSR2); //send permission
  if (ktmp > 0)
    kill(siginfo->si_pid,SIGUSR2);
}

void createProc(){

  pid_t *children = malloc(sizeof(pid_t)*n);
  printf("Parent PID: %d\n",getpid());

  struct sigaction saction;
  saction.sa_sigaction = getRequest;
  saction.sa_flags = SA_SIGINFO;

  for (int i = 0; i < n; i++){
    pid_t pid = fork();
    if (pid > 0){ //in parent
      sigaction(SIGUSR1,&saction,NULL);

    } else if (pid == 0) { // in child
      children[i] = pid;

      printf("Child PID: %d\n",pid);
      sleep(1);

      kill(getppid(),SIGUSR1); // send request

    } else {
      perror("Fork failed");
      exit(EXIT_FAILURE);
    }
  }
}
