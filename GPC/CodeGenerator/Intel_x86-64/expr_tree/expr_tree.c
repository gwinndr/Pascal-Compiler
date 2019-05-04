/*
    Damon Gwinn
    Tree of simple expressions for the gencode algorithm
    TODO: Does not handle function calls or arrays
    TODO: Does not handle real numbers
    TODO: Does not handle panic case (not enough registers)
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "expr_tree.h"
#include "../register_types.h"
#include "../codegen.h"
#include "../stackmng/stackmng.h"
#include "../../../Parser/List/List.h"
#include "../../../Parser/ParseTree/tree.h"
#include "../../../Parser/ParseTree/tree_types.h"
#include "../../../Parser/LexAndYacc/y.tab.h"

/* Helper functions */
ListNode_t *gencode_sign_term(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list);
ListNode_t *gencode_case0(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list);
ListNode_t *gencode_case1(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list);
ListNode_t *gencode_case2(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list);
ListNode_t *gencode_case3(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list);
ListNode_t *gencode_leaf_var(struct Expression *, ListNode_t *, char *, int );
ListNode_t *gencode_op(struct Expression *expr, char *left, char *right,
    ListNode_t *inst_list);
ListNode_t *gencode_op_deprecated(struct Expression *expr, ListNode_t *inst_list,
    char *buffer, int buf_len);

ListNode_t *gencode_divide_const_no_optimize(char *left, char *right, ListNode_t *inst_list);
ListNode_t *gencode_divide_no_const(char *left, char *right, ListNode_t *inst_list);

/* Builds an expression tree out of an expression */
/* WARNING: Does not make deep copy of expression */
/* WARNING: Does not do relational expressions */
expr_node_t *build_expr_tree(struct Expression *expr)
{
    assert(expr != NULL);

    expr_node_t *new_node;

    new_node = (expr_node_t *)malloc(sizeof(expr_node_t));
    new_node->expr = expr;

    /* Building the tree */
    switch(expr->type)
    {
        case EXPR_ADDOP:
            new_node->left_expr = build_expr_tree(expr->expr_data.addop_data.left_expr);
            new_node->right_expr = build_expr_tree(expr->expr_data.addop_data.right_term);
            break;
        case EXPR_MULOP:
            new_node->left_expr = build_expr_tree(expr->expr_data.mulop_data.left_term);
            new_node->right_expr = build_expr_tree(expr->expr_data.mulop_data.right_factor);
            break;

        case EXPR_SIGN_TERM:
            new_node->left_expr = build_expr_tree(expr->expr_data.sign_term);
            new_node->right_expr = NULL;
            break;

        case EXPR_VAR_ID:
        case EXPR_INUM:
            new_node->left_expr = NULL;
            new_node->right_expr = NULL;
            break;

        case EXPR_RELOP:
            new_node->left_expr = build_expr_tree(expr->expr_data.relop_data.left);
            assert(expr->expr_data.relop_data.right != NULL);
            new_node->right_expr = build_expr_tree(expr->expr_data.relop_data.right);
            break;


        default:
            fprintf(stderr, "ERROR: Unsupported expr_tree type: %d\n", expr->type);
            exit(1);
    }

    /* Setting the labels */
    if(new_node->left_expr != NULL)
    {
        if (expr_tree_is_leaf(new_node->left_expr) == 1)
        {
            new_node->left_expr->label = 1;
        }
    }

    if(new_node->left_expr == NULL && new_node->right_expr == NULL)
    {
        new_node->label = 0;
    }
    else if(new_node->left_expr == NULL)
    {
        new_node->label = new_node->right_expr->label;
    }
    else if(new_node->right_expr == NULL)
    {
        new_node->label = new_node->left_expr->label;
    }
    else if(new_node->left_expr->label > new_node->right_expr->label)
    {
        new_node->label = new_node->left_expr->label;
    }
    else if(new_node->left_expr->label < new_node->right_expr->label)
    {
        new_node->label = new_node->right_expr->label;
    }
    else /* (new_node->left_expr->label == new_node->right_expr->label) */
    {
        new_node->label = new_node->left_expr->label + 1;
    }

    return new_node;
}

/* The famous gencode algorithm */
ListNode_t *gencode_expr_tree(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list)
{
    assert(node != NULL);
    assert(node->expr != NULL);

    if(node->label > get_num_registers(reg_stack))
    {
        fprintf(stderr, "ERROR: codegen more complex than number of registers is unsupported!\n");
        exit(1);
    }

    /* Handle special cases first */
    if(node->expr->type == EXPR_SIGN_TERM)
    {
        inst_list = gencode_sign_term(node, reg_stack, inst_list);
    }

    /* CASE 0 */
    else if(expr_tree_is_leaf(node) == 1)
    {
        inst_list = gencode_case0(node, reg_stack, inst_list);
    }
    /* CASE 1 */
    else if(expr_tree_is_leaf(node->right_expr))
    {
        inst_list = gencode_case1(node, reg_stack, inst_list);
    }
    /* CASE 2 */
    else if(node->left_expr->label < node->right_expr->label)
    {
        inst_list = gencode_case2(node, reg_stack, inst_list);
    }
    /* CASE 3 */
    else if(node->left_expr->label >= node->right_expr->label)
    {
        inst_list = gencode_case3(node, reg_stack, inst_list);
    }
    else
    {
        fprintf(stderr, "ERROR: Unsupported case in codegen!\n");
        exit(1);
    }

    return inst_list;
}

/* Checks if node is a leaf */
int expr_tree_is_leaf(expr_node_t *node)
{
    assert(node != NULL);

    if(node->left_expr == NULL && node->right_expr == NULL)
        return 1;

    return 0;
}

/* Prints an expression tree */
void print_expr_tree(expr_node_t *node, int num_indent, FILE *f)
{
    assert(node != NULL);
    assert(node->expr != NULL);
    int i;

    for(i=0; i < num_indent; ++i)
        fprintf(f, "  ");

    fprintf(f, "[NODE:%d, L:%d]\n", node->expr->type, node->label);
    if(node->left_expr != NULL)
    {
        for(i=0; i < num_indent; ++i)
            fprintf(f, "  ");

        fprintf(f, "[LEFT]\n");
        print_expr_tree(node->left_expr, num_indent+1, f);
    }
    if(node->right_expr != NULL)
    {
        for(i=0; i < num_indent; ++i)
            fprintf(f, "  ");

        fprintf(f, "[RIGHT]\n");
        print_expr_tree(node->right_expr, num_indent+1, f);
    }
}

/* Frees an allocated expression tree */
void free_expr_tree(expr_node_t *node)
{
    if(node != NULL)
    {
        free_expr_tree(node->left_expr);
        free_expr_tree(node->right_expr);
        free(node);
    }
}

/* Special case for a sign term */
ListNode_t *gencode_sign_term(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list)
{
    assert(node != NULL);
    assert(node->expr != NULL);
    assert(node->expr->type == EXPR_SIGN_TERM);

    char buffer[50];
    Register_t *reg;

    inst_list = gencode_expr_tree(node->left_expr, reg_stack, inst_list);
    reg = front_reg_stack(reg_stack);

    snprintf(buffer, 50, "\tnegl\t%s\n", reg->bit_32);
    inst_list = add_inst(inst_list, buffer);

    return inst_list;
}

/* node is a leaf */
ListNode_t *gencode_case0(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list)
{
    assert(node != NULL);
    assert(node->expr != NULL);

    char buffer[50];
    char buf_leaf[30];
    struct Expression *expr;
    Register_t *reg;

    expr = node->expr;
    reg = front_reg_stack(reg_stack);
    assert(reg != NULL);

    inst_list = gencode_leaf_var(expr, inst_list, buf_leaf, 30);

    snprintf(buffer, 50, "\tmovl\t%s, %s\n", buf_leaf, reg->bit_32);

    return add_inst(inst_list, buffer);
}

/* right node is a leaf */
ListNode_t *gencode_case1(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list)
{
    assert(node != NULL);
    assert(node->expr != NULL);
    assert(node->right_expr != NULL);
    assert(node->right_expr->expr != NULL);

    char buffer[50];
    char name_buf[30];
    struct Expression *expr, *right_expr;
    Register_t *reg;

    inst_list = gencode_expr_tree(node->left_expr, reg_stack, inst_list);

    expr = node->expr;
    right_expr = node->right_expr->expr;
    assert(right_expr != NULL);
    gencode_leaf_var(right_expr, inst_list, name_buf, 30);
    reg = front_reg_stack(reg_stack);

    inst_list = gencode_op(expr, name_buf, reg->bit_32, inst_list);

    return inst_list;
}


ListNode_t *gencode_case2(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list)
{
    assert(node != NULL);
    assert(node->expr != NULL);

    char buffer[50];
    char op_buf[30];
    Register_t *reg1, *reg2;

    swap_reg_stack(reg_stack);
    gencode_expr_tree(node->right_expr, reg_stack, inst_list);
    reg1 = pop_reg_stack(reg_stack);
    gencode_expr_tree(node->left_expr, reg_stack, inst_list);

    reg2 = front_reg_stack(reg_stack);
    inst_list = gencode_op(node->expr, reg1->bit_32, reg2->bit_32, inst_list);

    push_reg_stack(reg_stack, reg1);
    swap_reg_stack(reg_stack);

    return inst_list;
}

ListNode_t *gencode_case3(expr_node_t *node, RegStack_t *reg_stack, ListNode_t *inst_list)
{
    assert(node != NULL);
    assert(node->expr != NULL);

    char buffer[50];
    char op_buf[30];
    Register_t *reg1, *reg2;

    gencode_expr_tree(node->left_expr, reg_stack, inst_list);
    reg1 = pop_reg_stack(reg_stack);
    gencode_expr_tree(node->right_expr, reg_stack, inst_list);

    reg2 = front_reg_stack(reg_stack);
    inst_list = gencode_op(node->expr, reg2->bit_32, reg1->bit_32, inst_list);

    push_reg_stack(reg_stack, reg1);

    return inst_list;
}

/* Returns the corresponding string and instructions for a leaf */
/* TODO: Only supports var_id and i_num */
ListNode_t *gencode_leaf_var(struct Expression *expr, ListNode_t *inst_list,
    char *buffer, int buf_len)
{
    assert(expr != NULL);

    StackNode_t *stack_node;

    switch(expr->type)
    {
        case EXPR_VAR_ID:
            stack_node = find_label(expr->expr_data.id);
            assert(stack_node != NULL);
            snprintf(buffer, buf_len, "-%d(%%rbp)", stack_node->offset);
            break;

        case EXPR_INUM:
            snprintf(buffer, buf_len, "$%d", expr->expr_data.i_num);
            break;

        default:
            fprintf(stderr, "ERROR: Unsupported expr type in gencode!\n");
            exit(1);
    }

    return inst_list;
}

/* TODO: Assumes eax and edx registers are free for division */
ListNode_t *gencode_op(struct Expression *expr, char *left, char *right,
    ListNode_t *inst_list)
{
    assert(expr != NULL);
    assert(left != NULL);
    assert(right != NULL);

    int type;
    char buffer[50];

    switch(expr->type)
    {
        case EXPR_ADDOP:
            type = expr->expr_data.addop_data.addop_type;
            if(type == PLUS)
            {
                snprintf(buffer, 50, "\taddl\t%s, %s\n", left, right);
                inst_list = add_inst(inst_list, buffer);
            }
            else if(type == MINUS)
            {
                snprintf(buffer, 50, "\tsubl\t%s, %s\n", left, right);
                inst_list = add_inst(inst_list, buffer);
            }
            else
            {
                fprintf(stderr, "ERROR: Bad addop type!\n");
                exit(1);
            }

            break;

        case EXPR_MULOP:
            type = expr->expr_data.mulop_data.mulop_type;
            if(type == STAR)
            {
                snprintf(buffer, 50, "\timull\t%s, %s\n", left, right);
                inst_list = add_inst(inst_list, buffer);
            }
            /* NOTE: Division and modulus is a more special case */
            else if(type == SLASH)
            {
                /* Constant divisor */
                /* TODO: Optimize */
                if(left[0] == '$')
                {
                    inst_list = gencode_divide_const_no_optimize(left, right, inst_list);
                }
                /* Non-constant divisor */
                else
                {
                    inst_list = gencode_divide_no_const(left, right, inst_list);
                }
            }
            else
            {
                fprintf(stderr, "ERROR: Bad mulop type!\n");
                exit(1);
            }

        case EXPR_RELOP:
            snprintf(buffer, 50, "\tcmpl\t%s, %s\n", left, right);
            inst_list = add_inst(inst_list, buffer);

            break;

        default:
            fprintf(stderr, "ERROR: Unsupported expr type in gencode!\n");
            exit(1);
    }

    return inst_list;
}

/* Gencode for division with constant divisor (no optimization) */
/* Throws constant divisor into temporary stack (TODO: This is bad) */
ListNode_t *gencode_divide_const_no_optimize(char *left, char *right, ListNode_t *inst_list)
{
    StackNode_t *temp;
    char buffer[50];

    temp = find_in_temp("TEMP_DIV");
    if(temp == NULL)
        temp = add_l_t("TEMP_DIV");

    snprintf(buffer, 50, "\tmovl\t%s, %%eax\n", right);
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tmovl\t%s, -%d(%%rbp)\n", left, temp->offset);
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tcltd\n");
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tidivl\t-%d(%%rbp)\n", temp->offset);
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tmovl\t%%eax, %s\n", right);
    inst_list = add_inst(inst_list, buffer);

    return inst_list;
}

/* Gencode for division with non-constant divisor */
ListNode_t *gencode_divide_no_const(char *left, char *right, ListNode_t *inst_list)
{
    char buffer[50];

    snprintf(buffer, 50, "\tmovl\t%s, %%eax\n", right);
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tcltd\n");
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tidivl\t%s\n", left);
    inst_list = add_inst(inst_list, buffer);

    snprintf(buffer, 50, "\tmovl\t%%eax, %s\n", right);
    inst_list = add_inst(inst_list, buffer);

    return inst_list;
}

/* Gets simple operation of a node */
/* DEPRECATED */
ListNode_t *gencode_op_deprecated(struct Expression *expr, ListNode_t *inst_list,
    char *buffer, int buf_len)
{
    assert(expr != NULL);
    int type;

    switch(expr->type)
    {
        case EXPR_ADDOP:
            type = expr->expr_data.addop_data.addop_type;
            if(type == PLUS)
                snprintf(buffer, buf_len, "addl");
            else if(type == MINUS)
                snprintf(buffer, buf_len, "subl");
            else
            {
                fprintf(stderr, "ERROR: Bad addop type!\n");
                exit(1);
            }

            break;

        case EXPR_MULOP:
            type = expr->expr_data.addop_data.addop_type;
            if(type == STAR)
                snprintf(buffer, buf_len, "imull");
            else
            {
                fprintf(stderr, "ERROR: Bad mulop type!\n");
                exit(1);
            }

            break;

        default:
            fprintf(stderr, "ERROR: Unsupported expr type in gencode!\n");
            exit(1);
    }

    return inst_list;
}
