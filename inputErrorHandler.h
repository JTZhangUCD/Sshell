#ifndef inputErrorHandler_h
#define inputErrorHandler_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "func.h"

bool isMissingCMD (char* input);
bool parsesErrorHandler (struct PipedParses *parses);


#define MAX_INPUT_SIZE 512
#define MAX_ARGS_NUM 17 // 16+1 (space for null)

#endif /* inputErrorHandler_h */
