/*
    Damon Gwinn
    Code generation

    Stack structure (64-bit):
        This is the current stack structure, subject to changes as needed
        Last modified 4-21-19

        d_word refers to a doubleword or 4 bytes

        LOW ADDRESSES
        ============
        t   d_words (temporaries)
        ============
        x   d_words (local variables)
        ============
        z   d_words (argument variables)
        ============
        1   d_word (static parent pointer)
        ============
        1   d_word (subprogram return addr)
        ============
        HIGH ADDRESSES
*/
