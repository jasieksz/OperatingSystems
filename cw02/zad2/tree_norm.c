#include <wait.h>
#include <time.h>
#include "tree_norm.h"

char com;
time_t time1;

int main(int argc, char *argv[]){
    if (argc != 4) {
        printf("Bledna ilosc argumentow");
        exit(EXIT_FAILURE);
    }

    char root[PATH_MAX + 1];
    realpath(argv[1], root);
    com = argv[2][0];
    char *stime = argv[3];
    struct tm tm;
    strptime(stime, "%Y-%m-%d", &tm);
    time1 = mktime(&tm);
    printDirectory(root);

    return 0;
}

void printDirectory(char *filePath) {
    struct dirent *dirent;
    char *newPath;
    struct stat file;
    DIR *pDir;
    char timeBuffer[20];

    pDir = opendir(filePath);
    if (pDir == NULL) {
        printf("Opening directory failed\n");
        exit(EXIT_FAILURE);
    }

    while ((dirent = readdir(pDir)) != NULL) {
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
           newPath = malloc(strlen(filePath) + strlen(dirent->d_name) + 2);
            sprintf(newPath, "%s/%s", filePath, dirent->d_name);
            if (stat(newPath, &file) < 0) {
                printf("Geting stats failed\n");
                exit(EXIT_FAILURE);
            }
            if(S_ISREG(file.st_mode)) {
                double dt = difftime(file.st_mtime, time1); // mtime - time1
                if (com == '<' && dt < 0){
                    printFileInfo(file, newPath);
                } else if (com == '>' && dt > 0){
                    printFileInfo(file, newPath);
                } else if (com == '=' && dt == 0){
                    printFileInfo(file, newPath);
                }
            }
            if(S_ISDIR(file.st_mode)) {
                pid_t pid = fork();
                if (pid == 0){ //child
                    printf("CHILD\n");
                    printDirectory(newPath);
                    exit(EXIT_SUCCESS);
                } else if (pid > 0){
                    int status;
                    wait(&status);

                    printf("PARENT\n");
                }
            }
            free(newPath);
        }
    }
    closedir(pDir);
}

void printFileInfo(const struct stat file, const char *newPath) {
    char timeBuffer[20];
    char *permisions;
    strftime(timeBuffer, 20, "%d.%m.%Y %H:%M:%S", localtime(&((file).st_mtime)));
    permisions = getPermissions((file));

    printf("\nPath : %s\n",newPath);
    printf("Size : %i\n",(int)(file).st_size);
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
