/*
    Damon Gwinn
    Hash table of identifiers
    Used for scoping (semantic checking)
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../../List/List.h"
#include "HashTable.h"

/* Gives a new hash tables with NULL'd out list pointers */
HashTable_t *InitHashTable()
{
    HashTable_t *new_table = (HashTable_t *)malloc(sizeof(HashTable_t));

    int i;
    for(i = 0; i < TABLE_SIZE; ++i)
        new_table->table[i] = NULL;

    return new_table;
}

/* Adds an identifier to the table */
/* Returns 1 if successfully added, 0 if the identifier already exists */
int AddIdentToTable(HashTable_t *table, char *id, enum HashType type)
{
    ListNode_t *list, *cur;
    HashNode_t *hash_node;
    int hash;

    hash = hashpjw(id);
    list = table->table[hash];
    if(list == NULL)
    {
        hash_node = (HashNode_t *)malloc(sizeof(HashNode_t));
        hash_node->type = type;
        hash_node->id = id;


        table->table[hash] = CreateListNode(hash_node, LIST_UNSPECIFIED);
        return 1;
    }
    else
    {
        cur = list;
        while(cur != NULL)
        {
            hash_node = (HashNode_t *)cur->cur;
            if(hash_node->id == id)
                return 0;

            cur = cur->next;
        }

        /* Success if here */
        hash_node = (HashNode_t *)malloc(sizeof(HashNode_t));
        hash_node->type = type;
        hash_node->id = id;

        table->table[hash] = PushListNodeFront(list, CreateListNode(hash_node, LIST_UNSPECIFIED));
        return 1;
    }
}

/* Searches for the given identifier in the table. Returns NULL if not found */
HashNode_t *FindIdentInTable(HashTable_t *table, char *id)
{
    ListNode_t *list;
    HashNode_t *hash_node;
    int hash;

    hash = hashpjw(id);
    list = table->table[hash];
    if(list == NULL)
    {
        return NULL;
    }
    else
    {
        while(list != NULL)
        {
            hash_node = (HashNode_t *)list->cur;
            if(hash_node->id == id)
                return hash_node;

            list = list->next;
        }

        return NULL;
    }
}

/* Frees any and all allocated ListNode_t pointers */
void DestroyHashTable(HashTable_t *table)
{
    ListNode_t *cur, *temp;
    HashNode_t *hash_node;

    int i;
    for(i = 0; i < TABLE_SIZE; ++i)
    {
        cur = table->table[i];
        while(cur != NULL)
        {
            free(cur->cur);
            temp = cur->next;
            free(cur);

            cur = temp;
        }
    }
}

/* Prints all entries in the HashTable */
void PrintHashTable(HashTable_t *table, FILE *f)
{
    int i;
    ListNode_t *list;
    HashNode_t *hash_node;

    for(i = 0; i < TABLE_SIZE; ++i)
    {
        list = table->table[i];
        while(list != NULL)
        {
            hash_node = (HashNode_t *)list->cur;
            fprintf(f, "ID: %s  TYPE: %d  HASH: %d\n", hash_node->id, hash_node->type, i);

            list = list->next;
        }
    }
}

/* The well-known symbol hash function
/* -----------------------------------------------------------------------------
 * hashpjw
 * Peter J. Weinberger's hash function
 * Source: Aho, Sethi, and Ullman, "Compilers", Addison-Wesley, 1986 (page 436).
 */
int hashpjw( char *s )
{
	char *p;
	unsigned h = 0, g;

	for ( p = s; *p != '\0'; p++ )
	{
		h = (h << 4) + (*p);
		if ( g = h & 0xf0000000 )
		{
			h = h ^ ( g >> 24 );
			h = h ^ g;
		}
	}
	return h % TABLE_SIZE;
}