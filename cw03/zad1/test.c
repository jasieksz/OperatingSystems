#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[]){
  //test zmienna
  if (argc != 2){
    perror("Wrong arguments");
    exit(EXIT_FAILURE);
  }

  const char *env = getenv(argv[1]);
  if (env == NULL){
    printf("%s\n","Variable doesnt exist");
  } else {
    printf("\tZmienna %s, wartosc %s\n",argv[1],env);
  }
  return 0;
}
