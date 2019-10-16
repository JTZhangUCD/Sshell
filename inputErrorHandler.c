#include "inputErrorHandler.h"

bool isMissingCMD (char* input) {
    bool hasCMD = false;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isSpecialChar(input[i])) {
            hasCMD = true;
        }
        if (input[i] == '|') {
            if (!hasCMD) {
                return true;
            }
            hasCMD = false;
        }
    }
    if (!hasCMD) {
        return true;
    }
    return false;
}

bool parsesErrorHandler (struct PipedParses *parses)
{
    // single:
    if (parses->numOfParses == 1) {
        // input redirection related:
        if (parses->parses[0].isInputRedir) {
            if (parses->parses[0].irFilename == NULL) {
                fprintf(stderr, "Error: no input file\n");
                return true;
            } else if (open(parses->parses[0].irFilename, O_RDONLY) == -1) {
                fprintf(stderr, "Error: cannot open input file\n");
                return true;
            }
        // output redirection related:
        } else if (parses->parses[0].isOutputRedir) {
            if (parses->parses[0].orFilename == NULL) {
                fprintf(stderr, "Error: no output file\n");
                return true;
            } else if (strcmp(parses->parses[0].orFilename, "/etc/passwd") == 0) {
                fprintf(stderr, "Error: cannot open output file\n");
                return true;
            }
        }
    // pipe
    } else {
        const int start = 0;
        const int end = parses->numOfParses - 1;
        
        for (int i = start+1; i <= end; i++) {
            struct Parse curparse = parses->parses[i];
            if (curparse.isInputRedir) {
                fprintf(stderr, "Error: mislocated input redirection\n");
                return true;
            }
        }
        for (int i = start; i < end; i++) {
            struct Parse curparse = parses->parses[i];
            if (curparse.isOutputRedir) {
                fprintf(stderr, "Error: mislocated output redirection\n");
                return true;
            }
        }
    }
    return false;
}
