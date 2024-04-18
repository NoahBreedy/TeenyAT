; Bouncing Ball Move it with WASD
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
.const LAVENDER 2689
.const RADIUS 10


; Length of 361

JMP !init

!cosine
    6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 5 5 5 5 5 5 5 5 5 5 5 5 5 4 4 4 4 4 4 4 4 4 4 4 3 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -1 -1 -1 -1 -1 -1 -1 -1 -2 -2 -2 -2 -2 -2 -2 -2 -2 -3 -3 -3 -3 -3 -3 -3 -3 -3 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -7 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -3 -3 -3 -3 -3 -3 -3 -3 -3 -2 -2 -2 -2 -2 -2 -2 -2 -2 -1 -1 -1 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 7
!sine
    0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 7 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 5 5 5 5 5 5 5 5 5 5 5 5 5 4 4 4 4 4 4 4 4 4 4 4 3 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -1 -1 -1 -1 -1 -1 -1 -1 -2 -2 -2 -2 -2 -2 -2 -2 -2 -3 -3 -3 -3 -3 -3 -3 -3 -3 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -7 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -6 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -5 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -3 -3 -3 -3 -3 -3 -3 -3 -3 -2 -2 -2 -2 -2 -2 -2 -2 -2 -1 -1 -1 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0

.var DX 0
.var DY 0
.var CIRCLE_COLOR WHITE

!init
SET rD, 32
SET rE, 32
!main
    ; Background Clear
    STR [X1], rZ
    STR [Y1], rZ
    SET rA, 63
    STR [X2], rA
    STR [Y2], rA
    SET rA, WHITE
    STR [STROKE], rA
    SET rA, BLACK
    STR [FILL], rA
    STR [RECT], rZ
    STR [UPDATE], rZ

    ; Circle Draw
    CAL !update_circle
    CAL !draw_circle
    STR [UPDATE], rZ

    LOD rB, [KEY] 

    ; Cannot call make_dx_negative and others due to how RET works :( (i may be wrong tho....)
    CMP rB, 'W' ; W
    JE !UP

    CMP rB, 'S' ; S
    JE !DOWN

    CMP rB, 'A' ; A
    JE !LEFT

    CMP rB, 'D' ; D
    JE !RIGHT

    DLY 50000

    JMP !main


!draw_circle
    LOD rA, [CIRCLE_COLOR]
    STR [STROKE], rA
    SET rA, 0
    !degree_loop

        ; cosine
        LOD rB, [ rA + 0x0002 ]
        ;sine
        LOD rC, [ rA + 0x016a ]
        
        DIV rB, 2
        DIV rC, 2

        ADD rB, rD ; half the screen width
        ADD rC, rE ; half the screen height

        STR [X1], rB
        STR [Y1], rC

        STR [POINT], rZ

        INC rA
        CMP rA, 360
        JLE !degree_loop
    RET

!update_circle
    LOD rA, [DX]
    LOD rB, [DY]

    ADD rD, rA
    
    CMP rD, 59  ; width and radius
    JGE !make_dx_negative
    CMP rD, 3
    JLE !make_dx_positive
    !update_dx_return

    ADD rE, rB
    
    CMP rE, 59 ; height and radius
    JGE !make_dy_negative
    CMP rE, 3
    JLE !make_dy_positive
    !update_dy_return

    RET
    
!make_dx_negative
    SET rC, -1
    STR [DX], rC
    LOD rC, [RAND]
    STR [CIRCLE_COLOR], rC
    JMP !update_dx_return

!make_dx_positive
    SET rC, 1
    STR [DX], rC
    LOD rC, [RAND]
    STR [CIRCLE_COLOR], rC
    JMP !update_dx_return

!make_dy_negative
    SET rC, -1
    STR [DY], rC
    LOD rC, [RAND]
    STR [CIRCLE_COLOR], rC
    JMP !update_dy_return

!make_dy_positive
    SET rC, 1
    STR [DY], rC
    LOD rC, [RAND]
    STR [CIRCLE_COLOR], rC
    JMP !update_dy_return

!UP
   SET rC, -1
   STR [DY], rC
   JMP !main


!DOWN
   SET rC, 1
   STR [DY], rC
   JMP !main
    

!LEFT
   SET rC, -1
   STR [DX], rC
   JMP !main
   

!RIGHT
    SET rC, 1
    STR [DX], rC
    JMP !main
   