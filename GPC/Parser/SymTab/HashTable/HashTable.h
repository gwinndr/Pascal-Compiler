/*
    Damon Gwinn
    Hash table of identifiers
    Used for scoping (semantic checking)

    WARNING: Hash table will NOT free given identifier strings when destroyed
        Remember to free given identifier strings manually
*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#define TABLE_SIZE	211

#include <stdio.h>
#include "../../List/List.h"

enum HashType{VAR, ARRAY};

/* Items we put in the hash table (either normal variale, or array variable for now) */
typedef struct HashNode
{
    enum HashType type;
    char *id;
} HashNode_t;

/* Our actual hash table */
typedef struct HashTable
{
    ListNode_t *table[TABLE_SIZE];

} HashTable_t;

/* Gives a new hash tables with NULL'd out list pointers */
HashTable_t *InitHashTable();

/* Adds an identifier to the table */
/* Returns 1 if successfully added, 0 if the identifier already exists */
int AddIdentToTable(HashTable_t *table, char *id, enum HashType type);

/* Searches for the given identifier in the table. Returns NULL if not found */
HashNode_t *FindIdentInTable(HashTable_t *table, char *id);

/* Frees any and all allocated ListNode_t pointers */
void DestroyHashTable(HashTable_t *table);

/* Prints all entries in the HashTable */
void PrintHashTable(HashTable_t *table, FILE *f);

/* The well-known symbol hash function
/* -----------------------------------------------------------------------------
 * hashpjw
 * Peter J. Weinberger's hash function
 * Source: Aho, Sethi, and Ullman, "Compilers", Addison-Wesley, 1986 (page 436).
 */
int hashpjw( char *s );

#endif
