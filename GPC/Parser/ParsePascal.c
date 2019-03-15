/*
    Damon Gwinn
    Runs the pascal parser on the given file
*/

#include <stdio.h>
#include <stdlib.h>
#include "ParsePascal.h"
#include "ErrVars.h"
#include "LexAndYacc/y.tab.h"

extern FILE *yyin;
extern int yyparse();

int ParsePascal(char *file)
{
    if(file != NULL)
    {
        yyin = fopen(file, "r");
        if(yyin == NULL)
        {
            fprintf(stderr, "Error opening file: %s\n", file);
            exit(1);
        }
        file_to_parse = file;
    }
    else
        file_to_parse = NULL;

    line_num = 1;
    col_num = 1;
    yyparse();
}
