#ifndef BACKGROUND_HANDLER_H
#define BACKGROUND_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include<signal.h>

extern int process_count;

extern char cur[4096];
extern int foreground_pid;
extern int num_background_processes;
extern pid_t background_pids[100];
extern void remove_process(int pid);
extern char home[4096];
void check_background_processes();
extern void add_process(int pid, const char* command);
extern void update_process_state(int pid, int running);
void execute_system_command(char **tokens, int num_tokens, int background);

#endif 