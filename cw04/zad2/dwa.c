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

int n,k,ktmp,index; //n k current counter
pid_t *procQ; //continue
pid_t *children;

void childAction();
void parentAction();
void parentHandler(int sig, siginfo_t *info, void *context);
void parentHandler2(int sig, siginfo_t *info, void *context);

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
    childAction();
  else
	  parentAction();

  return 0;
}

void childAction(){
  execl("./child",NULL);
}


void parentAction(){
  struct sigaction saction;
  memset(&saction, '\0', sizeof(saction));
  saction.sa_sigaction = &parentHandler2;
  saction.sa_flags = SA_SIGINFO;
  sigemptyset(&saction.sa_mask);

  sigaction(SIGUSR1, &saction, NULL);
  sigaction(SIGINT, &saction, NULL);
  sigaction(SIGCHLD, &saction, NULL);
  for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
    sigaction(i, &saction, NULL);

  while (wait(NULL))
    if (errno == ECHILD)
      break;
}

void parentHandler2(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1){
    printf("CAUGHT SIGUSR1 FROM: %d\n",info->si_pid);
  } else if (sig == SIGINT){
    printf("CAUGHT SIGINT FROM: %d\n",info->si_pid);
  } else {
    printf("%s\n","COS INNEGO AABASJFAKSF");
  }

}

void parentHandler(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1){
    printf("CAUGHT SIGUSR1 FROM: %d\n",info->si_pid);
    procQ[ktmp + k] = info->si_pid;
    ktmp++;
    if (ktmp == 0){
      for (int i=0; i < k; i++){
        kill(procQ[i], SIGUSR1);
        waitpid(procQ[i],NULL,0);
      }
    }
  } else if (sig == SIGINT){
    for (int i=0; i < n;i++){
      kill(children[i],SIGINT);
    }
    exit(1);
  } else if (sig == SIGCHLD){
    printf("PROC: %d ENDED WITH: %d\n",info->si_pid,info->si_status);
  } else if (sig >= SIGRTMIN && sig <= SIGRTMAX){
    printf("CAUGHT SIGNAL: %d\n",sig);
  }
}

/*
void createProc(){
  pid_t *children = malloc(sizeof(pid_t)*n);
  pid_t *procQ = malloc(sizeof(pid_t)*n);
  int pid = 1;

  printf("%s\n","TEST 111111");
  for (int i = 0; i < n; i++){
    if (pid > 0){
      pid = fork();
      children[i] = pid;
      printf("NEW CHILD : %d\n",pid);
    } else if (pid < 0){
      perror("Fork failed");
      exit(EXIT_FAILURE);
    }
  }
  printf("%s PID : %d\n","TEST 2222",pid);
  if (pid == 0){
    parentAction();
  } else
	 childAction2();
}

void childAction(){

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

void childAction2(){

  struct sigaction saction;
  memset(&saction, '\0', sizeof(saction));
  saction.sa_sigaction = &childHandler;
  saction.sa_flags = SA_SIGINFO;
  sigemptyset(&saction.sa_mask);

  time_t startTime,endTime;
  printf("CHILD PID: %d\n",getpid());
  //srand((unsigned int) getpid());
  sleep(3);
  //sleep((unsigned int) rand()%10);

  printf("SENDING SIGUSR FROM: %d\n",getpid());
  kill(getppid(), SIGUSR1);

  //signal(SIGUSR1, childHandler);
  //signal(SIGINT, childHandler);
  sigaction(SIGUSR1,&saction,NULL);
  sigaction(SIGINT,&saction,NULL);

  time(&startTime);
  pause();
  time(&endTime);

  printf("UNPAUSED PID: %d\n",getpid());
  int diffTime = (int) difftime(endTime,startTime);
  printf("TIME DIFF: %d\n",diffTime);
  exit(diffTime);
}

void childHandler(int sig, siginfo_t *info, void *context){
  if (sig == SIGUSR1){
    //kill(getppid(),SIGTSTP);
    kill(getppid(), SIGRTMIN + (rand() % (SIGRTMAX - SIGRTMIN + 1)));
  } else if (sig == SIGINT) {
    exit(1);
  }
}
*/
