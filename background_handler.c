#include"background_handler.h"
#include"headers.h"

int elapse = 0;
char cmd[4096] = {'\0'};
int elapsed_time = 0;
 // Global variable to track the current foreground process


void check_background_processes(pid_t background_pids[], int num_background_processes,struct Process process_list[])
{
    int status;
    pid_t pid;

    for (int i = 0; i < num_background_processes; i++)
    {
        // If the process PID is valid (i.e., not already marked as completed)
        if (background_pids[i] > 0)
        {
            // Check the process status using waitpid with WNOHANG
            pid = waitpid(background_pids[i], &status, WNOHANG);

            if (pid == 0)
            {
                // Process is still running
                continue;
            }
            else if (pid > 0)
            {
                if (WIFEXITED(status))
                {
                    printf("Process %d exited normally\n", pid);
                    // process_status[i] = 1; // Mark process as completed (1 for normal exit)
                    for(int j = 0;j<process_count;j++){
                        if(process_list[j].pid == pid){
                            process_list[j].running = 0;
                            break;
                        }
                    }
                }
                else if (WIFSIGNALED(status))
                {
                    printf("Process %d exited abnormally\n", pid);
                     for(int j = 0;j<process_count;j++){
                        if(process_list[j].pid == pid){
                            process_list[j].running = 0;
                            break;
                        }
                    }
                }

                // Mark PID as 0 to indicate that the process has been handled
                background_pids[i] = 0;
            }
        }
    }
}

// void handle_sigint(int sig)
// {
//     if (fore_pid > 0)
//     {
//         kill(fore_pid, SIGINT);  // Send SIGINT to the foreground process
//     }
// }



// void handle_eof()
// {
//     // Kill all background processes
//     kill_all_background_processes();
//     printf("Exiting shell\n");
//     exit(0);
// }

void execute_system_command(char **tokens, int num_tokens, int background)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return;
    }
    
    if (pid == 0)  // Child process
    {
        setpgid(pid,pid);
        if (background)
        {
            if (num_background_processes < 100) {
                background_pids[num_background_processes++] = pid;
            }
            // Open /dev/null for reading and writing
            int dev_null = open("/dev/null", O_RDWR);
            if (dev_null == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            // Redirect stdin, stdout, and stderr to /dev/null
            dup2(dev_null, STDIN_FILENO);
            dup2(dev_null, STDOUT_FILENO);
            dup2(dev_null, STDERR_FILENO);
            close(dev_null);  // Close /dev/null as it is now duplicated
        }

        if (execvp(tokens[0], tokens) < 0)
        {
            printf("%s is not a valid command\n", tokens[0]);
            exit(1);
        }
    }
    else  // Parent process
    {
        add_process(pid, tokens[0]);  // Add the process to the process list

        if (background)
        {
            printf("Background process PID: %d\n", pid);
        }
        else
        {
           foreground_pid = pid;
            time_t start_time = time(NULL);
            int status;
            waitpid(pid, &status, 0);  // Wait for the child process to complete
            time_t end_time = time(NULL);
           foreground_pid = -1;
            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
            {
                // Command was invalid
                // printf("%s is not a valid command\n", tokens[0]);
                remove_process(pid);
            }

            // Update the process state to stopped (not running)
            update_process_state(pid, 0);

            elapsed_time = (int)difftime(end_time, start_time);
            if (elapsed_time > 2)
            {
                elapse = 1;
                strcpy(cmd, tokens[0]);
            }
        }
    }
}