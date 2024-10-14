#include"alias_parsing.h"

#include"headers.h"
#include <ctype.h>

// Global arrays to store aliases and functions
Alias aliases[MAX_ALIASES];
int alias_count = 0;

Function functions[MAX_FUNCTIONS];
int function_count = 0;

// Function to add alias
void add_alias(const char *name, const char *command) {
    if (alias_count < MAX_ALIASES) {
        strcpy(aliases[alias_count].alias, name);
        strcpy(aliases[alias_count].command, command);
        alias_count++;
    } else {
        printf("Alias storage limit reached!\n");
    }
}

// Function to add function definition
void add_function(const char *name, const char *body) {
    if (function_count < MAX_FUNCTIONS) {
        strcpy(functions[function_count].function_name, name);
        strcpy(functions[function_count].function_body, body);
        function_count++;
    } else {
        printf("Function storage limit reached!\n");
    }
}

// Function to find an alias
const char* find_alias(const char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].alias, name) == 0) {
            return aliases[i].command;
        }
    }
    return NULL; // If alias is not found
}



// Function to trim leading and trailing whitespace from a string
void trim_whitespace(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // All spaces, return empty string
    if (*str == 0) return;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate after the last non-space character
    *(end + 1) = '\0';
}

// Function to trim quotes from the beginning and end of the string
void trim_quotes(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[0] == '\'' || str[0] == '"') && (str[len - 1] == '\'' || str[len - 1] == '"')) {
        // Remove leading and trailing quotes
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// Updated function to parse a line and detect aliases and functions
void parse_line(char *line) {
    char *token;
    char name[50], command[200], body[500];

    // Remove newline character
    line[strcspn(line, "\n")] = 0;

    // Check for alias with `alias` keyword
    if (strncmp(line, "alias ", 6) == 0) {
        token = strtok(line + 6, "=");
        if (token != NULL) {
            strcpy(name, token);
            token = strtok(NULL, "=");
            if (token != NULL) {
                strcpy(command, token);

                // Trim any leading/trailing spaces and quotes
                trim_whitespace(name);
                trim_whitespace(command);
                trim_quotes(command);

                add_alias(name, command);
            }
        }
    }
    // Check for alias without `alias` keyword
    else if (strchr(line, '=') != NULL) {
        token = strtok(line, "=");
        if (token != NULL) {
            strcpy(name, token);
            token = strtok(NULL, "=");
            if (token != NULL) {
                strcpy(command, token);

                // Trim any leading/trailing spaces and quotes
                trim_whitespace(name);
                trim_whitespace(command);
                trim_quotes(command);

                add_alias(name, command);
            }
        }
    }
    // Function parsing (no change)
    else if (strstr(line, "() {") != NULL) {
        token = strtok(line, "(");
        if (token != NULL) {
            strcpy(name, token); // get the function name
            strcpy(body, "");    // start with an empty body

            // Read the body of the function
            while (fgets(line, MAX_LINE, stdin)) {
                line[strcspn(line, "\n")] = 0; // strip newline
                if (strcmp(line, "}") == 0) {
                    break; // end of function body
                }
                strcat(body, line);
                strcat(body, ";"); // add semicolon between lines
            }
            add_function(name, body);
        }
    }
}

// int main() {
//     FILE *file = fopen(".myshrc", "r");
//     if (file == NULL) {
//         perror("Error opening .myshrc file");
//         return EXIT_FAILURE;
//     }

//     char line[MAX_LINE];

//     // Read file line by line
//     while (fgets(line, sizeof(line), file)) {
//         parse_line(line);
//     }

//     fclose(file);

//     // Display the parsed aliases
//     printf("Aliases:\n");
//     for (int i = 0; i < alias_count; i++) {
//         printf("%s = %s\n", aliases[i].alias, aliases[i].command);
//     }

//     // Display the parsed functions
//     printf("\nFunctions:\n");
//     for (int i = 0; i < function_count; i++) {
//         printf("%s() { %s }\n", functions[i].function_name, functions[i].function_body);
//     }

//     return 0;
// }
