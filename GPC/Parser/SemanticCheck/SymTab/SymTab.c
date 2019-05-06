/*
    Damon Gwinn
    Creates a symbol table which is simply a stack of hash tables for identifiers
    Used to perform semantic checking on a ParseTree

    WARNING: Symbol table will NOT free given identifier strings or args when destroyed
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
    new_symtab->builtins = InitHashTable();

    return new_symtab;
}

/* Adds a built-in procedure call */
/* NOTE: Built-ins reflected on all scope levels */
/* Returns 1 if failed, 0 otherwise */
int AddBuiltinProc(SymTab_t *symtab, char *id, ListNode_t *args)
{
    assert(symtab != NULL);
    assert(id != NULL);

    return AddIdentToTable(symtab->builtins, id,
            HASHVAR_PROCEDURE, HASHTYPE_BUILTIN_PROCEDURE, args);
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
int PushVarOntoScope(SymTab_t *symtab, enum VarType var_type, char *id)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    /* Make sure it's not a builtin */
    if(FindIdentInTable(symtab->builtins, id) == NULL)
    {
        cur_hash = (HashTable_t *)symtab->stack_head->cur;
        return AddIdentToTable(cur_hash, id, var_type, HASHTYPE_VAR, NULL);
    }
    else
    {
        return 1;
    }
}

/* Pushes a new array onto the current scope (head) */
int PushArrayOntoScope(SymTab_t *symtab, enum VarType var_type, char *id)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    /* Make sure it's not a builtin */
    if(FindIdentInTable(symtab->builtins, id) == NULL)
    {
        cur_hash = (HashTable_t *)symtab->stack_head->cur;
        return AddIdentToTable(cur_hash, id, var_type, HASHTYPE_ARRAY, NULL);
    }
    else
    {
        return 1;
    }
}

/* Pushes a new procedure onto the current scope (head) */
/* NOTE: args can be NULL to represent no args */
int PushProcedureOntoScope(SymTab_t *symtab, char *id, ListNode_t *args)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    /* Make sure it's not a builtin */
    if(FindIdentInTable(symtab->builtins, id) == NULL)
    {
        cur_hash = (HashTable_t *)symtab->stack_head->cur;
        return AddIdentToTable(cur_hash, id, HASHVAR_PROCEDURE, HASHTYPE_PROCEDURE, args);
    }
    else
    {
        return 1;
    }
}

/* Pushes a new function onto the current scope (head) */
/* NOTE: args can be NULL to represent no args */
int PushFunctionOntoScope(SymTab_t *symtab, char *id, enum VarType var_type, ListNode_t *args)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    /* Make sure it's not a builtin */
    if(FindIdentInTable(symtab->builtins, id) == NULL)
    {
        cur_hash = (HashTable_t *)symtab->stack_head->cur;
        return AddIdentToTable(cur_hash, id, var_type, HASHTYPE_FUNCTION, args);
    }
    else
    {
        return 1;
    }
}

/* Pushes a new function return type var onto the current scope (head) */
/* NOTE: args can be NULL to represent no args */
int PushFuncRetOntoScope(SymTab_t *symtab, char *id, enum VarType var_type, ListNode_t *args)
{
    assert(symtab != NULL);
    assert(symtab->stack_head != NULL);

    HashTable_t *cur_hash;

    /* Make sure it's not a builtin */
    if(FindIdentInTable(symtab->builtins, id) == NULL)
    {
        cur_hash = (HashTable_t *)symtab->stack_head->cur;
        return AddIdentToTable(cur_hash, id, var_type, HASHTYPE_FUNCTION_RETURN, args);
    }
    else
    {
        return 1;
    }
}

/* Searches for an identifier and sets the hash_return that contains the id and type information */
/* Returns -1 and sets hash_return to NULL if not found */
/* Returns >= 0 tells what scope level it was found at */
/* Mutating tells whether it's being referenced in an assignment context */
int FindIdent(HashNode_t **hash_return, SymTab_t *symtab, char *id)
{
    int return_val = 0;
    assert(symtab != NULL);
    assert(id != NULL);

    ListNode_t *cur;
    HashNode_t *hash_node;

    /* First check built-ins */
    hash_node = FindIdentInTable(symtab->builtins, id);
    if(hash_node != NULL)
    {
        *hash_return = hash_node;
        return return_val;
    }

    /* Then check user definitions */
    cur = symtab->stack_head;
    while(cur != NULL)
    {
        hash_node = FindIdentInTable((HashTable_t *)cur->cur, id);
        if(hash_node != NULL)
        {
            *hash_return = hash_node;
            return return_val;
        }

        ++return_val;
        cur = cur->next;
    }

    *hash_return = NULL;
    return_val = -1;

    return return_val;
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

    DestroyHashTable(symtab->builtins);
    free(symtab);
}

/* Prints the table for debugging */
void PrintSymTab(SymTab_t *symtab, FILE *f, int num_indent)
{
    assert(symtab != NULL);

    int i, scope;
    ListNode_t *cur;

    for(i = 0; i < num_indent; ++i)
        fprintf(f, "  ");
    fprintf(f, "[BUILT-INS]:\n");
    PrintHashTable(symtab->builtins, f, num_indent+1);

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
