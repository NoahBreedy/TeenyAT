.const RAND 0x8010
.const RAND_BITS 0x8011

.const LIVESCREEN 0x9000
.const UPDATESCREEN 0xA000
.const X1 0xD000
.const Y1 0xD001
.const X2 0xD002
.const Y2 0xD003
.const STROKE 0xD010
.const FILL 0xD011
.const DRAWFILL 0xD012
.const DRAWSTROKE 0xD013
.const UPDATE 0xE000
.const RECT 0xE010
.const LINE 0xE011
.const POINT 0xE012
.const MOUSEX 0xFFFC
.const MOUSEY 0xFFFD
.const MOUSEB 0xFFFB
.const TERM 0xFFFF
.const KEY 0xFFFE

.const COLORAMT 50

; rA = X
; rB = Y
; rC = KEY_HIT

set rA, 20
set rB, 20
set rC, 0

!main
    LOD rD, [RAND]
    MOD rD, 8
  
    ; Player Movement
    CMP rD, 0
    JE !UP

    CMP rD, 1
    JE !DOWN

    CMP rD, 2
    JE !LEFT

    CMP rD, 3
    JE !RIGHT

    CMP rD, 4
    JE !UP_LEFT

    CMP rD, 5
    JE !UP_RIGHT

    CMP rD, 6
    JE !DOWN_LEFT

    CMP rD, 7
    JE !DOWN_RIGHT

    !end_movement
        ADD rE, COLORAMT

    STR [X1], rA
    STR [Y1], rB
    STR [STROKE], rE
    STR [POINT], rZ
    STR [UPDATE], rZ

    JMP !main

!UP
    CMP rB, 0
    JLE !end_movement
    DEC rB
    JMP !end_movement

!DOWN
    CMP rB, 63 ; the lowest point
    JGE !end_movement
    INC rB
    JMP !end_movement

!LEFT
    CMP rA, 0
    JLE !end_movement
    DEC rA
    JMP !end_movement

!RIGHT
    CMP rA, 63
    JGE !end_movement
    INC rA
    JMP !end_movement

!UP_LEFT
    CMP rB, 0
    JLE !end_movement
    DEC rB
    CMP rA, 0
    JLE !end_movement
    DEC rA
    JMP !end_movement

!UP_RIGHT
    CMP rB, 0
    JLE !end_movement
    DEC rB
    CMP rA, 0
    JLE !end_movement
    INC rA
    JMP !end_movement

!DOWN_LEFT
    CMP rB, 0
    JLE !end_movement
    INC rB
    CMP rA, 0
    JLE !end_movement
    DEC rA
    JMP !end_movement

!DOWN_RIGHT
    CMP rB, 0
    JLE !end_movement
    INC rB
    CMP rA, 0
    JLE !end_movement
    INC rA
    JMP !end_movement