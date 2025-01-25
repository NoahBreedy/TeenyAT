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

.const FIRST_CHARACTER 33
.const LAST_CHARACTER 247

!reset
SET rB, '\r'
SET rA, FIRST_CHARACTER

!loop

CMP rA, LAST_CHARACTER
JG !reset
STR [LCD_CURSOR], rA
inc rA

; inc rC
; MOD rC, 18
; CMP rC, 
;JE !return
;CMP rC, 4 
; JGE !return

JMP !loop

!return
    STR [LCD_CURSOR], rB
    JMP !loop

!kill_loop
    JMP !kill_loop