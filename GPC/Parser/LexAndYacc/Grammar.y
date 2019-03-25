/* The Pascal grammar! */
/* If byacc panics from conflicts, look in y.output */

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../ErrVars.h"
    #include "../ParseTree/tree.h"
    #include "../ParseTree/tree_types.h"
    #include "../List/List.h"
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

    /* For Subprogram Headers */
    struct SubprogramHead
    {
        int sub_type;

        char *id;
        ListNode_t *args;
        int return_type; /* -1 if procedure */
    } subprogram_head_s;

    /* For the for_assign rule */
    struct ForAssign
    {
        int assign_type;
        union for_assign_bison
        {
            struct Statement *stmt;
            struct Expression *expr;
        } for_assign_bison_union;
    } for_assign_bison;

    /* Tree pointers */
    Tree_t *tree;
    struct Statement *stmt;
    struct Expression *expr;

    /* List */
    ListNode_t *list;
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

/* Extra tokens, DO NOT USE IN GRAMMAR RULES! */
%token VAR_ASSIGN;
%token VAR;

/* Easy fix for the dangling else (borrowed from "lex and yacc" [Levine et al.]) */
%nonassoc THEN
%nonassoc ELSE

/* TYPES FOR THE GRAMMAR */
%type<list> identifier_list
%type<list> declarations
%type<list> subprogram_declarations
%type<stmt> compound_statement

%type<type_s> type
%type<type_s> array_range
%type<i_val> standard_type

%type<tree> subprogram_declaration
%type<subprogram_head_s> subprogram_head
%type<list> arguments
%type<list> parameter_list

%type<list> optional_statements
%type<list> statement_list
%type<stmt> statement
%type<stmt> variable_assignment
%type<stmt> procedure_statement
%type<stmt> if_statement
%type<for_assign_bison> for_assign

%type<expr> variable;
%type<expr> relop_expression
%type<expr> relop_and
%type<expr> relop_not
%type<expr> relop_paren
%type<expr> relop_expression_single

%type<list> expression_list
%type<expr> expression
%type<expr> term
%type<expr> factor
%type<op_val> sign

/* Rules to extract union values */
%type<id> ident
%type<i_val> int_num
%type<f_val> real_num
%type<op_val> relop
%type<op_val> addop
%type<op_val> mulop

%%

program
    : PROGRAM ident '(' identifier_list ')' ';'
     declarations
     subprogram_declarations
     compound_statement
     '.'
     END_OF_FILE
     {
         parse_tree = mk_program($2, $4, $7, $8, $9);
         return -1;
     }
    ;

ident
    : ID { $$ = yylval.id; }
    ;

int_num
    : INT_NUM {$$ = yylval.i_val;}
    ;

real_num
    : REAL_NUM {$$ = yylval.f_val;}
    ;

relop
    : RELOP { $$ = yylval.op_val;}
    ;

addop
    : ADDOP {$$ = yylval.op_val;}
    ;

mulop
    : MULOP {$$ = yylval.op_val;}

identifier_list
    : ident
        {
            $$ = CreateListNode($1, LIST_STRING);
        }
    | identifier_list ',' ident
        {
            $$ = PushListNodeBack($1, CreateListNode($3, LIST_STRING));
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
    | ARRAY '[' int_num DOTDOT int_num ']' OF standard_type
        {
            $$.type = ARRAY;
            $$.actual_type = $8;
            $$.start = $3;
            $$.end = $5;
        }

    ;

standard_type
    : INT_TYPE {$$ = INT_TYPE;}
    | REAL_TYPE {$$ = REAL_TYPE;}
    ;

subprogram_declarations
    : subprogram_declarations subprogram_declaration ';'
        {
            if($1 == NULL)
                $$ = CreateListNode($2, LIST_TREE);
            else
                $$ = PushListNodeBack($1, CreateListNode($2, LIST_TREE));
        }
    | /* empty */ {$$ = NULL;}
    ;

subprogram_declaration
    : subprogram_head
    declarations
    subprogram_declarations
    compound_statement
        {
            if($1.sub_type == PROCEDURE)
                $$ = mk_procedure($1.id, $1.args, $2, $3, $4);
            else
                $$ = mk_function($1.id, $1.args, $2, $3, $4, $1.return_type);
        }
    ;

subprogram_head
    : FUNCTION ident arguments ':' standard_type ';'
        {
            $$.sub_type = FUNCTION;
            $$.args = $3;
            $$.return_type = $5;

            $$.id = $2;
        }
    | PROCEDURE ident arguments ';'
        {
            $$.sub_type = PROCEDURE;
            $$.args = $3;
            $$.return_type = -1;

            $$.id = $2;
        }
    ;

arguments
    : '(' parameter_list ')' {$$ = $2;}
    | /* empty */ {$$ = NULL;}
    ;

parameter_list
    : identifier_list ':' type
        {
            Tree_t *tree;
            if($3.type == ARRAY)
                tree = mk_arraydecl($1, $3.actual_type, $3.start, $3.end);
            else
                tree = mk_vardecl($1, $3.actual_type);

            $$ = CreateListNode(tree, LIST_TREE);
        }
    | parameter_list ';' identifier_list ':' type
        {
            Tree_t *tree;
            if($5.type == ARRAY)
                tree = mk_arraydecl($3, $5.actual_type, $5.start, $5.end);
            else
                tree = mk_vardecl($3, $5.actual_type);

            $$ = PushListNodeBack($1, CreateListNode(tree, LIST_TREE));
        }
    ;

compound_statement
    : BBEGIN optional_statements END
        {
            $$ = mk_compoundstatement($2);
        }
    ;

optional_statements
    : statement_list {$$ = $1;}
    | /* empty */ {$$ = NULL;}
    ;

statement_list
    : statement
        {
            $$ = CreateListNode($1, LIST_STMT);
        }
    | statement_list ';' statement
        {
            $$ = PushListNodeBack($1, CreateListNode($3, LIST_STMT));
        }
    ;

statement
    : variable_assignment
        {
            $$ = $1;
        }
    | procedure_statement
        {
            $$ = $1;
        }
    | compound_statement
        {
            $$ = $1;
        }
    | if_statement
        {
            $$ = $1;
        }
    | WHILE relop_expression DO statement
        {
            $$ = mk_while($2, $4);
        }
    | FOR for_assign TO expression DO statement
        {
            if($2.assign_type == VAR_ASSIGN)
                $$ = mk_forassign($2.for_assign_bison_union.stmt, $4, $6);
            else
                $$ = mk_forvar($2.for_assign_bison_union.expr, $4, $6);
        }
    ;

/* Dangling else solution is in the token section */
if_statement
    : IF relop_expression THEN statement
        {
            $$ = mk_ifthen($2, $4, NULL);
        }
    | IF relop_expression THEN statement ELSE statement
        {
            $$ = mk_ifthen($2, $4, $6);
        }

variable_assignment
    : variable ASSIGNOP expression
        {
            $$ = mk_varassign($1, $3);
        }

for_assign
    : variable_assignment
        {
            $$.assign_type = VAR_ASSIGN;
            $$.for_assign_bison_union.stmt = $1;
        }
    | variable
        {
            $$.assign_type = VAR;
            $$.for_assign_bison_union.expr = $1;
        }

variable
    : ident
        {
            $$ = mk_varid($1);
        }
    | ident '[' expression ']'
        {
            $$ = mk_arrayaccess($1, $3);
        }
    ;

procedure_statement
    : ident
        {
            $$ = mk_procedurecall($1, NULL);
        }
    | ident '(' expression_list ')'
        {
            $$ = mk_procedurecall($1, $3);
        }
    ;

/* RELATIONAL_EXPRESSIONS */

relop_expression
    : relop_expression OR relop_and
        {
            $$ = mk_relop(OR, $1, $3);
        }
    | relop_and {$$ = $1;}
    ;

relop_and
    : relop_and AND relop_paren
        {
            $$ = mk_relop(AND, $1, $3);
        }
    | relop_paren {$$ = $1;}
    ;

relop_not
    : NOT relop_not
        {
            $$ = mk_relop(NOT, $2, NULL);
        }
    | relop_paren {$$ = $1;}
    ;

relop_paren
    : '(' relop_expression ')' {$$ = $2;}
    | relop_expression_single {$$ = $1;}
    ;

relop_expression_single
    : expression relop expression
        {
            $$ = mk_relop($2, $1, $3);
        }
    ;

/* END RELATIONAL_EXPRESSIONS */

expression_list
    : expression
        {
            $$ = CreateListNode($1, LIST_EXPR);
        }
    | expression_list ',' expression
        {
            $$ = PushListNodeBack($1, CreateListNode($3, LIST_EXPR));
        }
    ;

expression
    : term {$$ = $1;}
    | sign term
        {
            $$ = mk_signterm($2);
        }
    | expression addop term
        {
            $$ = mk_addop($2, $1, $3);
        }
    ;

term
    : factor {$$ = $1;}
    | term mulop factor
        {
            $$ = mk_mulop($2, $1, $3);
        }
    ;

factor
    : ident
        {
            $$ = mk_varid($1);
        }
    | ident '[' expression ']'
        {
            $$ = mk_arrayaccess($1, $3);
        }
    | ident '(' expression_list ')'
        {
            $$ = mk_functioncall($1, $3);
        }
    | int_num
        {
            $$ = mk_inum($1);
        }
    | real_num
        {
            $$ = mk_rnum($1);
        }
    | '(' expression ')'
        {
            $$ = $2;
        }
    ;

sign
    : '-'
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
