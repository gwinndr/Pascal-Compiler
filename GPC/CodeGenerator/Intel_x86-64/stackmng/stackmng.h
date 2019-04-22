/*
    Damon Gwinn
    Stack Manager structure (see codegen.h for details)

    TODO: Turns linked lists into hash table of linked lists
*/
#ifndef STACK_MNG_H
#define STACK_MNG_H

#include <stdlib.h>
#include <stdio.h>
#include "../../../Parser/List/List.h"

#define CONST_STACK_OFFSET_BYTES 0 /* gcc will handle what's there */
#define DOUBLEWORD 4

typedef struct StackScope StackScope_t;
typedef struct StackNode StackNode_t;

/****** stackmng *******/
typedef struct stackmng
{
    /* Still in progress */
    StackScope_t *cur_scope;

} stackmng_t;

void init_stackmng();
int get_full_stack_offset();
int get_needed_stack_space();
void push_stackscope();
void pop_stackscope();
StackNode_t *add_l_t(char *);
StackNode_t *add_l_x(char *);
StackNode_t *add_l_z(char *);
StackNode_t *find_label(char *);
void free_stackmng();

/********* StackScope_t **********/
typedef struct StackScope
{
    int t_offset, x_offset, z_offset;
    ListNode_t *t, *x, *z;

    StackScope_t *prev_scope;
} StackScope_t;

StackScope_t *init_stackscope();
StackScope_t *free_stackscope(StackScope_t *);
void free_all_stackscopes(StackScope_t *);
void free_stackscope_list(ListNode_t *);

/********* StackNode_t **********/
/* Used to encapsulate offset, label, and size */
typedef struct StackNode
{
    int offset;
    char *label;
    int size;
} StackNode_t;

/* WARNING: init_stack_node makes copy of given label */
StackNode_t *init_stack_node(int offset, char *label, int size);
void destroy_stack_node(StackNode_t *);

#endif
