# Gwinn Pascal Compiler (GPC)
#### Author: Damon Gwinn

Pascal Compiler written in C. Takes Pascal code and outputs gcc-targeted x86-64 assembly. Mileage may vary!

---

## Overview
This is a Pascal compiler written in C with help from the tools Flex and Bison for the grammar parsing. The compiler is still a work in progress, but sports procedures, reading and writing from stdin/stdout, and basic expressions with two registers (a full feature list is given in the *Features* section). It also has a basic parse tree and expression optimizer (still a work in progress, but still does some basic optimizations). 

The compiler outputs x86-64 assembly designed to work with gcc. Run the compiler on a Pascal program to generate assembly output, then run gcc on that output to get the final executable. I essentially use gcc as my assembler.

### No Memory Leaks!
Despite the thousands of lines of C code (complete with all sorts of heap allocation), this compiler has **absolutely no memory leaks** of any kind. This was verified using valgrind with the *--leak-check=full* flag enabled. Of course, it's impossible to test this on every possible Pascal program, so if you manage to find a memory leak (assuming you didn't edit the code), please send me the Pascal file so I can fix it.

---

## How to run
Before attempting to use this, note that **I have only tested this on Ubuntu-based Linux operating systems**, I have no idea if it will work on anything else. Also, this does not work with any architecture other than 64-bit Intel x86.

After cloning the repository, enter the *GPC* directory and run *make*. This will generate the compiler executable in that same directory called *gpc*. The usage is *gpc [Input File] [Output File] [Optional Flag 1] ... *. The output file is assembly meant to run with gcc so I recommend you create the output file with the *.s* extension to follow naming conventions.

Once your output file is generated (assuming no compiler errors occurred), run gcc on it to get the final executable. You do not need to use any gcc flags.

### Compiler Flags
In addition to base behavior, there are optional flags you can turn on to activate features such as optimizations. Note that higher-level optimizations implicitely activate lower level optimizations (ex: -O2 activates level 2 and level 1 optimizations). The flags are listed below:
- *-non-local* allows procedures to reference variables in higher scope. THIS IS A VERY BUGGY WORK IN PROGRESS!
- *-O1* enables level-1 optimizations (simplifies expressions with constant numbers).
- *-O2* enables level-2 optimizations (removes unreferenced variables and their assignments).

---

## Features
This section aims to list the features currently supported and the features that will be supported but is not currently. Optimization levels are also discussed in more detail. Note that if I missed a Pascal feature, please reach out to me on this repository so I can clarify or add it to my compiler TODO.

### Hardware Support
Currently, only x86-64 Intel assembly is supported. An overhaul to the Code Generator is needed to support any other architecture at the moment. I currently only use two registers as general purpose registers. These, in practice, tend to be enough for fairly complex expressions. If an expression is too complicated for two registers, the code generator will fail gracefully and inform you of the problem (doesn't currently use the stack as a panic buffer).

Note that keeping in line with IEEE standards (and so the assmebly call to scanf doesn't crash), the stack is always incremented in batches of 16 bytes.

Hardware optimizations such as division by constant divisor, are not currently supported. Division or multiplication by constant powers of two does not currently bit shift like it should.

### Features Supported
Supported features are listed below:

- Program and subprogram declarations
- Nested subprograms
- Procedure calls with arguments (no function calls yet)
- Function return assignments and expressions
- Integer variable declarations and assignments
- Two-register expressions
- All expression operations except modulus
- If-then, while, and for statements
- Non-local variable references (buggy, activate with flag)
- Read and write built-in procedures

### Work-In-Progress Features
Features not yet supported are listed below:

- Function call expressions
- Modulus operator
- All applicable registers as general purpose
- Temporary stack use for expressions too complicated for available registers
- Division by constant divisor optimization
- Division or multiplication by constant power of two optimization

### Optimizations
As mentioned previously, there are currently two optimization levels you can activate via flags. Note that a higher level optimization implicitely activates lower level optimizations.

#### Level-1 Optimizations (O1)
This level simplifies expressions involving constant numbers. As an example, if there is a variable assignment *x := 3+5*, this will be optimized to instead be *x := 8* which results in less assembly instructions.

Note that this level can be improved upon by simplifying repeats of expressions (ex: *(x+5) + (x+5)*)

These optimizations are simple and involve only minor changes to the Parse Tree that only have an effect on expressions.

#### Level-2 Optimizations (O2)
This level works in tandem with the Semantic Checker to simplify the Parse Tree. This level specifically removes variables (and their assignments) that are never meaningfully referenced. *Meaningfully referenced* variables are passed to a subprogram, given to a write built-in, or are referenced by another meaningfully referenced variable.

Every time a variable is referenced by another variable, a reference counter is kept. If this count is 0, it means that a variable was created, but never meaningfully referenced.

First, the reference counts are pruned by eliminating self-references (ex: *x := x + 1*). After, every variable has its reference count checked. If a variable has a reference count of 0, the variable declaration, along with all it's accompanying assignment statements, are erased from the Parse Tree. If one of the removed assignments involves another variable, that variable will also have its reference count decremented. This process is repeated until every variable has a reference count greater than 0.

This leads to much simpler code that's faster (less assignment statements) and in some cases uses less memory (can free up stack space for other uses). As noted, this optimization is a bit more involved, but can have more dramatic effects on run time and memory usage.

---

## Contributing
This is currently only authored by me (Damon Gwinn), but if you want to contribute, just reach out to me. Always willing to accept help.


