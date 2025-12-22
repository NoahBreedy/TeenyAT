; NTASM (Noah's tnasm)
; The main goal of this is to learn more about compiler front-end design
; As well as to explore concepts related to LL(1) grammars and its implementation through the use of EBNF & RD
; I basically want this to be able to assemble normal tnasm asm files


@include "syntax/test.S"

jmp !main

.var my_var [12]

.raw !main 10 20 30 40

!main
    add rA, 10
    jmp !main

