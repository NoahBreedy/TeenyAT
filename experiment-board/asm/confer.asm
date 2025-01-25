.const PORTA_DIR 0x8000
.const PORTB_DIR 0x8001
.const PORTA 0x8002
.const PORTB 0x8003

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

.const LEROY 0xD1

    jmp !main

.var test 0

!main

    ;-- Set up the buttons for input, lower 8 bits for LED output
    set rA, 0xFF00   ; buttons are the top 8 bits of Port A
    str [PORTA_DIR], rA

!run_test

    ;-- Identify test 0 through 7 on Port A LEDs
    set rA, 1
    lod rB, [test]
    neg rB
    shf rA, rB
    str [PORTB], rA

    str [LCD_CLEAR_SCREEN], rZ   ; clear the screen

    ;-- Print the "Move" string
    str [LCD_CURSOR_XY], rZ   ; move cursor to top left
    set rA, !MOVE_STR
    cal !print_str

    ;-- Print which direction test
    lod rA, [test]
    mpy rA, 11   ; each string is 11 words long, including NULL 
    add rA, !DIRECTION_CHARS
    cal !print_str



    ;-- Set cursor and get counter for next test
    lod rA, [test]
    mpy rA, 3
    add rA, !TEST_DATA   ; rA === addr of this test's data
    lod rB, [rA]
    str [LCD_CURSOR_X], rB
    inc rA
    lod rB, [rA]
    str [LCD_CURSOR_Y], rB
    inc rA

    lod rB, [rA]   ; how many times to move in the test direction

    lod rA, [test]
    add rA, LCD_MOVE_LEFT   ; addr of this test operation offset from first test
    str [rA], rB

    ;-- Draw Leroy in his final position
    set rA, LEROY
    str [LCD_CURSOR], rA


    

    ;-- Print the "Button" string
    set rA, 0x00_03   ; X:Y == 0, 3
    str [LCD_CURSOR_XY], rA   ; move cursor to bottom left
    set rA, !BUTTON_STR
    cal !print_str



    ;-- Wait for one of the buttons to be pressed
!button_wait_press
    lod rA, [PORTA]
    and rA, 0xFF00   ; test for any 1s in button bits
    je  !button_wait_press

    ;-- Wait for one of the buttons to be unpressed
!button_wait_unpress
    lod rA, [PORTA]
    and rA, 0xFF00   ; test for any 1s in button bits
    jne !button_wait_unpress



    ;-- Advance to next test
    lod rA, [test]
    inc rA
    mod rA, 8
    str [test], rA

    jmp !run_test

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


!DIRECTION_CHARS
'L''e''f''t'' '' '' '' '' '' ' 0
'R''i''g''h''t'' '' '' '' '' ' 0
'U''p'' '' '' '' '' '' '' '' ' 0
'D''o''w''n'' '' '' '' '' '' ' 0
'L''e''f''t'' ''W''r''a''p'' ' 0
'R''i''g''h''t'' ''W''r''a''p' 0
'U''p'' ''W''r''a''p'' '' '' ' 0
'D''o''w''n'' ''W''r''a''p'' ' 0
'L''e''f''t'' ''W''r''a''p'' ' 0

!BUTTON_STR
'P''r''e''s''s'' ''a''n''y'' ''b''u''t''t''o''n' 0

!MOVE_STR
'M''o''v''e'' ' 0

;-- Starting X,Y of the test and # of positions to move
!TEST_DATA
5 2 10
14 1 10
17 1 3
17 2 3
5 2 10
14 1 10
17 1 3
17 2 3
