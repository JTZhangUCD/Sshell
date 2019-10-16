#ifndef func_h
#define func_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "parse.h"

#define MAX_INPUT_SIZE 512
#define MAX_ARGS_NUM 17 // 16+1 (space for null)

bool isSpecialChar(char c);
int parseCounter (char *input);
int argCounter (char *input);
void inputRedir(char *filename);
void outputRedir(char *filename);

#endif /* func_h */
