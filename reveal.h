#ifndef REVEAL_H
#define REVEAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
extern char home[4096];
extern char prev_dir[4096];
extern char cur[4096];


// ANSI color codes
#define COLOR_RESET1  "\x1b[0m"
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_BLUE   "\x1b[34m"
#define COLOR_WHITE  "\x1b[37m"

char *permiso(mode_t file_mode);
int number_links(struct stat fl_st);
char *owner_info(struct stat fl_st);
char *rootinfo(struct stat fl_st);
long long int fileSize_det(struct stat fl_st);
char *last_modified_time_details(struct stat fl_st);
int compare_entries(const struct dirent **a, const struct dirent **b);
void reveal(char **args);

#endif