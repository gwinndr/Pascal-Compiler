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

    AddIdentToTable(table, "meow", VAR, NULL);
    AddIdentToTable(table, "lol", ARRAY, NULL);

    fprintf(stderr, "TABLE:\n");
    PrintHashTable(table, stderr, 0);

    fprintf(stderr, "%d\n", (FindIdentInTable(table, "meow") != NULL));
    fprintf(stderr, "%d\n", (FindIdentInTable(table, "meow1") != NULL));

    fprintf(stderr, "%d\n", AddIdentToTable(table, "meow", ARRAY, NULL));
    AddIdentToTable(table, "meow1", ARRAY, NULL);
    fprintf(stderr, "TABLE:\n");
    PrintHashTable(table, stderr, 0);

    AddIdentToTable(table, "test_proc", PROCEDURE,
        PushListNodeBack(CreateListNode("arg1", LIST_STRING),
        CreateListNode("arg1", LIST_STRING)));

    PrintHashTable(table, stderr, 0);

    DestroyHashTable(table);
    table = NULL;
    return 0;
}
