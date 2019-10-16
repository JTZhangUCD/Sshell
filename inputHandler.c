#include "inputHandler.h"
#include "parse.h"
#include "func.h"

// prints the values of a parse (for debug purpose)
void printParse(struct Parse *parse)
{
    printf("Print parse:\n");
    printf("--> Number of args: %d\n", parse->argCount);
    int argIndex = 0;
    while (parse->args[argIndex] != NULL) {
        printf("--> args[%d]: '%s'\n", argIndex, parse->args[argIndex]);
        argIndex++;
    }
    if (parse->isInputRedir) {
        printf("input Redirected; Filename: '%s'\n", parse->irFilename);
    }
    if (parse->isOutputRedir) {
        printf("output Redirected; Filename: '%s'\n", parse->orFilename);
    }
    printf("\n");
}

/* build_parse
* update on inputHandler to adapt to new data structure
* will generate a parse based on one single parse
* dont neet to worry about '|'; handled in build_parse_set
*/

struct Parse build_parse(char *input)
{
    struct Parse parse;
    initParse(&parse);
    
    char* temp = malloc(MAX_INPUT_SIZE);
    int tempIndex = 0;
    bool lastWord = true;
    
    for (int inputIndex = 0; input[inputIndex] != '\0'; inputIndex++) {
        // check max num of args
        if (parse.argCount > 16) {
            break;
        }
        if (input[inputIndex] == '<') {
            parse.isInputRedir = true;
        }
        if (input[inputIndex] == '>') {
            parse.isOutputRedir = true;
        }
        if (isSpecialChar(input[inputIndex])) {
            if (tempIndex == 0) continue;
            if (parse.isInputRedir && parse.irFilename == NULL) {
                parse.irFilename = malloc(MAX_INPUT_SIZE);
                strcpy(parse.irFilename, temp);
                memset(temp, 0, MAX_INPUT_SIZE);
                tempIndex = 0;
            } else if (parse.isOutputRedir && parse.orFilename == NULL) {
                parse.orFilename = malloc(MAX_INPUT_SIZE);
                strcpy(parse.orFilename, temp);
                memset(temp, 0, MAX_INPUT_SIZE);
                tempIndex = 0;
            } else {
                parse.argIsEmpty = true;
                parse.args[parse.argCount] = malloc(strlen(temp));
                strcpy(parse.args[parse.argCount], temp);
                parse.argCount++;
                memset(temp, 0, MAX_INPUT_SIZE);
                tempIndex = 0;
            }
            lastWord = false;
        } else {
            temp[tempIndex] = input[inputIndex];
            tempIndex++;
            lastWord = true;
        }
    }
    if (lastWord) {
        if (temp != NULL) {
            if (parse.isInputRedir && parse.irFilename == NULL) {
                parse.irFilename = malloc(MAX_INPUT_SIZE);
                strcpy(parse.irFilename, temp);
            } else if (parse.isOutputRedir && parse.orFilename == NULL) {
                parse.orFilename = malloc(MAX_INPUT_SIZE);
                strcpy(parse.orFilename, temp);
            } else {
                parse.argIsEmpty = true;
                parse.args[parse.argCount] = malloc(strlen(temp));
                strcpy(parse.args[parse.argCount], temp);
                parse.argCount++;
            }
        }
    }
    free(temp);
    parse.args[parse.argCount] = NULL;
//    printParse(&parse);
    return parse;
}

/* build_parse_set
 * --> adapted to the new data structure
 * --> instead of returning a error index, return PipedParses
 * --> will use build_parse to handle each single parse
 */

struct PipedParses build_parse_set(char *input)
{
    struct PipedParses parses;
    int numOfParses = parseCounter(input);  // find number of parses:
    parses.numOfParses = numOfParses;
    
    if (numOfParses == 1) {
        struct Parse parse = build_parse(input);
        parses.parses[0] = parse;
    } else {
        int parseIndex = 0;
        char* parseString = malloc(MAX_INPUT_SIZE);
        int parseStringIndex = 0;
        bool lastWord = true;
        for (int inputIndex = 0; input[inputIndex] != '\0'; inputIndex++) {
            if (input[inputIndex] == '|') {
                parses.parses[parseIndex] = build_parse(parseString);
                memset(parseString, 0, MAX_INPUT_SIZE);
                parseStringIndex = 0;
                parseIndex++;
                lastWord = false;
            } else {
                parseString[parseStringIndex] = input[inputIndex];
                parseStringIndex++;
                lastWord = true;
            }
        }
        if (lastWord) {
            if (parseString != NULL) {
                parses.parses[parseIndex] = build_parse(parseString);
            }
        }
    }
    return parses;
}
