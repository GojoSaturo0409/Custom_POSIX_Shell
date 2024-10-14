#ifndef PIPING_H
#define PIPING_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

extern void process_command(char *command, char *self_commands[], char cur[], char home[]);
void  pied_piper(char *piped_commands[], int *num_pipes, char *self_commands[], char cur[], char home[]);

#endif