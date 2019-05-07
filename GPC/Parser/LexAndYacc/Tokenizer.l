%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../ErrVars.h"
    #include "../ParseTree/tree.h"
    #include "../ParseTree/tree_types.h"
    #include "../List/List.h"
    #include "y.tab.h"
%}

whitespace [ \t]+
integer  [0-9]+
real [0-9]*[.][0-9]+
id   [A-Za-z][A-Za-z0-9]*
comment [(][*][^*]*[*]+([^*)][^*]*[*]+)*[)]

%%

{whitespace} ;

{comment}|"//".*   { /* Well documented way to ignore comments */
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[COMMENT] ");
    #endif

    int i;
    for(i = 0; i < strlen(yytext); ++i)
    {
        if(yytext[i] == '\n')
            ++line_num;
    }
}

"\n" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "\n");
    #endif
    ++line_num;
}

<<EOF>> {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[EOF]\n");
    #endif
    return END_OF_FILE;
}

"program"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[PROGRAM] ");
    #endif
    return PROGRAM;
}

"procedure"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[PROCEDURE] ");
    #endif
    return PROCEDURE;
}

"function"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[FUNCTION] ");
    #endif
    return FUNCTION;
}

"begin"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[BEGIN] ");
    #endif
    return BBEGIN;
}

"end"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[END] ");
    #endif
    return END;
}

"var"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[VAR] ");
    #endif
    return VARIABLE;
}

"array" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ARRAY] ");
    #endif
    return ARRAY;
}

".." {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[DOTDOT] ");
    #endif
    return DOTDOT;
}

"of" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[OF] ");
    #endif
    return OF;
}

"if" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[IF] ");
    #endif
    return IF;
}

"then" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[THEN] ");
    #endif
    return THEN;
}

"else" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ELSE] ");
    #endif
    return ELSE;
}

"for" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[FOR] ");
    #endif
    return FOR;
}

"to" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[TO] ");
    #endif
    return TO;
}

"do" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[DO] ");
    #endif
    return DO;
}

"while" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[WHILE] ");
    #endif
    return WHILE;
}

":="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ASSIGNOP] ");
    #endif
    return ASSIGNOP;
}

">"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[RELOP:%s] ", yytext);
    #endif
    yylval.op_val = GT;
    return RELOP;
}

">="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[RELOP:%s] ", yytext);
    #endif
    yylval.op_val = GE;
    return RELOP;
}

"<"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[RELOP:%s] ", yytext);
    #endif
    yylval.op_val = LT;
    return RELOP;
}

"<="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[RELOP:%s] ", yytext);
    #endif
    yylval.op_val = LE;
    return RELOP;
}

"="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[RELOP:%s] ", yytext);
    #endif
    yylval.op_val = EQ;
    return RELOP;
}

"!="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[RELOP:%s] ", yytext);
    #endif
    yylval.op_val = NE;
    return RELOP;
}

"and"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[AND] ");
    #endif
    return AND;
}

"or"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[OR] ");
    #endif
    return OR;
}

"*" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[MULOP:%s] ", yytext);
    #endif
    yylval.op_val = STAR;
    return MULOP;
}

"div"|"/" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[MULOP:%s] ", yytext);
    #endif
    yylval.op_val = SLASH;
    return MULOP;
}

"+" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ADDOP:%s] ", yytext);
    #endif
    yylval.op_val = PLUS;
    return ADDOP;
}

"-" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ADDOP:%s] ", yytext);
    #endif
    yylval.op_val = MINUS;
    return ADDOP;
}

"integer" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[INT_TYPE] ");
    #endif
    return INT_TYPE;
}

"real" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[REAL_TYPE] ");
    #endif
    return REAL_TYPE;
}


{integer}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[INT_NUM:%d] ", atoi(yytext));
    #endif
    yylval.i_val = atoi(yytext);

    return INT_NUM;
}

{real}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[REAL_NUM:%f] ", atof(yytext));
    #endif
    yylval.f_val = atof(yytext);
    return REAL_NUM;
}

{id}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ID:%s] ", yytext);
    #endif
    yylval.id = strdup(yytext);
    return ID;
}

.   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "{%s} ", yytext);
    #endif
    return yytext[0];
}

%%
