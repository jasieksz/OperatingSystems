#include "lock.h"

/*
ustawienie rygla do odczytu na wybrany znak pliku,
ustawienie rygla do zapisu na wybrany znak pliku,
wyświetlenie listy zaryglowanych znaków pliku (z informacją o numerze PID procesu będącego właścicielem rygla oraz jego typie - odczyt/zapis),
zwolnienie wybranego rygla,
odczyt (funkcją read) wybranego znaku pliku,
zmiana (funkcją write) wybranego znaku pliku.
*/

int mode;
int main(int argc, char* argv[]) {

    char *fileName;
    int openDesc;

    if(argc != 2 && argc != 3){
        printf("Bledna ilosc argumentow\n");
        exit(EXIT_FAILURE);
    }

    mode = (argc == 3 && strcmp(argv[2],"wait")==0) ? 1 : 0;

    char* fileName = argv[1];
    if ((openDesc = open(fileName, O_RDWR)) == 0){
        perror("Opening file failed");
        exit(EXIT_FAILURE);
    }

      printf("Co chcesz zrobic? wpisz:\n");
      printf("1 Ustaw read lock znak pliku\n");
      printf("2 Ustaw write lock znak pliku\n");
      printf("3 Wyswietl zaryglowane znaki pliku \n");
      printf("4 Usun locka\n");
      printf("5 Odczyt znaku pliku,\n");

      int option;
      int offset;
      while((option = getOption())){
        switch (option){
            case 1:
                printf("Podaj nr znaku\n");
                scanf("%d", &offset);
                setReadLock(openDesc,offset);
                break;
            case 2:
                printf("Podaj nr znaku\n");
                scanf("%d", &offset); 
                setWriteLock(openDesc,offset);
                break;
            case 3:
                printf("Podaj nr znaku\n");
                scanf("%d", &offset);
                printLocks(openDesc,offset);
                break;
            case 4:
                printf("Podaj nr znaku\n");
                scanf("%d", &offset);
                removeLock(openDesc,offset);
                break;
            case 5:
                printf("Podaj nr znaku\n");
                scanf("%d", &offset);
                readChar(openDesc,offset);
                break;
            case 6:
                printf("Podaj nr znaku\n");
                scanf("%d", &offset);
                writeChar(openDesc,offset);
                break; 
            default:
                printf("Nie ma takiej funkcji\n");
                close(openDesc);
                exit(EXIT_SUCCES);
                break;
        }
    }
}

void setReadLock (int openDesc,int offset){
    int result;
    if (mode) 
      result = executeLock(openDesc, F_SETLKW, F_RDLCK, offset);
    else
      result = executeLock(openDesc, F_SETLK, F_RDLCK, offset);
    
    checkError(result);  
}

void setWriteLock (int openDesc,int offset){
    int result;
    if (mode) 
      result = executeLock(openDesc, F_SETLKW, F_WRLCK, offset);
    else
      result = executeLock(openDesc, F_SETLK, F_WRLCK, offset);
    
    checkError(result);     
}
void removeLock(int openDesc,int offset){
  int result;
  result = execute_lock(open_descriptor, F_SETLK, F_UNLCK, offset);
  checkError(result);
}

void printLocks(int openDesc){
    size_t length;
    if ((length = lseek(openDesc, 0, SEEK_END)) < 0){
        perror("Finding end of file failed");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < fileSize; i++) {
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start=i;
        flock.l_len=1;
        flock.l_pid=2;

        int res = fcntl(desc, F_GETLK, &flock);
        if(res < 0){
            perror("Error reading lock");
            exit(EXIT_FAILURE);
        }

        if(flock.l_type != F_UNLCK){
            if (flock.l_type == F_RDLCK) printf("\nRead ");
            else if (flock.l_type == F_WRLCK) printf("\nWrite ");
            printf("lock on offset: %i, PID: %i \n", i, flock.l_pid)
        }
      }
}
void readChar(int openDesc,int offset){
    char tmp;

    lseek(openDesc, offset, SEEK_SET);
    size_t result = read(desc,&tmp, 1);

    if(result!=1) 
        printf("Reading char failed\n"); 
    else 
        printf("Znak to: %c\n",tmp);
}
void writeChar(int openDesc,int offset){
    char tmp;
    printf("Podaj znak : ");
    scanf(" %c", &tmp);

    lseek(openDesc, offset, SEEK_SET);
    size_t result = write(file, &tmp, 1);

    if(result!=1) 
        printf("Writing char failed\n"); 
    else 
        printf("Wpisano : %c\n",tmp);
}

int executeLock(int openDesc, int f, int type, int offset) {
    struct flock flock;
    flock.l_type = type; 
    flock.l_whence = SEEK_SET;// poczatek
    flock.l_start = offset;
    flock.l_len = 1;
    flock.l_pid=getpid();
    return fcntl(open_descriptor, f, &flock);
}

void checkError(int result){
  if(result == -1) {
    printf("Setting lock failed\n");
    exit(EXIT_FAILURE);
  } else {
    printf("Lock successfull\n");
  }
}

int get_option() {
    int option;
    printf("\nMy choice: ");
    scanf("%d", &option);
    return option;
}