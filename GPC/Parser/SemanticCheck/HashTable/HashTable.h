/*
    Damon Gwinn
    Hash table of identifiers
    Used for scoping (semantic checking)

    WARNING: Hash table will NOT free given identifier strings or args when destroyed
        Remember to free given identifier strings and args manually
*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#define TABLE_SIZE	211

#include <stdio.h>
#include "../../List/List.h"

enum HashType{HASHTYPE_VAR, HASHTYPE_ARRAY, HASHTYPE_PROCEDURE, HASHTYPE_FUNCTION,
    HASHTYPE_FUNCTION_RETURN, HASHTYPE_BUILTIN_PROCEDURE};
enum VarType{HASHVAR_INTEGER, HASHVAR_REAL, HASHVAR_PROCEDURE, HASHVAR_UNTYPED};

/* Items we put in the hash table */
typedef struct HashNode
{
    char *id;
    enum HashType hash_type;
    enum VarType var_type;
    ListNode_t *args; /* NULL when no args (or not applicable to given type) */

    /* Symbol table resources */
    int referenced;
    int mutated;

} HashNode_t;

/* Our actual hash table */
typedef struct HashTable
{
    ListNode_t *table[TABLE_SIZE];

} HashTable_t;

/* Gives a new hash tables with NULL'd out list pointers */
HashTable_t *InitHashTable();

/* Adds an identifier to the table */
/* Returns 0 if successfully added, 1 if the identifier already exists */
int AddIdentToTable(HashTable_t *table, char *id, enum VarType var_type,
    enum HashType hash_type, ListNode_t *args);

/* Searches for the given identifier in the table. Returns NULL if not found */
/* Mutating tells whether it's being referenced in an assignment context */
HashNode_t *FindIdentInTable(HashTable_t *table, char *id);

/* Resets hash node mutation and reference status */
void ResetHashNodeStatus(HashNode_t *hash_node);

/* Frees any and all allocated ListNode_t pointers */
void DestroyHashTable(HashTable_t *table);

/* Destroys special builtin procedure addons */
void DestroyBuiltin(HashNode_t *);

/* Prints all entries in the HashTable */
void PrintHashTable(HashTable_t *table, FILE *f, int num_indent);

/* The well-known symbol hash function
/* -----------------------------------------------------------------------------
 * hashpjw
 * Peter J. Weinberger's hash function
 * Source: Aho, Sethi, and Ullman, "Compilers", Addison-Wesley, 1986 (page 436).
 */
int hashpjw( char *s );

#endif
