/* Damon Gwinn */
/* Where it all begins */

#include <stdio.h>
#include <stdlib.h>
#include "Parser/LexAndYacc/y.tab.h"

extern FILE *yyin;
extern int yyparse();
extern int yylex();

int main(int argc, char **argv)
{
    int yylex_return;

    if(argc == 2)
    {
        yyin = fopen(argv[1], "r");
        if(yyin == NULL)
        {
            fprintf(stderr, "Error opening file: %s\n", argv[1]);
            exit(1);
        }
    }

    while(yylex() != END_OF_FILE)
    ;

}
