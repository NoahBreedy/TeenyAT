; Displays a pallete of sorts

.const TNY_PORTA_DIR_ADDRESS 0x8000
.const TNY_PORTA_ADDRESS 0x8002

.const SCREEN_DATA 0x9000
.const Y1_X1 0xD000
.const Y2_X2 0xD001
.const Y3_X3 0xD002
.const DRAW_COLOR 0xD003
.const FILL_SHAPES 0xD004
.const TRANSPARENT_CHAR 0xD005
.const UPDATE 0xE000
.const POINT 0xE001
.const RECT 0xE002
.const LINE 0xE003
.const CIRCLE 0xE004
.const TRIANGLE 0xE005
.const CHAR 0xE006
.const SLEEP_MS 0xE007
.const FILL_SCREEN 0xE008

.const OLED_WIDTH 128

.const A 1
.const B 2
.const UP 4 
.const LEFT 8
.const DOWN 16
.const RIGHT 32

set rA, rZ ; index to draw to
set rB, rZ ; general purpose 
set rC, rZ ; r 
set rD, rZ ; g
set rE, rZ ; b
!main
    ; get pixel_x 
    set rB, rA
    mod rB, OLED_WIDTH
    
    ; calculate and mask r 
    set rC, rB
    mpy rC, 2
    and rC, 0xf8

    ; calculate and mask b 
    set rE, 255
    sub rE, rC

    ; shift r and b over
    shf rC, 3
    shf rE, 3

    ; get pixel_y 
    set rB, rA
    div rB, OLED_WIDTH

    ; calculate and mask g 
    set rD, rB
    mpy rD, 2
    and rD, 0xfc
    shf rD, 2

    cal !make_color_rB

    str [rA + SCREEN_DATA], rB
    
    ; main comparison loop
    add rA, 5
    set rB, rA
    mod rB, 150
    jne !skip
    str [UPDATE], rZ
    !skip
    cmp rA, 0x3000
    jle !main
    
    str [FILL_SCREEN], rZ
    set rA, rZ
    
    jmp !main

!make_color_rB
    ; Prepare rB for masking 
    set rB, rZ
    
    ; left shift r & g to proper location
    shf rC, -11
    shf rD, -5
    
    ; standard 565-RGB
    or rB, rC
    or rB, rD
    or rB, rE
    
    ret 