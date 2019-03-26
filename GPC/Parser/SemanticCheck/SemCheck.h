/*
    Damon Gwinn
    Performs semantic checking on a given parse tree
*/

#ifndef SEM_CHECK_H
#define SEM_CHECK_H

#include "../ParseTree/tree.h"

/* The main function for checking a tree */
/* Return values:
    0       -> Check successful
    -1      -> Check successful with warnings
    >= 1    -> Check failed with n errors
*/
int start_semcheck(Tree_t *parse_tree);

#endif
