jmp !main
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

.const BROWN 3900
.const WHITE 2561
.const RED 3585
.const GREEN 1537
.const LIGHTGREEN 1736
.const YELLOW 2116
.const LAVENDER 2689
.const BLACK 0

.const KEY_BOARD_BUFFER_SIZE 8
.const MAX_ACCELERATION 1
.const CAR_FALL_DISTANCE 57
.const CAR_RISE_DISTANCE 40
.const CAR_LEFT_DISTANCE 4
.const CAR_RIGHT_DISTANCE 58
.const ROCK_TABLE_LENGTH 2
.const LINE_TABLE_LENGTH 30

.variable PLAYER_X 32
.variable PLAYER_Y 50
.variable PLAYER_VEL_X 0
.variable PLAYER_VEL_Y 0

!LINE_TABLE
    10 11 12 13 14 20 21 22 23 24 30 31 32 33 34 40 41 42 43 44 50 51 52 53 54 60 61 62 63 64
    
!ROCK_TABLE
    4 16 50 32

!main
    cal !background
    cal !draw_rocks
    cal !update_player
    cal !draw_player
    str [UPDATE], rZ
    jmp !main

!update_player

    psh rA
    psh rB

    ;;;;;;
    ;;  PLAYER LEFT/RIGHT MOVEMENT
    ;;
    set rB, KEY_BOARD_BUFFER_SIZE

    !key_x_loop
    lod rA, [KEY]
    cmp rA, 'A'
    je !move_player_left
    cmp rA, 'D'
    je !move_player_right
    lup rB, !key_x_loop
    
    lod rA, [PLAYER_VEL_X]
    cmp rA, 0
    je !change_in_x
    jg !dec_player_vel_x
    inc rA
    str [PLAYER_VEL_X], rA
    jmp !change_in_x
    !dec_player_vel_x
    dec rA
    str [PLAYER_VEL_X], rA
    jmp !change_in_x

     
    !change_in_x

    set rB, KEY_BOARD_BUFFER_SIZE
    !key_x_loop_2
    lod rA, [KEY]
    cmp rA, 'W'
    je !can_move_x
    lup rB, !key_x_loop_2
    jmp !start_player_accecl

    !can_move_x
    lod rA, [PLAYER_VEL_X]
    lod rB, [PLAYER_X]
    add rB, rA

    ; Prevent Player from moving left/right off screen
    cmp rB, CAR_LEFT_DISTANCE
    jge !keep_left_change_x
    set rB, CAR_LEFT_DISTANCE
    !keep_left_change_x

    cmp rB, CAR_RIGHT_DISTANCE
    jle !keep_right_change_x
    set rB, CAR_RIGHT_DISTANCE
    !keep_right_change_x
    str [PLAYER_X], rB


    ;;;;;;
    ;;  PLAYER ACCELERATION MOVEMENT
    ;;
    !start_player_accecl
    set rB, KEY_BOARD_BUFFER_SIZE
    !key_y_loop
    lod rA, [KEY]
    cmp rA, 'W'
    JE !accelerate_player
    lup rB, !key_y_loop
   
    lod rA, [PLAYER_VEL_Y]
    dec rA
    cmp rA, -1
    JG !keep_decrease
    set rA, -1
    !keep_decrease
    str [PLAYER_VEL_Y], rA


    !accelerate_player_ret
    lod rA, [PLAYER_VEL_Y]
    lod rB, [PLAYER_Y]
    
    sub rB, rA
    ; Prevent Player from falling off screen
    cmp rB, CAR_FALL_DISTANCE
    JLE !keep_fall_change_y
    set rB, CAR_FALL_DISTANCE
    str [PLAYER_VEL_Y], rZ
    !keep_fall_change_y

    cmp rB, CAR_RISE_DISTANCE
    JGE !keep_rise_change_y
    set rB, CAR_RISE_DISTANCE
    !keep_rise_change_y
    str [PLAYER_Y], rB


    pop rA
    pop rB

    ret

!move_player_left
    psh rB
    lod rB, [PLAYER_VEL_X]
    dec rB
    cmp rB, rZ - MAX_ACCELERATION
    jge !keep_left_increase
    set rB, rZ - MAX_ACCELERATION
    !keep_left_increase
    str [PLAYER_VEL_X], rB
    pop rB
    jmp !change_in_x

!move_player_right
    psh rB
    lod rB, [PLAYER_VEL_X]
    inc rB
    cmp rB, MAX_ACCELERATION
    jle !keep_right_increase
    set rB, MAX_ACCELERATION
    !keep_right_increase
    str [PLAYER_VEL_X], rB
    pop rB
    jmp !change_in_x


!accelerate_player
    psh rB
    lod rB, [PLAYER_VEL_Y]
    inc rB
    cmp rB, MAX_ACCELERATION
    jle !keep_acc_increase
    set rB, MAX_ACCELERATION
    !keep_acc_increase
    str [PLAYER_VEL_Y], rB
    pop rB
    jmp !accelerate_player_ret




!draw_player
    psh rA
    psh rB

    lod rA, [PLAYER_X]
    lod rB, [PLAYER_Y]

    sub rA, 2
    str [X1], rA
    str [Y1], rB

    add rA, 4
    add rB, 3

    str [X2], rA
    str [Y2], rB

    set rA, YELLOW
    str [FILL], rA
    str [DRAWSTROKE], rZ
    str [RECT], rZ

    pop rA
    pop rB
    ret


!background
    str [X1], rZ
    str [Y1], rZ
    psh rA
    set rA, 63
    str [X2], rA
    str [Y2], rA
    set rA, LIGHTGREEN
    str [STROKE], rA
    str [FILL], rA
    str [RECT], rZ
    pop rA
    cal !draw_track
    ret

!draw_rocks
    psh rA
    psh rB
    psh rC
    psh rD

    set rC, ROCK_TABLE_LENGTH
    mpy rC, 2

    set rA, 0
    !rock_draw_loop
    
    ; X
    lod rB, [ rA + !ROCK_TABLE ]
    str [X1], rB
    add rB, 4
    str [X2], rB

    ; Y
    inc rA
    lod rB, [ rA + !ROCK_TABLE ]

    lod rD, [PLAYER_VEL_Y]
    sub rB, rD
    str [rA + !ROCK_TABLE ], rB

    str [Y1], rB
    add rB, 4
    str [Y2], rB

    psh rA
    set rA, 50
    str [FILL], rA
    str [STROKE], rA
    str [RECT], rZ
    pop rA
    inc rA
    cmp rA, rC
    jl !rock_draw_loop
    pop rA
    pop rB
    pop rC
    pop rD

    ret
    
!draw_track
    ; god awful will rework later....
    psh rA
    set rA, 13
    str [X1], rA
    set rA, 63
    str [Y1], rA
    set rA, 19
    str [X2], rA
    set rA, 0
    str [Y2], rA
    set rA, BLACK
    str [STROKE], rA
    str [LINE], rZ

    set rA, 49
    str [X1], rA
    set rA, 63
    str [Y1], rA
    set rA, 45
    str [X2], rA
    set rA, 0
    str [Y2], rA 
    str [LINE], rZ

    set rA, 50
    str [FILL], rA
    str [STROKE], rA
    set rA, 20
    str [X1], rA
    set rA, 0
    str [Y1], rA
    set rA, 44
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [RECT], rZ

    set rA, 18
    str [X1], rA
    set rA, 6
    str [Y1], rA
    set rA, 44
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [RECT], rZ

    set rA, 17
    str [X1], rA
    set rA, 20
    str [Y1], rA
    set rA, 45
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [RECT], rZ

    set rA, 15
    str [X1], rA
    set rA, 40
    str [Y1], rA
    set rA, 47
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [RECT], rZ

    set rA, 45
    str [X1], rA
    set rA, 0
    str [Y1], rA
    set rA, 45
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [LINE], rZ

    set rA, 16
    str [X1], rA
    set rA, 37
    str [Y1], rA
    set rA, 16
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [LINE], rZ

    set rA, 46
    str [X1], rA
    set rA, 24
    str [Y1], rA
    set rA, 46
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [LINE], rZ

     set rA, 48
    str [X1], rA
    set rA, 56
    str [Y1], rA
    set rA, 48
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [LINE], rZ


    set rA, 14
    str [X1], rA
    set rA, 58
    str [Y1], rA
    set rA, 14
    str [X2], rA
    set rA, 63
    str [Y2], rA 
    str [LINE], rZ

    set rA, 32
    str [X1], rA
    str [X2], rA

    set rA, WHITE
    str [STROKE], rA
    
    set rA, 0
    
    psh rB
    psh rC

    !track_line_draw_loop
    lod rB, [PLAYER_VEL_Y]
    lod rC, [rA + !LINE_TABLE ]
   
    sub rC, rB

    str [Y1], rC
    str [rA + !LINE_TABLE ], rC
    str [POINT], rZ
    inc rA
    cmp rA, LINE_TABLE_LENGTH
    jl !track_line_draw_loop

    pop rB
    pop rC
    pop rA
    ret