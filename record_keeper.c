#include"record_keeper.h"

void record_keeper(char *args) {
    char input[4096];
    char most_recent_command[4096] = {0};
    FILE *fd;
    FILE *tmp;
    char filename[1024];
    char f_temp[1024];
    const char *filename_base = "example.txt";
    const char *f_temp_base = "temp.txt";
    int count = 0;
    char c;
    char buffer[4096];
    int f_fline = 1;


    FILE *recent_cmd_file = fopen(RECENT_CMD_FILE, "r");
    if (recent_cmd_file) {
        if (fgets(most_recent_command, sizeof(most_recent_command), recent_cmd_file) != NULL) {

            int len = strlen(most_recent_command);
            if (len > 0 && most_recent_command[len-1] == '\n') {
                most_recent_command[len-1] = '\0';
            }
        }
        fclose(recent_cmd_file);
    }


    char trimmed_args[4096];
    snprintf(trimmed_args, sizeof(trimmed_args), "%s", args);
    size_t len = strlen(trimmed_args);
    while (len > 0 && (trimmed_args[len-1] == '\n' || trimmed_args[len-1] == ' ')) {
        trimmed_args[--len] = '\0';
    }

 
    if (strcmp(most_recent_command, trimmed_args) == 0) {
        return; 
    }


    if (strstr(trimmed_args, "log") != NULL) {
        return;  
    }


    FILE *recent_cmd_file_write = fopen(RECENT_CMD_FILE, "w");
    if (recent_cmd_file_write) {
        fputs(trimmed_args, recent_cmd_file_write);
        fclose(recent_cmd_file_write);
    }


    strcpy(input, trimmed_args);
    char *token1;
    char *tokens1[4096];
    int i = 0;

    token1 = strtok(input, " ");
    while (token1 != NULL) {
        tokens1[i++] = token1;
        token1 = strtok(NULL, " ");
    }
    if (i > 0) {
        tokens1[i-1][strlen(tokens1[i-1])-1] = '\0';  
    }


    snprintf(filename, sizeof(filename), "%s/%s", LOG_DIR, filename_base);
    snprintf(f_temp, sizeof(f_temp), "%s/%s", LOG_DIR, f_temp_base);


    fd = fopen(filename, "a+");
    if (fd == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    while ((c = fgetc(fd)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }


    tmp = fopen(f_temp, "w+");
    if (tmp == NULL) {
        perror("Error opening temporary file");
        fclose(fd);
        exit(EXIT_FAILURE);
    }


    if (count == 15) {
        rewind(fd);  
        while (fgets(buffer, sizeof(buffer), fd) != NULL) {
            if (f_fline) {
                f_fline = 0;  
                continue;
            }
            fputs(buffer, tmp);
        }
        fputs(trimmed_args, tmp);
        fputc('\n', tmp);  
        fclose(fd);
        fclose(tmp);
        remove(filename);
        rename(f_temp, filename);
    } 
    else if (count < 15) {
        fputs(trimmed_args, fd);
        fputc('\n', fd);  
        fclose(fd);
        fclose(tmp);
    }
}
