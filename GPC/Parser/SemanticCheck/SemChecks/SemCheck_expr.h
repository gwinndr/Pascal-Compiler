/*
    Damon Gwinn
    Performs semantic checking on a given statement

    NOTE: Max scope level refers to the highest level scope we can reference a variable at
        - 0 is the current scope, 1 is the first above and so on
        - Functions can't have side effects, but they can contain procedures so this is a
            general way to define the maximum scope level
*/

#ifndef SEM_CHECK_EXPR_H
#define SEM_CHECK_EXPR_H

#include "../SymTab/SymTab.h"
#include "../../ParseTree/tree_types.h"

/* Semantic check on a normal expression */
int semcheck_expr(SymTab_t *symtab, struct Expression *expr);

/* Semantic check on a function expression (no side effects allowed) */
int semcheck_expr_func(SymTab_t *symtab, struct Expression *expr);

/* Main semantic checking */
/* NOTE: Using one of the above two functions is more readable */
int semcheck_expr_main(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);

#endif
