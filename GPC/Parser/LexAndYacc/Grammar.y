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
%token ASSIGNOP
%token NUM
%token ID
%token END_OF_FILE

%%
/* Test grammar for tokenizer */
grammar: VARIABLE
        | grammar END_OF_FILE {return 0;}
        |
        ;
%%

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}
