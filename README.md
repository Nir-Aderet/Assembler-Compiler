# Assembler-Compiler writen in C - Linux Environment

Full explanation is in the file "Compiler explanation - Hebrew".
Programming Language: C
Operating System: Linux - Ubuntu

The compiler generates four output files:

- The ".am" file which contains the source code after the pre-assembler phase, where macros are handled.
- The ".object" file which includes the machine code and has a unique format.
- The "externals" file which is created if there is an extern command. This file provides information on all the addresses of external-info-words.
- The "entries" file is generated if there is an entry command. It contains information on all entry assigned signs.

Project Description: Developed a custom assembler compiler for a hypothetical computer architecture, equipped with its own assembler, memory, and registers. This project focused on creating a comprehensive tool capable of efficiently processing assembly code, identifying different line types (empty, comment, instruction, and guidance lines), executing macros, and conducting complete syntax and coding error analysis.
Each word in the computer is 20 bits long, and the length of each machine command varies from 1 to 6 words.

Key Contributions:

Design and Implementation: Led the design and implementation of the assembler compiler, ensuring compatibility with the target architecture and adherence to assembly language standards.
Line Type Detection: Implemented a robust line type detection mechanism, allowing the compiler to differentiate between various types of assembly code lines, facilitating accurate parsing and analysis.
Macro Execution: Developed a macro processing system, enabling the compiler to expand macros during assembly, reducing code redundancy and enhancing readability.
Error Analysis: Integrated error-checking routines to identify syntax errors, coding errors, and inconsistencies within the assembly code. Generated detailed analysis reports to assist developers in debugging and improving code quality.
Testing and Validation: Rigorously tested the compiler with a variety of assembly programs, ensuring its reliability and correctness. Conducted comprehensive test suites to detect and rectify errors.

Results and Impact:

Error Detection: By identifying and reporting syntax and coding errors, the compiler contributed to improved code quality and reduced debugging efforts.
Enhanced Developer Experience: Developers benefited from the tool's ability to execute macros and provide informative analysis reports, leading to more efficient code development.
