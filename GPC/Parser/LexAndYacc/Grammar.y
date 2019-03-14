/* The Pascal grammar! */
/* If byacc panics from conflicts, look in y.output */

%{
    #include <stdio.h>
    #include <stdlib.h>
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
%token<opval> EQ NE LT LE GT GE
%token ADDOP
%token<opval> PLUS MINUS OR
%token MULOP
%token<opval> STAR SLASH AND

/* Easy fix for the dangling else (borrowed from "lex and yacc" [Levine et al.]) */
%nonassoc THEN
%nonassoc ELSE

/* TYPES FOR THE GRAMMAR */
%type<i_val> program

%%

pascal_file :
    pascal_file program
        {
            if($2 == END_OF_FILE)
            {
                return -1;
            }
        }
    | program

program
    : PROGRAM ID '(' identifier_list ')' ';'
     declarations
     subprogram_declarations
     compound_statement
     '.' {$$ = 0;}
    | END_OF_FILE {$$ = END_OF_FILE;}
    ;

identifier_list
    : ID
    | identifier_list ',' ID
    ;

declarations
    : declarations VARIABLE identifier_list ':' type ';'
    | /* empty */
    ;

type
    : standard_type
    | ARRAY '[' INT_NUM DOTDOT INT_NUM ']' OF standard_type
    ;

standard_type
    : INT_TYPE
    | REAL_TYPE
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
    | FOR for_assign TO expression DO compound_statement
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
    fprintf(stderr, "%s\n", s);
}
