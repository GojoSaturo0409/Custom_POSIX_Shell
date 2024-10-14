#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

extern char *hop(char **arg, char *home);
extern void reveal(char **args);
extern void log_in_file(int print, int purge, int *execute);
extern void proclore(char** args);
extern void seek(char** args) ;
extern void controller( char *argv[]);
extern void activities();
extern void ping_process(int pid, int signal_num);
extern void fg(int pid);
extern void bg(int pid);
extern void neonate(int time_interval);
void execute_custom_command(char **tokens, int num_tokens, char *self_commands[], char *cur, char *home);

#endif 