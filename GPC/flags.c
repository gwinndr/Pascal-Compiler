/*
    Damon Gwinn
    Flags for turning on different compiler functionalities
*/

#include "flags.h"

/* Flag for turning on non-local variable chasing */
/* Set with '-non-local' */
/* WARNING: Currently buggy */
int FLAG_NON_LOCAL_CHASING = 0;

/* Flag for turning on optimizations */
/* Set with -optimize */
int FLAG_OPTIMIZE = 0;

void set_nonlocal_flag()
{
    FLAG_NON_LOCAL_CHASING = 1;
}
void set_optimize_flag()
{
    FLAG_OPTIMIZE = 1;
}

int nonlocal_flag()
{
    return FLAG_NON_LOCAL_CHASING;
}
int optimize_flag()
{
    return FLAG_OPTIMIZE;
}
