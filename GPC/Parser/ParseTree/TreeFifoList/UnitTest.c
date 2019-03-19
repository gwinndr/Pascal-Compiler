#include <stdlib.h>
#include <stdio.h>
#include "TreeFifoList.h"

int main()
{
    TreeListNode_t *head;

    fprintf(stderr, "HEAD_NODE: %d\n", HEAD_TREE_NODE);


    head = CreateListNode(NULL, LIST_TREE);
    head = InsertTreeNode(head, CreateListNode(NULL, LIST_STMT));
    head = InsertTreeNode(head, CreateListNode(NULL, LIST_EXPR));

    PrintList(head, stderr, 0);
    fprintf(stderr, "%d, %d, %d\n", head->type, head->next->type, head->next->next->type);

    DeleteTreeListNode(head->next, head);
    PrintList(head, stderr, 0);

    head = InsertTreeNode(head, CreateListNode(NULL, LIST_TREE));
    PrintList(head, stderr, 0);
    fprintf(stderr, "%d, %d, %d\n", head->type, head->next->type, head->next->next->type);

    DestroyList(head);
    return 0;
}
