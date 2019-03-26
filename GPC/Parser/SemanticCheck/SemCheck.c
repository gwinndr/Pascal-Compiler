/*
    Damon Gwinn
    Performs semantic checking on a given parse tree
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "SemCheck.h"
#include "../ParseTree/tree.h"
#include "../ParseTree/tree_types.h"

int semcheck_tree(Tree_t *tree, SymTab *symtab);
int semcheck_stmt(Tree_t *stmt, SymTab *symtab);
int semcheck_expr(Tree_t *expr, SymTab *symtab);

/* The main function for checking a tree */
/* Return values:
    0       -> Check successful
    -1      -> Check successful with warnings
    >= 1    -> Check failed with n errors
*/
int start_semcheck(Tree_t *parse_tree);
