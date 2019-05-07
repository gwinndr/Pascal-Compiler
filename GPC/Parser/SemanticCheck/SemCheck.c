/*
    Damon Gwinn
    Performs semantic checking on a given parse tree

    NOTE: Max scope level refers to the highest level scope we can reference a variable at
        - 0 is the current scope, 1 is the first above and so on
        - Functions can't have side effects, but they can contain procedures so this is a
            general way to define the maximum scope level

    TODO: CHECK FOR RETURN IN FUNCTIONS (Add a "referenced" flag to symbol table elements. Need it for optimizations anyway...)
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "SemCheck.h"
#include "../../flags.h"
#include "../../Optimizer/optimizer.h"
#include "../ParseTree/tree.h"
#include "../ParseTree/tree_types.h"
#include "./SymTab/SymTab.h"
#include "./HashTable/HashTable.h"
#include "SemChecks/SemCheck_stmt.h"
#include "SemChecks/SemCheck_expr.h"
#include "../LexAndYacc/y.tab.h"

/* Adds built-in functions */
void semcheck_add_builtins(SymTab_t *symtab);

/* Main is a special keyword at the moment for code generation */
int semcheck_id_not_main(char *id)
{
    if(strcmp(id, "main") == 0)
    {
        fprintf(stderr, "ERROR: main is special keyword, it cannot be a program, or subprogram\n");
        return 1;
    }

    return 0;
}

int semcheck_program(SymTab_t *symtab, Tree_t *tree);

int semcheck_args(SymTab_t *symtab, ListNode_t *args, int line_num);
int semcheck_decls(SymTab_t *symtab, ListNode_t *decls);

int semcheck_subprogram(SymTab_t *symtab, Tree_t *subprogram, int max_scope_lev);
int semcheck_subprograms(SymTab_t *symtab, ListNode_t *subprograms, int max_scope_lev);

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
    semcheck_add_builtins(symtab);
    /*PrintSymTab(symtab, stderr, 0);*/

    return_val = semcheck_program(symtab, parse_tree);
    DestroySymTab(symtab);

    if(return_val > 0)
        fprintf(stderr, "\nCheck failed with %d error(s)!\n\n", return_val);
    else
        fprintf(stderr, "\nCheck successful!\n\n");

    return return_val;
}

/* Adds built-in functions */
void semcheck_add_builtins(SymTab_t *symtab)
{
    char *id;
    ListNode_t *args, *arg_ids;

    /**** READ PROCEDURE ****/
    id = strdup("read");

    /* Only arg is a variable to read into */
    arg_ids = CreateListNode(strdup("var"), LIST_STRING);
    args = CreateListNode(mk_vardecl(-1, arg_ids, BUILTIN_ANY_TYPE), LIST_TREE);

    AddBuiltinProc(symtab, id, args);

    /**** WRITE PROCEDURE ****/
    id = strdup("write");

    /* Only arg is a variable to read into */
    arg_ids = CreateListNode(strdup("var"), LIST_STRING);
    args = CreateListNode(mk_vardecl(-1, arg_ids, BUILTIN_ANY_TYPE), LIST_TREE);

    AddBuiltinProc(symtab, id, args);
}

/* Semantic check for a program */
int semcheck_program(SymTab_t *symtab, Tree_t *tree)
{
    int return_val;
    enum VarType var_type;
    assert(tree != NULL);
    assert(symtab != NULL);
    assert(tree->type == TREE_PROGRAM_TYPE);

    return_val = 0;

    PushScope(symtab);

    return_val += semcheck_id_not_main(tree->tree_data.program_data.program_id);

    /* TODO: Push program name onto scope */

    /* TODO: Fix line number bug here */
    return_val += semcheck_args(symtab, tree->tree_data.program_data.args_char,
      tree->line_num);

    return_val += semcheck_decls(symtab, tree->tree_data.program_data.var_declaration);

    return_val += semcheck_subprograms(symtab, tree->tree_data.program_data.subprograms, 0);

    return_val += semcheck_stmt(symtab, tree->tree_data.program_data.body_statement, 0);

    if(optimize_flag() > 0 && return_val == 0)
    {
        optimize(symtab, tree);
    }

    PopScope(symtab);
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
            fprintf(stderr, "Error on line %d, redeclaration of name %s!\n",
                line_num, (char *)cur->cur);
            return_val += func_return;
        }

        cur = cur->next;
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
                fprintf(stderr, "Error on line %d, redeclaration of name %s!\n",
                    tree->line_num, (char *)ids->cur);
                return_val += func_return;
            }

            ids = ids->next;
        }

        cur = cur->next;

    }

    return return_val;
}

/* Semantic check on an entire subprogram */
/* A return value greater than 0 indicates how many errors occurred */
int semcheck_subprogram(SymTab_t *symtab, Tree_t *subprogram, int max_scope_lev)
{
    int return_val, func_return, return_mutated;
    int new_max_scope;
    enum VarType var_type;
    enum TreeType sub_type;
    HashNode_t *hash_return;

    assert(symtab != NULL);
    assert(subprogram != NULL);
    assert(subprogram->type == TREE_SUBPROGRAM);

    sub_type = subprogram->tree_data.subprogram_data.sub_type;
    assert(sub_type == TREE_SUBPROGRAM_PROC || sub_type == TREE_SUBPROGRAM_FUNC);

    return_val += semcheck_id_not_main(subprogram->tree_data.subprogram_data.id);

    /**** FIRST PLACING SUBPROGRAM ON THE CURRENT SCOPE ****/
    return_val = 0;
    if(sub_type == TREE_SUBPROGRAM_PROC)
    {
        func_return = PushProcedureOntoScope(symtab, subprogram->tree_data.subprogram_data.id,
                        subprogram->tree_data.subprogram_data.args_var);

        /* Pushing new scope for the subprogram and allowing recursion */
        PushScope(symtab);
        PushProcedureOntoScope(symtab, subprogram->tree_data.subprogram_data.id,
            subprogram->tree_data.subprogram_data.args_var);

        new_max_scope = max_scope_lev+1;
    }
    /* Function */
    else
    {
        /* Need to additionally extract the return type */
        if(subprogram->tree_data.subprogram_data.return_type == INT_TYPE)
            var_type = HASHVAR_INTEGER;
        else
            var_type = HASHVAR_REAL;


        func_return = PushFunctionOntoScope(symtab, subprogram->tree_data.subprogram_data.id,
                        var_type, subprogram->tree_data.subprogram_data.args_var);

        /* Pushing new scope for the subprogram and allowing recursion and return statements */
        /* NOTE: HASHTYPE_FUNCTION_RETURN is a special type you can call and set (return stmt) */
        PushScope(symtab);
        PushFuncRetOntoScope(symtab, subprogram->tree_data.subprogram_data.id,
            var_type, subprogram->tree_data.subprogram_data.args_var);

        new_max_scope = 0;
    }

    /**** Check the subprogram internals now ****/

    /* Greater than 0 signifies an error */
    if(func_return > 0)
    {
        fprintf(stderr, "On line %d: redeclaration of name %s!\n",
            subprogram->line_num, subprogram->tree_data.subprogram_data.id);

        return_val += func_return;
    }

    /* These arguments are themselves like declarations */
    return_val += semcheck_decls(symtab, subprogram->tree_data.subprogram_data.args_var);

    return_val += semcheck_decls(symtab, subprogram->tree_data.subprogram_data.declarations);

    return_val += semcheck_subprograms(symtab, subprogram->tree_data.subprogram_data.subprograms,
                    new_max_scope);

    /* Functions cannot have side effects, so need to call a special function in that case */
    if(sub_type == TREE_SUBPROGRAM_PROC)
    {
        return_val += semcheck_stmt(symtab,
                subprogram->tree_data.subprogram_data.statement_list,
                new_max_scope);
    }
    else
    {
        assert(FindIdent(&hash_return, symtab, subprogram->tree_data.subprogram_data.id)
                    == 0);

        ResetHashNodeStatus(hash_return);
        return_val += semcheck_func_stmt(symtab,
                subprogram->tree_data.subprogram_data.statement_list);
        if(hash_return->mutated == NO_MUTATE)
        {
            fprintf(stderr,
                "Error in function %s: no return statement declared in function body!\n\n",
                subprogram->tree_data.subprogram_data.id);
            ++return_val;
        }
    }

    if(optimize_flag() > 0 && return_val == 0)
    {
        optimize(symtab, subprogram);
    }

    PopScope(symtab);
    return return_val;
}


/* Semantic check on multiple subprograms */
/* A return value greater than 0 indicates how many errors occurred */
int semcheck_subprograms(SymTab_t *symtab, ListNode_t *subprograms, int max_scope_lev)
{
    ListNode_t *cur;
    int return_val;
    assert(symtab != NULL);

    return_val = 0;
    cur = subprograms;
    while(cur != NULL)
    {
        assert(cur->type == LIST_TREE);
        return_val += semcheck_subprogram(symtab, (Tree_t *)cur->cur, max_scope_lev);
        cur = cur->next;
    }

    return return_val;
}
