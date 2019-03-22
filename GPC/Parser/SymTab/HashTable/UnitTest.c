/*
    Damon Gwinn
    For unit testing the HashTable
*/

#include <stdlib.h>
#include <stdio.h>

#include "HashTable.h"

int main()
{
    HashTable_t *table;
    table = InitHashTable();

    fprintf(stderr, "INITIAL TABLE:\n");
    PrintHashTable(table, stderr);

    AddIdentToTable(table, "meow", VAR);
    AddIdentToTable(table, "lol", ARRAY);

    fprintf(stderr, "TABLE:\n");
    PrintHashTable(table, stderr);

    fprintf(stderr, "%d\n", (FindIdentInTable(table, "meow") != NULL));
    fprintf(stderr, "%d\n", (FindIdentInTable(table, "meow1") != NULL));

    fprintf(stderr, "%d\n", AddIdentToTable(table, "meow", ARRAY));
    AddIdentToTable(table, "meow1", ARRAY);
    fprintf(stderr, "TABLE:\n");
    PrintHashTable(table, stderr);

    DestroyHashTable(table);
    return 0;
}
