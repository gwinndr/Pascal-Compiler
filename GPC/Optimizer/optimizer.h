/*
    Damon Gwinn
    Simple optimizer for a parse tree

    OPTIMIZER INFO:
        - All non-referenced stack variables removed
        - All constant number expressions simplified to a single number

    WARNING: Optimizer designed to work in unison with the parser
*/

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#define DEBUG_OPTIMIZER

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../Parser/ParseTree/tree.h"
#include "../Parser/ParseTree/tree_types.h"
#include "../Parser/SemanticCheck/SymTab/SymTab.h"
#include "../Parser/SemanticCheck/HashTable/HashTable.h"

void optimize(SymTab_t *, Tree_t *);

#endif
