/* Damon Gwinn */
/* Where it all begins */

#include <stdio.h>
#include "Parser/LexAndYacc/y.tab.h"

/*extern FILE *yyin;*/
extern int yyparse();
extern int yylex();

int main(int argc, char **argv)
{
    /*yyin = fopen("argv[1]", "r");*/
    while(1)
        yylex();
}
