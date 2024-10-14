#ifndef SEEK_H
#define SEEK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#define COLOR_RESET "\033[0m"
#define COLOR_DIR "\033[1;34m"
#define COLOR_FILE "\033[1;32m"
extern char cur[4096];
extern char prev_dir[4096];

void seek_helper(char* target, char* dir_path, int flags, int* file_count, int* dir_count);
void seek(char** args) ;

#endif