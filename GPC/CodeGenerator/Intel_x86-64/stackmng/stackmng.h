/*
    Damon Gwinn
    Stack Manager structure (see codegen.h for details)

    TODO:
        - Turns linked lists into hash table of linked lists
        - Support 16 and 8 bit registers
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
typedef struct RegStack RegStack_t;
typedef struct Register Register_t;

/* Helper for getting special registers */
int num_args_alloced;
void free_arg_regs();
char *get_arg_reg64_num(int num);
char *get_arg_reg32_num(int num);

/****** stackmng *******/
typedef struct stackmng
{
    /* Still in progress */
    StackScope_t *cur_scope;
    RegStack_t *reg_stack;
} stackmng_t;

void init_stackmng();
StackScope_t *get_cur_scope();
int get_full_stack_offset();
int get_needed_stack_space();
void push_stackscope();
void pop_stackscope();
StackNode_t *add_l_t(char *);
StackNode_t *add_l_x(char *);
StackNode_t *add_l_z(char *);
StackNode_t *find_in_temp(char *);
StackNode_t *find_label(char *);
RegStack_t *get_reg_stack();
void free_stackmng();

/********* RegStack_t **********/

typedef struct RegStack
{
    ListNode_t *registers_free;
    int num_registers_alloced;
    int num_registers;
} RegStack_t;

RegStack_t *init_reg_stack();

/* NOTE: Getters return number greater than 1 if it had to kick a value out to temp */
/* The returned int is the temp offset to restore the value */
int get_register_64bit(RegStack_t *, char *reg_64, Register_t **);
int get_register_32bit(RegStack_t *, char *reg_32, Register_t **);
void restore_register_64bit(RegStack_t *, Register_t *, int temp_offset);
void restore_register_32bit(RegStack_t *, Register_t *, int temp_offset);
void push_reg_stack(RegStack_t *, Register_t *);
void swap_reg_stack(RegStack_t *);
Register_t *front_reg_stack(RegStack_t *);
Register_t *pop_reg_stack(RegStack_t *);
int get_num_registers(RegStack_t *);

void free_reg_stack(RegStack_t *);

/********* Register_t **********/
typedef struct Register
{
    char *bit_64;
    char *bit_32;
} Register_t;


/********* StackScope_t **********/

typedef struct StackScope
{
    int t_offset, x_offset, z_offset;
    ListNode_t *t, *x, *z;

    StackScope_t *prev_scope;
} StackScope_t;

StackScope_t *init_stackscope();
StackNode_t *stackscope_find_t(StackScope_t *, char *);
StackNode_t *stackscope_find_x(StackScope_t *, char *);
StackNode_t *stackscope_find_z(StackScope_t *, char *);
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
