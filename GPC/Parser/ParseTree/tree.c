/* Parse tree for the calculator */
#include "tree.h"
#include "y.tab.h"
#include <stdlib.h>
#include <stdio.h>

Tree_t *make_tree(int type, Tree_t *left, Tree_t *right)
{
    Tree_t *new_tree;

    new_tree = (Tree_t*)malloc(sizeof(Tree_t));
    new_tree->type = type;
    new_tree->left = left;
    new_tree->right = right;

    return new_tree;
}


/* Helper for print_tree */
void print_tree_indents(Tree_t *tree, int num_indents)
{
    if(tree != NULL)
    {
        int i;
        for(i = 0; i < num_indents; ++i)
            fprintf(stderr, "  ");

        switch(tree->type)
        {
            case PROGRAM:
                fprintf(stderr, "[PROGRAM: %s]", tree->data.id);
            case :
                return (eval_tree(tree->left) - eval_tree(tree->right));
            case '*':
                return (eval_tree(tree->left) * eval_tree(tree->right));
            case '/':

                return (eval_tree(tree->left) / right_eval);
            case NUM:
                return tree->val;
            default:
                fprintf(stderr, "Critical error in eval_tree: Unrecognized type\n");
                exit(1);
        }

        fprintf(stderr, "\n");
        print_tree_indents(tree->left, num_indents+1);
        print_tree_indents(tree->right, num_indents+1);

    }
}

void print_tree(Tree_t *tree)
{
    print_tree_indents(tree, 0);
}

void destroy_tree(Tree_t *tree)
{
    if(tree != NULL)
    {
        destroy_tree(tree->left);
        destroy_tree(tree->right);
        free(tree);
    }
}
