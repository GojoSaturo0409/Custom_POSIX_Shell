#ifndef ALIAS_PARSING_H
#define ALIAS_PARSING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void add_alias(const char *name, const char *command);
void add_function(const char *name, const char *body);
void parse_line(char *line);
const char* find_alias(const char *name) ;
void trim_quotes(char *str);
void trim_whitespace(char *str);


#endif