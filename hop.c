#include"hop.h"

char prev_dir[4096] = {'\0'};
char home_dir[4096] = {'\0'};

char *hop(char **arg, char *home) {
    getcwd(home_dir, sizeof(home_dir));
    char *cur_dir = (char *)malloc(4096 * sizeof(char));
    memset(cur_dir, 0, 4096 * sizeof(char));
    getcwd(cur_dir, sizeof(cur_dir));
    strcpy(cur_dir, home_dir);

    if (arg == NULL || arg[0] == NULL) {
        strcpy(prev_dir, cur_dir);
        strcpy(cur_dir, home);
        printf("%s\n", cur_dir);
        chdir(cur_dir);
        return cur_dir;
    }

    int ctr = 0;
    int dd_counter = 0;

    // Count the occurrences of '..'
    for (int i = 0; arg[i] != 0; i++) {
        for (int j = 0; j < strlen(arg[i]) - 1; j++) {
            if (arg[i][j] == '.' && arg[i][j + 1] == '.') {
                dd_counter++;
                j++;  // Move to the next character after the '..' to avoid double-counting
            }
        }
    }

    while (arg[ctr] != NULL) {
        if (dd_counter > 0) {
            strcpy(prev_dir, cur_dir);
            for (int k = 0; k < dd_counter; k++) {
                char *new_directory = (char *)malloc(4096 * sizeof(char));
                memset(new_directory, 0, 4096 * sizeof(char));

                
                char *token2;
                char *tokens2[4096];
                char dir_copy[4096];
                strncpy(dir_copy, cur_dir, sizeof(dir_copy));
                dir_copy[sizeof(dir_copy) - 1] = '\0';
                token2 = strtok(dir_copy, "/");
                int j = 0;
                while (token2 != NULL) {
                    tokens2[j++] = token2;
                    token2 = strtok(NULL, "/");
                }

                // Move up one level
                for (int q = 0; q < j - 1; q++) {
                    strcat(new_directory, "/");
                    strcat(new_directory, tokens2[q]);
                }

                if (strlen(new_directory) == 0) {
                    strcpy(new_directory, "/");
                }

                chdir(new_directory);
                strcpy(cur_dir, new_directory);
            }
            printf("%s\n", cur_dir);
            return cur_dir;
        }
        else if (strcmp(arg[ctr], "-") == 0) {
            if (prev_dir[0] == '\0') {
                printf("Error: prevdir not set\n");
                return cur_dir;
            }
            chdir(prev_dir);
            printf("%s\n", prev_dir);

            char temp[4096];
            strcpy(temp, prev_dir);
            strcpy(prev_dir, cur_dir);
            strcpy(cur_dir, temp);
            return cur_dir;
        }
        else if (arg[ctr][0] == '~')
        {
            if (arg[ctr][1] != '\0')
            {
                // copy string to some buffer
                // tokenise it
                // add to the path
                char new_path[4096];
                snprintf(new_path, sizeof(new_path), "%s%s", home, &arg[ctr][1]);
                // printf("%s\n",new_path);

                if (access(new_path, F_OK) == 0)
                {
                    strcpy(prev_dir, cur_dir);
                    chdir(new_path);
                    strcpy(cur_dir, new_path);
                    printf("%s\n", cur_dir);
                }
                else
                {
                    printf("Invalid Path\n");
                }
                return cur_dir;
            }
            else
            {
                strcpy(prev_dir, cur_dir);
                strcpy(cur_dir, home);
                printf("%s\n", cur_dir);
                chdir(cur_dir);
                return cur_dir;
            }
        }

           else if (arg[ctr][0] == '.' && arg[ctr][1] != '.')
        {
            if (arg[ctr][0] == '.')
            {
                if (arg[ctr][1] != '\0')
                {
                    // copy string to some buffer
                    // tokenise it
                    // add to the path
                    char new_path[4096];
                    snprintf(new_path, sizeof(new_path), "%s%s", home, &arg[ctr][1]);
                    // printf("%s\n",new_path);

                    if (access(new_path, F_OK) == 0)
                    {
                        strcpy(prev_dir, cur_dir);
                        chdir(new_path);
                        strcpy(cur_dir, new_path);
                        printf("%s\n", cur_dir);
                    }
                    else
                    {
                        printf("Invalid Path\n");
                    }
                    return cur_dir;
                }
                else
                {
                    // strcpy(prev_dir,cur_dir);
                    // strcpy(cur_dir, home);
                    printf("%s\n", cur_dir);
                    // chdir(cur_dir);
                    return cur_dir;
                }
            }
        }


        else if (arg[ctr][0] == '/') {
            strcpy(prev_dir, cur_dir);
            if (access(arg[ctr], F_OK) == 0) {
                chdir(arg[ctr]);
                strcpy(cur_dir, arg[ctr]);
                printf("%s\n", cur_dir);
                return cur_dir;
            } else {
                printf("Invalid Path");
            }
        } else {
            char *new_dir = (char *)malloc(4096 * sizeof(char));
            strcpy(new_dir, cur_dir);
            strcat(new_dir, "/");
            strcat(new_dir, arg[ctr]);

            if (access(new_dir, F_OK) == 0) {
                printf("%s\n", new_dir);
                strcpy(prev_dir, cur_dir);
                chdir(new_dir);
                strcpy(cur_dir, new_dir);
                return new_dir;
            } else {
                printf("Invalid Path\n");
                return cur_dir;
            }
        }
        ctr++;
    }
    return cur_dir;
}
