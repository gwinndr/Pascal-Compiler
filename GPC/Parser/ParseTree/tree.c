/*
    Damon Gwinn
    Parse tree for the Pascal Grammar
*/

#include "tree.h"
#include "tree_types.h"
#include "../LexAndYacc/y.tab.h"
#include <stdlib.h>
#include <stdio.h>

Tree_t *mk_program(char *id, TreeListNode_t *args, TreeListNode_t *var_decl,
    TreeListNode_t *subprograms, struct Statement *compound_statement)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->type = TREE_PROGRAM_TYPE;
    new_tree->tree_data.program_data.args_char = args;
    new_tree->tree_data.program_data.var_declaration = var_decl;
    new_tree->tree_data.program_data.subprograms = subprograms;
    new_tree->tree_data.program_data.body_statement = compound_statement;

    return new_tree;
}


Tree_t *mk_procedure(char *id, TreeListNode_t *args, TreeListNode_t *var_decl,
    TreeListNode_t *subprograms, struct Statement *compound_statement)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->type = TREE_SUBPROGRAM;
    new_tree->tree_data.subprogram_data.sub_type = TREE_SUBPROGRAM_PROC;
    new_tree->tree_data.subprogram_data.id = id;
    new_tree->tree_data.subprogram_data.args_var = args;
    new_tree->tree_data.subprogram_data.return_type = -1;
    new_tree->tree_data.subprogram_data.declarations = var_decl;
    new_tree->tree_data.subprogram_data.subprograms = subprograms;
    new_tree->tree_data.subprogram_data.statement_list = compound_statement;

    return new_tree;
}

/*enum TreeType{TREE_PROGRAM_TYPE, TREE_SUBPROGRAM, TREE_VAR_DECL, TREE_STATEMENT_TYPE};*/

Tree_t *mk_vardecl(char *id, int type)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->type = TREE_VAR_DECL;
    new_tree->tree_data.var_decl_data.id = id;
    new_tree->tree_data.var_decl_data.type = type;

    return new_tree;
}


/************** Statement routines **************/
struct Statement *mk_varassign(char *var, struct Expression *expr)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->type = STMT_VAR_ASSIGN;
    new_stmt->stmt_data.var_assign_data.var = var;
    new_stmt->stmt_data.var_assign_data.expr = expr;

    return new_stmt;
}

struct Statement *mk_procedurecall(char *id, TreeListNode_t *expr_args)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->type = STMT_PROCEDURE_CALL;
    new_stmt->stmt_data.procedure_call_data.id = id;
    new_stmt->stmt_data.procedure_call_data.expr_args = expr_args;

    return new_stmt;
}

struct Statement *mk_compoundstatement(TreeListNode_t *compound_statement)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->type = STMT_COMPOUND_STATEMENT;
    new_stmt->stmt_data.compound_statement = compound_statement;

    return new_stmt;
}

struct Statement *mk_ifthen(struct Expression *eval_relop, struct Statement *if_stmt,
                            struct Statement *else_stmt)
    {
        struct Statement *new_stmt;
        new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

        new_stmt->type = STMT_IF_THEN;
        new_stmt->stmt_data.if_then_data.relop_expr = eval_relop;
        new_stmt->stmt_data.if_then_data.if_stmt = if_stmt;
        new_stmt->stmt_data.if_then_data.else_stmt = else_stmt;

        return new_stmt;
    }

struct Statement *mk_while(struct Expression *eval_relop, struct Statement *while_stmt)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->type = STMT_WHILE;
    new_stmt->stmt_data.while_data.relop_expr = eval_relop;
    new_stmt->stmt_data.while_data.while_stmt = while_stmt;

    return new_stmt;
}

struct Statement *mk_forassign(struct Statement *for_assign, struct Expression *to,
                               struct Statement *do_for)
   {
       struct Statement *new_stmt;
       new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

       new_stmt->type = STMT_FOR;
       new_stmt->stmt_data.for_data.for_assign_type = STMT_FOR_ASSIGN_VAR;


       new_stmt->stmt_data.for_data.to = to;
       new_stmt->stmt_data.for_data.do_for = do_for;
       new_stmt->stmt_data.for_data.for_assign_data.var_assign = for_assign;

       return new_stmt;
   }

struct Statement *mk_forvar(struct Expression *for_var, struct Expression *to,
                              struct Statement *do_for)
  {
      struct Statement *new_stmt;
      new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

      new_stmt->type = STMT_FOR;
      new_stmt->stmt_data.for_data.for_assign_type = STMT_FOR_VAR;


      new_stmt->stmt_data.for_data.to = to;
      new_stmt->stmt_data.for_data.do_for = do_for;
      new_stmt->stmt_data.for_data.for_assign_data.var = for_var;

      return new_stmt;
  }

/*********** Expression routines ***************/
struct Expression *mk_relop(int type, struct Expression *left, struct Expression *right)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_RELOP;
    new_expr->expr_data.relop.type = type;
    new_expr->expr_data.relop.left = left;
    new_expr->expr_data.relop.right = right;

    return new_expr;
}

struct Expression *mk_signterm(struct Expression *sign_term)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_SIGN_TERM;
    new_expr->expr_data.sign_term = sign_term;

    return new_expr;
}

struct Expression *mk_addop(struct Expression *left, struct Expression *right)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_ADDOP;
    new_expr->expr_data.addop.left_expr = left;
    new_expr->expr_data.addop.right_term = right;

    return new_expr;
}

struct Expression *mk_mulop(struct Expression *left, struct Expression *right)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_MULOP;
    new_expr->expr_data.mulop.left_term = left;
    new_expr->expr_data.mulop.right_factor = right;

    return new_expr;
}

struct Expression *mk_varid(char *id)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_VAR_ID;
    new_expr->expr_data.id = id;

    return new_expr;
}

struct Expression *mk_arrayaccess(char *id, struct Expression *index_expr)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_ARRAY_ACCESS;
    new_expr->expr_data.array_access.id = id;
    new_expr->expr_data.array_access.array_expr = index_expr;

    return new_expr;
}

struct Expression *mk_functioncall(char *id, TreeListNode_t *args)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_FUNCTION_CALL;
    new_expr->expr_data.function_call.id = id;
    new_expr->expr_data.function_call.args_expr = args;

    return new_expr;
}

struct Expression *mk_inum(int i_num)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_INUM;
    new_expr->expr_data.i_num = i_num;

    return new_expr;
}

struct Expression *mk_rnum(float r_num)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->type = EXPR_RNUM;
    new_expr->expr_data.r_num = r_num;

    return new_expr;
}
