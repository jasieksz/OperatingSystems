#include "tree.h"

//./program [path] [size]
int size;

int main(int argc, char *argv[]){
    if (argc != 3){
        perror("Bledna ilsoc arguemtnow");
        exit(EXIT_FAILURE);
    } else {
        char root[PATH_MAX + 1];
        size = atoi(argv[2]);
        realpath(argv[1], root);
        return printDirectory(root);
    }
return 0;
}

int printDirectory(char *currentPath) {
    int result = nftw(currentPath, conditionChecker,20, FTW_PHYS);
    return result;
  
}
/* 20 -> max number of dir nftw can open at the same time
 * fpath - the pathname of the entry
 * sb is a pointer to the stat structure returned by a call to stat to fpath
 * tflag one of following values: FTW_F (fpath is a regular FILE), FTW_D (fpath is a directory), FTW_DNR(file is a directory which can't be read)
 * ftwbuf -> struct with int base, int level. Base: offset of the file, level: depth in the directory tree
*/

int conditionChecker(const char *fpath, const struct stat *file, int tflag, struct FTW *ftwbuf) {
    if(tflag == FTW_F && file->st_size < size){
        printFileInfo(file, fpath);
    }
    return 0;
}


void printFileInfo(const struct stat *file, const char *newPath) {
    char date[10];
    char *permisions;

    strftime(date, 10, "%d-%m-%y", localtime(&((*file).st_mtime)));
    permisions = getPermissions((*file));

    printf("\nPath : %s\n",newPath);
    printf("Size : %i\n",(int)(*file).st_size);
    printf("Rights : %s\n",permisions);
    printf("Date modified : %s\n",date);
    free(permisions);
}

char *getPermissions(struct stat file) {
    char *permisions = calloc(10, sizeof(char));
    permisions[0] = (char) ((file.st_mode & 0040000) ? 'd' : '-');
    permisions[1] = (char) ((file.st_mode & 00400) ? 'r' : '-');
    permisions[2] = (char) ((file.st_mode & 00200) ? 'w' : '-');
    permisions[3] = (char) ((file.st_mode & 00100) ? 'x' : '-');
    permisions[4] = (char) ((file.st_mode & 00040) ? 'r' : '-');
    permisions[5] = (char) ((file.st_mode & 00020) ? 'w' : '-');
    permisions[6] = (char) ((file.st_mode & 00010) ? 'x' : '-');
    permisions[7] = (char) ((file.st_mode & 00004) ? 'r' : '-');
    permisions[8] = (char) ((file.st_mode & 00002) ? 'w' : '-');
    permisions[9] = (char) ((file.st_mode & 00001) ? 'x' : '-');
    return permisions;
}

/*
opendir readdir stat
struct timespec st_mtim;
FTW_PHYS --> do not follow symoblic links
*/
/*
plik.size <= size
ścieżka bezwzględna pliku
rozmiar w bajtach
prawa dostępu do pliku (w formacie używanym przez ls -l, np. r--r--rw- )
datę ostatniej modyfikacji
*/