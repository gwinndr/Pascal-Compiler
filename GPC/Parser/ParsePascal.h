/*
    Damon Gwinn
    Runs the pascal parser on the given file
*/

#ifndef PARSE_PASCAL_H
#define PARSE_PASCAL_H

/*#define DEBUG_BISON*/

#include <stdio.h>
#include <stdlib.h>
#include "ParseTree/tree.h"

Tree_t *ParsePascal(char *file);

#endif
