#include <stdlib.h>
#include <stdio.h>
#include "TreeLinkedList.h"

int main()
{
    TreeListNode_t *head;
    head = CreateListNode(NULL, tree);
    InsertTreeNode(head, CreateListNode(NULL, stmt));
    InsertTreeNode(head, CreateListNode(NULL, expr));

    PrintList(head, stderr, 0);
    fprintf(stderr, "%d, %d, %d\n", head->type, head->prev->type, head->prev->prev->type);

    DeleteTreeListNode(head->next);
    PrintList(head, stderr, 0);

    InsertTreeNode(head->next, CreateListNode(NULL, tree));
    PrintList(head, stderr, 0);
    fprintf(stderr, "%d, %d, %d\n", head->type, head->prev->type, head->prev->prev->type);

    DestroyList(head);
    return 0;
}
