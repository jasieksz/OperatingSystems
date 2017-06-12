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

//CONSTANTS
#define MAX_THREADS 10
#define RECORD_SIZE 1024
#define MAX_RR 10 //records per read

//METHODS
void* search(void *arg);
void kill_threads(pthread_t);
int get_record_id(); //returns records id based on offset
int fit(char*); //returns 1 if buffer matches pattern, 0 if not
void *badfun(void *);
//VARIABLES
char *path; //file path
char *pattern; //pattern to search for
int fd, n_rr;
pthread_mutex_t read_mutex;

int n_threads; //number of thread, records per read, file desc
pthread_t *threads;
int pause_threads = 1; //bool - wait for all threads to create

int main(int argc, char *argv[]){
  if(argc != 5) {
		perror("Wrong args - ./programA path pattern n_threads n_rec_read");
		exit(1);
	}

  path = argv[1];
  pattern = argv[2];
  n_threads = atoi(argv[3]);
  n_rr = atoi(argv[4]);
  /*
  * INITIALIZE
  */
  pthread_mutex_init(&read_mutex, NULL);
  if((fd = open(path, O_RDONLY)) == -1) {
      perror("open failure");
      exit(1);
  }
  /*
  * CREATE THREADS
  */
  threads = malloc(sizeof(pthread_t) * n_threads);
  for(int i = 0; i < n_threads; i++) {
      if(pthread_create(&threads[i], NULL, &search, (void *) &i) != 0) {
          perror("pthread_create failure");
          exit(EXIT_FAILURE);
      }
  }
  pthread_t bad_thread;
  pthread_create(&bad_thread, NULL, &badfun, NULL);

  printf("Threads ready\n");
  pause_threads = 0;


  for(int i = 0; i < n_threads; i++) {
      if(pthread_join(threads[i], NULL) != 0) {
          perror("pthread_join error");
          exit(EXIT_FAILURE);
      }
  }
  /*
  * FINISH IT
  */
  pthread_mutex_destroy(&read_mutex);
  close(fd);
  free(threads);

return 0;
}

void* search(void *arg) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // kill me anytime

  int loopexit = 0;
  ssize_t read_bytes;
  size_t buffer_size = sizeof(char) * RECORD_SIZE;
  char *buffer;
  buffer = malloc(buffer_size);

  while(pause_threads); // wait for all

  while (!loopexit){
    pthread_mutex_lock(&read_mutex);
    for (int i = 0; i < n_rr; i++){
      read_bytes = read(fd, buffer, buffer_size);
      if (read_bytes > 0 && fit(buffer) != 0){
        printf("TID: %ld Record ID: %d\n", (long) pthread_self(), get_record_id());
        loopexit = 1;
        kill_threads(pthread_self());
        pthread_exit(NULL);
      }
      if (read_bytes == 0)
        loopexit = 1;
    }
    pthread_mutex_unlock(&read_mutex);
  }
  pthread_exit(NULL);
}

void kill_threads(pthread_t tid) {
  for(int i = 0; i < n_threads; i++) {
      if(threads[i] != tid) {
          pthread_cancel(threads[i]);
      }
  }
  printf("%s\n","Threads cancelled");
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

void *badfun(void *arg) {
    int x = 1/0;
    return NULL;
}
