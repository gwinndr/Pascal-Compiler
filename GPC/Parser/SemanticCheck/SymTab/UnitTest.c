#include <stdlib.h>
#include <stdio.h>
#include "SymTab.h"
#include "../HashTable/HashTable.h"

int main()
{
    SymTab_t *symtab;
    symtab = InitSymTab();

    PushScope(symtab);
    PushVarOntoScope(symtab, HASHVAR_INTEGER, "scope_0_0_v");
    PushArrayOntoScope(symtab, HASHVAR_REAL, "scope_0_1_a");

    PushScope(symtab);
    PushVarOntoScope(symtab, HASHVAR_REAL, "scope_1_0_v");
    PushArrayOntoScope(symtab, HASHVAR_REAL, "scope_1_1_a");
    PushArrayOntoScope(symtab, HASHVAR_INTEGER, "scope_1_2_a");

    PushScope(symtab);
    PushArrayOntoScope(symtab, HASHVAR_INTEGER, "scope_2_0_a");
    PushVarOntoScope(symtab, HASHVAR_REAL, "scope_2_1_v");

    PrintSymTab(symtab, stderr, 0);

    fprintf(stderr, "[AFTER_POP]\n\n");
    PopScope(symtab);
    PrintSymTab(symtab, stderr, 0);

    PushFunctionOntoScope(symtab, "func", HASHVAR_REAL, PushListNodeBack(
        CreateListNode("arg1", LIST_STRING),
        CreateListNode("arg2", LIST_STRING)
    ));

    PushProcedureOntoScope(symtab, "proc", PushListNodeBack(
        CreateListNode("arg1", LIST_STRING),
        CreateListNode("arg2", LIST_STRING)
    ));

    PrintSymTab(symtab, stderr, 0);

    DestroySymTab(symtab);
    symtab = NULL;

    return 0;
}
