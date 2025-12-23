; NTASM (Noah's tnasm)
; The main goal of this is to learn more about compiler front-end design
; As well as to explore concepts related to LL(1) grammars and its implementation through the use of EBNF & RD
; I basically want this to be able to assemble normal tnasm asm files


@include "syntax/test.S"

jmp !main

.const MY_CONST 10

.var first_var -10
.var second_var
.var var_3 "Hello World\n"

.raw !main 10 0b_0_110_1 30 -!test_label 0xBEEF 0o3745

!main
    set rA, MY_CONST
    set r5, -!main

.var buffer[10]

    set rd, rZ + !main
    set rb, -MY_CONST + rA
    set r4, var_3
    jmp !main ; coolio
