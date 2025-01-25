.const PORT_A_DIR 0x8000
.const PORT_A 0x8002
.const LCD_CURSOR 0xA000 
.const LCD_CURSOR_XY 0xA00C

.const ARROW_VALUE_1 157
.const ARROW_VALUE_2 151
!init
; Set all pins on portA to input 
set rA, 0xFFFF
str [PORT_A_DIR], rA
; Set location to print value
set rB, 0x0A01
!main

    set rA, ARROW_VALUE_1
    cal !print_value
    cal !wait_for_button
    
    set rA, ARROW_VALUE_2

    cal !print_value
    cal !wait_for_button

    set rA, ARROW_VALUE_1
    bts rA, 15

    cal !print_value
    cal !wait_for_button

    set rA, ARROW_VALUE_2
    bts rA, 15
    cal !print_value
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

!print_value
    str [LCD_CURSOR_XY], rB 
    str [LCD_CURSOR], rA
    ret