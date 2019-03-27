/*
    Damon Gwinn
    Performs semantic checking on a given parse tree
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "SemCheck.h"
#include "../ParseTree/tree.h"
#include "../ParseTree/tree_types.h"
#include "./SymTab/SymTab.h"
#include "./HashTable/HashTable.h"
#include "../LexAndYacc/y.tab.h"

int semcheck_args(SymTab_t *symtab, ListNode_t *args, int line_num);
int semcheck_decls(SymTab_t *symtab, ListNode_t *decls);

int semcheck_subprogram(SymTab_t *symtab, Tree_t *subprogram);
int semcheck_subprograms(SymTab_t *symtab, ListNode_t *decls);
int semcheck_tree(SymTab_t *symtab, Tree_t *tree);
int semcheck_stmt(SymTab_t *symtab, struct Statement *stmt);
int semcheck_expr(SymTab_t *symtab, struct Expression *expr);

/* The main function for checking a tree */
/* Return values:
    0       -> Check successful
    -1      -> Check successful with warnings
    >= 1    -> Check failed with n errors
*/
int start_semcheck(Tree_t *parse_tree)
{
    SymTab_t *symtab;
    int return_val;

    symtab = InitSymTab();

    return_val = semcheck_tree(symtab, parse_tree);
    DestroySymTab(symtab);

    return return_val;
}

/* Adds arguments to the symbol table */
/* A return value greater than 0 indicates how many errors occurred */
int semcheck_args(SymTab_t *symtab, ListNode_t *args, int line_num)
{
    ListNode_t *cur;
    int return_val, func_return;
    assert(symtab != NULL);

    return_val = 0;

    cur = args;

    /* Checking if they are declarations
        NOTE: Mismatching arg types is an error */
    if(cur != NULL)
        if(cur->type == LIST_TREE)
            return semcheck_decls(symtab, args);

    while(cur != NULL)
    {
        /* If not a list of declarations, must be a list of strings */
        assert(cur->type == LIST_STRING);

        func_return = PushVarOntoScope(symtab, HASHVAR_UNTYPED, (char *)cur->cur);

        /* Greater than 0 signifies an error */
        if(func_return > 0)
        {
            fprintf(stderr, "Error on line %d, redeclaration of variable %s!\n",
                line_num, (char *)cur->cur);
            return_val += func_return;
        }
    }

    return return_val;
}

/* Pushes a bunch of declarations onto the current scope */
/* A return value greater than 0 indicates how many errors occurred */
int semcheck_decls(SymTab_t *symtab, ListNode_t *decls)
{
    ListNode_t *cur, *ids;
    Tree_t *tree;
    int return_val, func_return;

    enum VarType var_type;

    assert(symtab != NULL);

    return_val = 0;
    cur = decls;
    while(cur != NULL)
    {
        /* Any declaration is always a tree */
        assert(cur->type == LIST_TREE);
        tree = (Tree_t *)cur->cur;

        ids = tree->tree_data.var_decl_data.ids;

        while(ids != NULL)
        {
            assert(ids->type == LIST_STRING);

            /* Variable declarations */
            if(tree->type == TREE_VAR_DECL)
            {
                if(tree->tree_data.var_decl_data.type == INT_TYPE)
                    var_type = HASHVAR_INTEGER;
                else
                    var_type = HASHVAR_REAL;

                func_return = PushVarOntoScope(symtab, var_type, (char *)ids->cur);
            }
            /* Array declarations */
            else
            {
                if(tree->tree_data.arr_decl_data.type == INT_TYPE)
                    var_type = HASHVAR_INTEGER;
                else
                    var_type = HASHVAR_REAL;

                func_return = PushArrayOntoScope(symtab, var_type, (char *)ids->cur);
            }

            /* Greater than 0 signifies an error */
            if(func_return > 0)
            {
                fprintf(stderr, "Error on line %d, redeclaration of variable %s!\n",
                    tree->line_num, (char *)ids->cur);
                return_val += func_return;
            }
        }

    }

    return return_val;
}

/* Semantic check on an entire subprogram */
/* A return value greater than 0 indicates how many errors occurred */
int semcheck_subprogram(SymTab_t *symtab, Tree_t *subprogram)
{
    int return_val, func_return;
    enum VarType var_type;

    assert(symtab != NULL);
    assert(subprogram != NULL);
    assert(subprogram->type == TREE_SUBPROGRAM);

    switch(subprogram->tree_data.subprogram_data.sub_type)
    {
        case TREE_SUBPROGRAM_PROC:
          func_return = PushProcedureOntoScope(symtab, subprogram->tree_data.subprogram_data.id,
                          subprogram->tree_data.subprogram_data.args_var);

          break;
        case TREE_SUBPROGRAM_FUNC:
          /* Need to additionally extract the return type */
          if(subprogram->tree_data.subprogram_data.return_type == INT_TYPE)
              var_type = HASHVAR_INTEGER;
          else
              var_type = HASHVAR_REAL;

          func_return = PushFunctionOntoScope(symtab, subprogram->tree_data.subprogram_data.id,
                          var_type, subprogram->tree_data.subprogram_data.args_var);

          break;

        default:
            fprintf(stderr, "BAD TYPE IN TREE_SUBPROGRAM (semcheck_subprogram)!\n");
            exit(1);
    }


    /* Greater than 0 signifies an error */
    if(func_return > 0)
    {
        fprintf(stderr, "Error on line %d, redeclaration of variable %s!\n",
            subprogram->line_num, subprogram->tree_data.subprogram_data.id);
        return_val += func_return;
    }

    /* Pushing new scope for the subprogram */
    PushScope(symtab);

    /* These arguments are themselves like declarations */
    return_val += semcheck_decls(symtab, subprogram->tree_data.subprogram_data.args_var);

    return_val += semcheck_decls(symtab, subprogram->tree_data.subprogram_data.declarations);

    return_val += semcheck_subprograms(symtab, subprogram->tree_data.subprogram_data.subprograms);

    return_val += semcheck_stmt(symtab, subprogram->tree_data.subprogram_data.statement_list);
}

/* Semantic check for a tree */
int semcheck_tree(SymTab_t *symtab, Tree_t *tree)
{
    int return_val;
    enum VarType var_type;
    assert(tree != NULL);
    assert(symtab != NULL);

    return_val = 0;

    switch(tree->type)
    {
        /* Only allow one program for now */
        case TREE_PROGRAM_TYPE:
          PushScope(symtab);

          /* TODO: Push program name onto scope */

          /* TODO: Fix line number bug here */
          return_val += semcheck_args(symtab, tree->tree_data.program_data.args_char,
            tree->line_num);

          return_val += semcheck_decls(symtab, tree->tree_data.program_data.var_declaration);

          return_val += semcheck_subprograms(symtab, tree->tree_data.program_data.subprograms);

          return_val += semcheck_stmt(symtab, tree->tree_data.program_data.body_statement);
          break;

        case TREE_SUBPROGRAM:
            return_val += semcheck_subprogram(symtab, tree);
            break;

        /*
        case TREE_VAR_DECL:
          fprintf(f, "[VARDECL of type %d]\n", tree->tree_data.var_decl_data.type);
          list_print(tree->tree_data.var_decl_data.ids, f, num_indent+1);
          break;

        case TREE_ARR_DECL:
          fprintf(f, "[ARRDECL of type %d in range(%d, %d)]\n",
            tree->tree_data.arr_decl_data.type, tree->tree_data.arr_decl_data.s_range,
            tree->tree_data.arr_decl_data.e_range);

          list_print(tree->tree_data.var_decl_data.ids, f, num_indent+1);
          break;
*/
        default:
        fprintf(stderr, "BAD TYPE IN semcheck_tree!\n");
        exit(1);
    }

    return return_val;
}
