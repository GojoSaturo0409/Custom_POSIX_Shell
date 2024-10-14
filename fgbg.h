#ifndef FGBG_H
#define FGBG_H
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include<time.h>

void fg(int pid);
void bg(int pid);
extern int process_count;
extern int shell_pid;
extern int elapse;
extern int elapsed_time;
extern void sigchld_handler(int sig);
extern void handle_sigstp(int signum);
extern char cmd[4096];
extern char* get_process_command(int pid);
extern void update_process_state(int pid, int running);
extern void remove_process(int pid);

#endif