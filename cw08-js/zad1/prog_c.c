#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

//CONSTANTS
#define MAX_THREADS 100
#define RECORD_SIZE 1024
#define MAX_RR 10 //records per read

//METHODS
void* search(void *arg);
//void kill_threads(pthread_t);
int get_record_id(); //returns records id based on offset
int fit(char*); //returns 1 if buffer matches pattern, 0 if not

//VARIABLES
char *path; //file path
char *pattern; //pattern to search for
int fd, n_rr;
pthread_mutex_t read_mutex;

int n_threads; //number of thread, records per read, file desc
pthread_t *threads;
int pause_threads = 1; //bool - wait for all threads to create
int threads_killed = 0;
sem_t* semaphore;

int main(int argc, char *argv[]){
  if(argc != 5) {
		perror("Wrong args - ./programC path pattern n_threads n_rec_read");
		exit(1);
	}

  path = argv[1];
  pattern = argv[2];
  n_threads = atoi(argv[3]);
  n_rr = atoi(argv[4]);
  /*
  * INITIALIZE
  */
  pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_mutex_init(&read_mutex, NULL);
  if((fd = open(path, O_RDONLY)) == -1) {
      perror("open failed");
      exit(1);
  }
  if( (semaphore = sem_open("/sem", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
    fprintf(stderr, "sem_open error %s\n", strerror(errno));
    exit(1);
  }
  /*
  * CREATE THREADS
  */
  threads = malloc(sizeof(pthread_t) * n_threads);
  for (int i = 0; i < n_threads; i++) {
      if(pthread_create(&threads[i], &attr, &search, (void *) &i) != 0) {
          perror("pthread_create failure");
          exit(EXIT_FAILURE);
      }
      pthread_detach(threads[i]);
  }
  printf("Threads ready\n");
  pause_threads = 0;

  for (int i = 0; i < n_threads; i++){
    sem_wait(semaphore);
  }
  /*
  * FINISH IT
  */
  if(sem_close(semaphore) < 0) {
		fprintf(stderr, "sem close error %s\n", strerror(errno));
		exit(1);
	}
	if(sem_unlink("/sem") < 0) {
		fprintf(stderr, "sem unlink error %s\n", strerror(errno));
		exit(1);
	}
  pthread_mutex_destroy(&read_mutex);
  close(fd);
  free(threads);

return 0;
}

void* search(void *arg) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  int loopexit = 0;
  ssize_t read_bytes;
  size_t buffer_size = sizeof(char) * RECORD_SIZE;
  char *buffer;
  char **buf = malloc(buffer_size * n_rr);
  buffer = malloc(buffer_size);

  while(pause_threads); // wait for all

  while (!loopexit){
    pthread_mutex_lock(&read_mutex);
    for (int i = 0; i < n_rr; i++){
      read_bytes = read(fd, buffer, buffer_size);
      if (read_bytes == 0){
        loopexit = 1;
        break;
      } else
        buf[i] = buffer;
    }

    for (int i = 0; i < n_rr; i++){
      buffer = buf[i];
      if (buffer != NULL && fit(buffer) != 0){
        printf("TID: %ld Record ID: %d\n", (long) pthread_self(), get_record_id());
      }
    }
    pthread_mutex_unlock(&read_mutex);

  }
  free(buf);
  free(buffer);
  sem_post(semaphore);
  pthread_exit(NULL);
}

int fit(char *buffer) {
  char *res = strstr (buffer, pattern);
  if (res != NULL)
    return 1;
  return 0;
}

int get_record_id() {
  off_t pos;
  pos = lseek(fd, 0, SEEK_CUR);
  return (pos / RECORD_SIZE);
}
