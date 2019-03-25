/*
    Damon Gwinn
    Creates a symbol table which is simply a stack of hash tables for identifiers
    Used to perform semantic checking on a ParseTree

    WARNING: Symbol table will NOT free given identifier strings when destroyed
        Remember to free given identifier strings manually
*/

#ifndef SYM_TAB_H
#define SYM_TAB_H

#include <stdio.h>
#include "../HashTable/HashTable.h"
#include "../../List/List.h"

/* A stack of hash tables */
typedef struct SymTab
{
    ListNode_t *stack_head;

} SymTab_t;

/* Initializes the SymTab with stack_head pointing to NULL */
SymTab_t *InitSymTab();

/* Pushes a new scope onto the stack (FIFO) */
void PushScope(SymTab_t *symtab);

/* Pushes a new identifier onto the current scope (head) */
/* HashType is VAR or ARRAY that describes the type of the given id */
void PushIdentOntoScope(SymTab_t *symtab, char *id, enum HashType type);

/* Searches for an identifier and returns the HashNode_t that gives the id and type information */
/* Returns NULL if not found */
HashNode_t *FindIdent(SymTab_t *symtab, char *id);

/* Pops the current scope */
void PopScope(SymTab_t *symtab);

/* Destroys the SymTab and all associated hash tables */
/* WARNING: Does not free given identifier strings */
void DestroySymTab(SymTab_t *symtab);

/* Prints the table for debugging */
void PrintSymTab(SymTab_t *symtab, FILE *f, int num_indent);

#endif
