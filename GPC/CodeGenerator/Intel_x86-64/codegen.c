/*
    Damon Gwinn
    Code generation
    This is the dragon slayer

    See codegen.h for stack and implementation details
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "codegen.h"
#include "stackmng/stackmng.h"
#include "../../Parser/List/List.h"
#include "../../Parser/ParseTree/tree.h"
#include "../../Parser/ParseTree/tree_types.h"
#include "../../Parser/LexAndYacc/y.tab.h"

/* Generates a function header */
void codegen_function_header(char *func_name, FILE *o_file)
{
    /*
        .globl	<func_name>
    	.type	<func_name>, @function
        <func_name>:
            pushq   %rbp
            movq    %rsp, %rbp
    */

    fprintf(o_file, ".globl\t%s\n.type\t%s, @function\n", func_name, func_name);
    fprintf(o_file, "%s:\n\tpushq\t%%rbp\n\tmovq\t%%rsp, %%rbp\n", func_name);

    return;
}

/* Generates a function footer */
void codegen_function_footer(char *func_name, FILE *o_file)
{
    /*
        popq    %rbp
        ret
        .size	<func_name>, .-<func_name>
    */

    fprintf(o_file, "\tpopq\t%%rbp\n\tret\n");
    fprintf(o_file, "\t.size\t%s, .-%s\n", func_name, func_name);

    return;
}


/* This is the entry function */
void codegen(Tree_t *tree, char *input_file_name, char *output_file_name)
{
    FILE *output_file;
    char *prgm_name;

    output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        fprintf(stderr, "ERROR: Failed to open output file: %s\n", output_file_name);
        exit(1);
    }

    init_stackmng();

    codegen_program_header(input_file_name, output_file);

    prgm_name = codegen_program(tree, output_file);
    codegen_main(prgm_name, output_file);

    codegen_program_footer(output_file);

    fclose(output_file);
    output_file = NULL;

    free_stackmng();

    return;
}

/* Generates standard gcc headers and labels for printf and scanf */
void codegen_program_header(char *fname, FILE *o_file)
{
    /*
        .file	"<FILE_NAME>"
        .section	.rodata
        .LC0:
            .string "%d\n"
            .text
        .LC1:
            .string "%d"
            .text
    */

    fprintf(o_file, "\t.file\t\"%s\"\n\t.section\t.rodata\n", fname);
    fprintf(o_file, "\t.LC0:\n\t\t.string\t\"%%d\\n\"\n\t\t.text\n");
    fprintf(o_file, "\t.LC1:\n\t\t.string\t\"%%d\"\n\t\t.text\n");
    return;
}

/* Generates main which calls our program */
void codegen_main(char *prgm_name, FILE *o_file)
{
    /*
        HEADER
            movl	$0, %eax
            call	<prgm_name>
            movl	$0, %eax
            FOOTER
    */
    codegen_function_header("main", o_file);

    fprintf(o_file, "\tmovl\t$0, %%eax\n\tcall\t%s\n\tmovl\t$0, %%eax\n", prgm_name);

    codegen_function_footer("main", o_file);
}

/* Generates the footer of the whole program */
/* TODO: Fix footer to reflect true gcc and os type */
void codegen_program_footer(FILE *o_file)
{
    /*
        .ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
    	.section	.note.GNU-stack,"",@progbits
    */

    fprintf(o_file, ".ident\t\"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609\"\n");
    fprintf(o_file, ".section\t.note.GNU-stack,\"\",@progbits\n");
}


/******* EVERYTHING BELOW THIS POINT IS VERY GRUESOME DRAGON SLAYING ********/

/* TODO: Currently only handles local variables and body_statement */
/* Returns the program name for use with main */
char * codegen_program(Tree_t *prgm, FILE *o_file)
{
    assert(prgm->type == TREE_PROGRAM_TYPE);

    char *prgm_name;
    struct Program *data;
    ListNode_t *inst_list;

    data = &prgm->tree_data.program_data;
    prgm_name = data->program_id;

    push_stackscope();

    codegen_function_header(prgm_name, o_file);

    /* TODO */
    codegen_function_locals(data->var_declaration, o_file);
    inst_list = codegen_function_body(data->body_statement, o_file);

    codegen_function_footer(prgm_name, o_file);

    pop_stackscope();

    return prgm_name;
}

/* Pushes function locals onto the stack */
void codegen_function_locals(ListNode_t *local_decl, FILE *o_file)
{
     ListNode_t *cur, *id_list;
     Tree_t *tree;

     cur = local_decl;

     while(cur != NULL)
     {
         tree = (Tree_t *)cur->cur;
         assert(tree != NULL);
         assert(tree->type == TREE_VAR_DECL);

         id_list = tree->tree_data.var_decl_data.ids;
         if(tree->tree_data.var_decl_data.type == REAL_TYPE)
         {
             fprintf(stderr, "Warning: REAL types not supported, treating as integer\n");
         }

         while(id_list != NULL)
         {
             add_l_x((char *)id_list->cur);
             id_list = id_list->next;
         };

         cur = cur->next;
     }
}

/* TODO: Only handles assignments and read/write builtins */
/* Returns a list of instructions */
ListNode_t *codegen_function_body(struct Statement *stmt, FILE *o_file)
{

}
