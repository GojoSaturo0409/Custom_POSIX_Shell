#include "main.h"
#include "headers.h"

char home[4096];
char cur[4096];
int shell_pid;
int backup_input;
// slight error while piping/redirection of custom commands,handling of flags,multiple args not getting passed

pid_t background_pids[100];
int num_background_processes = 0;
pid_t foreground_pid = -1; // To keep track of the foreground process

void handle_sigint(int signum)
{
    if (foreground_pid != -1)
    {
        // Send SIGINT to the entire foreground process group
        killpg(foreground_pid, SIGINT);
        foreground_pid = -1; // Reset foreground_pid after sending signal
        printf("\nForeground process interrupted.\n");
    }
    else
    {
        // No foreground process, do nothing
        printf("\nNo foreground process to interrupt.\n");
    }
    // fflush(stdout);
}

void handle_sigstp(int signum)
{
    if (foreground_pid != -1)
    {
        // Send SIGTSTP (Ctrl+Z) to the entire foreground process group
        killpg(foreground_pid, SIGTSTP);

        // Move the foreground process to the background process list
        background_pids[num_background_processes++] = foreground_pid;
        foreground_pid = -1; // Reset foreground_pid after stopping the process

        printf("\nForeground process stopped and moved to the background.\n");
    }
    else
    {
        // No foreground process, do nothing
        printf("\nNo foreground process to stop.\n");
    }
    // fflush(stdout);
}

// void handle_sigchld(int signum)
// {
//     // Clean up finished child processes
//     int status;
//     while (waitpid(-1, &status, WNOHANG) > 0)
//     {
//         // Process finished, you can also remove from background_pids here if needed
//     }
// }

void tokenize_input(char *input, char *tokens[], int *num_tokens, const char *delimiter)
{
    char *token;
    int i = 0;

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }

    token = strtok(input, delimiter);
    while (token != NULL)
    {
        tokens[i++] = token;
        token = strtok(NULL, delimiter);
    }
    tokens[i] = NULL;
    *num_tokens = i;
}

void tokenize(char *input, char *tokens[], int *numtokens)
{
    int u = 0;
    int j = 0;
    char tmp_buff[4096] = {'\0'};

    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '&')
        {

            tmp_buff[u++] = '&';
            tmp_buff[u] = '\0';

            tokens[j] = malloc(strlen(tmp_buff) + 1);
            strcpy(tokens[j++], tmp_buff);
            u = 0; // Reset index for tmp_buff
            memset(tmp_buff, 0, sizeof(tmp_buff));
        }
        else
        {
            tmp_buff[u++] = input[i];
        }
    }

    if (u > 0)
    {
        tmp_buff[u] = '\0';
        tokens[j] = malloc(strlen(tmp_buff) + 1);
        strcpy(tokens[j++], tmp_buff);
    }

    tokens[j] = NULL;
    *numtokens = j;
    for (int k = 0; k < *numtokens; k++)
    {
        int len = strlen(tokens[k]);
        for (int k = 0; k < *numtokens; k++)
        {
            for (int cc = 0; cc < strlen(tokens[k]); cc++)
            {
                if (tokens[k][cc] == '\t')
                {
                    tokens[k][cc] = ' ';
                }
            }
        }
    }
}

void process_command(char *command, char *self_commands[], char cur[], char home[])
{
    char *inp = (char *)malloc(4096 * sizeof(char));
    strcpy(inp, command);
    char *tokens[4096];
    int num_tokens = 0;

    // Tokenize the command into arguments
    tokenize_input(command, tokens, &num_tokens, " ");

    if (num_tokens == 0)
    {
        return;
    }

    // Handle alias command replacement
    const char *alias_command = find_alias(tokens[0]);
    if (alias_command != NULL)
    {
        printf("Executing alias: %s -> %s\n", tokens[0], alias_command);
        strcpy(inp, alias_command); // Replace input with alias command
        trim_quotes(inp);
        num_tokens = 0;
        tokenize_input(inp, tokens, &num_tokens, " ");
    }

    int isBack = (strcmp(tokens[num_tokens - 1], "&") == 0);
    if (isBack)
    {
        tokens[num_tokens - 1] = NULL;
        num_tokens--;
    }

    // Save the original stdin, stdout, and stderr
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    // Initialize file descriptors for redirection
    int redirect_in = -1, redirect_out = -1, append_out = -1;
    int redirection_index = -1;

    // Detect and handle redirection
    for (int i = 0; i < num_tokens; i++)
    {
        if (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], ">>") == 0 || strcmp(tokens[i], "<") == 0)
        {
            redirection_index = i; // Mark where the redirection starts
            break;
        }
    }

    // Process redirection, if any
if (redirection_index != -1)
{
    // Process all redirections (input, output, append)
    for (int i = redirection_index; i < num_tokens; i++)
    {
        if (tokens[i] == NULL)
        {
            continue; // Skip already handled tokens
        }

        if (strcmp(tokens[i], ">") == 0 && i + 1 < num_tokens)
        {
            // Output redirection (truncate)
            redirect_out = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (redirect_out < 0)
            {
                perror("open");
                return;
            }
            tokens[i] = NULL; // Mark the redirection operator as handled
            tokens[i + 1] = NULL; // Mark the file name as handled
        }
        else if (strcmp(tokens[i], ">>") == 0 && i + 1 < num_tokens)
        {
            // Output redirection (append)
            append_out = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (append_out < 0)
            {
                perror("open");
                return;
            }
            tokens[i] = NULL;
            tokens[i + 1] = NULL;
        }
        else if (strcmp(tokens[i], "<") == 0 && i + 1 < num_tokens)
        {
            // Input redirection
            redirect_in = open(tokens[i + 1], O_RDONLY);
            if (redirect_in < 0)
            {
                perror("open");
                return;
            }
            tokens[i] = NULL;
            tokens[i + 1] = NULL;
        }
    }

    // Apply redirections after processing all
    if (redirect_out != -1)
    {
        dup2(redirect_out, STDOUT_FILENO); // Redirect stdout to the file
        close(redirect_out);
    }
    if (append_out != -1)
    {
        dup2(append_out, STDOUT_FILENO); // Redirect stdout (appending) to the file
        close(append_out);
    }
    if (redirect_in != -1)
    {
        dup2(redirect_in, STDIN_FILENO); // Redirect stdin from the file
        close(redirect_in);
    }
}



    // Handle custom commands
    int is_custom = 0;
    for (int i = 0; i < 11; i++)
    {
        if (strcmp(tokens[0], self_commands[i]) == 0)
        {
            // Pass the tokens before redirection for execution
            execute_custom_command(tokens, redirection_index != -1 ? redirection_index : num_tokens, self_commands, cur, home);
            is_custom = 1;
            break;
        }
    }

    // Handle system commands if not custom
    if (!is_custom)
    {
        execute_system_command(tokens, num_tokens, isBack);
        for (int i = 0; i < num_background_processes; i++)
        {
            if (waitpid(background_pids[i], NULL, WNOHANG) > 0)
            {
                for (int j = i; j < num_background_processes - 1; j++)
                {
                    background_pids[j] = background_pids[j + 1];
                }
                num_background_processes--;
            }
        }
    }

    // Restore the original stdin and stdout
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);

    // Close the saved file descriptors
    close(saved_stdin);
    close(saved_stdout);
}

int main()
{
    char input[4096];
    getcwd(cur, sizeof(cur));
    getcwd(home, sizeof(home));
    shell_pid = getpid();
    char *self_commands[12] = {"hop", "reveal", "log", "proclore", "seek", "iMan", "activities", "ping", "fg", "bg", "neonate", NULL};
    FILE *file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        perror("Error opening .myshrc file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE];

    // Read file line by line
    while (fgets(line, sizeof(line), file))
    {
        parse_line(line);
    }

    fclose(file);

    struct sigaction sa_int, sa_tstp, sa_chld;

    // Handle Ctrl-C (SIGINT)
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    // Handle Ctrl-Z (SIGTSTP)
    sa_tstp.sa_handler = handle_sigstp;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    if (sigaction(SIGTSTP, &sa_tstp, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Handle child process signals (SIGCHLD)
    // sa_chld.sa_handler = handle_sigchld;
    // sigemptyset(&sa_chld.sa_mask);
    // sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    // if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
    //     perror("sigaction");
    //     exit(EXIT_FAILURE);
    // }

    struct sigaction sa;
    sa.sa_handler = &sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Restart interrupted syscalls, don't trigger on stopped child
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    while (1)
    {

        prompt(cur, home);
        fgets(input, sizeof(input), stdin);
        int yu = strlen(input);
        char pagal[4096];
        strcpy(pagal, input);
        pagal[yu - 1] = '\0';
        record_keeper(input);
        if (strcmp(pagal, "exit") == 0)
        {
            break;
        }
        if (feof(stdin))
        { // Handle Ctrl-D
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
            for (int j = 0; j < num_ampersand_commands; j++)
            {
                int num_pipes = 0;
                char *piped_cmds[4096];

                // Tokenize ampersand commands based on pipes
                tokenize_input(ampersand_commands[j], piped_cmds, &num_pipes, "|");

                if (num_pipes > 1)
                {
                    // If the command contains pipes, handle them
                    pied_piper(piped_cmds, &num_pipes, self_commands, cur, home);
                }
                else
                {
                    // If no pipes, process command normally
                    process_command(piped_cmds[0], self_commands, cur, home);
                }
            }
        }

        check_background_processes(background_pids, num_background_processes, process_list);
    }
}
