/*
    Damon Gwinn
    Parse tree for the Pascal Grammar
*/
#ifndef TREE_H
#define TREE_H

#define LEAF NULL

#include "TreeFifoList/TreeFifoList.h"
#include "tree_types.h"

/* Types */
typedef struct Tree Tree_t;

/* WARNING: Copies are NOT made. Make sure given pointers are safe! */
/* WARNING: Destroying the tree WILL free given pointers. Do not reference after free! */

/* Tree routines */
Tree_t *mk_program(char *id, TreeListNode_t *args, TreeListNode_t *var_decl,
    TreeListNode_t *subprograms, struct Statement *compound_statement);

Tree_t *mk_procedure(char *id, TreeListNode_t *args, TreeListNode_t *var_decl,
    TreeListNode_t *subprograms, struct Statement *compound_statement);

Tree_t *mk_vardecl(char *id, int type);

/* Statement routines */
struct Statement *mk_varassign(char *var, struct Expression *expr);

struct Statement *mk_procedurecall(char *id, TreeListNode_t *expr_args);

struct Statement *mk_compoundstatement(TreeListNode_t *compound_statement);

struct Statement *mk_ifthen(struct Expression *eval_relop, struct Statement *if_stmt,
                            struct Statement *else_stmt);

struct Statement *mk_while(struct Expression *eval_relop, struct Statement *while_stmt);

struct Statement *mk_forassign(struct Statement *for_assign, struct Expression *to,
                               struct Statement *do_for);

struct Statement *mk_forvar(struct Expression *for_var, struct Expression *to,
                              struct Statement *do_for);

/* Expression routines */
struct Expression *mk_relop(int type, struct Expression *left, struct Expression *right);

struct Expression *mk_signterm(struct Expression *sign_term);

struct Expression *mk_addop(struct Expression *left, struct Expression *right);

struct Expression *mk_mulop(struct Expression *left, struct Expression *right);

struct Expression *mk_varid(char *id);

struct Expression *mk_arrayaccess(char *id, struct Expression *index_expr);

struct Expression *mk_functioncall(char *id, TreeListNode_t *args);

struct Expression *mk_inum(int i_num);

struct Expression *mk_rnum(float r_num);


/******* Trees and statement types ********/
/* Enum for readability */
enum TreeType{TREE_PROGRAM_TYPE, TREE_SUBPROGRAM, TREE_VAR_DECL, TREE_STATEMENT_TYPE,
              TREE_SUBPROGRAM_PROC, TREE_SUBPROGRAM_FUNC};

typedef struct Tree
{
    int type;
    union tree_data
    {
        /* Program Variables */
        struct Program
        {
            char *program_id;

            TreeListNode_t *args_char;
            TreeListNode_t *var_declaration;
            TreeListNode_t *subprograms;
            struct Statement *body_statement;
        } program_data;

        /* A subprogram */
        struct Subprogram
        {
            /* FUNCTION or PROCEDURE */
            enum TreeType sub_type;
            char *id;
            TreeListNode_t *args_var;
            int return_type; /* Should be -1 for PROCEDURE */

            TreeListNode_t *declarations;
            TreeListNode_t *subprograms;
            struct Statement *statement_list;
        } subprogram_data;

        /* A variable declaration */
        struct Var
        {
            char *id;
            int type; /* Array, int, or real */
        } var_decl_data;

        /* A single statement (Can be made up of multiple statements) */
        /* See "tree_types.h" for details */
        struct Statement *statement_data;

    } tree_data;
} Tree_t;



#endif
