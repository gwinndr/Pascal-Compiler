/*
    Damon Gwinn
    Parse tree for the Pascal Grammar
*/

#include "tree.h"
#include "tree_types.h"
#include "../LexAndYacc/y.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* NOTE: tree_print and destroy_tree implicitely call stmt and expr functions */
/* Tree printing */
void print_indent(FILE *f, int num_indent)
{
    int i;
    for(i=0; i<num_indent; ++i)
        fprintf(f, "  ");
}

void list_print(ListNode_t *list, FILE *f, int num_indent)
{
    ListNode_t *cur;

    cur = list;
    while(cur != NULL)
    {
        assert(cur->cur != NULL);
        switch(cur->type)
        {
            case LIST_TREE:
                tree_print((Tree_t *)cur->cur, f, num_indent);
                break;
            case LIST_EXPR:
                expr_print((struct Expression *)cur->cur, f, num_indent);
                break;
            case LIST_STMT:
                stmt_print((struct Statement *)cur->cur, f, num_indent);
                break;
            case LIST_STRING:
                print_indent(f, num_indent);
                fprintf(f, "%s\n", (char *)cur->cur);
                break;
            default:
                fprintf(stderr, "BAD TYPE IN list_print!\n");
                exit(1);
        }
        cur = cur->next;
    }
}

void tree_print(Tree_t *tree, FILE *f, int num_indent)
{
    print_indent(f, num_indent);

    switch(tree->type)
    {
        case TREE_PROGRAM_TYPE:
          assert(tree->tree_data.program_data.program_id != NULL);
          fprintf(f, "[PROGRAM:%s]\n", tree->tree_data.program_data.program_id);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[ARGS]:\n");
          list_print(tree->tree_data.program_data.args_char, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[VAR_DECLS]:\n");
          list_print(tree->tree_data.program_data.var_declaration, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[SUBPROGRAMS]:\n");
          list_print(tree->tree_data.program_data.subprograms, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[BODY]:\n");
          stmt_print(tree->tree_data.program_data.body_statement, f, num_indent+1);
          break;

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
                  fprintf(stderr, "BAD TYPE IN TREE_SUBPROGRAM!\n");
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

        default:
        fprintf(stderr, "BAD TYPE IN tree_print!\n");
        exit(1);
    }
}

void stmt_print(struct Statement *stmt, FILE *f, int num_indent)
{
    print_indent(f, num_indent);
    switch(stmt->type)
    {
        case STMT_VAR_ASSIGN:
          fprintf(f, "[VARASSIGN]:\n");
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[VAR]:\n");
          expr_print(stmt->stmt_data.var_assign_data.var, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[EXPR]:\n");
          expr_print(stmt->stmt_data.var_assign_data.expr, f, num_indent+1);
          break;

        case STMT_PROCEDURE_CALL:
          fprintf(f, "[PROCEDURE_CALL:%s]:\n", stmt->stmt_data.procedure_call_data.id);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[ARGS]:\n");
          list_print(stmt->stmt_data.procedure_call_data.expr_args, f, num_indent+1);
          break;

        case STMT_COMPOUND_STATEMENT:
          fprintf(f, "[COMPOUND_STMT]:\n");
          list_print(stmt->stmt_data.compound_statement, f, num_indent+1);
          break;

        case STMT_IF_THEN:
          fprintf(f, "[IF]:\n");
          expr_print(stmt->stmt_data.if_then_data.relop_expr, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[THEN]:\n");
          stmt_print(stmt->stmt_data.if_then_data.if_stmt, f, num_indent+1);

          if(stmt->stmt_data.if_then_data.else_stmt != NULL)
          {
              print_indent(f, num_indent);
              fprintf(f, "[ELSE]:\n");
              stmt_print(stmt->stmt_data.if_then_data.else_stmt, f, num_indent+1);
          }
          break;

        case STMT_WHILE:
          fprintf(f, "[WHILE]:\n");
          expr_print(stmt->stmt_data.while_data.relop_expr, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[DO]:\n");
          stmt_print(stmt->stmt_data.while_data.while_stmt, f, num_indent+1);
          break;

        case STMT_FOR:
          fprintf(f, "[FOR]:\n");
          switch(stmt->stmt_data.for_data.for_assign_type)
          {
              case STMT_FOR_VAR:
                expr_print(stmt->stmt_data.for_data.for_assign_data.var, f, num_indent+1);
                break;
              case STMT_FOR_ASSIGN_VAR:
                stmt_print(stmt->stmt_data.for_data.for_assign_data.var_assign, f, num_indent+1);
                break;
              default:
                fprintf(stderr, "BAD TYPE IN STMT_FOR!\n");
                exit(1);
          }

          print_indent(f, num_indent);
          fprintf(f, "[TO]:\n");
          expr_print(stmt->stmt_data.for_data.to, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[DO]:\n");
          stmt_print(stmt->stmt_data.for_data.do_for, f, num_indent+1);
          break;

          default:
            fprintf(stderr, "BAD TYPE IN stmt_print!\n");
            exit(1);
    }
}

void expr_print(struct Expression *expr, FILE *f, int num_indent)
{
    print_indent(f, num_indent);
    switch(expr->type)
    {
        case EXPR_RELOP:
          fprintf(f, "[RELOP:%d]:\n", expr->expr_data.relop_data.type);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[LEFT]:\n");
          expr_print(expr->expr_data.relop_data.left, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[RIGHT]:\n");
          expr_print(expr->expr_data.relop_data.right, f, num_indent+1);
          break;

        case EXPR_SIGN_TERM:
          fprintf(f, "[SIGN]:\n");
          expr_print(expr->expr_data.sign_term, f, num_indent+1);
          break;

        case EXPR_ADDOP:
          fprintf(f, "[ADDOP:%d]:\n", expr->expr_data.addop_data.addop_type);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[LEFT]:\n");
          expr_print(expr->expr_data.addop_data.left_expr, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[RIGHT]:\n");
          expr_print(expr->expr_data.addop_data.right_term, f, num_indent+1);
          break;

        case EXPR_MULOP:
          fprintf(f, "[MULOP:%d]:\n", expr->expr_data.mulop_data.mulop_type);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[LEFT]:\n");
          expr_print(expr->expr_data.mulop_data.left_term, f, num_indent+1);

          print_indent(f, num_indent);
          fprintf(f, "[RIGHT]:\n");
          expr_print(expr->expr_data.mulop_data.right_factor, f, num_indent+1);
          break;

        case EXPR_VAR_ID:
          fprintf(f, "[VAR_ID:%s]\n", expr->expr_data.id);
          break;

        case EXPR_ARRAY_ACCESS:
          fprintf(f, "[ARRAY_ACC:%s]\n", expr->expr_data.array_access_data.id);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[INDEX]:\n");
          expr_print(expr->expr_data.array_access_data.array_expr, f, num_indent+1);
          break;

        case EXPR_FUNCTION_CALL:
          fprintf(f, "[FUNC_CALL:%s]:\n", expr->expr_data.function_call_data.id);
          ++num_indent;

          print_indent(f, num_indent);
          fprintf(f, "[ARGS]:\n");
          list_print(expr->expr_data.function_call_data.args_expr, f, num_indent+1);
          break;

        case EXPR_INUM:
          fprintf(f, "[I_NUM:%d]\n", expr->expr_data.i_num);
          break;

        case EXPR_RNUM:
          fprintf(f, "[R_NUM:%f]\n", expr->expr_data.r_num);
          break;

        default:
          fprintf(stderr, "BAD TYPE IN expr_print!\n");
          exit(1);
    }
}

/* Tree freeing */
/* WARNING: Also frees all c strings and other such types */
void destroy_list(ListNode_t *list)
{
    ListNode_t *cur, *prev;
    if(list != NULL)
    {
        cur = list;
        while(cur != NULL)
        {
            assert(cur->cur != NULL);
            switch(cur->type)
            {
                case LIST_TREE:
                    destroy_tree((Tree_t *)cur->cur);
                    break;
                case LIST_STMT:
                    destroy_stmt((struct Statement *)cur->cur);
                    break;
                case LIST_EXPR:
                    destroy_expr((struct Expression *)cur->cur);
                    break;
                case LIST_STRING:
                    free((char *)cur->cur);
                    break;
                default:
                    fprintf(stderr, "BAD TYPE IN destroy_list [%d]!\n", cur->type);
                    exit(1);
            }
            prev = cur;
            cur = cur->next;
            free(prev);
        }
    }
}

void destroy_tree(Tree_t *tree)
{
    switch(tree->type)
    {
        case TREE_PROGRAM_TYPE:
          free(tree->tree_data.program_data.program_id);
          destroy_list(tree->tree_data.program_data.args_char);

          destroy_list(tree->tree_data.program_data.var_declaration);

          destroy_list(tree->tree_data.program_data.subprograms);

          destroy_stmt(tree->tree_data.program_data.body_statement);
          break;

        case TREE_SUBPROGRAM:
          free(tree->tree_data.subprogram_data.id);

          destroy_list(tree->tree_data.subprogram_data.args_var);

          destroy_list(tree->tree_data.subprogram_data.declarations);

          destroy_list(tree->tree_data.subprogram_data.subprograms);

          destroy_stmt(tree->tree_data.subprogram_data.statement_list);
          break;

        case TREE_VAR_DECL:
          destroy_list(tree->tree_data.var_decl_data.ids);
          break;

        case TREE_ARR_DECL:
          destroy_list(tree->tree_data.var_decl_data.ids);
          break;

        default:
          fprintf(stderr, "BAD TYPE IN destroy_tree!\n");
          exit(1);
    }
    free(tree);
}

void destroy_stmt(struct Statement *stmt)
{
    switch(stmt->type)
    {
        case STMT_VAR_ASSIGN:
          destroy_expr(stmt->stmt_data.var_assign_data.var);
          destroy_expr(stmt->stmt_data.var_assign_data.expr);
          break;

        case STMT_PROCEDURE_CALL:
          free(stmt->stmt_data.procedure_call_data.id);
          destroy_list(stmt->stmt_data.procedure_call_data.expr_args);
          break;

        case STMT_COMPOUND_STATEMENT:
          destroy_list(stmt->stmt_data.compound_statement);
          break;

        case STMT_IF_THEN:
          destroy_expr(stmt->stmt_data.if_then_data.relop_expr);
          destroy_stmt(stmt->stmt_data.if_then_data.if_stmt);

          if(stmt->stmt_data.if_then_data.else_stmt != NULL)
          {
              destroy_stmt(stmt->stmt_data.if_then_data.else_stmt);
          }
          break;

        case STMT_WHILE:
          destroy_expr(stmt->stmt_data.while_data.relop_expr);
          destroy_stmt(stmt->stmt_data.while_data.while_stmt);
          break;

        case STMT_FOR:
          switch(stmt->stmt_data.for_data.for_assign_type)
          {
              case STMT_FOR_VAR:
                destroy_expr(stmt->stmt_data.for_data.for_assign_data.var);
                break;
              case STMT_FOR_ASSIGN_VAR:
                destroy_stmt(stmt->stmt_data.for_data.for_assign_data.var_assign);
                break;
              default:
                fprintf(stderr, "BAD TYPE IN STMT_FOR (destroy_stmt)!\n");
                exit(1);
          }

          destroy_expr(stmt->stmt_data.for_data.to);
          destroy_stmt(stmt->stmt_data.for_data.do_for);
          break;

          default:
            fprintf(stderr, "BAD TYPE IN stmt_print!\n");
            exit(1);
    }
    free(stmt);
}

void destroy_expr(struct Expression *expr)
{
    switch(expr->type)
    {
        case EXPR_RELOP:
          destroy_expr(expr->expr_data.relop_data.left);
          destroy_expr(expr->expr_data.relop_data.right);
          break;

        case EXPR_SIGN_TERM:
          destroy_expr(expr->expr_data.sign_term);
          break;

        case EXPR_ADDOP:
          destroy_expr(expr->expr_data.addop_data.left_expr);
          destroy_expr(expr->expr_data.addop_data.right_term);
          break;

        case EXPR_MULOP:
          destroy_expr(expr->expr_data.mulop_data.left_term);
          destroy_expr(expr->expr_data.mulop_data.right_factor);
          break;

        case EXPR_VAR_ID:
          free(expr->expr_data.id);
          break;

        case EXPR_ARRAY_ACCESS:
          free(expr->expr_data.array_access_data.id);
          destroy_expr(expr->expr_data.array_access_data.array_expr);
          break;

        case EXPR_FUNCTION_CALL:
          free(expr->expr_data.function_call_data.id);
          destroy_list(expr->expr_data.function_call_data.args_expr);
          break;

        case EXPR_INUM:
          break;

        case EXPR_RNUM:
          break;

        default:
          fprintf(stderr, "BAD TYPE IN expr_print!\n");
          exit(1);
    }
    free(expr);
}

Tree_t *mk_program(int line_num, char *id, ListNode_t *args, ListNode_t *var_decl,
    ListNode_t *subprograms, struct Statement *compound_statement)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->line_num = line_num;
    new_tree->type = TREE_PROGRAM_TYPE;
    new_tree->tree_data.program_data.program_id = id;
    new_tree->tree_data.program_data.args_char = args;
    new_tree->tree_data.program_data.var_declaration = var_decl;
    new_tree->tree_data.program_data.subprograms = subprograms;
    new_tree->tree_data.program_data.body_statement = compound_statement;

    return new_tree;
}


Tree_t *mk_procedure(int line_num, char *id, ListNode_t *args, ListNode_t *var_decl,
    ListNode_t *subprograms, struct Statement *compound_statement)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->line_num = line_num;
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

Tree_t *mk_function(int line_num, char *id, ListNode_t *args, ListNode_t *var_decl,
    ListNode_t *subprograms, struct Statement *compound_statement, int return_type)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->line_num = line_num;
    new_tree->type = TREE_SUBPROGRAM;
    new_tree->tree_data.subprogram_data.sub_type = TREE_SUBPROGRAM_FUNC;
    new_tree->tree_data.subprogram_data.id = id;
    new_tree->tree_data.subprogram_data.args_var = args;
    new_tree->tree_data.subprogram_data.return_type = return_type;
    new_tree->tree_data.subprogram_data.declarations = var_decl;
    new_tree->tree_data.subprogram_data.subprograms = subprograms;
    new_tree->tree_data.subprogram_data.statement_list = compound_statement;

    return new_tree;
}

/*enum TreeType{TREE_PROGRAM_TYPE, TREE_SUBPROGRAM, TREE_VAR_DECL, TREE_STATEMENT_TYPE};*/

Tree_t *mk_vardecl(int line_num, ListNode_t *ids, int type)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->line_num = line_num;
    new_tree->type = TREE_VAR_DECL;
    new_tree->tree_data.var_decl_data.ids = ids;
    new_tree->tree_data.var_decl_data.type = type;

    return new_tree;
}

Tree_t *mk_arraydecl(int line_num, ListNode_t *ids, int type, int start, int end)
{
    Tree_t *new_tree;
    new_tree = (Tree_t *)malloc(sizeof(Tree_t));

    new_tree->line_num = line_num;
    new_tree->type = TREE_ARR_DECL;
    new_tree->tree_data.arr_decl_data.ids = ids;
    new_tree->tree_data.arr_decl_data.type = type;
    new_tree->tree_data.arr_decl_data.s_range = start;
    new_tree->tree_data.arr_decl_data.e_range = end;

    return new_tree;
}


/************** Statement routines **************/
struct Statement *mk_varassign(int line_num, struct Expression *var, struct Expression *expr)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->line_num = line_num;
    new_stmt->type = STMT_VAR_ASSIGN;
    new_stmt->stmt_data.var_assign_data.var = var;
    new_stmt->stmt_data.var_assign_data.expr = expr;

    return new_stmt;
}

struct Statement *mk_procedurecall(int line_num, char *id, ListNode_t *expr_args)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->line_num = line_num;
    new_stmt->type = STMT_PROCEDURE_CALL;
    new_stmt->stmt_data.procedure_call_data.id = id;
    new_stmt->stmt_data.procedure_call_data.expr_args = expr_args;

    return new_stmt;
}

struct Statement *mk_compoundstatement(int line_num, ListNode_t *compound_statement)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->line_num = line_num;
    new_stmt->type = STMT_COMPOUND_STATEMENT;
    new_stmt->stmt_data.compound_statement = compound_statement;

    return new_stmt;
}

struct Statement *mk_ifthen(int line_num, struct Expression *eval_relop, struct Statement *if_stmt,
                            struct Statement *else_stmt)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->line_num = line_num;
    new_stmt->type = STMT_IF_THEN;
    new_stmt->stmt_data.if_then_data.relop_expr = eval_relop;
    new_stmt->stmt_data.if_then_data.if_stmt = if_stmt;
    new_stmt->stmt_data.if_then_data.else_stmt = else_stmt;

    return new_stmt;
}

struct Statement *mk_while(int line_num, struct Expression *eval_relop,
                            struct Statement *while_stmt)
{
    struct Statement *new_stmt;
    new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

    new_stmt->line_num = line_num;
    new_stmt->type = STMT_WHILE;
    new_stmt->stmt_data.while_data.relop_expr = eval_relop;
    new_stmt->stmt_data.while_data.while_stmt = while_stmt;

    return new_stmt;
}

struct Statement *mk_forassign(int line_num, struct Statement *for_assign, struct Expression *to,
                               struct Statement *do_for)
{
   struct Statement *new_stmt;
   new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

   new_stmt->line_num = line_num;
   new_stmt->type = STMT_FOR;
   new_stmt->stmt_data.for_data.for_assign_type = STMT_FOR_ASSIGN_VAR;


   new_stmt->stmt_data.for_data.to = to;
   new_stmt->stmt_data.for_data.do_for = do_for;
   new_stmt->stmt_data.for_data.for_assign_data.var_assign = for_assign;

   return new_stmt;
}

struct Statement *mk_forvar(int line_num, struct Expression *for_var, struct Expression *to,
                              struct Statement *do_for)
{
  struct Statement *new_stmt;
  new_stmt = (struct Statement *)malloc(sizeof(struct Statement));

  new_stmt->line_num = line_num;
  new_stmt->type = STMT_FOR;
  new_stmt->stmt_data.for_data.for_assign_type = STMT_FOR_VAR;


  new_stmt->stmt_data.for_data.to = to;
  new_stmt->stmt_data.for_data.do_for = do_for;
  new_stmt->stmt_data.for_data.for_assign_data.var = for_var;

  return new_stmt;
}

/*********** Expression routines ***************/
struct Expression *mk_relop(int line_num, int type, struct Expression *left,
                                struct Expression *right)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_RELOP;
    new_expr->expr_data.relop_data.type = type;
    new_expr->expr_data.relop_data.left = left;
    new_expr->expr_data.relop_data.right = right;

    return new_expr;
}

struct Expression *mk_signterm(int line_num, struct Expression *sign_term)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_SIGN_TERM;
    new_expr->expr_data.sign_term = sign_term;

    return new_expr;
}

struct Expression *mk_addop(int line_num, int type, struct Expression *left,
                                struct Expression *right)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_ADDOP;
    new_expr->expr_data.addop_data.addop_type = type;
    new_expr->expr_data.addop_data.left_expr = left;
    new_expr->expr_data.addop_data.right_term = right;

    return new_expr;
}

struct Expression *mk_mulop(int line_num, int type, struct Expression *left,
                                struct Expression *right)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_MULOP;
    new_expr->expr_data.mulop_data.mulop_type = type;
    new_expr->expr_data.mulop_data.left_term = left;
    new_expr->expr_data.mulop_data.right_factor = right;

    return new_expr;
}

struct Expression *mk_varid(int line_num, char *id)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_VAR_ID;
    new_expr->expr_data.id = id;

    return new_expr;
}

struct Expression *mk_arrayaccess(int line_num, char *id, struct Expression *index_expr)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_ARRAY_ACCESS;
    new_expr->expr_data.array_access_data.id = id;
    new_expr->expr_data.array_access_data.array_expr = index_expr;

    return new_expr;
}

struct Expression *mk_functioncall(int line_num, char *id, ListNode_t *args)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_FUNCTION_CALL;
    new_expr->expr_data.function_call_data.id = id;
    new_expr->expr_data.function_call_data.args_expr = args;

    return new_expr;
}

struct Expression *mk_inum(int line_num, int i_num)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_INUM;
    new_expr->expr_data.i_num = i_num;

    return new_expr;
}

struct Expression *mk_rnum(int line_num, float r_num)
{
    struct Expression *new_expr;
    new_expr = (struct Expression *)malloc(sizeof(struct Expression));

    new_expr->line_num = line_num;
    new_expr->type = EXPR_RNUM;
    new_expr->expr_data.r_num = r_num;

    return new_expr;
}
