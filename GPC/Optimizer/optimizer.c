/*
    Damon Gwinn
    Simple optimizer for a parse tree

    OPTIMIZER INFO:
        - All non-referenced stack variables removed
        - All constant number expressions simplified to a single number

    NOTE: Optimizer designed to work in unison with the parser

    TODO: Support arrays
        - All local-only variables only mutated to constant values and referenced afterwards
            are removed
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "optimizer.h"
#include "../Parser/ParseTree/tree.h"
#include "../Parser/ParseTree/tree_types.h"
#include "../Parser/SemanticCheck/SymTab/SymTab.h"
#include "../Parser/SemanticCheck/HashTable/HashTable.h"

void optimize_prog(SymTab_t *symtab, Tree_t *prog);

void remove_var_decls(char *id, ListNode_t *var_decls);
int remove_mutation_statement(char *id, struct Statement *stmt);
int remove_mutation_var_assign(char *id, struct Statement *var_assign);
int remove_mutation_compound_statement(char *id, struct Statement *body_statement);
void set_vars_lists(SymTab_t *, ListNode_t *, ListNode_t **, ListNode_t **);
void add_to_list(ListNode_t **, void *obj);

/* The main entry point for the optimizer */
void optimize(SymTab_t *symtab, Tree_t *tree)
{
    assert(symtab != NULL);
    assert(tree != NULL);

    switch(tree->type)
    {
        case TREE_PROGRAM_TYPE:
            optimize_prog(symtab, tree);
            break;

        default:
            fprintf(stderr, "Non-critical error: Unsupported tree type given to optimizer!\n");
            return;
    }
}

/* Optimizes a program */
void optimize_prog(SymTab_t *symtab, Tree_t *prog)
{
    assert(symtab != NULL);
    assert(prog != NULL);
    assert(prog->type == TREE_PROGRAM_TYPE);

    ListNode_t *vars_to_check, *vars_to_remove, *cur;
    struct Program *prog_data;
    HashNode_t *node;
    int replace_with;

    prog_data = &prog->tree_data.program_data;
    vars_to_check = vars_to_remove = NULL;

    set_vars_lists(symtab, prog_data->var_declaration, &vars_to_check, &vars_to_remove);

    cur = vars_to_remove;
    while(cur != NULL)
    {
        #ifdef DEBUG_OPTIMIZER
            fprintf(stderr, "OPTIMIZER: Removing unreferenced variable %s\n",
                (char *)cur->cur);
        #endif

        remove_mutation_statement((char *)cur->cur, prog_data->body_statement);
        remove_var_decls((char *)cur->cur, prog_data->var_declaration);

        cur = cur->next;
    }

    DestroyList(vars_to_check);
    DestroyList(vars_to_remove);
}

/* Removes all variable declarations matching an id */
void remove_var_decls(char *id, ListNode_t *var_decls)
{
    Tree_t *var_decl;
    ListNode_t *prev, *ids, *temp;

    while(var_decls != NULL)
    {
        var_decl = (Tree_t *)var_decls->cur;
        assert(var_decl->type == TREE_VAR_DECL);

        ids = var_decl->tree_data.var_decl_data.ids;
        prev = NULL;
        while(ids != NULL)
        {
            if(strcmp((char *)ids->cur, id) == 0)
            {
                free(ids->cur);
                temp = ids->next;
                free(ids);
                ids = temp;
                if(prev == NULL)
                {
                    var_decl->tree_data.var_decl_data.ids = ids;
                }
                else
                {
                    prev->next = ids;
                }

                return;
            }

            prev = ids;
            ids = ids->next;
        }

        var_decls = var_decls->next;
    }
}

/* Removes mutation statements that mutate an ID */
/* Returns 1 if removal occurred */
int remove_mutation_statement(char *id, struct Statement *stmt)
{
    assert(stmt != NULL);
    switch(stmt->type)
    {
        case STMT_VAR_ASSIGN:
            return remove_mutation_var_assign(id, stmt);

        case STMT_COMPOUND_STATEMENT:
            return remove_mutation_compound_statement(id, stmt);

        case STMT_PROCEDURE_CALL:
            return 0;

        default:
            #ifdef DEBUG_OPTIMIZER
                fprintf(stderr, "OPTIMIZER: Unsupported statement at line %d\n", stmt->line_num);
            #endif

            return 0;
    }
}

/* Removes the var assign if applicable */
int remove_mutation_var_assign(char *id, struct Statement *var_assign)
{
    assert(var_assign != NULL);
    assert(var_assign->type == STMT_VAR_ASSIGN);

    struct Expression *var;

    var = var_assign->stmt_data.var_assign_data.var;
    assert(var->type == EXPR_VAR_ID);
    if(strcmp(var->expr_data.id, id) == 0)
    {
        #ifdef DEBUG_OPTIMIZER
            fprintf(stderr, "OPTIMIZER: Removing var assign at line %d\n", var_assign->line_num);
        #endif

        destroy_stmt(var_assign);
        return 1;
    }

    return 0;
}

/* Removes all mutation statements from a list of statements */
int remove_mutation_compound_statement(char *id, struct Statement *body_statement)
{
    assert(body_statement != NULL);
    assert(body_statement->type == STMT_COMPOUND_STATEMENT);

    ListNode_t *statement_list, *prev, *temp;
    struct Statement *stmt;
    int return_val;

    statement_list = body_statement->stmt_data.compound_statement;
    prev = NULL;
    return_val = 0;
    while(statement_list != NULL)
    {
        stmt = (struct Statement *)statement_list->cur;
        if(remove_mutation_statement(id, stmt) == 1)
        {
            ++return_val;

            temp = statement_list->next;
            free(statement_list);
            statement_list = temp;

            if(prev == NULL)
            {
                body_statement->stmt_data.compound_statement = statement_list;
            }
            else
            {
                prev->next = statement_list;
            }
        }

        prev = statement_list;
        statement_list = statement_list->next;
    }

    return return_val;
}

/* Gets a list of variables that can be safely removed (not referenced) and ones that will need
        to be checked
*/
void set_vars_lists(SymTab_t *symtab, ListNode_t *vars, ListNode_t **vars_to_check,
    ListNode_t **vars_to_remove)
{
    ListNode_t *check, *remove;
    ListNode_t *ids;
    HashNode_t *node;
    Tree_t *var_decl;

    while(vars != NULL)
    {
        var_decl = (Tree_t *)vars->cur;
        if(var_decl->type != TREE_ARR_DECL)
        {
            assert(var_decl->type == TREE_VAR_DECL);
            ids = var_decl->tree_data.var_decl_data.ids;

            while(ids != NULL)
            {
                assert(FindIdent(&node, symtab, (char *)ids->cur) == 0);
                assert(node != NULL);
                if(node->referenced == 0 && node->hash_type != HASHTYPE_FUNCTION_RETURN)
                {
                    add_to_list(vars_to_remove, ids->cur);
                }
                else if(node->hash_type != HASHTYPE_FUNCTION_RETURN)
                {
                    add_to_list(vars_to_check, ids->cur);
                }

                ids = ids->next;
            }
        }

        vars = vars->next;
    }
}

/* Adds to a list */
void add_to_list(ListNode_t **list, void *obj)
{
    if(*list == NULL)
        *list = CreateListNode(obj, LIST_UNSPECIFIED);
    else
        *list = PushListNodeBack(*list, CreateListNode(obj, LIST_UNSPECIFIED));
}
