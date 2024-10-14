#include"reveal.h"

char *permiso(mode_t file_mode) {
    static char ans[11];  
    strcpy(ans, "----------");

    // File type
    if (S_ISDIR(file_mode)) {
        ans[0] = 'd';
    } else if (S_ISLNK(file_mode)) {
        ans[0] = 'l';
    } else {
        ans[0] = '-';
    }

    // User permissions
    if (file_mode & S_IRUSR){ 
        ans[1] = 'r';
        }
    if (file_mode & S_IWUSR){ 
        ans[2] = 'w';
        }
    if (file_mode & S_IXUSR) {
        ans[3] = 'x';
        }

    // Group permissions
    if (file_mode & S_IRGRP) {
        ans[4] = 'r';
        }
    if (file_mode & S_IWGRP) {
        ans[5] = 'w';
        }
    if (file_mode & S_IXGRP) {
        ans[6] = 'x';
        }

    // Other permissions
    if (file_mode & S_IROTH) {
        ans[7] = 'r';
        }
    if (file_mode & S_IWOTH) {
        ans[8] = 'w';
        }
    if (file_mode & S_IXOTH) {
        ans[9] = 'x';
        }

    ans[10] = '\0';  
    return ans;
}


int number_links(struct stat fl_st) {
    return fl_st.st_nlink;
}


char *owner_info(struct stat fl_st) {
    static char ans[4096];
    struct passwd *owner_info = getpwuid(fl_st.st_uid);
    strcpy(ans, owner_info->pw_name);
    return ans;
}

char *rootinfo(struct stat fl_st) {
    static char ans[4096];
    struct group *group_info = getgrgid(fl_st.st_gid);
    strcpy(ans, group_info->gr_name);
    return ans;
}


long long int fileSize_det(struct stat fl_st) {
    return fl_st.st_size;
}


char *last_modified_time_details(struct stat fl_st) {
    static char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&fl_st.st_mtime));
    return timebuf;
}


int compare_entries(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void reveal(char **args) {
    int a_enc = 0;
    int l_enc = 0;
    char dirp[4096] = {'\0'};

  
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '-' && args[i][1]!='\0') {
           
            for (int j = 1; args[i][j] != '\0'; j++) {
                if (args[i][j] == 'a') {
                    a_enc = 1;
                } else if (args[i][j] == 'l') {
                    l_enc = 1;
                } else if (args[i][j] == '-') {
                    continue; 
                } else {
                    fprintf(stderr, "Invalid flag: -%c\n", args[i][j]);
                    return;
                }
            }
        } else {
            strcpy(dirp, args[i]);
        }
    }

    if (dirp[0] == '\0') {
        strcpy(dirp, ".");
    }
    else if(dirp[0] == '~'){
        if (strcmp(dirp, "~") == 0) {
        strcpy(dirp, home);
    }
    else{
        char new_path[4096];
        char ty[4096] = {'\0'};
        int ww =0;
        for(int i = 1;dirp[i]!='\0';i++){
            ty[ww++] = dirp[i];
        }
        snprintf(new_path, sizeof(new_path), "%s%s", home, ty);
        if (access(new_path, F_OK) == 0)
                {                   
                    
                    strcpy(dirp,new_path);
                    
                }
                else
                {
                    printf("Invalid Path\n");
                }
    }

    } 
     else if (strcmp(dirp, "-") == 0) {
        if (prev_dir[0] == '\0') {
            printf("Error: prevdir not specified\n");
            return;
        }
        strcpy(dirp, prev_dir);
    }
    else if(strcmp(dirp,"..") == 0){
        char *token2;
            char *tokens2[4096];
            char dir_copy[4096];
            strncpy(dir_copy, cur, sizeof(dir_copy));
            dir_copy[sizeof(dir_copy) - 1] = '\0';
            token2 = strtok(dir_copy, "/");
            int j = 0;
            while (token2 != NULL)
            {
                tokens2[j++] = token2;
                token2 = strtok(NULL, "/");
            }

            char *new_directory = (char *)malloc(4096 * sizeof(char));
            memset(new_directory, 0, 4096 * sizeof(char));
            for (int q = 0; q < j - 1; q++)
            {
                strcat(new_directory, "/");
                strcat(new_directory, tokens2[q]);
            }

            if (strlen(new_directory) == 0)
            {
                strcpy(new_directory, "/");
            }

            // printf("%s\n", new_directory);
            // chdir(new_directory);
            strcpy(dirp, new_directory);
    }

    struct dirent **namelist;
    int n;

  
    n = scandir(dirp, &namelist, NULL, compare_entries);
    if (n < 0) {
        perror("scandir");
        return;
    }

    
    for (int i = 0; i < n; i++) {
        struct dirent *entry = namelist[i];

        
        if (!a_enc && entry->d_name[0] == '.') {
            free(namelist[i]);
            continue;
        }

      
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", dirp, entry->d_name);

     
        struct stat statbuf;
        if (stat(full_path, &statbuf) == -1) {
            perror("stat");
            free(namelist[i]);
            continue;
        }

        char *color = COLOR_WHITE;
        if (S_ISDIR(statbuf.st_mode)) {
            color = COLOR_BLUE;
        } else if (statbuf.st_mode & S_IXUSR) {
            color = COLOR_GREEN;
        }


        if (l_enc) {
            printf("%s%s %3d %s %s %8lld %s %s%s\n",
                   color,
                   permiso(statbuf.st_mode),
                   number_links(statbuf),
                   owner_info(statbuf),
                   rootinfo(statbuf),
                   fileSize_det(statbuf),
                   last_modified_time_details(statbuf),
                   entry->d_name,
                   COLOR_RESET1);
        } else {
            printf("%s%s%s\n", color, entry->d_name, COLOR_RESET1);
        }

        free(namelist[i]);
    }
    free(namelist);
}
