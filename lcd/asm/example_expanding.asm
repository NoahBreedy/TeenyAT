jmp !init

.const LIVESCREEN 0x8000
.const UPDATESCREEN 0x9000
.const X1 0xD000
.const Y1 0xD001
.const X2 0xD002
.const Y2 0xD003
.const STROKE 0xD010
.const FILL 0xD011
.const DRAWFILL 0xD012
.const DRAWSTROKE 0xD013
.const RAND 0xD700
.const UPDATE 0xE000
.const RECT 0xE010
.const LINE 0xE011
.const POINT 0xE012
.const MOUSEX 0xFFFC
.const MOUSEY 0xFFFD
.const MOUSEB 0xFFFB
.const TERM 0xFFFF
.const KEY 0xFFFE

.const WHITE 2561
.const BLACK 0

!function_array
    0 0 0 0

.variable loop_count 0
.variable inc_amt 1

!init
set rA, !function_array

set rB, !right
str [rA], rB
inc rA

set rB, !up
str [rA], rB
inc rA

set rB, !left
str [rA], rB
inc rA

set rB, !down
str [rA], rB
inc rA

set rA, 23
set rB, 23
set rC, 23
set rD, 23

cal !background

!main
    str [X1], rA
    str [Y1], rB
    str [X2], rC
    str [Y2], rD

    lod rA, [RAND]
    mod rA, 500
    set rE, rA + WHITE
    str [STROKE], rE
    str [LINE], rZ
    str [UPDATE], rZ
    

    lod rA, [loop_count]
    mod rA, 4
    lod rB, [rA + !function_array ]
    inc rA
    str [loop_count], rA

    set rA, rC
    cal rB
    

    jmp !main

!background
    str [X1], rZ
    str [Y1], rZ
    psh rA
    set rA, 63
    str [X2], rA
    str [Y2], rA
    set rA, 0
    str [STROKE], rA
    str [FILL], rA
    pop rA
    str [RECT], rZ
    str [UPDATE], rZ
    ret

!right
    set rB, rD
    psh rA
    lod rA, [inc_amt]
    add rC, rA
    pop rA
    ret

!up 
    set rB, rD
    psh rA
    lod rA, [inc_amt]
    sub rD, rA
    pop rA
    ret 

!left
    set rB, rD
    psh rA
    lod rA, [inc_amt]
    inc rA
    str [inc_amt], rA
    sub rC, rA
    pop rA
    ret

!down
    set rB, rD
    psh rA
    lod rA, [inc_amt]
    add rD, rA
    pop rA
    ret
