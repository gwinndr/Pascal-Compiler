/*
    Damon Gwinn
    Performs semantic checking on a given parse tree
*/

#ifndef SEM_CHECK_H
#define SEM_CHECK_H

#include "../ParseTree/tree.h"
#include "../ParseTree/tree_types.h"

/* The main function for checking a tree */
bool start_semcheck(Tree_t *parse_tree);

#endif
