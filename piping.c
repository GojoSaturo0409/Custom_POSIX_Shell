#include"piping.h"


void pied_piper(char *piped_commands[], int *num_pipes, char *self_commands[], char cur[], char home[]) {
    int pipefd[2 * (*num_pipes - 1)];  // Create file descriptors for pipes
    pid_t pid;

    // Create pipes
    for (int i = 0; i < (*num_pipes - 1); i++) {
        if (pipe(pipefd + i * 2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int j = 0;
    for (int i = 0; i < *num_pipes; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // Child process
            // If not the first command, redirect stdin to the previous pipe
            if (i > 0) {
                if (dup2(pipefd[j - 2], STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // If not the last command, redirect stdout to the next pipe
            if (i < *num_pipes - 1) {
                if (dup2(pipefd[j + 1], STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Close all pipe file descriptors in the child
            for (int k = 0; k < 2 * (*num_pipes - 1); k++) {
                close(pipefd[k]);
            }

            // Call process_command for the current command
            process_command(piped_commands[i], self_commands, cur, home);

            exit(EXIT_SUCCESS);  // Ensure the child process terminates after processing the command
        }

        j += 2;  // Move to the next pipe pair
    }

    // Close all pipe file descriptors in the parent
    for (int i = 0; i < 2 * (*num_pipes - 1); i++) {
        close(pipefd[i]);
    }

    // Wait for all children to finish
    for (int i = 0; i < *num_pipes; i++) {
        wait(NULL);
    }
}
