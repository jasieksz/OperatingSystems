#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define RMAX 1
#define RMIN -2
#define IMAX 1
#define IMIN -1
#define K 100

int R;

void setData(int **T);

int main(int argc, char *argv[]){

  if (argc != 3){
    perror("Wrong number of args");
    exit(0);
  }
  char *path = argv[1];
  R = atoi(argv[2]);

  mkfifo(path, 0666);
  FILE *pip = fopen(path,"r");
  if (pip == NULL){
    perror("Fopen failed");
    exit(1);
  }

  int **T = calloc(R, sizeof(int*));
  for (int i=0; i < R; i++) T[i] = calloc(R, sizeof(int));

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line,&len,pip)) != -1){
    if (read >= 1){
      double re = 0;
      double im = 0;
      int iter = 0;
      sscanf(line, "%lf %lf %i\n", &re, &im, &iter);
      if (re > RMIN && re < RMAX && im > IMIN && im < IMAX)
        T[(int)((re+2)*(R/3))][(int)((im+1.0)*(R/2))] = iter;
    }
  }

  fclose(pip);
  unlink(path);

  setData(T);

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
    for (int j=0;j<R;j++)
      fprintf(data,"%d %d %d\n",i,j,T[i][j]);
  fclose(data);
}
