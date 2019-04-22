/*
    Damon Gwinn
    Runs the pascal parser on the given file
*/

#include <stdio.h>
#include <stdlib.h>
#include "ParsePascal.h"
#include "ErrVars.h"
#include "ParseTree/tree.h"
#include "ParseTree/tree_types.h"
#include "List/List.h"
#include "SemanticCheck/SemCheck.h"
#include "LexAndYacc/y.tab.h"

extern FILE *yyin;
extern int yyparse();

/* Initializes parser globals */
void InitParser();

Tree_t *ParsePascal(char *file)
{
    int semcheck_return;

    /**** CREATING THE PARSE TREE ****/
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

    InitParser();
    yyparse();

    #ifdef DEBUG_BISON
        if(parse_tree != NULL)
            tree_print(parse_tree, stderr, 0);
    #endif

    /**** SEMANTIC CHECKING ****/
    semcheck_return = start_semcheck(parse_tree);

    if(semcheck_return > 0)
    {
        destroy_tree(parse_tree);
        parse_tree = NULL;
    }

    return parse_tree;
}

void InitParser()
{
    line_num = 1;
    col_num = 1;
    parse_tree = NULL;
}
