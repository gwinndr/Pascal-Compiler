/*
    Damon Gwinn
    Creates a symbol table which is simply a stack of hash tables for identifiers
    Used to perform semantic checking on a ParseTree

    WARNING: Symbol table will NOT free given identifier strings when destroyed
        Remember to free given identifier strings manually
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "SymTab.h"
#include "../HashTable/HashTable.h"
#include "../../List/List.h"

/* Initializes the SymTab with stack_head pointing to NULL */
SymTab_t *InitSymTab()
{
    SymTab_t *new_symtab;

    new_symtab = (SymTab_t *)malloc(sizeof(SymTab_t));
    new_symtab->stack_head = NULL;

    return new_symtab;
}

/* Pushes a new scope onto the stack (FIFO) */
void PushScope(SymTab_t *symtab)
{
    assert(symtab != NULL);

    HashTable_t *new_hash;
    new_hash = InitHashTable();

    if(symtab->stack_head == NULL)
        symtab->stack_head = CreateListNode(new_hash, LIST_UNSPECIFIED);
    else
        symtab->stack_head = PushListNodeFront(symtab->stack_head,
            CreateListNode(new_hash, LIST_UNSPECIFIED));
}

/* Pushes a new variable onto the current scope (head) */
void PushVarOntoScope(SymTab_t *symtab, char *id)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    cur_hash = (HashTable_t *)symtab->stack_head->cur;
    AddIdentToTable(cur_hash, id, VAR, NULL);
}

/* Pushes a new array onto the current scope (head) */
void PushArrayOntoScope(SymTab_t *symtab, char *id)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    cur_hash = (HashTable_t *)symtab->stack_head->cur;
    AddIdentToTable(cur_hash, id, ARRAY, NULL);
}

/* Pushes a new procedure onto the current scope (head) */
/* NOTE: args can be NULL to represent no args */
void PushProcedureOntoScope(SymTab_t *symtab, char *id, ListNode_t *args)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    cur_hash = (HashTable_t *)symtab->stack_head->cur;
    AddIdentToTable(cur_hash, id, PROCEDURE, args);
}

/* Pushes a new function onto the current scope (head) */
/* NOTE: args can be NULL to represent no args */
void PushFunctionOntoScope(SymTab_t *symtab, char *id, ListNode_t *args)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    cur_hash = (HashTable_t *)symtab->stack_head->cur;
    AddIdentToTable(cur_hash, id, FUNCTION, args);
}

/* Searches for an identifier and returns the HashNode_t that gives the id and type information */
/* Returns NULL if not found */
HashNode_t *FindIdent(SymTab_t *symtab, char *id)
{
    assert(symtab != NULL);

    ListNode_t *cur, *next;
    HashNode_t *hash_node;

    cur = symtab->stack_head;
    while(cur != NULL)
    {
        hash_node = FindIdentInTable((HashTable_t *)cur->cur, id);
        if(hash_node != NULL)
            return hash_node;
    }
    return NULL;
}

/* Pops the current scope */
void PopScope(SymTab_t *symtab)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    ListNode_t *cur;
    cur = symtab->stack_head;
    symtab->stack_head = symtab->stack_head->next;

    DestroyHashTable((HashTable_t *)cur->cur);
    free(cur);
}

/* Destroys the SymTab and all associated hash tables */
/* WARNING: Does not free given identifier strings */
void DestroySymTab(SymTab_t *symtab)
{
    assert(symtab != NULL);

    ListNode_t *cur, *next;

    cur = symtab->stack_head;
    while(cur != NULL)
    {
        next = cur->next;
        DestroyHashTable((HashTable_t *)cur->cur);
        free(cur);

        cur = next;
    }

    free(symtab);
}

/* Prints the table for debugging */
void PrintSymTab(SymTab_t *symtab, FILE *f, int num_indent)
{
    assert(symtab != NULL);

    int i, scope;
    ListNode_t *cur;

    cur = symtab->stack_head;
    scope = 0;
    while(cur != NULL)
    {
        for(i = 0; i < num_indent; ++i)
            fprintf(f, "  ");

        fprintf(f, "[SCOPE:%d]\n", scope);
        PrintHashTable((HashTable_t *)cur->cur, f, num_indent+1);
        fprintf(f, "\n");

        cur = cur->next;
        ++scope;
    }
}
