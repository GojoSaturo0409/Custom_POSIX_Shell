#ifndef PROMPT_H
#define PROMPT_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pwd.h>
#include<sys/types.h>
#include<string.h>
extern int elapse;
extern char cmd[4096];
extern int elapsed_time;
void prompt(char dir[],char home_dir[]);


#endif