#include"seek.h"


void seek_helper(char* target, char* dir_path, int flags, int* file_count, int* dir_count) {
    DIR* dp = opendir(dir_path);
    if (dp == NULL) {
        printf("Cannot open directory: %s\n", dir_path);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char abs_path[4096];
        snprintf(abs_path, sizeof(abs_path), "%s/%s", dir_path, entry->d_name);
        struct stat buff;
        stat(abs_path, &buff);

     
        if (S_ISDIR(buff.st_mode)) {
            seek_helper(target, abs_path, flags, file_count, dir_count);
        }

// logic of flags impkementation by performing ORS: taken from GPT
   
        if (strncmp(target, entry->d_name, strlen(target)) == 0 &&
            (entry->d_name[strlen(target)] == '\0' || entry->d_name[strlen(target)] == '.')) {
            
            if ((flags & 1) && !S_ISDIR(buff.st_mode)) {
                continue;
            }
            if ((flags & 2) && !S_ISREG(buff.st_mode)) {
                continue;
            }

            if (S_ISDIR(buff.st_mode)) {
                printf(COLOR_DIR "Directory: %s\n" COLOR_RESET, abs_path);
                (*dir_count)++;
            } else if (S_ISREG(buff.st_mode)) {
                printf(COLOR_FILE "File: %s\n" COLOR_RESET, abs_path);
                (*file_count)++;
            }
        }
    }

    closedir(dp);
}

void seek(char** args) {
    int flags = 0;
    int i = 0;
    char* target = NULL;
    char* dir_path = ".";

    while (args[i] != NULL) {
        if (strcmp(args[i], "-d") == 0) {
            flags |= 1;
        } else if (strcmp(args[i], "-f") == 0) {
            flags |= 2;
        } else if (strcmp(args[i], "-e") == 0) {
            flags |= 4;
        } else if (target == NULL) {
            target = args[i];
        } else {
            dir_path = args[i];
        }
        i++;
    }


    if ((flags & 1) && (flags & 2)) {
        printf("Invalid Flags!\n");
        return;
    }

    int file_count = 0;
    int dir_count = 0;

    seek_helper(target, dir_path, flags, &file_count, &dir_count);

    if (dir_count == 0 && file_count == 0) {
        printf("No match found\n");
    }

    if (flags & 4) {
        if (file_count == 1 && dir_count == 0) {
      
            char file_path[4096];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, target);


            if (access(file_path, R_OK) != 0) {
                printf("Missing permissions for task!\n");
                return;
            }

            FILE* file = fopen(file_path, "r");
            if (file == NULL) {
                printf("Error opening file: %s\n", file_path);
                return;
            }
            char line[4096];
            while (fgets(line, sizeof(line), file) != NULL) {
                printf("%s", line);
            }
            fclose(file);

            printf("\n");

        } else if (file_count == 0 && dir_count == 1) {

            char dir_path_full[4096];
            snprintf(dir_path_full, sizeof(dir_path_full), "%s/%s", dir_path, target);


            if (access(dir_path_full, X_OK) != 0) {
                printf("Missing permissions for task!\n");
                return;
            }

           
            if (chdir(dir_path_full) != 0) {
                printf("Missing permissions for task!\n");
                return;
            }
            char tmpo[4096] = {'\0'};
            strcpy(tmpo,dir_path_full);
            strcpy(prev_dir,cur);
            strcpy(cur,tmpo);
        }
    }
}
