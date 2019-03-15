/* Parse tree for the calculator */
#ifndef TREE_H
#define TREE_H

#define LEAF NULL

#include "tree_types.h"

typedef struct Tree Tree_t;
typedef struct Tree
{
    int type
    union data
    {
        /* Program Variables */
        struct Program
        {
            char *program_id;
            char **args;
            Tree_t **declarations;
            Tree_t **subprograms;
            Tree_t *body;
        };

        /* Declaration */
        struct Declaration
        {
            char **var_ids;
            int type;
        };

        struct Subprogram
        {
            
        }
    };

    Tree_t *left;
    Tree_t *right;

} Tree_t;

Tree_t *make_tree(int type, Tree_t *left, Tree_t *right);

int eval_tree(Tree_t *tree);

void print_tree(Tree_t *tree);

void destroy_tree(Tree_t *tree);

#endif
