/*
    Damon Gwinn
    Performs semantic checking on a given statement

    NOTE: Max scope level refers to the highest level scope we can reference a variable at
        - 0 is the current scope, 1 is the first above and so on
        - Functions can't have side effects, but they can contain procedures so this is a
            general way to define the maximum scope level
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "SemCheck_stmt.h"
#include "SemCheck_expr.h"
#include "../SymTab/SymTab.h"
#include "../../ParseTree/tree_types.h"
#include "../../List/List.h"

int semcheck_stmt_main(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);

int semcheck_varassign(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);
int semcheck_proccall(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);
int semcheck_compoundstmt(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);
int semcheck_ifthen(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);
int semcheck_while(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);
int semcheck_for(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev);

/* Semantic check on a normal statement */
int semcheck_stmt(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    return semcheck_stmt_main(symtab, stmt, max_scope_lev);
}

/* Semantic check on a function statement (no side effects allowed) */
int semcheck_func_stmt(SymTab_t *symtab, struct Statement *stmt)
{
    return semcheck_stmt_main(symtab, stmt, 0);
}

/* Main semantic checking */
int semcheck_stmt_main(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val;

    assert(symtab != NULL);
    assert(stmt != NULL);

    return_val = 0;
    switch(stmt->type)
    {
        case STMT_VAR_ASSIGN:
            return_val += semcheck_varassign(symtab, stmt, max_scope_lev);
            break;

        case STMT_PROCEDURE_CALL:
            return_val += semcheck_proccall(symtab, stmt, max_scope_lev);
            break;

        case STMT_COMPOUND_STATEMENT:
            return_val += semcheck_compoundstmt(symtab, stmt, max_scope_lev);
            break;

        case STMT_IF_THEN:
            return_val += semcheck_ifthen(symtab, stmt, max_scope_lev);
            break;

        case STMT_WHILE:
            return_val += semcheck_while(symtab, stmt, max_scope_lev);
            break;

        case STMT_FOR:
            return_val += semcheck_for(symtab, stmt, max_scope_lev);
            break;

        default:
            fprintf(stderr, "ERROR: Bad type in semcheck_stmt!\n");
            exit(1);
    }

    return return_val;
}


/****** STMT SEMCHECKS *******/

/** VAR_ASSIGN **/
int semcheck_varassign(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val;
    struct Expression *var, *expr;

    assert(symtab != NULL);
    assert(stmt != NULL);
    assert(stmt->type == STMT_VAR_ASSIGN);

    return_val = 0;

    var = stmt->stmt_data.var_assign_data.var;
    expr = stmt->stmt_data.var_assign_data.expr;

    return_val += semcheck_expr_main(symtab, var, max_scope_lev);
    return_val += semcheck_expr_main(symtab, expr, max_scope_lev);

    return return_val;
}

/** PROCEDURE_CALL **/
/* TODO: Type check arguments */
int semcheck_proccall(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val, scope_return;
    HashNode_t *sym_return;
    ListNode_t *cur_expr;

    assert(symtab != NULL);
    assert(stmt != NULL);
    assert(stmt->type == STMT_PROCEDURE_CALL);

    return_val = 0;

    cur_expr = stmt->stmt_data.procedure_call_data.expr_args;

    scope_return = FindIdent(&sym_return, symtab, (char *)stmt->stmt_data.procedure_call_data.id);
    if(scope_return == -1)
    {
        fprintf(stderr, "Error on line %d, unrecognized name %s\n", stmt->line_num,
            (char *)stmt->stmt_data.procedure_call_data.id);

            /*PrintSymTab(symtab, stderr, 0);*/

        ++return_val;
    }
    else
    {
        if(scope_return > max_scope_lev)
        {
            fprintf(stderr, "Error on line %d, %s cannot be called in the current context!\n",
                stmt->line_num, (char *)stmt->stmt_data.procedure_call_data.id);
            fprintf(stderr, "[Was it defined above the current function context?]\n");

            ++return_val;
        }
        if(sym_return->hash_type != HASHTYPE_PROCEDURE)
        {
            fprintf(stderr, "Error on line %d, expected %s to be a procedure!\n",
                stmt->line_num, (char *)stmt->stmt_data.procedure_call_data.id);

            ++return_val;
        }
    }

    return return_val;
}

/** COMPOUNT_STMT **/
int semcheck_compoundstmt(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val;
    ListNode_t *stmt_list;
    assert(symtab != NULL);
    assert(stmt != NULL);
    assert(stmt->type == STMT_COMPOUND_STATEMENT);

    return_val = 0;
    stmt_list = stmt->stmt_data.compound_statement;
    while(stmt_list != NULL)
    {
        assert(stmt_list->type == LIST_STMT);

        return_val += semcheck_stmt_main(symtab,
            (struct Statement *)stmt_list->cur, max_scope_lev);

        stmt_list = stmt_list->next;
    }


    return return_val;
}

/** IF_THEN **/
int semcheck_ifthen(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val;
    struct Expression *relop_expr;
    struct Statement *if_stmt, *else_stmt;

    assert(symtab != NULL);
    assert(stmt != NULL);
    assert(stmt->type == STMT_IF_THEN);

    return_val = 0;
    relop_expr = stmt->stmt_data.if_then_data.relop_expr;
    if_stmt = stmt->stmt_data.if_then_data.if_stmt;
    else_stmt = stmt->stmt_data.if_then_data.else_stmt;

    return_val += semcheck_expr_main(symtab, relop_expr, max_scope_lev);
    return_val += semcheck_stmt_main(symtab, if_stmt, max_scope_lev);
    if(else_stmt != NULL)
        return_val += semcheck_stmt_main(symtab, if_stmt, max_scope_lev);

    return return_val;
}

/** WHILE **/
int semcheck_while(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val;
    struct Expression *relop_expr;
    struct Statement *while_stmt;

    assert(symtab != NULL);
    assert(stmt != NULL);
    assert(stmt->type == STMT_WHILE);

    return_val = 0;
    relop_expr = stmt->stmt_data.while_data.relop_expr;
    while_stmt = stmt->stmt_data.while_data.while_stmt;

    return_val += semcheck_expr_main(symtab, relop_expr, max_scope_lev);
    return_val += semcheck_stmt_main(symtab, while_stmt, max_scope_lev);

    return return_val;
}

/** FOR **/
int semcheck_for(SymTab_t *symtab, struct Statement *stmt, int max_scope_lev)
{
    int return_val;
    enum StmtType for_assign_type; /* Either var or var_assign */
    struct Statement *for_assign;
    struct Expression *for_var;

    struct Expression *to_expr;
    struct Statement *do_for;

    assert(symtab != NULL);
    assert(stmt != NULL);
    assert(stmt->type == STMT_FOR);

    for_assign_type = stmt->stmt_data.for_data.for_assign_type;
    assert(for_assign_type == STMT_FOR_VAR || for_assign_type == STMT_FOR_ASSIGN_VAR);

    return_val = 0;
    if(for_assign_type == STMT_FOR_VAR)
    {
        for_var = stmt->stmt_data.for_data.for_assign_data.var;
        semcheck_expr_main(symtab, for_var, max_scope_lev);
    }
    else
    {
        for_assign = stmt->stmt_data.for_data.for_assign_data.var_assign;
        semcheck_stmt_main(symtab, for_assign, max_scope_lev);
    }


    to_expr = stmt->stmt_data.for_data.to;
    do_for = stmt->stmt_data.for_data.do_for;

    return_val += semcheck_expr_main(symtab, to_expr, max_scope_lev);
    return_val += semcheck_stmt_main(symtab, do_for, max_scope_lev);

    return return_val;
}
