#include "tree_norm.h"

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Bledna ilosc argumentow");
        exit(EXIT_FAILURE);
    }

    char root[PATH_MAX + 1];
    size = atoi(argv[2]);
    realpath(argv[1], root);
    list_directory(root, size);
    return 0;
}
/*
void printDirectory(char *filePath, int size){
    DIR *directory;
    struct dirent *dir;
    struct stat file;

    directory = opendir(filePath);
    if (directory == NULL){ 
        printf("Opening directory failed\n");
        exit(EXIT_FAILURE);
    };

    while ((dir = readdir(directory)) != NULL) {
        char *tmpPath = malloc((strlen(filePath) + strlen(dir->d_name) + 2) * sizeof(char));
        strcpy(tmpPath, filePath);
        if (filePath[strlen(filePath) - 1] != '/') strcat(tmpPath, "/");
        strcat(tmpPath, dir->d_name);
	    if (dir->d_type == DT_DIR && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
            search(tmpPath, size);
        } else  {
            int result = lstat(tmpPath, &file);
            if (result < 0) error();
            if (S_ISDIR(file.st_mode) && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
                search(tmpPath, size);
            } else if (S_ISREG(file.st_mode) && file.st_size <= size) {
                print_info(&file, tmpPath);
            }
        }
        free(tmpPath);
    }
}
*/
void list_directory(char *path, int size) {
    struct dirent *dirent;
    char *file_path;
    struct stat file_stat;
    int stat_result;
    DIR *pDir;
    char time_buffer[20];

    pDir = opendir(path);
    if (pDir == NULL) {
        printf("Error while opening the directory\n");
        exit(1);
    }

    while ((dirent = readdir(pDir)) != NULL) {
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            /* allocates memory for concatened path string and dirent name.
             2 additional bytes for 2*(\n) */
           file_path = malloc(strlen(path) + strlen(dirent->d_name) + 2);
            /* writes path, dirent->d_name to file_path string */
            sprintf(file_path, "%s/%s", path, dirent->d_name);
            /* retrievs stats about file_path and saves to file_stat */
            stat_result = stat(file_path, &file_stat);
            if (stat_result < 0) {
                printf("Error while retrieving stats\n");
                exit(1);
            }
            if(S_ISREG(file_stat.st_mode) && file_stat.st_size < size) {
                strftime(time_buffer, sizeof(time_buffer), "%d.%m.%Y %H:%M:%S", localtime(&file_stat.st_atime));
                printf("File name: %s\n", file_path);
                printf("File size in bytes: %d\n", (int)file_stat.st_size);
                printf("Last access %s\n", time_buffer);
                printf("----------------------\n");
            }
            if(S_ISDIR(file_stat.st_mode)) {
                /* recursively execute this function for
                 every directory found in tree */
                list_directory(file_path, size);
            }
            free(file_path);
        }
    }
    closedir(pDir);
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
