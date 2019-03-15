/* Damon Gwinn */
/* Where it all begins */

#include <stdio.h>
#include <stdlib.h>
#include "Parser/ParsePascal.h"

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        ParsePascal(argv[1]);
    }
    else
    {
        ParsePascal(NULL);
    }
}
