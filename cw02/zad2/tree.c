#include "tree.h"

//./program [path] [size]
char com;
time_t time1;

int main(int argc, char *argv[]){
    if (argc != 4){
        perror("Bledna ilsoc arguemtnow - ./nftwprogram path {<,>,=} data");
        exit(EXIT_FAILURE);
    } else {
        char root[PATH_MAX + 1];
        realpath(argv[1], root);
        com = argv[2][0];
        char *stime = argv[3];
        struct tm tm;
        strptime(stime, "%Y-%m-%d", &tm);
        time1 = mktime(&tm);

        return printDirectory(root);

    }
return 0;
}

int printDirectory(char *currentPath) {
    int result = nftw(currentPath, conditionChecker,20, FTW_PHYS);
    return result;

}

int conditionChecker(const char *fpath, const struct stat *file, int tflag, struct FTW *ftwbuf) {
    if(tflag == FTW_F){
        double dt = difftime(file->st_mtime, time1); // mtime - time1
        if (com == '<' && dt < 0){
            printFileInfo(file, fpath);
        } else if (com == '>' && dt > 0){
            printFileInfo(file, fpath);
        } else if (com == '=' && dt == 0){
            printFileInfo(file, fpath);
        }
    }
    return 0;
}


void printFileInfo(const struct stat *file, const char *newPath) {
    char timeBuffer[20];
    char *permisions;
    strftime(timeBuffer, 20, "%d.%m.%Y %H:%M:%S", localtime(&((*file).st_mtime)));
    permisions = getPermissions((*file));

    printf("\nPath : %s\n",newPath);
    printf("Size : %i\n",(int)(*file).st_size);
    printf("Rights : %s\n",permisions);
    printf("Date modified : %s\n",timeBuffer);
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
 struct timespec st_mtim;
 FTW_PHYS --> do not follow symoblic links
 20 -> max number of dir nftw can open at the same time
 fpath - the pathname of the entry
 sb is a pointer to the stat structure returned by a call to stat to fpath
 tflag one of following values: FTW_F (fpath is a regular FILE), FTW_D (fpath is a directory), FTW_DNR(file is a directory which can't be read)
 ftwbuf -> struct with int base, int level. Base: offset of the file, level: depth in the directory tree
*/
