#include <stdlib.h>
#include <stdio.h>
#include "SymTab.h"
#include "../HashTable/HashTable.h"

int main()
{
    SymTab_t *symtab;
    symtab = InitSymTab();

    PushScope(symtab);
    PushIdentOntoScope(symtab, "scope_0_0_v", VAR);
    PushIdentOntoScope(symtab, "scope_0_1_a", ARRAY);

    PushScope(symtab);
    PushIdentOntoScope(symtab, "scope_1_0_v", VAR);
    PushIdentOntoScope(symtab, "scope_1_1_a", ARRAY);
    PushIdentOntoScope(symtab, "scope_1_2_a", ARRAY);

    PushScope(symtab);
    PushIdentOntoScope(symtab, "scope_2_0_a", ARRAY);
    PushIdentOntoScope(symtab, "scope_2_1_v", VAR);

    PrintSymTab(symtab, stderr, 0);

    fprintf(stderr, "\n[AFTER_POP]\n\n");
    PopScope(symtab);
    PushIdentOntoScope(symtab, "scope_1_3_v", VAR);
    PrintSymTab(symtab, stderr, 0);

    DestroySymTab(symtab);
    symtab = NULL;

    return 0;
}
