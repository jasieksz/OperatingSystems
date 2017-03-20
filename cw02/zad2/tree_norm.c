#include "tree_norm.h"

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Bledna ilosc argumentow");
        exit(EXIT_FAILURE);
    }

    char root[PATH_MAX + 1];
    int size = atoi(argv[2]);
    realpath(argv[1], root);
    printDirectory(root, size);
    return 0;
}

void printDirectory(char *filePath, int size) {
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
            if(S_ISREG(file.st_mode) && file.st_size < size) {
                strftime(timeBuffer, sizeof(timeBuffer), "%d.%m.%Y %H:%M:%S", localtime(&file.st_mtime));
                char *permisions = getPermissions(file);
                printf("\nPath: %s\n", newPath);
                printf("Size: %d\n", (int)file.st_size);
                printf("Rights : %s\n",permisions);
                printf("Last modified %s\n", timeBuffer);
                free(permisions);
            }
            if(S_ISDIR(file.st_mode)) {
                printDirectory(newPath, size);
            }
            free(newPath);
        }
    }
    closedir(pDir);
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
