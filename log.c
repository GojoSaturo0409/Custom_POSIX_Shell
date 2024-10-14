#include"log.h"
#include"headers.h"

void execute_helper(char*input){
    char *self_commands[12] = {"hop", "reveal", "log", "proclore", "seek", "iMan","activities","ping","fg","bg","neonate", NULL};
      int yu = strlen(input);
        char pagal[4096];
        strcpy(pagal, input);
        pagal[yu - 1] = '\0';
        record_keeper(input);
        // if (strcmp(pagal, "exit") == 0)
        // {
        //     break;
        // }
        if (feof(stdin)) {  // Handle Ctrl-D
            handle_logout();
        }

        char *semicolon_commands[4096];
        int num_semicolon_commands = 0;
        tokenize_input(input, semicolon_commands, &num_semicolon_commands, ";");

        for (int i = 0; i < num_semicolon_commands; i++)
        {
            char *ampersand_commands[4096];
            int num_ampersand_commands = 0;
            char tmpooo[4096];
            strcpy(tmpooo, semicolon_commands[i]);
            // tokenize_input(tmpooo, ampersand_commands, &num_ampersand_commands, "&");
            tokenize(tmpooo, ampersand_commands, &num_ampersand_commands);
            int num_pipes = 0;
            char *piped_cmds[4096];

            for (int j = 0; j < num_ampersand_commands; j++)
            {
                // process_command(ampersand_commands[j], self_commands, cur, home);
                tokenize_input(ampersand_commands[j], piped_cmds, &num_pipes, "|");
            }
            if ((num_pipes == num_ampersand_commands))
            {
                for (int j = 0; j < num_pipes; j++)
                {
                    process_command(piped_cmds[j], self_commands, cur, home);
                }
            }
            else
            {
                pied_piper(piped_cmds, &num_pipes, self_commands, cur, home);
            }
        }

        check_background_processes(background_pids,num_background_processes,process_list);
     
}


void log_in_file(int print, int purge, int *execute) {
    FILE *file;
    char filename[1024];
    char f_temp[1024];
    int count = 0;
    char c;
    char buffer[4096];
    int f_fline = 1;

    // Construct the absolute paths
    snprintf(filename, sizeof(filename), "%s/example.txt", home);
    snprintf(f_temp, sizeof(f_temp), "%s/temp.txt", home);

    // Open the file, creating it if it doesn't exist
    file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Count the number of lines in the file
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    fseek(file, 0, SEEK_SET); // Rewind the file to the beginning for further reading

    if (print) {
        char inp[4097];
        int ctr;
        for (ctr = 0; ctr < count; ctr++) {
            if (fgets(inp, sizeof(inp), file) != NULL) {
                // Remove the newline character if present
                size_t len = strlen(inp);
                if (len > 0 && inp[len - 1] == '\n') {
                    inp[len - 1] = '\0';
                }
                // Print the content
                printf("%s\n", inp);
            }
        }
        fflush(stdout);
        fclose(file);
        return;
    }

    if (purge) {
        fclose(file);
        // Reopen file in write mode to clear its content
        file = fopen(filename, "w");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        fclose(file);
        return;
    }

    if (execute[0]) {
        file = fopen(filename,"r");
        int n_lines = 0;
        char** ans = readLines(file,&n_lines);
        char command[4096];
        int index = execute[1];
        strcpy(command,ans[n_lines-index]);
        execute_helper(command);                  
    }

    fclose(file);
}