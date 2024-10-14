#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include<fcntl.h>
#include<sys/wait.h>
#include <signal.h>
#include<time.h>
#define MAX_TOKENS 100

extern int process_count;

extern void check_background_processes();
extern void sigchld_handler(int sig) ;
void handle_sigint(int signum);
void handle_sigstp(int signum);
extern void add_alias(const char *name, const char *command);
extern void add_function(const char *name, const char *body);
extern void parse_line(char *line);
extern const char* find_alias(const char *name);
extern void trim_quotes(char *str);  
extern void handle_logout();
extern void execute_custom_command(char **tokens, int num_tokens, char *self_commands[], char *cur, char *home);
extern void execute_system_command(char **tokens, int num_tokens, int background);
extern void record_keeper(char *args);
extern void prompt(char dir[],char home_dir[]);
extern void pied_piper(char *piped_commands[], int *num_pipes, char *self_commands[], char cur[], char home[]);
void tokenize_input(char *input, char *tokens[], int *num_tokens, const char *delimiter);
void tokenize(char *input, char *tokens[],int*numtokens);
void process_command(char *command, char *self_commands[], char cur[], char home[]);


#endif