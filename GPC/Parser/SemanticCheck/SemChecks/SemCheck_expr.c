/*
    Damon Gwinn
    Performs semantic checking on a given statement

    NOTE: Max scope level refers to the highest level scope we can reference a variable at
        - 0 is the current scope, 1 is the first above and so on
        - Functions can't have side effects, but they can contain procedures so this is a
            general way to define the maximum scope level

    TODO: Check types match
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "SemCheck_expr.h"
#include "../HashTable/HashTable.h"
#include "../SymTab/SymTab.h"
#include "../../ParseTree/tree.h"
#include "../../ParseTree/tree_types.h"
#include "../../LexAndYacc/y.tab.h"

int semcheck_relop(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);
int semcheck_signterm(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);
int semcheck_addop(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);
int semcheck_mulop(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);
int semcheck_varid(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);
int semcheck_arrayaccess(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);
int semcheck_funccall(SymTab_t *symtab, struct Expression *expr, int max_scope_lev);

/* Semantic check on a normal expression */
int semcheck_expr(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    semcheck_expr_main(symtab, expr, max_scope_lev);
}

/* Semantic check on a function expression (no side effects allowed) */
int semcheck_expr_func(SymTab_t *symtab, struct Expression *expr)
{
    semcheck_expr_main(symtab, expr, 0);
}

/* Main semantic checking */
int semcheck_expr_main(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val;
    assert(symtab != NULL);
    assert(expr != NULL);

    return_val = 0;
    switch(expr->type)
    {
        case EXPR_RELOP:
            return_val += semcheck_relop(symtab, expr, max_scope_lev);
            break;

        case EXPR_SIGN_TERM:
            return_val += semcheck_signterm(symtab, expr, max_scope_lev);
            break;

        case EXPR_ADDOP:
            return_val += semcheck_addop(symtab, expr, max_scope_lev);
            break;

        case EXPR_MULOP:
            return_val += semcheck_mulop(symtab, expr, max_scope_lev);
            break;

        case EXPR_VAR_ID:
            return_val += semcheck_varid(symtab, expr, max_scope_lev);
            break;

        case EXPR_ARRAY_ACCESS:
            return_val += semcheck_arrayaccess(symtab, expr, max_scope_lev);
            break;

        case EXPR_FUNCTION_CALL:
            return_val += semcheck_funccall(symtab, expr, max_scope_lev);
            break;

        /*** BASE CASES ***/
        case EXPR_INUM:
        case EXPR_RNUM:
            break;

        default:
            fprintf(stderr, "ERROR: Bad type in semcheck_expr_main!\n");
            exit(1);
    }

    return return_val;
}

/****** EXPR SEMCHECKS *******/

/** RELOP **/
int semcheck_relop(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val;
    struct Expression *expr1, *expr2;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_RELOP);

    return_val = 0;
    expr1 = expr->expr_data.relop_data.left;
    expr2 = expr->expr_data.relop_data.right;

    return_val += semcheck_expr_main(symtab, expr1, max_scope_lev);
    if(expr2 != NULL)
        return_val += semcheck_expr_main(symtab, expr2, max_scope_lev);

    return return_val;
}

/** SIGN_TERM **/
int semcheck_signterm(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val;
    struct Expression *sign_expr;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_SIGN_TERM);

    return_val = 0;
    sign_expr = expr->expr_data.sign_term;

    return_val += semcheck_expr_main(symtab, sign_expr, max_scope_lev);

    return return_val;
}

/** ADDOP **/
int semcheck_addop(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val;
    struct Expression *expr1, *expr2;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_ADDOP);

    return_val = 0;
    expr1 = expr->expr_data.addop_data.left_expr;
    expr2 = expr->expr_data.addop_data.right_term;

    return_val += semcheck_expr_main(symtab, expr1, max_scope_lev);
    return_val += semcheck_expr_main(symtab, expr2, max_scope_lev);

    return return_val;
}

/** MULOP **/
int semcheck_mulop(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val;
    struct Expression *expr1, *expr2;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_MULOP);

    return_val = 0;
    expr1 = expr->expr_data.mulop_data.left_term;
    expr2 = expr->expr_data.mulop_data.right_factor;

    return_val += semcheck_expr_main(symtab, expr1, max_scope_lev);
    return_val += semcheck_expr_main(symtab, expr2, max_scope_lev);

    return return_val;
}

/** VAR_ID **/
int semcheck_varid(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val, scope_return;
    char *id;
    HashNode_t *hash_return;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_VAR_ID);

    return_val = 0;
    id = expr->expr_data.id;

    scope_return = FindIdent(&hash_return, symtab, id);
    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, undeclared identifier \"%s\"!\n\n", expr->line_num, id);
        ++return_val;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, \"%s\" defined in unreachable scope!\n",
                expr->line_num, id);
            fprintf(stderr, "[Was it defined above a function declaration?]\n\n");
            ++return_val;
        }
        if(hash_return->hash_type != HASHTYPE_VAR &&
            hash_return->hash_type != HASHTYPE_FUNCTION_RETURN)
        {
            fprintf(stderr, "Error on line %d, \"%s\" is not a scalar variable!\n\n",
                expr->line_num, id);
            ++return_val;
        }
    }

    return return_val;
}

/** ARRAY_ACCESS **/
int semcheck_arrayaccess(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val, scope_return;
    char *id;
    struct Expression *access_expr;
    HashNode_t *hash_return;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_ARRAY_ACCESS);

    return_val = 0;
    id = expr->expr_data.array_access_data.id;
    access_expr = expr->expr_data.array_access_data.array_expr;

    /***** FIRST VERIFY ARRAY IDENTIFIER *****/

    scope_return = FindIdent(&hash_return, symtab, id);
    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, undeclared identifier \"%s\"!\n\n", expr->line_num, id);
        ++return_val;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, \"%s\" defined in unreachable scope!\n",
                expr->line_num, id);
            fprintf(stderr, "[Was it defined above a function declaration?]\n\n");
            ++return_val;
        }
        if(hash_return->hash_type != HASHTYPE_ARRAY)
        {
            fprintf(stderr, "Error on line %d, \"%s\" is not an array variable!\n\n",
                expr->line_num, id);
            ++return_val;
        }
    }

    /***** THEN VERIFY EXPRESSION INSIDE *****/
    return_val += semcheck_expr_main(symtab, access_expr, max_scope_lev);

    return return_val;
}

/** FUNC_CALL **/
/* TODO: Verify args */
int semcheck_funccall(SymTab_t *symtab, struct Expression *expr, int max_scope_lev)
{
    int return_val, scope_return;
    char *id;
    ListNode_t *args;
    HashNode_t *hash_return;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_FUNCTION_CALL);

    return_val = 0;
    id = expr->expr_data.function_call_data.id;
    args = expr->expr_data.function_call_data.args_expr;

    /***** FIRST VERIFY FUNCTION IDENTIFIER *****/

    scope_return = FindIdent(&hash_return, symtab, id);
    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, undeclared identifier %s!\n\n", expr->line_num, id);
        ++return_val;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, \"%s\" defined in unreachable scope!\n\n",
                expr->line_num, id);
            fprintf(stderr, "[Was it defined above a function declaration?]\n\n");
            ++return_val;
        }
        if(hash_return->hash_type != HASHTYPE_FUNCTION &&
            hash_return->hash_type != HASHTYPE_FUNCTION_RETURN)
        {
            fprintf(stderr, "Error on line %d, \"%s\" is not a function!\n\n",
                expr->line_num, id);
            ++return_val;
        }
    }

    /***** THEN VERIFY ARGS INSIDE *****/
    while(args != NULL)
    {
        assert(args->type == LIST_EXPR);
        return_val += semcheck_expr_main(symtab, (struct Expression *)args->cur, max_scope_lev);
        args = args->next;
    }

    return return_val;
}
