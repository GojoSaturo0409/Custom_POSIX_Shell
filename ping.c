#include"ping.h"
#include"headers.h"



// List of tracked processes
struct Process process_list[100];
int process_count = 0;

// Function to add a new process to the process list
void add_process(int pid, const char* command) {
    process_list[process_count].pid = pid;
    strcpy(process_list[process_count].command, command);
    process_list[process_count].running = 1;  // By default, process is running
    process_count++;
}


char* get_process_command(int pid) {
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            return process_list[i].command;
        }
    }
    return NULL;  // Return NULL if no process with the given PID is found
}

int compare_processes(const void *a, const void *b) {
    char pid_str_a[12], pid_str_b[12];
    
    // Convert the PIDs to strings for lexicographic comparison
    snprintf(pid_str_a, sizeof(pid_str_a), "%d", ((struct Process *)a)->pid);
    snprintf(pid_str_b, sizeof(pid_str_b), "%d", ((struct Process *)b)->pid);
    
    // Compare the PID strings lexicographically
    return strcmp(pid_str_a, pid_str_b);
}

// Function to display current activities
void activities() {
    qsort(process_list, process_count, sizeof(struct Process), compare_processes);
    for (int i = 0; i < process_count; i++) {
        printf("%d : %s - %s\n", process_list[i].pid, process_list[i].command,
               process_list[i].running ? "Running" : "Stopped");
    }
}

// Function to update the status of a process (running/stopped)
void update_process_state(int pid, int running) {
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            process_list[i].running = running;
            break;
        }
    }
}

void remove_process(int pid) {
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            // Shift the remaining processes in the list to fill the gap
            for (int j = i; j < process_count - 1; j++) {
                process_list[j] = process_list[j + 1];
            }
            process_count--;
            break;
        }
    }
}

// void sigchld_handler(int sig) {
//     int status;
//     pid_t pid;
//     int found  = 0;
//     // for(int i = 0;i<num_background_processes;i++){
//     //     if(background_pids[i] == pid){
//     //         found = 1;
//     //         break;
//     //     }
//     // }

//     // Use a non-blocking waitpid to reap any background processes that have terminated
//     while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
//         // Process has terminated, remove it from the process list
//         void check_background_processes(pid_t background_pids[], int num_background_processes,struct Process process_list[]);
      
//                 // printf("Background process with PID %d has completed.\n", pid);
            
    
//         // check_background_processes();
//         update_process_state(pid,0);
//         // remove_process(pid);
//     }
// }


void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    // Use a non-blocking waitpid to reap any background processes that have terminated
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("Process %d exited normally with status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Process %d exited abnormally with signal %d\n", pid, WTERMSIG(status));
        }
        
        // Update the process list or mark the process as done
        update_process_state(pid, 0);
    }
}

// Function to handle ping command (sending signals)
void ping_process(int pid, int signal_num) {
    int signal = signal_num % 32;  // Take signal number modulo 32

    // Send the signal to the process with the given PID
    if (kill(pid, signal) == -1) {
        if (errno == ESRCH) {
            printf("No such process found\n");
        } else {
            perror("Error sending signal");
        }
    } else {
        printf("Sent signal %d to process with pid %d\n", signal, pid);

         if (signal == SIGKILL) {
            // Mark process as stopped and remove it from the list
            update_process_state(pid, 0);  // Mark process as stopped
            remove_process(pid);           // Remove process from the list
        } else if (signal == SIGTERM) {
            // Mark process as stopped but don't remove it
            update_process_state(pid, 0);  // Mark process as stopped
        } else if (signal == SIGSTOP) {
            // Mark process as stopped
            update_process_state(pid, 0);  // Mark process as stopped
        } else if (signal == SIGCONT) {
            // Resume the process
            update_process_state(pid, 1);  // Mark process as running
        }
    }
}

// void handle_sigint(int sig) {
//     printf("\nCaught SIGINT (Ctrl-C). Sending SIGINT to foreground process...\n");
    
//     // Check if a foreground process is running
//     if (fore_pid > 0) {
//         // Send SIGINT to the foreground process
//         if (kill(fore_pid, SIGINT) == -1) {
//             perror("Failed to send SIGINT to foreground process");
//         }
       
//     }
//     // Bring control back to the shell
    
//     printf("Shell resumed. Waiting for commands...\n");
// }

// void handle_sigtstp(int sig) {
//     printf("\nCaught SIGTSTP (Ctrl-Z). Stopping foreground process...\n");

//     // Check if a foreground process is running
//     if (fore_pid > 0) {
//         printf("%d\n",fore_pid);
//         printf("%d\n",shell_pid);
//         // Send SIGTSTP to the foreground process
//         if (kill(fore_pid, SIGTSTP) == -1) {
//             perror("Failed to send SIGTSTP to foreground process");
//         }
//         int s_gid = getpgid(shell_pid);
//         signal(SIGTTOU, SIG_IGN);
//         tcsetpgrp(0, s_gid);
//     }
//     // Bring control back to the shell
    
//     printf("Shell resumed. Waiting for commands...\n");
//     // kill(shell_pid,SIGCONT);
// }


// Function to handle Ctrl-D (EOF) - Logout
void handle_logout() {
    printf("\nLogging out. Killing all processes...\n");
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].running) {
            kill(process_list[i].pid, SIGKILL);  // Kill all running processes
        }
    }
    exit(0);  // Exit the shell
}

// int main() {
//     // Signal handling for Ctrl-C and Ctrl-Z
//     signal(SIGINT, handle_sigint);
//     signal(SIGTSTP, handle_sigtstp);

//     int pid, signal_num;
//     char input[100];

//     // Simulating adding processes to activity list
//     add_process(221, "emacs new.txt");
//     add_process(430, "vim");
//     add_process(620, "gedit");
//     update_process_state(620, 0); // gedit is stopped

//     while (1) {
//         printf("<JohnDoe@SYS:~> ");
//         fgets(input, sizeof(input), stdin);

//         if (feof(stdin)) {  // Handle Ctrl-D
//             handle_logout();
//         }

//         // Parse the input
//         if (strncmp(input, "ping", 4) == 0) {
//             sscanf(input + 5, "%d %d", &pid, &signal_num);
//             ping_process(pid, signal_num);
//         } else if (strncmp(input, "activities", 10) == 0) {
//             activities();
//         } else {
//             printf("Unknown command: %s", input);
//         }
//     }

    
// }
