# Assembler-Compiler writen in C

Full explanation is in the file "Compiler explanation - Hebrew".

Writen for a fictional computer with it's own Assembler, memory and registers.
Each word is 20 bits length and each machine command is between 1 to 6 words.
The compiler distinguishes between empty lines, comment line, instruction lines and guidance lines.
It also detects syntax errors and coding errors and knows how to handle macros.

The compiler outputs:
- .am file: contains the source code after the pre-assembler phase (after handling the macros).
- .object file: contains the machine code.
This file has a special format.
- externals file: if there is an extern command then the compiler will create this file. It contains information on all the addresses of external-info-words.
- entries file: if there is an entry command then the compiler will create this file. It contains information on an entry assigned sign.
