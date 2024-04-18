jmp !clear_screen

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

; Length 361 valid values
!sine
    0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 5 5 5 5 5 5 5 5 5 5 5 5 5 4 4 4 4 4 4 4 4 4 4 4 3 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -1 -1 -1 -1 -1 -1 -1 -1 -2 -2 -2 -2 -2 -2 -2 -2 -2 -3 -3 -3 -3 -3 -3 -3 -3 -3 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -3 -3 -3 -3 -3 -3 -3 -3 -3 -2 -2 -2 -2 -2 -2 -2 -2 -2 -1 -1 -1 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0

.variable MPY_VAL 10

!main  
   cmp rA, 64
   JGE !clear_screen
   !clear_screen_return

   set rB, 32
   set rC, rA
   lod rD, [MPY_VAL]
   mpy rC, rD
   mod rC, 360
   lod rB, [ rC + !sine ]
   add rB, 32
   str [X1], rA
   str [Y1], rB
   str [POINT], rZ
   inc rA
   
   str [UPDATE], rZ
   jmp !main

!clear_screen
    str [X1], rZ
    str [Y1], rZ
    set rA, 63
    str [X2], rA
    str [Y2], rA
    set rA, BLACK
    str [STROKE], rA
    set rA, BLACK
    str [FILL], rA
    str [RECT], rZ
    str [UPDATE], rZ
    set rA, WHITE
    str [STROKE], rA
    str [UPDATE], rZ
    lod rA, [RAND]
    mod rA, 30
    str [MPY_VAL], rA
    set rA, 0
    jmp !clear_screen_return