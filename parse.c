#include "parse.h"
#include "func.h"

/* this func initialized a parse with everything set to 0 */
void initParse(struct Parse *parse)
{
    parse->argCount = 0;
    
    // default redirection
    parse->isInputRedir = false;
    parse->isOutputRedir = false;
    
    // set filenames to null
    parse->irFilename = malloc(MAX_INPUT_SIZE);
    parse->orFilename = malloc(MAX_INPUT_SIZE);
    parse->irFilename = NULL;
    parse->orFilename = NULL;
    
    // set error indicator
    parse->argIsEmpty = true;
}
