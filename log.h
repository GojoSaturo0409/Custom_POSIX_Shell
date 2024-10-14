#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

extern char home[4096];
extern char cur[4096];
extern void handle_logout();
extern void record_keeper(char *args);
extern void  pied_piper(char *piped_commands[], int *num_pipes, char *self_commands[], char cur[], char home[]);
extern void tokenize_input(char *input, char *tokens[], int *num_tokens, const char *delimiter);
extern void execute_custom_command(char **tokens, int num_tokens, char *self_commands[], char *cur, char *home);
extern void check_background_processes();
extern void process_command(char *command, char *self_commands[], char cur[], char home[]);
extern void execute_system_command(char **tokens, int num_tokens, int background);
extern void tokenize(char *input, char *tokens[],int*numtokens);
extern char **readLines(FILE *file, int *numLines);
void execute_helper(char*input);
void log_in_file(int print, int purge, int *execute);
extern pid_t background_pids[100];
extern int num_background_processes;

#endif