/*
    Damon Gwinn
    Linked list for tree_t, Statement, and Expression structures

    WARNING: TreeList will NOT free the contents of the void pointer cur.
        This must be freed by the user.
*/
#ifndef TREE_LINKED_LIST_H
#define TREE_LINKED_LIST_H

#include <stdio.h>

enum ListType{HEAD_TREE_NODE, LIST_TREE, LIST_STMT, LIST_EXPR, LIST_C, LIST_VAR};

/* Our linked list of tree type nodes */
typedef struct TreeList TreeListNode_t;
typedef struct TreeList
{
    enum ListType type;
    void *cur;

    TreeListNode_t *next;
} TreeListNode_t;

/* Creates a list node */
TreeListNode_t *CreateListNode(void *new_obj, enum ListType type);

/* NOTE: No error checking performed on PushTreeList for types */
/* This is FIFO style (to mimic a parse tree structure) */
/* Returns the new head node */
TreeListNode_t *InsertTreeNode(TreeListNode_t *head_node, TreeListNode_t *new_head);

/* Returns pointer to the next node */
TreeListNode_t *DeleteTreeListNode(TreeListNode_t *node, TreeListNode_t *prev);

void DestroyList(TreeListNode_t *head_node);

void PrintList(TreeListNode_t *head_node, FILE *f, int num_indent);

#endif
