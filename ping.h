#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void add_process(int pid, const char* command);
void activities();
void update_process_state(int pid, int running);
void ping_process(int pid, int signal_num);
void handle_sigint(int sig);
void handle_sigtstp(int sig);
void handle_logout();
char* get_process_command(int pid);
extern pid_t pid;
extern int shell_pid;
extern void check_background_processes();
extern pid_t background_pids[100];
extern int num_background_processes;

#endif