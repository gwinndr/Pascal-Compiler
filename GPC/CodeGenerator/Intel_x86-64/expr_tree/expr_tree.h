/*
    Damon Gwinn
    Tree of simple expressions for the gencode algorithm
    TODO: Does not handle panic case (not enough registers)
*/

#ifndef EXPR_TREE_H
#define EXPR_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include "../../../Parser/List/List.h"
#include "../stackmng/stackmng.h"
#include "../../../Parser/ParseTree/tree.h"
#include "../../../Parser/ParseTree/tree_types.h"
#include "../../../Parser/LexAndYacc/y.tab.h"

typedef struct expr_node expr_node_t;

typedef struct expr_node
{
    int label;

    expr_node_t *left_expr;
    expr_node_t *right_expr;

    struct Expression *expr;
} expr_node_t;

expr_node_t *build_expr_tree(struct Expression *);
ListNode_t *gencode_expr_tree(expr_node_t *, RegStack_t *, ListNode_t *);
int expr_tree_is_leaf(expr_node_t *);
void print_expr_tree(expr_node_t *, int num_indent, FILE *);
void free_expr_tree(expr_node_t *);

#endif
