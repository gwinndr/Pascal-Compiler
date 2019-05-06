/*
    Damon Gwinn
    Performs semantic checking on a given statement

    NOTE: Max scope level refers to the highest level scope we can reference a variable at
        - 0 is the current scope, 1 is the first above and so on
        - Functions can't have side effects, but they can contain procedures so this is a
            general way to define the maximum scope level

    All functions set the pointer type_return to the type the expression will return
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

int is_type_ir(int *type);
int is_and_or(int *type);
int set_type_from_hashtype(int *type, HashNode_t *hash_node);

int semcheck_relop(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);
int semcheck_signterm(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);
int semcheck_addop(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);
int semcheck_mulop(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);
int semcheck_varid(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);
int semcheck_arrayaccess(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);
int semcheck_funccall(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating);

/* Sets hash meta based on given mutating flag */
void set_hash_meta(HashNode_t *node, int mutating)
{
    if(mutating == BOTH_MUTATE_REFERENCE)
    {
        node->referenced += 1;
        node->mutated += 1;
    }
    else
    {
        node->referenced += 1-mutating;
        node->mutated += mutating;
    }
}

/* Verifies a type is an INT_TYPE or REAL_TYPE */
int is_type_ir(int *type)
{
    assert(type != NULL);
    return (*type == INT_TYPE || *type == REAL_TYPE);
}
/* Checks if a type is a relational AND or OR */
int is_and_or(int *type)
{
    assert(type != NULL);
    return (*type == AND || *type == OR);
}
/* Sets a type based on a hash_type */
int set_type_from_hashtype(int *type, HashNode_t *hash_node)
{
    assert(type != NULL);
    assert(hash_node != NULL);

    switch(hash_node->var_type)
    {
        case HASHVAR_INTEGER:
            *type = INT_TYPE;
            break;
        case HASHVAR_REAL:
            *type = REAL_TYPE;
            break;
        case HASHVAR_PROCEDURE:
            *type = PROCEDURE;
            break;
        case HASHVAR_UNTYPED:
            *type = UNKNOWN_TYPE;
            break;
        default:
            fprintf(stderr, "ERROR in set_type_from_hashtype, bad types!\n");
            exit(1);
    }
}

/* Semantic check on a normal expression */
int semcheck_expr(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    semcheck_expr_main(type_return, symtab, expr, max_scope_lev, mutating);
}

/* Semantic check on a function expression (no side effects allowed) */
int semcheck_expr_func(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int mutating)
{
    semcheck_expr_main(type_return, symtab, expr, 0, mutating);
}

/* Main semantic checking */
int semcheck_expr_main(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val;
    assert(symtab != NULL);
    assert(expr != NULL);

    return_val = 0;
    switch(expr->type)
    {
        case EXPR_RELOP:
            return_val += semcheck_relop(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        case EXPR_SIGN_TERM:
            return_val += semcheck_signterm(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        case EXPR_ADDOP:
            return_val += semcheck_addop(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        case EXPR_MULOP:
            return_val += semcheck_mulop(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        case EXPR_VAR_ID:
            return_val += semcheck_varid(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        case EXPR_ARRAY_ACCESS:
            return_val += semcheck_arrayaccess(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        case EXPR_FUNCTION_CALL:
            return_val += semcheck_funccall(type_return, symtab, expr, max_scope_lev, mutating);
            break;

        /*** BASE CASES ***/
        case EXPR_INUM:
            *type_return = INT_TYPE;
            break;

        case EXPR_RNUM:
            *type_return = REAL_TYPE;
            break;

        default:
            fprintf(stderr, "ERROR: Bad type in semcheck_expr_main!\n");
            exit(1);
    }

    return return_val;
}

/****** EXPR SEMCHECKS *******/

/** RELOP **/
int semcheck_relop(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val;
    int type_first, type_second;
    struct Expression *expr1, *expr2;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_RELOP);

    return_val = 0;
    expr1 = expr->expr_data.relop_data.left;
    expr2 = expr->expr_data.relop_data.right;

    return_val += semcheck_expr_main(&type_first, symtab, expr1, max_scope_lev, mutating);
    if(expr2 != NULL)
        return_val += semcheck_expr_main(&type_second, symtab, expr2, max_scope_lev, mutating);

    /* Verifying types */

    /* Type must be a bool (this only happens with a NOT operator) */
    if(expr2 == NULL && type_first != BOOL)
    {
        fprintf(stderr, "Error on line %d, expected relational type after \"NOT\"!\n\n",
            expr->line_num);
        ++return_val;
    }
    else
    {
        if(type_first != type_second)
        {
            fprintf(stderr, "Error on line %d, relational types do not match!\n\n",
                expr->line_num);
            ++return_val;
        }

        /* Checking AND OR semantics */
        if(is_and_or(&expr->expr_data.relop_data.type))
        {
            if(type_first != BOOL || type_second != BOOL)
            {
                fprintf(stderr,
                    "Error on line %d, expected two relational types between AND/OR!\n\n",
                    expr->line_num);
                ++return_val;
            }
        }
        else
        {
            if(!is_type_ir(&type_first) || !is_type_ir(&type_second))
            {
                fprintf(stderr, "Error on line %d, expected two int/reals between relational op!\n\n",
                    expr->line_num);
                ++return_val;
            }
        }
    }

    *type_return = BOOL;
    return return_val;
}

/** SIGN_TERM **/
int semcheck_signterm(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val;
    struct Expression *sign_expr;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_SIGN_TERM);

    return_val = 0;
    sign_expr = expr->expr_data.sign_term;

    return_val += semcheck_expr_main(type_return, symtab, sign_expr, max_scope_lev, mutating);

    /* Checking types */
    if(!is_type_ir(type_return))
    {
        fprintf(stderr, "Error on line %d, expected int or real after \"-\"!\n\n",
            expr->line_num);
        ++return_val;
    }

    return return_val;
}

/** ADDOP **/
int semcheck_addop(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val;
    int type_first, type_second;
    struct Expression *expr1, *expr2;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_ADDOP);

    return_val = 0;
    expr1 = expr->expr_data.addop_data.left_expr;
    expr2 = expr->expr_data.addop_data.right_term;

    return_val += semcheck_expr_main(&type_first, symtab, expr1, max_scope_lev, mutating);
    return_val += semcheck_expr_main(&type_second, symtab, expr2, max_scope_lev, mutating);

    /* Checking types */
    if(type_first != type_second)
    {
        fprintf(stderr, "Error on line %d, type mismatch on addop!\n\n",
            expr->line_num);
        ++return_val;
    }
    if(!is_type_ir(&type_first) || !is_type_ir(&type_second))
    {
        fprintf(stderr, "Error on line %d, expected int/real on both sides of addop!\n\n",
            expr->line_num);
        ++return_val;
    }

    *type_return = type_first;
    return return_val;
}

/** MULOP **/
int semcheck_mulop(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val;
    int type_first, type_second;
    struct Expression *expr1, *expr2;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_MULOP);

    return_val = 0;
    expr1 = expr->expr_data.mulop_data.left_term;
    expr2 = expr->expr_data.mulop_data.right_factor;

    return_val += semcheck_expr_main(&type_first, symtab, expr1, max_scope_lev, mutating);
    return_val += semcheck_expr_main(&type_second, symtab, expr2, max_scope_lev, mutating);

    /* Checking types */
    if(type_first != type_second)
    {
        fprintf(stderr, "Error on line %d, type mismatch on mulop!\n\n",
            expr->line_num);
        ++return_val;
    }
    if(!is_type_ir(&type_first) || !is_type_ir(&type_second))
    {
        fprintf(stderr, "Error on line %d, expected int/real on both sides of mulop!\n\n",
            expr->line_num);
        ++return_val;
    }

    *type_return = type_first;
    return return_val;
}

/** VAR_ID **/
int semcheck_varid(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
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
    set_hash_meta(hash_return, mutating);

    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, undeclared identifier \"%s\"!\n\n", expr->line_num, id);
        ++return_val;

        *type_return = UNKNOWN_TYPE;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, cannot change \"%s\", invalid scope!\n",
                expr->line_num, id);
            fprintf(stderr, "[Was it defined above a function declaration?]\n\n");
            ++return_val;
        }
        if(hash_return->hash_type != HASHTYPE_VAR &&
            hash_return->hash_type != HASHTYPE_FUNCTION_RETURN)
        {
            fprintf(stderr, "Error on line %d, cannot assign \"%s\", is not a scalar variable!\n\n",
                expr->line_num, id);
            ++return_val;
        }
        set_type_from_hashtype(type_return, hash_return);
    }

    return return_val;
}

/** ARRAY_ACCESS **/
int semcheck_arrayaccess(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val, scope_return;
    char *id;
    int expr_type;
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
    set_hash_meta(hash_return, mutating);

    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, undeclared identifier \"%s\"!\n\n", expr->line_num, id);
        ++return_val;

        *type_return = UNKNOWN_TYPE;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, cannot change \"%s\", invalid scope!\n",
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

        set_type_from_hashtype(type_return, hash_return);
    }

    /***** THEN VERIFY EXPRESSION INSIDE *****/
    return_val += semcheck_expr_main(&expr_type, symtab, access_expr, max_scope_lev, NO_MUTATE);
    if(expr_type != INT_TYPE)
    {
        fprintf(stderr, "Error on line %d, expected int in array index expression!\n\n",
            expr->line_num);
        ++return_val;
    }

    return return_val;
}

/** FUNC_CALL **/
int semcheck_funccall(int *type_return,
    SymTab_t *symtab, struct Expression *expr, int max_scope_lev, int mutating)
{
    int return_val, scope_return;
    char *id;
    int arg_type, cur_arg;
    ListNode_t *true_args, *true_arg_ids, *args_given;
    HashNode_t *hash_return;
    Tree_t *arg_decl;
    assert(symtab != NULL);
    assert(expr != NULL);
    assert(expr->type == EXPR_FUNCTION_CALL);

    return_val = 0;
    id = expr->expr_data.function_call_data.id;
    args_given = expr->expr_data.function_call_data.args_expr;

    /***** FIRST VERIFY FUNCTION IDENTIFIER *****/

    scope_return = FindIdent(&hash_return, symtab, id);
    set_hash_meta(hash_return, mutating);

    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, undeclared identifier %s!\n\n", expr->line_num, id);
        ++return_val;

        *type_return = UNKNOWN_TYPE;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, cannot change \"%s\", invalid scope!\n\n",
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

        set_type_from_hashtype(type_return, hash_return);

        /***** THEN VERIFY ARGS INSIDE *****/
        cur_arg = 0;
        true_args = hash_return->args;
        while(args_given != NULL && true_args != NULL)
        {
            ++cur_arg;
            assert(args_given->type == LIST_EXPR);
            assert(true_args->type == LIST_TREE);
            return_val += semcheck_expr_main(&arg_type,
                symtab, (struct Expression *)args_given->cur, max_scope_lev, NO_MUTATE);

            arg_decl = (Tree_t *)true_args->cur;
            assert(arg_decl->type == TREE_VAR_DECL);
            true_arg_ids = arg_decl->tree_data.var_decl_data.ids;

            while(true_arg_ids != NULL && args_given != NULL)
            {
                if(arg_type != arg_decl->tree_data.var_decl_data.type &&
                    arg_decl->tree_data.var_decl_data.type != BUILTIN_ANY_TYPE)
                {
                    fprintf(stderr, "Error on line %d, on function call %s, argument %d: Type mismatch!\n\n",
                        expr->line_num, id, cur_arg);
                    ++return_val;
                }

                true_arg_ids = true_arg_ids->next;
                args_given = args_given->next;
            }

            true_args = true_args->next;
        }
        if(true_args == NULL && args_given != NULL)
        {
            fprintf(stderr, "Error on line %d, on function call %s, too many arguments given!\n\n",
                expr->line_num, id);
            ++return_val;
        }
        else if(true_args != NULL && args_given == NULL)
        {
            fprintf(stderr, "Error on line %d, on function call %s, not enough arguments given!\n\n",
                expr->line_num, id);
            ++return_val;
        }
    }

    return return_val;
}
