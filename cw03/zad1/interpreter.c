#include "interpreter.h"

int main(int argc, char *argv[]){

  if (argc != 2){
    printf("Bledna ilosc argumentow\n");
    exit(EXIT_FAILURE);
  }
  char *fileName;
  int openDesc;

  fileName = argv[1];
  if ((openDesc = open(fileName, O_RDR)) == 0){ // O_RDR ???
    perror("Opening file failed");
    exit(EXIT_FAILURE);
  }

  parse(openDesc);

  return 0;
}

void readChar(int openDesc,int offset){
    char tmp;

    lseek(openDesc, offset, SEEK_SET);
    size_t result = read(openDesc,&tmp, 1);

    if(result!=1)
        printf("Reading char failed\n");
    else
        printf("Znak to: %c\n",tmp);
}

void parseLine(){

}

void enviromentVariable(int mode){//mode = 1 ==> add , mode = 0 ==> delete
      /*
    int setenv(const char *name, const char *value, int overwrite);
    int unsetenv(const char *name);
      */

}

void executeProg(){
  //fork exec wait
  //int execve(const char *filename, char *const argv[],char *const envp[]);

}
