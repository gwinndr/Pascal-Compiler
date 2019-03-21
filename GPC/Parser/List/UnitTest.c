#include <stdlib.h>
#include <stdio.h>
#include "List.h"

int main()
{
    ListNode_t *head;
    head = CreateListNode(NULL, LIST_TREE);
    head = PushListNodeFront(head, CreateListNode(NULL, LIST_STMT));
    head = PushListNodeFront(head, CreateListNode(NULL, LIST_EXPR));

    PrintList(head, stderr, 0);
    fprintf(stderr, "%d, %d, %d\n", head->type, head->next->type, head->next->next->type);

    DeleteListNode(head->next, head);
    PrintList(head, stderr, 0);

    head = PushListNodeFront(head, CreateListNode(NULL, LIST_TREE));
    PrintList(head, stderr, 0);
    fprintf(stderr, "%d, %d, %d\n", head->type, head->next->type, head->next->next->type);

    DestroyList(head);
    return 0;
}
