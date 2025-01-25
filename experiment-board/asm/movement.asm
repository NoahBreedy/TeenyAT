.const PORT_A_DIR 0x8000
.const PORT_A 0x8002
.const LCD_CURSOR 0xA000 
.const LCD_CLEAR_SCREEN 0xA001
.const LCD_MOVE_LEFT 0xA002  
.const LCD_MOVE_RIGHT 0xA003
.const LCD_MOVE_UP 0xA004
.const LCD_MOVE_DOWN 0xA005
.const LCD_MOVE_LEFT_WRAP 0xA006  
.const LCD_MOVE_RIGHT_WRAP 0xA007
.const LCD_MOVE_UP_WRAP 0xA008
.const LCD_MOVE_DOWN_WRAP 0xA009
.const LCD_CURSOR_X 0xA00A
.const LCD_CURSOR_Y 0xA00B
.const LCD_CURSOR_XY 0xA00C

.const FIRST_CHARACTER 225
.const MOVE_INCREMENT 1

!init
set rA, 0xFFFF
str [PORT_A_DIR], rA
set rA, 0x0A01
str [LCD_CURSOR_XY], rA
set rC, rA
set rA, FIRST_CHARACTER
set rB, MOVE_INCREMENT
!main
    
    str [LCD_CLEAR_SCREEN], rZ

    str [LCD_CURSOR], rA

    str [LCD_CURSOR_XY], rC
    str [LCD_MOVE_RIGHT], rB
    lod rC, [LCD_CURSOR_XY]
    
    cal !wait_for_button
    
    jmp !main



!wait_for_button
    psh rA
    ; ---- This mimics falling edge as output only changes on high -> low transition 
    !button_down_loop
        lod rA, [PORT_A]
        cmp rA, 0
        je !button_down_loop
    
    !button_up_loop
        lod rA, [PORT_A]
        cmp rA, 0
        jne !button_up_loop
    ; -------------------------
    pop rA
    ret 




