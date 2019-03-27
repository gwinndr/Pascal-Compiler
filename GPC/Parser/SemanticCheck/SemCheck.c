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

int semcheck_decls(SymTab_t *symtab, ListNode_t *idents);
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

    return_val =  semcheck_tree(symtab, parse_tree);
    DestroySymTab(symtab);

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

        if(tree->tree_data.var_decl_data.type == INT_TYPE)
            var_type = HASHVAR_INTEGER;
        else
            var_type = HASHVAR_REAL;

        while(ids != NULL)
        {
            assert(ids->type == LIST_STRING);

            if(tree->type == TREE_VAR_DECL)
                func_return = PushVarOntoScope(symtab, var_type, (char *)ids->cur);
            else
                func_return = PushArrayOntoScope(symtab, var_type, (char *)ids->cur);

            /* Greater than 0 signifies an error */
            if(func_return > 0)
            {
                fprintf(stderr, "Error on line %d, redeclaration of variable %s!\n",
                    tree->line_num, (char *)ids->cur);
                ++return_val;
            }
        }

    }

    return return_val;
}

/* Semantic check for a tree */
int semcheck_tree(SymTab_t *symtab, Tree_t *tree)
{
    int return_val;
    assert(tree != NULL);
    assert(symtab != NULL);

    return_val = 0;

    switch(tree->type)
    {
        /* Only allow one program for now */
        case TREE_PROGRAM_TYPE:
          PushScope(symtab);

          /*list_print(tree->tree_data.program_data.args_char, f, num_indent+1);*/

          return_val += semcheck_decls(symtab, tree->tree_data.program_data.var_declaration);

          return_val += semcheck_subprograms(symtab, tree->tree_data.program_data.subprograms);

          return_val += semcheck_stmt(symtab, tree->tree_data.program_data.body_statement);
          break;
        /*
        case TREE_SUBPROGRAM:
          switch(tree->tree_data.subprogram_data.sub_type)
          {
              case TREE_SUBPROGRAM_PROC:
                fprintf(f, "[PROCEDURE:%s]:\n", tree->tree_data.subprogram_data.id);
                print_indent(f, num_indent);
                break;
              case TREE_SUBPROGRAM_FUNC:
                fprintf(f, "[FUNCTION:%s]:\n", tree->tree_data.subprogram_data.id);
                print_indent(f, num_indent);
                fprintf(f, "[RETURNS:%d]\n", tree->tree_data.subprogram_data.return_type);
                break;

              default:
                  fprintf(stderr, "BAD TYPE IN TREE_SUBPROGRAM (semcheck_tree)!\n");
                  exit(1);
          }
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[ARGS]:\n");
          list_print(tree->tree_data.subprogram_data.args_var, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[VAR_DECLS]:\n");
          list_print(tree->tree_data.subprogram_data.declarations, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[SUBPROGRAMS]:\n");
          list_print(tree->tree_data.subprogram_data.subprograms, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[BODY]:\n");
          stmt_print(tree->tree_data.subprogram_data.statement_list, f, num_indent+1);
          break;

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
