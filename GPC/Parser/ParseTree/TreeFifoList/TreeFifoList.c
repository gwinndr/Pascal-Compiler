/*
    Damon Gwinn
    Linked list for tree_t, Statement, and Expression structures
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "TreeFifoList.h"

/* Creates a list node */
TreeListNode_t *CreateListNode(void *new_obj, enum ListType type)
{
    TreeListNode_t *new_node;

    new_node = (TreeListNode_t *)malloc(sizeof(TreeListNode_t));

    new_node->type = type;
    new_node->cur = new_obj;
    new_node->next = NULL;

    return new_node;
}

/* Implemented FIFO style */
/* NOTE: No error checking performed on InsertTreeNode for types */
TreeListNode_t *InsertTreeNode(TreeListNode_t *head_node, TreeListNode_t *new_head)
{
    assert(head_node != NULL);
    assert(new_head != NULL);

    new_head->next = head_node;
    return new_head;
}

TreeListNode_t *DeleteTreeListNode(TreeListNode_t *node, TreeListNode_t *prev)
{
    TreeListNode_t *next;
    assert(node != NULL);

    next = node->next;
    if(prev != NULL)
        prev->next = next;

    free(node);

    return next;
}

void DestroyList(TreeListNode_t *head_node)
{
    TreeListNode_t *cur, *next;
    cur = head_node;
    while(cur != NULL)
    {
        cur = DeleteTreeListNode(cur, NULL);
    }
}

void PrintList(TreeListNode_t *head_node, FILE *f, int num_indent)
{
    int i;
    TreeListNode_t *cur;

    for(i=0; i<num_indent; ++i)
        fprintf(f, "  ");

    cur = head_node;
    while(cur != NULL)
    {
        fprintf(f, "%d, ", cur->type);
        cur = cur->next;
    }
    fprintf(f, "\n");
}
