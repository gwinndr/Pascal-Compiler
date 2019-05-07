Damon Gwinn
Gwinn Pascal Compiler

Compile by running 'make'

The exectutable is called 'gpc'
    gpc [INPUT_FILE] [OUTPUT_FILE] [OPTIONAL_FLAG_1] ...

FLAGS:
    -non-local: Enables the buggy non-local variable references from procedures

    -O1: Enables level-1 optimizations (simplifies expressions with constant numbers)

    -O2: Enables level-2 optimizations (removes unreferenced variables and their assignments)
