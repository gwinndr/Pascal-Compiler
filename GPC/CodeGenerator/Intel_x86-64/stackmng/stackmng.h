/*
    Damon Gwinn
    Stack Manager structure (see codegen.h for details)
*/
#ifndef STACK_MNG_H
#define STACK_MNG_H

#include <stdlib.h>
#include <stdio.h>
#include "../../../Parser/List/List.h"

#define CONST_STACK_OFFSET_BYTES 0 /* gcc will handle what's there */
#define DOUBLEWORD 4

typedef struct StackScope StackScope_t;

/****** stackmng *******/
typedef struct stackmng
{
    /* Still in progress */
    StackScope_t *cur_scope;

} stackmng_t;

void init_stackmng();
int get_full_stack_offset();
void push_stackscope();
void pop_stackscope();
void add_l_t(char *);
void add_l_x(char *);
void add_l_z(char *);
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

/* WARNING: init_stack_node makes copy of give label */
StackNode_t *init_stack_node(int offset, char *label, int size);
void destroy_stack_node(StackNode_t *);

#endif
