# CS204-Project1
Making an assembler for risc v.

The instructions supported are:
R format - add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem
I format - addi, andi, ori, lb, ld, lh, lw, jalr
S format - sb, sw, sd, sh
SB format - beq, bne, bge, blt
U format - auipc, lui
UJ format - jal

These instructions can be written in their standard format.

Additional support for the following directives:
.text, .data, .byte, .half, .word, .dword, .asciz.

immediate values and data values can only be given in integer format.

All types of error checking are included.
If there is an error in some line the program stops immediately and tells which line has the error.

Blank spaces and comments can be included anywhere in the code.
' ' and ',' serve the same purpose.

Standard RiscV formats should be followed to avoid any error.