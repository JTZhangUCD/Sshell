#ifndef inputHandler_h
#define inputHandler_h

#include "parse.h"
#include "parse.h"

struct Parse build_parse(char *input);
struct PipedParses build_parse_set(char *input);
void printParse(struct Parse *parse);

#endif /* inputHandler_h */
