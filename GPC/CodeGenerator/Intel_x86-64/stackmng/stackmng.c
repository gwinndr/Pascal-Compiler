/*
    Damon Gwinn
    Stack Manager structure (see codegen.h for details)
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "stackmng.h"
#include "../register_types.h"
#include "../codegen.h"
#include "../../../Parser/List/List.h"

/* Sets num_args_alloced to 0 */
void free_arg_regs()
{
    num_args_alloced = 0;
}

/* Helpers for getting special registers */
/* TODO: Support loading arguments into temp if needed */
char *get_arg_reg64_num(int num)
{
    if(num_args_alloced > num)
        return NULL;

    if(num == 0)
    {
        return ARG_REG_1_64;
    }
    else if(num == 1)
    {
        return ARG_REG_2_64;
    }
    else if(num == 2)
    {
        return ARG_REG_3_64;
    }
    else if(num == 3)
    {
        return ARG_REG_4_64;
    }

    ++num_args_alloced;
    return NULL;
}

char *get_arg_reg32_num(int num)
{
    if(num == 0)
    {
        return ARG_REG_1_32;
    }
    else if(num == 1)
    {
        return ARG_REG_2_32;
    }
    else if(num == 2)
    {
        return ARG_REG_3_32;
    }
    else if(num == 3)
    {
        return ARG_REG_4_32;
    }

    return NULL;
}

/******** stackmng *********/
stackmng_t *global_stackmng = NULL;

void init_stackmng()
{
    assert(global_stackmng == NULL);

    num_args_alloced = 0;
    global_stackmng = (stackmng_t *)malloc(sizeof(stackmng_t));

    global_stackmng->cur_scope = NULL;
    global_stackmng->reg_stack = init_reg_stack();
}

/* For when you need low level access to the stack */
StackScope_t *get_cur_scope()
{
    return global_stackmng->cur_scope;
}

int get_full_stack_offset()
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);
    StackScope_t *scope;
    int actual_offset, div, rem;

    scope = global_stackmng->cur_scope;

    actual_offset = CONST_STACK_OFFSET_BYTES +
                        scope->t_offset + scope->x_offset + scope->z_offset;

    /* x86_64 requires stack offsets to avoid undefined behavior */
    div = actual_offset / REQUIRED_OFFSET;
    rem = actual_offset % REQUIRED_OFFSET;

    if(rem > 0)
        ++div;

    return div * REQUIRED_OFFSET;

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
        fprintf(stderr, "DEBUG: Added %s to x_offset %d\n", new_node->label, new_node->offset);
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

RegStack_t *get_reg_stack()
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->reg_stack != NULL);

    return global_stackmng->reg_stack;
}

/* Finds variable on temp in current stack */
/* Returns NULL if not found */
StackNode_t *find_in_temp(char *label)
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    StackScope_t *cur_scope;
    StackNode_t *cur_node;

    cur_scope = global_stackmng->cur_scope;

    cur_node = stackscope_find_t(cur_scope, label);
    return cur_node;
}

/* TODO: Does not find variables outside the current scope */
StackNode_t *find_label(char *label)
{
    assert(global_stackmng != NULL);
    assert(global_stackmng->cur_scope != NULL);

    StackScope_t *cur_scope;
    StackNode_t *cur_node;

    cur_scope = global_stackmng->cur_scope;

    cur_node = stackscope_find_z(cur_scope, label);
    if(cur_node != NULL)
    {
        return cur_node;
    }

    cur_node = stackscope_find_x(cur_scope, label);
    if(cur_node != NULL)
    {
        return cur_node;
    }

    cur_node = stackscope_find_t(cur_scope, label);
    if(cur_node != NULL)
    {
        return cur_node;
    }

    return NULL;
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
    rbx->bit_64 = strdup("%rbx");
    rbx->bit_32 = strdup("%ebx");

    /* RDI */
    Register_t *rdi;
    rdi = (Register_t *)malloc(sizeof(Register_t));
    rdi->bit_64 = strdup("%rdi");
    rdi->bit_32 = strdup("%edi");

    /* RSI */
    /*Register_t *rsi;
    rsi = (Register_t *)malloc(sizeof(Register_t));
    rsi->bit_64 = strdup("%rsi");
    rsi->bit_32 = strdup("%esi");*/


    registers = CreateListNode(rbx, LIST_UNSPECIFIED);
    registers = PushListNodeBack(registers, CreateListNode(rdi, LIST_UNSPECIFIED));
    /*registers = PushListNodeBack(registers, CreateListNode(rsi, LIST_UNSPECIFIED));*/

    /*
    registers = CreateListNode(rdi, LIST_UNSPECIFIED);
    registers = PushListNodeBack(registers, CreateListNode(rsi, LIST_UNSPECIFIED));
    registers = PushListNodeBack(registers, CreateListNode(rbx, LIST_UNSPECIFIED));
    */

    reg_stack->num_registers_alloced = 0;
    reg_stack->registers_free = registers;
    reg_stack->num_registers = 2;

    return reg_stack;
}

/* NOTE: Getters return number greater than 0 if it had to kick a value out to temp */
/* The returned int is the temp offset to restore the value */
/* TODO: Doesn't actually kick variable out to temp yet */
int get_register_64bit(RegStack_t *regstack, char *reg_64, Register_t **return_reg)
{
    assert(regstack != NULL);
    assert(reg_64 != NULL);

    ListNode_t *cur_reg, *prev_reg;
    Register_t *reg;

    cur_reg = regstack->registers_free;
    prev_reg = NULL;
    while(cur_reg != NULL)
    {
        reg = (Register_t *)cur_reg->cur;
        if(strcmp(reg->bit_64, reg_64) == 0)
        {
            regstack->num_registers_alloced++;

            if(prev_reg == NULL)
                regstack->registers_free = cur_reg->next;
            else
                prev_reg->next = cur_reg->next;

            free(cur_reg);
            *return_reg = reg;

            return 0;
        }

        prev_reg = cur_reg;
        cur_reg = cur_reg->next;
    }

    fprintf(stderr, "ERROR: Kicking out values in registers not currently supported!\n");
    exit(1);
}

/* NOTE: Getters return number greater than 0 if it had to kick a value out to temp */
/* The returned int is the temp offset to restore the value */
/* TODO: Doesn't actually kick variable out to temp yet */
int get_register_32bit(RegStack_t *regstack, char *reg_32, Register_t **return_reg)
{
    assert(regstack != NULL);
    assert(reg_32 != NULL);

    ListNode_t *cur_reg, *prev_reg;
    Register_t *reg;

    cur_reg = regstack->registers_free;
    prev_reg = NULL;
    while(cur_reg != NULL)
    {
        reg = (Register_t *)cur_reg->cur;
        if(strcmp(reg->bit_32, reg_32) == 0)
        {
            regstack->num_registers_alloced++;

            if(prev_reg == NULL)
                regstack->registers_free = cur_reg->next;
            else
                prev_reg->next = cur_reg->next;

            free(cur_reg);
            *return_reg = reg;

            return 0;
        }

        prev_reg = cur_reg;
        cur_reg = cur_reg->next;
    }

    fprintf(stderr, "ERROR: Kicking out values in registers not currently supported!\n");
    exit(1);
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
        reg_stack->registers_free = PushListNodeFront(reg_stack->registers_free,
            CreateListNode(reg, LIST_UNSPECIFIED));
    }
}

void swap_reg_stack(RegStack_t *reg_stack)
{
    /* Need at least two registers to do a swap */
    assert(reg_stack != NULL);
    assert(reg_stack->registers_free != NULL);
    assert(reg_stack->registers_free->next != NULL);

    ListNode_t *top, *next;
    top = reg_stack->registers_free;
    next = top->next;

    top->next = next->next;
    next->next = top;
    reg_stack->registers_free = next;
}

Register_t *front_reg_stack(RegStack_t *reg_stack)
{
    assert(reg_stack != NULL);

    return (Register_t *)reg_stack->registers_free->cur;
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

int get_num_registers(RegStack_t *reg_stack)
{
    return reg_stack->num_registers - reg_stack->num_registers_alloced;
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

StackNode_t *stackscope_find_t(StackScope_t *cur_scope, char *label)
{
    ListNode_t *cur_li;
    StackNode_t *cur_node;

    cur_li = cur_scope->t;
    while(cur_li != NULL)
    {
        cur_node = (StackNode_t *)cur_li->cur;
        if(strcmp(cur_node->label, label) == 0)
        {
            return cur_node;
        }

        cur_li = cur_li->next;
    }

    return NULL;
}

StackNode_t *stackscope_find_x(StackScope_t *cur_scope, char *label)
{
    ListNode_t *cur_li;
    StackNode_t *cur_node;

    cur_li = cur_scope->x;
    while(cur_li != NULL)
    {
        cur_node = (StackNode_t *)cur_li->cur;
        if(strcmp(cur_node->label, label) == 0)
        {
            return cur_node;
        }

        cur_li = cur_li->next;
    }

    return NULL;
}

StackNode_t *stackscope_find_z(StackScope_t *cur_scope, char *label)
{
    ListNode_t *cur_li;
    StackNode_t *cur_node;

    cur_li = cur_scope->z;
    while(cur_li != NULL)
    {
        cur_node = (StackNode_t *)cur_li->cur;
        if(strcmp(cur_node->label, label) == 0)
        {
            return cur_node;
        }

        cur_li = cur_li->next;
    }

    return NULL;
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
    new_node->size = size;

    return new_node;
}

void destroy_stack_node(StackNode_t *node)
{
    assert(node != NULL);
    assert(node->label != NULL);

    free(node->label);
    free(node);
}
