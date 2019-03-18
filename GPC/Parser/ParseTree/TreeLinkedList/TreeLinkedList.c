/*
    Damon Gwinn
    Linked list for tree_t, Statement, and Expression structures
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "TreeLinkedList.h"

/* Creates a list node */
TreeListNode_t *CreateListNode(void *new_obj, enum ListType type)
{
    TreeListNode_t *new_node;
    assert(type == tree || type == stmt || type == expr);

    new_node = (TreeListNode_t *)malloc(sizeof(TreeListNode_t));
    new_node->type = type;
    new_node->cur = new_obj;
    new_node->next = NULL;
    new_node->prev = NULL;
}

/* Implemented FIFO style */
/* NOTE: No error checking performed on InsertTreeNode for types */
void InsertTreeNode(TreeListNode_t *head_node, TreeListNode_t *new_node)
{
    TreeListNode_t *next_node;
    next_node = head_node->next;

    assert(new_node != NULL);
    assert(next_node != NULL);

    new_node->prev = head_node;
    new_node->next = next_node;

    head_node->next = new_node;
    next_node->prev = new_node;
}

TreeListNode_t *DeleteTreeListNode(TreeListNode_t *node)
{
    TreeListNode_t *next, *prev;

    next = node->next;
    prev = node->prev;

    next->prev = prev;
    prev->next = next;

    free(node);
    node = NULL;

    return next;
}

void DestroyList(TreeListNode_t *head_node)
{
    TreeListNode_t *cur, *next;
    cur = head_node->next;
    while(cur != head_node)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(head_node);
}

void PrintList(TreeListNode_t *head_node, FILE *f, int num_indent)
{
    int i;
    TreeListNode_t *cur;

    for(i=0; i<num_indent; ++i)
        fprintf(f, "  ");

    cur = head_node->next;
    while(cur != head_node)
    {
        fprintf(f, "%d, ", cur->type);
        cur = cur->next;
    }
    fprintf(f, "%d\n", head_node->type);
}
