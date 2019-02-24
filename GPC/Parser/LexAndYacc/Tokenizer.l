%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
%}

whitespace [ \t]+
number  [0-9]+
id   [A-Za-z][A-Za-z0-9]*

%%

{whitespace} ;
"\n"  return END_OF_FILE;

<<EOF>> {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [EOF]\n");
    #endif
    return END_OF_FILE;
}

"program"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [PROGRAM]\n");
    #endif
    return PROGRAM;
}

"var"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [VAR]\n");
    #endif
    return VARIABLE;
}

"procedure"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [PROCEDURE]\n");
    #endif
    return PROCEDURE;
}

"function"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [FUNCTION]\n");
    #endif
    return FUNCTION;
}

"begin"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [BEGIN]\n");
    #endif
    return BBEGIN;
}

"end"   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [END]\n");
    #endif
    return END;
}


":="   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [ASSIGNOP]\n");
    #endif
    return ASSIGNOP;
}

{number}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [NUM]\n");
    #endif
    return NUM;
}

{id}   {
    #ifdef DEBUG_FLEX
        fprintf(stderr, "FLEX: [ID]\n");
    #endif
    return ID;
}

.   return yytext[0];

%%
