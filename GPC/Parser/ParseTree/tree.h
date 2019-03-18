/*
    Damon Gwinn
    Parse tree for the Pascal Grammar
*/
#ifndef TREE_H
#define TREE_H

#define LEAF NULL

#include "tree_types.h"

/* Types */
typedef struct Tree Tree_t;

/* Tree routines */



/******* Trees and statement types ********/

typedef struct Tree
{
    int type
    union data
    {
        /* Program Variables */
        struct Program
        {
            char *program_id;

            char *arg;
            char *next_arg;

            Tree_t *var_declaration;
            Tree_t *next_var_decl;

            Tree_t *subprogram;
            Tree_t *next_subprogrm;

            Tree_t *body_statement;
            Tree_t *next_body_stmt;
        };

        /* A subprogram */
        struct Subprogram
        {
            /* FUNCTION or PROCEDURE */
            int sub_type;
            char *id;
            Tree_t **args;
            int return_type; /* Should be -1 for PROCEDURE */

            Tree_t **declarations;
            Tree_t **subprograms;
            Tree_t **statement_list
        };

        /* A variable declaration */
        struct Var
        {
            char *id
            int type; /* Array, int, or real */
        };

        /* A single statement (Can be made up of multiple statements) */
        /* See "tree_types.h" for details */
        struct Statement *statement;

    };
} Tree_t;



#endif
