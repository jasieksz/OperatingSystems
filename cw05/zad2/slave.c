#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <time.h>

#define RMAX 1
#define RMIN -2
#define IMAX 1
#define IMIN -1

void writePoints(int fd, int K);
int getIter(double complex c, int K);

int main(int argc, char *argv[]){

  if (argc != 4){
    perror("Wrong number of args");
    exit(0);
  }
  char *path = argv[1];
  int N = atoi(argv[2]);
  int K = atoi(argv[3]);

  srand(time(NULL));
  int fd;
  if ((fd = open(path,O_WRONLY)) < 0){
    perror("Opening failed");
    exit(1);
  }

  for (int i=0; i < N; i++)
    writePoint(fd,K);
  close(fd);

  return 0;
}

void writePoint(int fd, int K){
  char *buf = (char *) malloc(100 * sizeof(char));

  double re = ((double) rand() / (double) RAND_MAX) * (double)(RMAX - RMIN) + RMIN;
  double im = ((double) rand() / (double) RAND_MAX) * (double)(IMAX - IMIN) + IMIN;
  double complex c = re + im * I;

  int iter = getIter(c, K);

  sprintf(buf, "%lf %lf %i\n", re, im, it); //if ?
  write(fd,buf,100);
  free(buf);
}

int getIter(double complex c, int K){
  double complex tmp = 0.0;
  for(int i=0; i < K; i++){
    tmp = tmp * tmp + c;
    if (cabs(tmp) >= 2)
      return i;
  }
  return K;
}
