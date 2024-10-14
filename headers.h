#ifndef HEADER_H
#define HEADER_H

struct Process {
    int pid;
    char command[100];
    int running; // 1 if running, 0 if stopped
};
#define MAX_LINE 256
#define MAX_ALIASES 100
#define MAX_FUNCTIONS 100

// Structures for storing aliases and functions
typedef struct {
    char alias[50];
    char command[200];
} Alias;

typedef struct {
    char function_name[50];
    char function_body[500];  // stores function commands as one string for simplicity
} Function;
extern struct Process process_list[100];


#endif
