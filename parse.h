#ifndef parse_h
#define parse_h

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

#define MAX_INPUT_SIZE 512
#define MAX_ARGS_NUM 17 // 16+1 (space for null)

struct Parse {
    char *args[MAX_ARGS_NUM];
    int argCount;
    
    // replace cmdFlag:
    bool isInputRedir;
    bool isOutputRedir;
    
    // replace filename:
    char *irFilename;
    char *orFilename;
    
    // check error
    bool argIsEmpty;
};

struct PipedParses {
    struct Parse parses[MAX_ARGS_NUM];
    int numOfParses;
};

void initParse(struct Parse *parse);

#endif /* parse_h */
