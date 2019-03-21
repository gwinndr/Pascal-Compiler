/* The Pascal grammar! */
/* If byacc panics from conflicts, look in y.output */

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../ErrVars.h"
    #include "../ParseTree/tree.h"
    #include "../ParseTree/tree_types.h"
    #include "../List/List.h"
    #include "IdFifo.h"
    #include "y.tab.h"

    /*extern FILE *yyin;*/
    extern int yylex();
%}

%union{
    /* Numbers */
    int i_val;
    float f_val;

    /* Operators */
    int op_val;

    /* Identifier */
    char *id;

    /* For Types */
    struct Type
    {
        int type;

        /* For arrays */
        int actual_type;
        int start;
        int end;
    } type_s;

    /* Tree pointers */
    Tree_t *tree;
    struct Statement *stmt;
    struct Expression *expr;

    /* FIFO List */
    ListNode_t *fifo_list;
}

/* Token keywords */
%token PROGRAM
%token VARIABLE
%token PROCEDURE
%token FUNCTION
%token BBEGIN
%token END

/* A NUM is the actual number, a type is the type declaration (ex: real) */
%token INT_NUM
%token REAL_NUM
%token INT_TYPE
%token REAL_TYPE

%token ID
%token ARRAY
%token SINGLE
%token OF
%token DOTDOT

%token IF
%token THEN
%token ELSE

%token WHILE
%token DO
%token NOT

%token FOR
%token TO

%token END_OF_FILE

/* Expression Tokens */
%token ASSIGNOP
%token RELOP
%token<op_val> EQ NE LT LE GT GE
%token ADDOP
%token<op_val> PLUS MINUS OR
%token MULOP
%token<op_val> STAR SLASH AND
%token PAREN

/* Easy fix for the dangling else (borrowed from "lex and yacc" [Levine et al.]) */
%nonassoc THEN
%nonassoc ELSE

/* TYPES FOR THE GRAMMAR */
%type<fifo_list> identifier_list
%type<fifo_list> declarations
%type<fifo_list> subprogram_declarations
%type<stmt> compound_statement

%type<type_s> type
%type<type_s> array_range
%type<i_val> array_end
%type<i_val> standard_type

%%

program
    : PROGRAM ID '(' identifier_list ')' ';'
     declarations
     subprogram_declarations
     compound_statement
     '.'
     END_OF_FILE
     {
         /*$$ = mk_program(yylval.id, $4, $7, $8, $9);*/
         parse_tree = mk_program((char *)id_fifo->cur, $4, $7, NULL, mk_compoundstatement(NULL));
         id_fifo = DeleteListNode(id_fifo, NULL);
         return -1;
     }
    ;

identifier_list
    : ID
        {
            $$ = CreateListNode((char *)id_fifo->cur, LIST_STRING);
            id_fifo = DeleteListNode(id_fifo, NULL);
        }
    | identifier_list ',' ID
        {
            $$ = PushListNodeBack($1, CreateListNode((char *)id_fifo->cur, LIST_STRING));
            id_fifo = DeleteListNode(id_fifo, NULL);
        }
    ;

declarations
    : declarations VARIABLE identifier_list ':' type ';'
        {
            Tree_t *tree;
            if($5.type == ARRAY)
                tree = mk_arraydecl($3, $5.actual_type, $5.start, $5.end);
            else
                tree = mk_vardecl($3, $5.actual_type);

            if($1 == NULL)
                $$ = CreateListNode(tree, LIST_TREE);
            else
                $$ = PushListNodeBack($1, CreateListNode(tree, LIST_TREE));
        }
    | /* empty */ {$$ = NULL;}
    ;

type
    : standard_type
        {
            $$.type = SINGLE;
            $$.actual_type = $1;
        }
    | ARRAY '[' array_range ']' OF standard_type
        {
            $$.type = ARRAY;
            $$.actual_type = $6;
            $$.start = $3.start;
            $$.end = $3.end;
        }

    ;

/* Duct tape for the union */
array_range
    : INT_NUM DOTDOT array_end
        {
            $$.start = yylval.i_val;
            $$.end = $3;
        }
    ;
array_end
    : INT_NUM {$$ = yylval.i_val;}
    ;

standard_type
    : INT_TYPE {$$ = INT_TYPE;}
    | REAL_TYPE {$$ = REAL_TYPE;}
    ;

subprogram_declarations
    : subprogram_declarations subprogram_declaration ';'
    | /* empty */
    ;

subprogram_declaration
    : subprogram_head
    declarations
    subprogram_declarations
    compound_statement
    ;

subprogram_head
    : FUNCTION ID arguments ':' standard_type ';'
    | PROCEDURE ID arguments ';'
    ;

arguments
    : '(' parameter_list ')'
    | /* empty */
    ;

parameter_list
    : identifier_list ':' type
    | parameter_list ';' identifier_list ':' type
    ;

compound_statement
    : BBEGIN optional_statements END
    ;

optional_statements
    : statement_list
    | /* empty */
    ;

statement_list
    : statement
    | statement_list ';' statement
    ;

statement
    : variable_assignment
    | procedure_statement
    | compound_statement
    | if_statement
    | WHILE relop_expression DO statement
    | FOR for_assign TO expression DO statement
    ;

/* Dangling else solution is in the token section */
if_statement
    : IF relop_expression THEN statement
    | IF relop_expression THEN statement ELSE statement

variable_assignment
    : variable ASSIGNOP expression

for_assign
    : variable_assignment
    | variable

variable
    : ID
    | ID '[' expression ']'
    ;

procedure_statement
    : ID
    | ID '(' expression_list ')'
    ;

/* RELATIONAL_EXPRESSIONS */

/* Not has the lowest precedence */
relop_expression
    : NOT relop_expression
    | relop_or

relop_or
    : relop_or OR relop_and
    | relop_and

relop_and
    : relop_and AND relop_paren
    | relop_paren

relop_paren
    : '(' relop_expression ')'
    | relop_expression_single

relop_expression_single
    : expression RELOP expression

/* END RELATIONAL_EXPRESSIONS */

expression_list
    : expression
    | expression_list ',' expression
    ;

expression
    : term
    | sign term
    | expression ADDOP term
    ;

term
    : factor
    | term MULOP factor
    ;

factor
    : ID
    | ID '[' expression ']'
    | ID '(' expression_list ')'
    | INT_NUM
    | REAL_NUM
    | '(' expression ')'
    ;

sign
    : '+'
    | '-'
    ;

%%

void yyerror(char *s)
{
    /*fprintf(stderr, "Error on line %d:%d\n", line_num, col_num);*/
    if(file_to_parse != NULL)
    {
        fprintf(stderr, "Error parsing %s:\n", file_to_parse);
        fprintf(stderr, "On line %d:\n", line_num);
    }
    else
        fprintf(stderr, "Error on line %d:\n", line_num);

    fprintf(stderr, "%s\n", s);
}
