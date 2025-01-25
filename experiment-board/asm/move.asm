.const PORT_A_DIR 0x8000
.const PORT_A 0x8002
.const RANDOM 0x8010

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
.const MAX_STEP_COUNT 20

jmp !init

.var test 0

!init
; Set all pins on portA to input 
set rA, 0xFFFF
str [PORT_A_DIR], rA
str [LCD_CLEAR_SCREEN], rZ   ; clear the screen
!main

    str [LCD_CLEAR_SCREEN], rZ 
    
    cal !print_test_info
    
    cal !wait_for_button

    cal !run_test

    cal !print_done

    cal !wait_for_button
    
    jmp !main

;-----------------
; print_str
;
; Expects address of NULL-terminated string in rA
;
!print_str
    psh rB

!str_loop
    lod rB, [rA]             ; get the next character
    cmp rB, rZ
    je  !string_terminated   ; stop if NULL

    str [LCD_CURSOR], rB   ; draw the character
    inc rA                 ; go to the next character in th string

    jmp !str_loop
!string_terminated
    pop rB
    ret

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


!DIRECTION_CHARS
'L' ' ' 0
'R' ' ' 0
'U' ' ' 0
'D' ' ' 0
'L' 'W' 0
'R' 'W' 0
'U' 'W' 0
'D' 'W' 0

!BUTTON_STR
'P' 'r' 'e' 's' 's' ' ' 'a' 'n' 'y' ' ' 'b' 'u' 't' 't' 'o' 'n' 0

!MOVE_STR
'M' 'o' 'v' 'e' ' ' 0

!DONE_STR
'D' 'o' 'n' 'e' ' ' 0

!TEST_ADDRESSES
    LCD_MOVE_LEFT LCD_MOVE_RIGHT LCD_MOVE_UP LCD_MOVE_DOWN LCD_MOVE_LEFT_WRAP LCD_MOVE_RIGHT_WRAP LCD_MOVE_UP_WRAP LCD_MOVE_DOWN_WRAP


!print_done
    psh rA
    set rA, 3
    str [LCD_CURSOR_XY],  rA  ; move cursor to bottom left
    set rA, !DONE_STR
    cal !print_str
    pop rA
    ret 

!print_test_info
    psh rA
    ;-- Print the "Move" string
    str [LCD_CURSOR_XY],  rZ   ; move cursor to top left
    set rA, !MOVE_STR
    cal !print_str

    ;-- Print which direction test
    lod rA, [test]
    mpy rA, 3 
    add rA, !DIRECTION_CHARS
    cal !print_str

    pop rA
    ret 

!run_test 
    psh rA
    !init_test
        set rA, rZ
        set rC, 0x0A01
        str [LCD_CURSOR_XY], rC
        set rD, rC
        lod rE, [test]
        lod rE, [ rE + !TEST_ADDRESSES ]

    !test_loop
        
        str [LCD_CLEAR_SCREEN], rZ   
    
        cal !print_test_info

        str [LCD_CURSOR_XY], rD

        set rC, FIRST_CHARACTER
        str [LCD_CURSOR], rC
        str [LCD_CURSOR_XY], rD

        
        set rC, MOVE_INCREMENT
        str [rE], rC
        

        lod rD, [LCD_CURSOR_XY]

        ;----- Edit this if your thing is tooo slowwww
        ; psh rA
        ; set rA, 0
        ; !delay_loop
        ;     dly 10000
        ;     inc rA
        ;     cmp rA, 10
        ;     jl !delay_loop
        ; pop rA
        ;-----------

        inc rA
        cmp rA, MAX_STEP_COUNT
        jl !test_loop

    lod rA, [test]
    inc rA
    mod rA, 8
    str [test], rA
    pop rA
    ret