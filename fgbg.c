#include"fgbg.h"
#include"headers.h"

void fg(int pid) {
    int shell_gid = getpgid(shell_pid);
    int t_gid = getpgid(pid);
    if (kill(pid, 0)) {
        printf("No such process found\n");
        return;
    }

    // Set the process group ID to the target process group (bring the job to the foreground)
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, t_gid);

    // Record the start time
    time_t start_time = time(NULL);

    // Wait for the job to report (handle stopped or completed processes)
    int status;
    waitpid(pid, &status, WUNTRACED);

    // Record the end time
    time_t end_time = time(NULL);

    // Put the shell back in the foreground
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, shell_gid);

    // Calculate elapsed time
    elapsed_time = (int)difftime(end_time, start_time);

    // Check if the process ran for more than 2 seconds
    if (elapsed_time > 2) {
        elapse = 1;  // Flag for long-running command
        strcpy(cmd, get_process_command(pid));  // Assuming you have a way to retrieve the command by PID
    }

    // Update the process state based on exit status
    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
        // Remove process if it failed
        remove_process(pid);
    }

    // If the process is stopped (e.g., via Ctrl+Z), update its state
    if (WIFSTOPPED(status)) {
        update_process_state(pid, 0);  // Process is stopped
    } else {
        update_process_state(pid, 0);  // Process is running
    }
}
// Function to resume process in the background (bg)
void bg(int pid) {
    // Check if the process exists
    if (kill(pid, 0) != 0) {
        printf("No such process found\n");
        return;
    }


    // Continue the process if it was stopped
    if (kill(pid, SIGCONT) == -1) {
        perror("Failed to continue process");
        return;
    }
    printf("Process with pid: %d Resumed\n",pid);

    // Update process state to running in the background
    update_process_state(pid, 1);

    // Optionally, you could also handle waiting for the process here
    // This is commented out, but you might want to uncomment and adapt it if needed
    
    int status;
    if (waitpid(pid, &status, WNOHANG | WUNTRACED) == -1) {
        perror("Failed to wait for process");
    } else {
        if (WIFSTOPPED(status)) {
            update_process_state(pid, 0);  // Process is stopped again
        // } else if (WIFSIGNALED(status) || WIFEXITED(status)) {
        //     remove_process(pid);  // Process is terminated or exited
        // }
    }
    
}
}

