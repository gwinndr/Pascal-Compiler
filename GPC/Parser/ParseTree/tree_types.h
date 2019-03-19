/*
    Damon Gwinn
    Helper types for tree.h
*/

#ifndef TREE_TYPES_H
#define TREE_TYPES_H

#include "TreeFifoList/TreeFifoList.h"

/* Enums for readability with types */
enum StmtType{STMT_VAR_ASSIGN, STMT_PROCEDURE_CALL, STMT_COMPOUND_STATEMENT,
    STMT_IF_THEN, STMT_WHILE, STMT_FOR, STMT_FOR_VAR, STMT_FOR_ASSIGN_VAR};

enum ExprType{EXPR_RELOP, EXPR_SIGN_TERM, EXPR_ADDOP, EXPR_MULOP,
    EXPR_VAR_ID, EXPR_ARRAY_ACCESS, EXPR_FUNCTION_CALL, EXPR_INUM, EXPR_RNUM};

/* A statement subtree */
struct Statement
{
    enum StmtType type;
    union stmt_data
    {
        /* Variable assignment */
        struct VarAssign
        {
            char *var;
            struct Expression *expr;
        } var_assign_data;

        /* Procedure call */
        struct ProcedureCall
        {
            char *id;
            TreeListNode_t *expr_args;
        } procedure_call_data;

        /* Compound Statements */
        TreeListNode_t *compound_statement;

        /* IF THEN ELSE */
        struct IfThenElse
        {
            struct Expression *relop_expr;
            struct Statement *if_stmt;
            struct Statement *else_stmt; /* NOTE: can be null */
        } if_then_data;

        /* WHILE */
        struct While
        {
            struct Expression *relop_expr;
            struct Statement *while_stmt;
        } while_data;

        /* FOR */
        struct For
        {
            enum StmtType for_assign_type;
            struct Expression *to;
            struct Statement *do_for;

            union for_assign
            {
                struct Statement *var_assign;
                struct Expression *var; /* Grammar will validate the correctness */
            } for_assign_data;
        } for_data;
    } stmt_data;
};

/* An expression subtree */
struct Expression
{
    int type;
    union expr_data
    {
        /* Relational expression */
        struct Relop
        {
            int type;

            /* NOTE: right subtree will be NULL when type is NOT or PAREN */
            struct Expression *left;
            struct Expression *right;
        } relop;

        /* Sign term */
        struct Expression *sign_term;

        /* ADDOP */
        struct Addop
        {
            struct Expression *left_expr;
            struct Expression *right_term;
        } addop;

        /* MULOP */
        struct Mulop
        {
            struct Expression *left_term;
            struct Expression *right_factor;
        } mulop;

        /* A variable ID */
        char *id;

        /* An indexed array */
        struct ArrayAccess
        {
            char *id;
            struct Expression *array_expr;
        } array_access;

        /* Function call */
        struct FunctionCall
        {
            char *id;
            TreeListNode_t *args_expr;
        } function_call;

        /* Integer number */
        int i_num;

        /* Real number */
        float r_num;
    } expr_data;
};


#endif
