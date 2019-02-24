%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
%}

whitespace [ \t]+
number  [0-9]+
id   [A-Za-z][A-Za-z0-9]*
comment [(][*][^*]*[*]+([^*)][^*]*[*]+)*[)]

%%

{whitespace} ;

{comment}   { /* Well documented way to ignore comments */
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[COMMENT] ");
    #endif
}

"\n" {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "\n");
    #endif
    /*return 0;*/
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

"var"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[VAR] ");
    #endif
    return VARIABLE;
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


":="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ASSIGNOP] ");
    #endif
    return ASSIGNOP;
}

{number}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[NUM:%d] ", atoi(yytext));
    #endif
    return NUM;
}

{id}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "[ID:%s] ", yytext);
    #endif
    return ID;
}

.   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "{%s} ", yytext);
    #endif
    return yytext[0];
}

%%
