# Assembler-Compiler writen in C

Full explanation is in the file "Compiler explanation - Hebrew".

The assembler compiler is designed for a hypothetical computer that has its own assembler, memory, and registers. Each word in the computer is 20 bits long, and the length of each machine command varies from 1 to 6 words. The compiler is capable of identifying and differentiating empty lines, comment lines, instruction lines, and guidance lines. Moreover, it can identify syntax errors and coding errors, and also handle macros.

The compiler generates four output files:

- The ".am" file which contains the source code after the pre-assembler phase, where macros are handled.
- The ".object" file which includes the machine code and has a unique format.
- The "externals" file which is created if there is an extern command. This file provides information on all the addresses of external-info-words.
- The "entries" file is generated if there is an entry command. It contains information on an entry assigned sign.
