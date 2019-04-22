/*
    Damon Gwinn
    Stack Manager structure (see codegen.h for details)
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "stackmng.h"
#include "../codegen.h"
#include "../../../Parser/List/List.h"

/******** stackmng *********/
stackmng_t *global_stackmng = NULL;

void init_stackmng()
{
    assert(global_stackmng == NULL);

    global_stackmng = (stackmng_t *)malloc(sizeof(stackmng_t));

    global_stackmng->cur_scope = NULL;
    global_stackmng->reg_stack = init_reg_stack();
}

int get_full_stack_offset()
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);
    StackScope_t *scope;

    scope = global_stackmng->cur_scope;

    return CONST_STACK_OFFSET_BYTES +
        scope->t_offset + scope->x_offset + scope->z_offset;
}

int get_needed_stack_space()
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);
    StackScope_t *scope;

    scope = global_stackmng->cur_scope;

    return scope->t_offset + scope->x_offset + scope->z_offset;
}

void push_stackscope()
{
    assert(global_stackmng != NULL);

    StackScope_t *new_scope;
    new_scope = init_stackscope();

    new_scope->prev_scope = global_stackmng->cur_scope;
    global_stackmng->cur_scope = new_scope;
}

void pop_stackscope()
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    global_stackmng->cur_scope = free_stackscope(global_stackmng->cur_scope);
}

/* Adds doubleword to t */
StackNode_t *add_l_t(char *label)
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    StackScope_t *cur_scope;
    StackNode_t *new_node;
    int offset;

    cur_scope = global_stackmng->cur_scope;

    cur_scope->t_offset += DOUBLEWORD;

    offset = CONST_STACK_OFFSET_BYTES +
        cur_scope->z_offset + cur_scope->x_offset + cur_scope->t_offset;

    new_node = init_stack_node(offset, label, DOUBLEWORD);

    if(cur_scope->t == NULL)
    {
        cur_scope->t = CreateListNode(new_node, LIST_UNSPECIFIED);
    }
    else
    {
        cur_scope->t = PushListNodeBack(cur_scope->t,
            CreateListNode(new_node, LIST_UNSPECIFIED));
    }

    #ifdef DEBUG_CODEGEN
        fprintf(stderr, "DEBUG: Added %s to t_offset %d\n", label, offset);
    #endif

    return new_node;
}

/* Adds doubleword to x */
StackNode_t *add_l_x(char *label)
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    StackScope_t *cur_scope;
    StackNode_t *new_node;
    int offset;

    cur_scope = global_stackmng->cur_scope;

    cur_scope->x_offset += DOUBLEWORD;

    offset = CONST_STACK_OFFSET_BYTES +
        cur_scope->z_offset + cur_scope->x_offset;

    new_node = init_stack_node(offset, label, DOUBLEWORD);

    if(cur_scope->x == NULL)
    {
        cur_scope->x = CreateListNode(new_node, LIST_UNSPECIFIED);
    }
    else
    {
        cur_scope->x = PushListNodeBack(cur_scope->x,
            CreateListNode(new_node, LIST_UNSPECIFIED));
    }

    #ifdef DEBUG_CODEGEN
        fprintf(stderr, "DEBUG: Added %s to x_offset %d\n", label, offset);
    #endif

    return new_node;
}

/* Adds doubleword to z */
StackNode_t *add_l_z(char *label)
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    StackScope_t *cur_scope;
    StackNode_t *new_node;
    int offset;

    cur_scope = global_stackmng->cur_scope;

    cur_scope->z_offset += DOUBLEWORD;

    offset = CONST_STACK_OFFSET_BYTES +
        cur_scope->z_offset;

    new_node = init_stack_node(offset, label, DOUBLEWORD);

    if(cur_scope->z == NULL)
    {
        cur_scope->z = CreateListNode(new_node, LIST_UNSPECIFIED);
    }
    else
    {
        cur_scope->z = PushListNodeBack(cur_scope->z,
            CreateListNode(new_node, LIST_UNSPECIFIED));
    }

    #ifdef DEBUG_CODEGEN
        fprintf(stderr, "DEBUG: Added %s to z_offset %d\n", label, offset);
    #endif

    return new_node;
}

/* TODO: Does not find variables outside the current scope */
StackNode_t *find_label(char *label)
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    StackScope_t *cur_scope;
    ListNode_t *cur_li;
    StackNode_t *cur_node;

    cur_scope = global_stackmng->cur_scope;

    cur_li = cur_scope->z;
    while(cur_li != NULL)
    {
        cur_node = (StackNode_t *)cur_li->cur;
        if(strcmp(cur_node->label, label))
        {
            return cur_node;
        }

        cur_li = cur_li->next;
    }
    cur_li = cur_scope->x;
    while(cur_li != NULL)
    {
        cur_node = (StackNode_t *)cur_li->cur;
        if(strcmp(cur_node->label, label))
        {
            return cur_node;
        }

        cur_li = cur_li->next;
    }
    cur_li = cur_scope->t;
    while(cur_li != NULL)
    {
        cur_node = (StackNode_t *)cur_li->cur;
        if(strcmp(cur_node->label, label))
        {
            return cur_node;
        }

        cur_li = cur_li->next;
    }

    /* Should never get here */
    fprintf(stderr, "Critical error: Could not find variable on stackmng!\n");
    assert(0);
}

void free_stackmng()
{
    assert(global_stackmng != NULL);

    free_all_stackscopes(global_stackmng->cur_scope);
    free_reg_stack(global_stackmng->reg_stack);
    free(global_stackmng);
    global_stackmng = NULL;
}


/********* RegStack_t **********/

RegStack_t *init_reg_stack()
{
    /* See codegen.h for information on available general purpose registers */
    ListNode_t *registers;

    RegStack_t *reg_stack;
    reg_stack = (RegStack_t *)malloc(sizeof(RegStack_t));

    /* RBX */
    Register_t *rbx;
    rbx = (Register_t *)malloc(sizeof(Register_t));
    rbx->bit_64 = strdup("%%rbx");
    rbx->bit_32 = strdup("%%ebx");

    /* RDI */
    Register_t *rdi;
    rdi = (Register_t *)malloc(sizeof(Register_t));
    rdi->bit_64 = strdup("%%rdi");
    rdi->bit_32 = strdup("%%edi");

    /* RSI */
    Register_t *rsi;
    rsi = (Register_t *)malloc(sizeof(Register_t));
    rsi->bit_64 = strdup("%%rsi");
    rsi->bit_32 = strdup("%%esi");


    registers = CreateListNode(rbx, LIST_UNSPECIFIED);
    registers = PushListNodeBack(registers, CreateListNode(rdi, LIST_UNSPECIFIED));
    registers = PushListNodeBack(registers, CreateListNode(rsi, LIST_UNSPECIFIED));

    reg_stack->num_registers_alloced = 0;
    reg_stack->registers_free = registers;

    return reg_stack;
}

void push_reg_stack(RegStack_t *reg_stack, Register_t *reg)
{
    assert(reg_stack != NULL);
    assert(reg != NULL);

    reg_stack->num_registers_alloced--;

    if(reg_stack->registers_free == NULL)
    {
        reg_stack->registers_free = CreateListNode(reg, LIST_UNSPECIFIED);
    }

    else
    {
        reg_stack->registers_free = PushListNodeBack(reg_stack->registers_free,
            CreateListNode(reg, LIST_UNSPECIFIED));
    }
}

Register_t *pop_reg_stack(RegStack_t *reg_stack)
{
    assert(reg_stack != NULL);

    ListNode_t *register_node;
    Register_t *reg;

    if(reg_stack->registers_free != NULL)
    {
        reg_stack->num_registers_alloced++;

        register_node = reg_stack->registers_free;
        reg_stack->registers_free = reg_stack->registers_free->next;

        reg = (Register_t *)register_node->cur;
        free(register_node);

        return reg;
    }
    else
        return NULL;
}

void free_reg_stack(RegStack_t *reg_stack)
{
    assert(reg_stack != NULL);

    if(reg_stack->num_registers_alloced != 0)
    {
        fprintf(stderr, "WARNING: Not all registers freed, %d still remaining!\n",
            reg_stack->num_registers_alloced);
    }

    ListNode_t *cur;
    Register_t *reg;
    while(reg_stack->registers_free != NULL)
    {
        cur = reg_stack->registers_free;
        reg_stack->registers_free = cur->next;

        reg = (Register_t *)cur->cur;

        free(reg->bit_64);
        free(reg->bit_32);
        free(reg);
        free(cur);
    }

    free(reg_stack);
}


/*********** StackScope *************/

StackScope_t *init_stackscope()
{
    StackScope_t *new_scope;
    new_scope = (StackScope_t *)malloc(sizeof(StackScope_t));

    new_scope->t_offset = 0;
    new_scope->x_offset = 0;
    new_scope->z_offset = 0;

    new_scope->t = NULL;
    new_scope->x = NULL;
    new_scope->z = NULL;

    new_scope->prev_scope = NULL;

    return new_scope;
}

/* Returns pointer to previous stack scope */
StackScope_t *free_stackscope(StackScope_t *stackscope)
{
    StackScope_t *prev_scope;

    prev_scope = NULL;
    if(stackscope != NULL)
    {
        prev_scope = stackscope->prev_scope;

        free_stackscope_list(stackscope->t);
        free_stackscope_list(stackscope->x);
        free_stackscope_list(stackscope->z);
        free(stackscope);
    }
    return prev_scope;
}

void free_all_stackscopes(StackScope_t *stackscope)
{
    StackScope_t *prev_scope;

    while(stackscope != NULL)
    {
        prev_scope = stackscope->prev_scope;

        free_stackscope_list(stackscope->t);
        free_stackscope_list(stackscope->x);
        free_stackscope_list(stackscope->z);
        free(stackscope);

        stackscope = prev_scope;
    }
}

void free_stackscope_list(ListNode_t *li)
{
    ListNode_t *cur;
    cur = li;

    while(cur != NULL)
    {
        destroy_stack_node((StackNode_t *)cur->cur);
        cur = cur->next;
    }
    DestroyList(li);
}

/*********** StackNode *************/

/* WARNING: Copy is made of label */
StackNode_t *init_stack_node(int offset, char *label, int size)
{
    assert(label != NULL);

    StackNode_t *new_node;
    new_node = (StackNode_t *)malloc(sizeof(StackNode_t));

    new_node->offset = offset;
    new_node->label = strdup(label);
    new_node->offset = size;

    return new_node;
}

void destroy_stack_node(StackNode_t *node)
{
    assert(node != NULL);
    assert(node->label != NULL);

    free(node->label);
    free(node);
}
