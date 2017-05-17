#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>

#define RMAX 1
#define RMIN -2
#define IMAX 1
#define IMIN -1

int R;

void setData(int **T);

int nslaves = 0;
int S;
//volatile int fifoReady = 0;

void rtHandler(int signo){
  if(signo == SIGRTMAX)
    nslaves++;
}

int main(int argc, char *argv[]){

  if (argc != 6){ //./master path R Slave_num N K
    perror("Wrong number of args");
    exit(0);
  }
  char *path = argv[1];
  R = atoi(argv[2]);
  S = atoi(argv[3]);
  char *N = argv[4];
  char *K = argv[5];

  if(signal(SIGRTMAX, rtHandler) == SIG_ERR){
    perror("Sig handler failed");
    exit(EXIT_FAILURE);
  }

  if(mkfifo(argv[1], 0666) == -1){ //S_IRUSR| S_IWUSR
    perror("FIFO failed");
    exit(EXIT_FAILURE);
  }
  sleep(1);
//todo
  char* slave_args[5];
  slave_args[0] = "./slave";
  slave_args[1] = path;
  slave_args[2] = N;
  slave_args[3] = K;
  slave_args[4] = NULL;

  //int slave_num = S;

  for(int i=0; i<S; i++) {
    int pid = fork();
    if(pid == -1){
      perror("Fork failed");
      exit(EXIT_FAILURE);
    } else if(pid == 0){
      execvp(slave_args[0], slave_args);
      return 0; //2
    }
  }

  FILE *pip = fopen(path,"r");
  if (pip == NULL){
    perror("Fopen failed");
    exit(1);
  }

  while(nslaves != S){
    sleep(1);
  }
  printf("Slaves ready.\n");

  int **T = calloc(R, sizeof(int*));
  for (int i=0; i < R; i++)
    T[i] = calloc(R, sizeof(int));

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while (1){
    double re = 0;
    double im = 0;
    int iter = 0;
    while ((read = getline(&line,&len,pip)) != -1){
      if (read >= 1){
        sscanf(line, "%lf %lf %i\n", &re, &im, &iter);
        if (re > RMIN && re < RMAX && im > IMIN && im < IMAX){
          int x = (int)((re+2)*(R/3));
          int y = (int)((im+1.0)*(R/2));
          T[x][y] = iter;
        }
      }
    }
    sleep(2);
    if ((read = getline(&line,&len,pip)) != -1){
      sscanf(line, "%lf %lf %i\n", &re, &im, &iter);
      if (re > RMIN && re < RMAX && im > IMIN && im < IMAX){
        int x = (int)((re+2)*(R/3));
        int y = (int)((im+1.0)*(R/2));
        T[x][y] = iter;
      }
    } else
      break;
  }

  setData(T);

  fclose(pip);
  unlink(path);



  FILE *gplot = popen("gnuplot","w");
  fprintf(gplot,"set view map\n");
  fprintf(gplot,"set xrange[0:%d]\n",R);
  fprintf(gplot,"set yrange[0:%d]\n",R);
  fprintf(gplot,"plot 'data' with image\n");
  fflush(gplot);
  getc(stdin);
  pclose(gplot);

  return 0;
}

void setData(int **T){
  FILE *data = fopen("data","w");
  if (data == NULL){
    perror("Writing data failed");
    exit(1);
  }
  for (int i=0;i<R;i++)
    for (int j=0;j<R;j++){
      if (T[i][j] <= 100 && T[i][j] > 0)
      fprintf(data,"%d %d %d\n",i,j,T[i][j]);
      else
        fprintf(data,"%d %d %d\n",i,j,0);
    }
  fclose(data);
}
