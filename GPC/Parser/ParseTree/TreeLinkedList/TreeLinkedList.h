/*
    Damon Gwinn
    Linked list for tree_t, Statement, and Expression structures

    WARNING: TreeList will NOT free the contents of the void pointer cur.
        This must be freed by the user.
*/
#ifndef TREE_LINKED_LIST_H
#define TREE_LINKED_LIST_H

#include <stdio.h>

enum ListType{tree, stmt, expr};

/* Our linked list of tree type nodes */
typedef struct TreeList TreeListNode_t;
typedef struct TreeList
{
    enum ListType type;
    void *cur;

    TreeListNode_t *next;
    TreeListNode_t *prev;
} TreeListNode_t;

/* Creates a list node */
TreeListNode_t *CreateListNode(void *new_obj, enum ListType type);

/* NOTE: No error checking performed on PushTreeList for types */
void PushTreeList(TreeListNode_t *head_node, TreeListNode_t *new_node);

/* Returns pointer to the next node */
TreeListNode_t *DeleteTreeListNode(TreeListNode_t *node);

void DestroyList(TreeListNode_t *head_node);

void PrintList(TreeListNode_t *head_node, FILE *f, int num_indent);

#endif
