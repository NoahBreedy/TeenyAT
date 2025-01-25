; Simple Paint App 
; rA stores player X
; rB stores player Y
; rC occasionaly stores Y_X
; rD stores the color to be drawn
; Press A to paint on canvas
; Press B to cycle through colors

jmp !init

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

.const A 1
.const B 2
.const UP 4 
.const LEFT 8
.const DOWN 16
.const RIGHT 32

; 565 rgb color values
.const RED  0xF800
.const BLUE 0x001F 
.const GREEN 0x07E0
.const YELLOW 0xFFE0
.const ORANGE 0xFCA0
.const PURPLE 0xA81F
.const BLACK 0x1082 ; sudo black dosent look nice all zeros...
.const WHITE 0xFFFF

.var OLD_COLOR 0xFFFF
.var OLD_POSITION 0x1E1E

!COLOR_ARRAY
    RED BLUE GREEN YELLOW ORANGE PURPLE BLACK WHITE

!init
set rA, 0x003F ; Enable buttons on board as input
str [TNY_PORTA_DIR_ADDRESS], rA
set rA, 0xFFFF
str [FILL_SCREEN], rA
set rA, 30
set rB, rA
!main
    cal !process_buttons
    lod rC, [rD + !COLOR_ARRAY]
    str [POINT], rC
    str [UPDATE], rZ
    jmp !main

; Reads in portA levels and moves x & y accordingly
!process_buttons
    lod rC, [TNY_PORTA_ADDRESS]

    ; Mask out bit levels and shift with corresponding adds/subs ;
    ; UP ;
    set rE, rC
    and rE, UP
    shf rE, 2
    sub rB, rE

    ; DOWN ;
    set rE, rC
    and rE, DOWN
    shf rE, 4
    add rB, rE

    ; LEFT ;
    set rE, rC
    and rE, LEFT
    shf rE, 3
    sub rA, rE

    ; RIGHT ;
    set rE, rC
    and rE, RIGHT  
    shf rE, 5
    add rA, rE

    ; CHANGE COLOR B BUTTON ;
    set rE, rC
    and rE, B
    cmp rE, B
    jne !end_check0
    inc rD
    mod rD, 8
    set rE, 150
    str [SLEEP_MS], rE ; sleep to prevent super fast color changes
    
    ; This checks if the A button is pressed 
    ; if it is it will "paint" else it will keep previous state
    !end_check0
    set rE, rC
    and rE, A
    cmp rE, A
    
    je !express_check

    cal !refresh_paint
    
    !end_check1
    cal !store_player_x_y
    lod rE, [OLD_POSITION]
    cmp rE, rC
    
    je !end_check2
    
    !express_check
    cal !store_player_x_y
    lod rE, [POINT]
    str [OLD_COLOR], rE

    !end_check2
    str [OLD_POSITION], rC

    ret

; Masks in player X Y into register rC then stores to Y1_X1
!store_player_x_y
    set rC, rZ 
    or rC, rB
    shf rC, -8
    or rC, rA
    str [Y1_X1], rC
    ret

; Preserves old state of the drawing array
!refresh_paint
    lod rE, [OLD_POSITION]
    str [Y1_X1], rE
    lod rE, [OLD_COLOR]
    str [POINT], rE
    ret