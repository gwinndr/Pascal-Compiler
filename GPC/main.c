/* Damon Gwinn */
/* Where it all begins */

#include <stdio.h>
#include <stdlib.h>
#include "Parser/ParseTree/tree.h"
#include "Parser/ParsePascal.h"
#include "CodeGenerator/Intel_x86-64/codegen.h"

int main(int argc, char **argv)
{
    Tree_t * parse_tree;

    if(argc != 3)
    {
        fprintf(stderr, "USAGE: [exec] [INPUT_PASCAL_FILE] [OUTPUT_ASSEMBLY_FILE]\n");
        exit(1);
    }

    parse_tree = ParsePascal(argv[1]);
    if(parse_tree != NULL)
    {
        fprintf(stderr, "Generating code to file: %s\n", argv[2]);
        codegen(parse_tree, argv[1], argv[2]);

        destroy_tree(parse_tree);
        parse_tree = NULL;
    }
}
