/*
    Damon Gwinn
    For unit testing the HashTable
*/

#include <stdlib.h>
#include <stdio.h>

#include "../../List/List.h"
#include "HashTable.h"

int main()
{
    HashTable_t *table;
    table = InitHashTable();

    fprintf(stderr, "INITIAL TABLE:\n");
    PrintHashTable(table, stderr, 0);

    AddIdentToTable(table, "meow", HASHVAR_INTEGER, HASHTYPE_VAR, NULL);
    AddIdentToTable(table, "lol", HASHVAR_REAL, HASHTYPE_ARRAY, NULL);

    fprintf(stderr, "TABLE:\n");
    PrintHashTable(table, stderr, 0);

    fprintf(stderr, "%d\n", (FindIdentInTable(table, "meow") != NULL));
    fprintf(stderr, "%d\n", (FindIdentInTable(table, "meow1") != NULL));

    fprintf(stderr, "%d\n", AddIdentToTable(table, "meow", HASHVAR_INTEGER, HASHTYPE_ARRAY, NULL));
    AddIdentToTable(table, "meow1", HASHVAR_REAL, HASHTYPE_ARRAY, NULL);
    fprintf(stderr, "TABLE:\n");
    PrintHashTable(table, stderr, 0);

    AddIdentToTable(table, "test_proc", HASHVAR_PROCEDURE, HASHTYPE_PROCEDURE,
        PushListNodeBack(CreateListNode("arg1", LIST_STRING),
        CreateListNode("arg1", LIST_STRING)));

    PrintHashTable(table, stderr, 0);

    DestroyHashTable(table);
    table = NULL;
    return 0;
}
