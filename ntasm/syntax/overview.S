; NTASM (NOTtnasm)
; The main goal of this is to learn more about compiler front-end design
; As well as to explore concepts related to top down parsers without backtracking
; as well as learning how we can write grammars rules through EBN/BNF and then finally 
; these rules through Recursive Descent (RD)
;
; I basically want this to be able to assemble normal tnasm asm file (.asm) as well introduce some
; quality of life features that I would personally like to see when write assembly (.S)


@include "syntax/test.S"

jmp !main

.const MY_CONST 10

.var first_var -10
.var second_var
.var var_3 "Hello World\n"

.raw !main 10 0b_0_110_1 30 -!test_label 0xBEEF 0o3745 ; octal babyy

!main
    set rA, MY_CONST
    set r5, -!main

.var buffer[10]

    set rd, rZ + !main
    set rb, -MY_CONST + rA
    set r4, var_3
    jmp !main ; coolio
