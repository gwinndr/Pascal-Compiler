/*
    Damon Gwinn
    Defines for special registers with their purpose
*/

#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

    /* TODO: Add division and stack chasing registers */

    /* Return register */
    #define RETURN_REG_64 "%rax"
    #define RETURN_REG_32 "%eax"

    /* Non-local register */
    #define NON_LOCAL_REG_64 "%rcx"
    #define NON_LOCAL_REG_32 "%ecx"

    /* Argument registers */
    /* TODO: Add remaining registers */
    #define NUM_ARG_REG 4

    #define ARG_REG_1_64 "%rdi"
    #define ARG_REG_1_32 "%edi"

    #define ARG_REG_2_64 "%rsi"
    #define ARG_REG_2_32 "%esi"

    #define ARG_REG_3_64 "%rdx"
    #define ARG_REG_3_32 "%edx"

    #define ARG_REG_4_64 "%rcx"
    #define ARG_REG_4_32 "%ecx"

#endif
