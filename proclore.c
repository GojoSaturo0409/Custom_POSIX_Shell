#include "proclore.h"
#include "headers.h"

// code idea taken from ChatGPT, written by myself
void proclore(char** args) {
    // part 1: Determine the process ID
    int process_id;
    if (args[0] == NULL) {
        process_id = getpid();
    } else {
        process_id = atoi(args[0]);
    }

    // Check if the process exists by checking if /proc/<pid> exists
    char proc_path[4096];
    sprintf(proc_path, "/proc/%d", process_id);
    
    if (access(proc_path, F_OK) == -1) {
        // Process does not exist
        printf("Error: No such process with PID %d\n", process_id);
        return;
    }

    // part 3: Get process group ID
    int process_group_id = getpgid(process_id);

    // Check if the process is in the foreground or background
    int fg_process_group_id = tcgetpgrp(STDIN_FILENO);

    // part 2: Get process status
    char status_path[4096];
    sprintf(status_path, "/proc/%d/stat", process_id);
    FILE *file1 = fopen(status_path, "r");
    if (file1 == NULL) {
        printf("Error: Could not open process status file for PID %d\n", process_id);
        return;
    }

    int pid;
    char comm[256];
    char status;
    fscanf(file1, "%d %s %c", &pid, comm, &status);  // Process ID, command, and status
    fclose(file1);

    // part 4: Get virtual memory size
    char memory_path[4096];
    sprintf(memory_path, "/proc/%d/statm", process_id);
    FILE *file = fopen(memory_path, "r");
    if (file == NULL) {
        printf("Error: Could not open memory file for PID %d\n", process_id);
        return;
    }

    long unsigned vm;
    fscanf(file, "%lu", &vm);
    vm = vm * 4;  // Convert pages to kilobytes
    fclose(file);

    // part 5: Get path to executable
    char path_to_exe[4096];
    sprintf(path_to_exe, "/proc/%d/exe", process_id);
    char buff[4096];
    ssize_t len = readlink(path_to_exe, buff, sizeof(buff));
    if (len == -1) {
        printf("Error: Could not read path to executable for PID %d\n", process_id);
        return;
    }
    buff[len] = '\0';

    // Print all information
    printf("PID: %d\n", process_id);
    printf("Process Status: %c", status);
    
    if (process_group_id == fg_process_group_id) {
        printf("+\n");
    } else {
        printf("\n");
    }
    
    printf("Process Group: %d\n", process_group_id);
    printf("Virtual Memory: %lu KB\n", vm);
    printf("Path to executable: %s\n", buff);
}
