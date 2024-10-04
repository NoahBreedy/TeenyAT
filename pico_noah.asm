; Blink the onboard led with the teenyAT
; Stores to light on will automaticaly delay the amount in the register

.const UPDATE_SCREEN 0x8000
.const UPDATE 0xE000
.const A_BUTTON_ADDRESS 0xD000
.const B_BUTTON_ADDRESS 0xD001
.const UP_BUTTON_ADDRESS 0xD002
.const LEFT_BUTTON_ADDRESS 0xD003
.const DOWN_BUTTON_ADDRESS 0xD004
.const RIGHT_BUTTON_ADDRESS 0xD005
.const X1 0xD006
.const Y1 0xD007
.const X2 0xD008
.const Y2 0xD009
.const X3 0xD00A
.const Y3 0xD00B
.const TENNY_COLOR_ADDRESS 0xD00C
.const DRAW_FILL 0xD00D
.const RANDOM 0xD00E
.const TEXT_COLOR 0xD00F
.const UPDATE 0xE000
.const POINT 0xE001
.const RECT 0xE002
.const LINE 0xE003
.const CIRCLE 0xE004
.const TRIANGLE 0xE005
.const CHAR 0xE006
.const BOARD_SLEEP 0xE007

.const COLOR_AMT 50
set rA, 0
set rC, 0
set rD, 50
!main
    set rB, 35
    str [Y1], rB
    set rB, 40
    str [X1], rB
    set rA, 'N'
    str [TEXT_COLOR], rC
    str [CHAR], rA
    str [UPDATE], rZ
    add rC, COLOR_AMT
    add rB, 10
    str [X1], rB
    set rA, 'O'
    str [TEXT_COLOR], rC
    str [CHAR], rA
    str [UPDATE], rZ
    add rC, COLOR_AMT
    add rB, 10
    str [X1], rB
    set rA, 'A'
    str [TEXT_COLOR], rC
    str [CHAR], rA
    str [UPDATE], rZ
    add rC, COLOR_AMT
    add rB, 10
    str [X1], rB
    set rA, 'H'
    str [TEXT_COLOR], rC
    str [UPDATE], rZ
    str [CHAR], rA
    add rC, COLOR_AMT
    str [BOARD_SLEEP], rD
    str [UPDATE], rZ
    jmp !main