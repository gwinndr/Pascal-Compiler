/*
    Damon Gwinn
    Code generation
    This is the dragon slayer

    Stack structure (64-bit):
        This is the current stack structure, subject to changes as needed
        Last modified 4-21-19

        d_word refers to a doubleword or 4 bytes
        q_word refers to a quadword or 8 bytes

        TODO:
            - Support non-local variables
            - Support generalized reading and writing
            - Support real numbers
            - Support temporary block reuse

        LOW ADDRESSES
        ============
        t   d_words (temporaries)
        ============
        x   d_words (local variables)
        ============
        z   d_words (argument variables)
        ============
        1   q_word (static parent pointer)
        ============
        1   q_word (subprogram return addr)
        ============
        HIGH ADDRESSES

    NOTE: Add arguements, then locals, then temps to avoid copying

    STACK MANAGEMENT:
        A stack management structure will be utilized (see stackmng.h)

        The stack manager shall consist of:
            - StackScope structure which contains:
                - The current z offset (in bytes)
                - The current x offset (in bytes)
                - The current t offset (in bytes)
                - z blocks in use with labels and offsets
                - x blocks in use with labels and offsets
                - z blocks in use with labels and offsets
                - t blocks in use with labels and offsets
                - StackScope pointer to previous scope
            - Register stack of free registers
            - Registers in use with labels

    INSTRUCTION LAYOUT:
        The first section of instructions for a function is known as the "prologue."
        This section performs gcc standard stack management first, then allocates needed stack
        space.

        The next section is the body which consists of the desired user functionality of the
        function.

        The final section is the epilogue which performs required gcc function exiting standards

    OUTPUT:
        Output is written as a gcc assembly file (.s). Assembly is then assembled using gcc.

        The write bultin currently only takes integer types and has the
            label LC0 with %d\n. The function call is "call printf"

        The read bultin currently only reads integer types and has the
            label LC1 with "%d". The function call is "call __isoc99_scanf"
            - NOTE: Using c99 gcc standard here for simplicity

    TEMPORARIES:
        Temporaries are allocated as needed and reused once freed for efficiency
*/

#ifndef CODE_GEN_H
#define CODE_GEN_H

#define DEBUG_CODEGEN

#include <stdlib.h>
#include <stdio.h>
#include "stackmng/stackmng.h"
#include "../../Parser/List/List.h"
#include "../../Parser/ParseTree/tree.h"
#include "../../Parser/ParseTree/tree_types.h"

/* This is the entry function */
void codegen(Tree_t *, char *input_file_name, char *output_file_name);

void codegen_program_header(char *, FILE *);;
void codegen_program_footer(FILE *);
void codegen_main(char *prgm_name, FILE *o_file);
void codegen_stack_space(FILE *);
void codegen_inst_list(ListNode_t *, FILE *);

char * codegen_program(Tree_t *, FILE *);
void codegen_function_locals(ListNode_t *, FILE *);
ListNode_t *codegen_function_body(struct Statement *, FILE *);

ListNode_t *codegen_var_assignment(struct Statement *, ListNode_t *, FILE *);
ListNode_t *codegen_proc_call(struct Statement *, ListNode_t *, FILE *);

#endif
