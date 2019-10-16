#include "func.h"
#include "parse.h"

/* check if a character is a special character */
bool isSpecialChar(char c) {
    if (c == ' ' || c == '\n' || c == '<' || c == '>'
        || c == '\0' || c == '|' || c == '&') {
        return true;
    }
    return false;
}

// count number of parses in an input
int parseCounter (char *input) {
    if (input == NULL) return 0;
//    printf("parseCounter input: '%s'\n", input);
    int numOfParses = 1;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '|') {
            numOfParses++;
        }
    }
    return numOfParses;
}

// count number of arguments in an input
int argCounter (char *input) {
    if (input == NULL) return 0;
    int count = 0;
    bool inWord = false;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isSpecialChar(input[i]) && inWord == true) {
            count++;
            inWord = false;
        } else {
            inWord = true;
        }
    }
    return count;
}

/* input redirection */
void inputRedir(char *filename)
{
    int f = open(filename, O_RDONLY, 0644);
    if (f < 0) {
        perror("Error");
    }
    dup2(f, STDIN_FILENO);
    close(f);
    filename = malloc(MAX_INPUT_SIZE);
    filename = NULL;
}

/* output redirection */
void outputRedir(char *filename)
{
    int f = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (f < 0) {
        perror("Error");
    }
    dup2(f, STDOUT_FILENO);
    close(f);
    filename = malloc(MAX_INPUT_SIZE);
    filename = NULL;
}
