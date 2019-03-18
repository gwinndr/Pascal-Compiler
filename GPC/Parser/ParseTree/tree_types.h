/*
    Damon Gwinn
    Helper types for tree.h
*/

#ifndef TREE_TYPES_H
#define TREE_TYPES_H

#include "../LexAndYacc/y.tab.h"

struct Expression;
struct RelopExpression;

/* A statement subtree */
struct Statement
{
    int type;
    union data
    {
        /* Variable assignment */
        struct VarAssign
        {
            char *var;
            struct Expression *expr;
        };

        /* Procedure call */
        struct ProcedureCall
        {
            char *id;
            struct Expression **args;
        };

        /* Compound Statement */
        struct Statement **compound_statements;

        /* IF THEN ELSE */
        struct IfThenElse
        {
            struct Expression *relop_expr;
            struct Statement *if_stmt;
            struct Statement *else_stmt; /* NOTE: can be null */
        };

        /* WHILE */
        struct While
        {
            struct Expression *relop_expr;
            struct Statement *while_stmt;
        };

        /* FOR */
        struct For
        {
            int for_assign_type;
            struct Expression *to;
            struct Statement *do;

            union for_assign
            {
                struct Statement *var_assign;
                struct Expression *var; /* Grammar will validate the correctness */
            };
        };
    };
};

/* An expression subtree */
struct Expression
{
    int type;
    union data
    {
        /* Relational expression */
        struct Relop
        {
            int type;

            /* NOTE: right subtree will be NULL when type is NOT or PAREN */
            struct Expression *left;
            struct Expression *right;
        };

        /* Sign term */
        struct Expression *sign_term;

        /* ADDOP */
        struct Addop
        {
            struct Expression *left_expr;
            struct Expression *right_term;
        };

        /* MULOP */
        struct Mulop
        {
            struct Expression *left_term;
            struct Expression *right_factor;
        };

        /* A variable ID */
        char *id;

        /* An indexed array */
        struct ArrayAccess
        {
            char *id;
            struct Expression *array_expr;
        };

        /* Function call */
        struct FunctionCall
        {
            char *id;
            struct Expression **arg_list;
        };

        /* Integer number */
        int i_num;

        /* Real number */
        int r_num;
    };
};


#endif
