set rB, 1000
jmp !main
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

.const PLAYER_HEIGHT 5
.const CHANGE_AMT 10
.var player_x 30
.var player_y 30

!main
    !up
        lod rA, [UP_BUTTON_ADDRESS]
        cmp rA, 1
        jne !down

        lod rA, [player_y]
        dec rA
        cmp rA, 5
        jg !end_up
        set rA, 5
        !end_up
        str [player_y], rA
        str [Y1], rA
        add rA, PLAYER_HEIGHT
        str [Y2], rA

    !down
        lod rA, [DOWN_BUTTON_ADDRESS]
        cmp rA, 1
        jne !left

        lod rA, [player_y]
        inc rA
        cmp rA, 90 ; OLED_HEIGHT -  PLAYER_HEIGHT
        jl !end_down
        set rA, 90
        !end_down
        str [player_y], rA
        str [Y1], rA
        add rA, PLAYER_HEIGHT
        str [Y2], rA
        
    

    !left
        lod rA, [LEFT_BUTTON_ADDRESS]
        cmp rA, 1
        jne !right

        lod rA, [player_x]
        dec rA
        cmp rA, 0 
        jg !end_left
        set rA, 0
        !end_left
        str [player_x], rA
        str [X1], rA
        add rA, PLAYER_HEIGHT
        str [X2], rA
    
    !right
        lod rA, [RIGHT_BUTTON_ADDRESS]
        cmp rA, 1
        jne !a_button

        lod rA, [player_x]
        inc rA
        cmp rA, 120 ; OLED_WIDTH - PLAYER_HEIGHT 
        jg !end_right
        set rA, 120
        !end_right
        str [player_x], rA
        str [X1], rA
        add rA, PLAYER_HEIGHT
        str [X2], rA


    !a_button
        lod rA, [A_BUTTON_ADDRESS]
        cmp rA, 1
        jne !b_button
        add rB, CHANGE_AMT

    !b_button
        lod rA, [B_BUTTON_ADDRESS]
        cmp rA, 1
        jne !draw_player
        sub rB, CHANGE_AMT

    !draw_player
        str [RECT], rB
        str [UPDATE], rZ

    jmp !main