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
/* Set with -O1 and -O2 */
int FLAG_OPTIMIZE = 0;

void set_nonlocal_flag()
{
    FLAG_NON_LOCAL_CHASING = 1;
}
void set_o1_flag()
{
    if(FLAG_OPTIMIZE < 1)
        FLAG_OPTIMIZE = 1;
}

void set_o2_flag()
{
    if(FLAG_OPTIMIZE < 2)
        FLAG_OPTIMIZE = 2;
}

int nonlocal_flag()
{
    return FLAG_NON_LOCAL_CHASING;
}
int optimize_flag()
{
    return FLAG_OPTIMIZE;
}
