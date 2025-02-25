.const PORT_A_DIR 0x8000
.const PORT_B_DIR 0x8001
.const PORT_A 0x8002
.const PORT_B 0x8003

.const LCD_CURSOR 0xA000 
.const LCD_CLEAR_SCREEN 0xA001
.const LCD_CURSOR_X 0xA00A
.const LCD_CURSOR_Y 0xA00B
.const LCD_CURSOR_XY 0xA00C
.const LCD_MOVE_RIGHT 0xA003

.const BUZZER_LEFT  0xA010
.const BUZZER_RIGHT 0xA011
.const SOUND_C4     0x0105  ; C4 (261 Hz)
.const SOUND_CSHARP4 0x0115 ; C#4 (277 Hz)
.const SOUND_D4     0x0125  ; D4 (293 Hz)
.const SOUND_DSHARP4 0x0137 ; D#4 (311 Hz)
.const SOUND_E4     0x0149  ; E4 (329 Hz)
.const SOUND_F4     0x015D  ; F4 (349 Hz)
.const SOUND_FSHARP4 0x0171 ; F#4 (369 Hz)
.const SOUND_G4     0x0188  ; G4 (392 Hz)
.const SOUND_GSHARP4 0x019F ; G#4 (415 Hz)
.const SOUND_A4     0x01B8  ; A4 (440 Hz)
.const SOUND_ASHARP4 0x01D2 ; A#4 (466 Hz)
.const SOUND_B4     0x01ED  ; B4 (494 Hz)
.const SOUND_C5     0x020B  ; C5 (523 Hz)
.const SOUND_CSHARP5 0x022A ; C#5 (554 Hz)
.const SOUND_D5     0x024B  ; D5 (587 Hz)
.const SOUND_DSHARP5 0x026E ; D#5 (622 Hz)
.const SOUND_E5     0x0293  ; E5 (659 Hz)
.const SOUND_F5     0x02BA  ; F5 (698 Hz)
.const SOUND_FSHARP5 0x02E3 ; F#5 (739 Hz)
.const SOUND_G5     0x030F  ; G5 (783 Hz)
.const SOUND_GSHARP5 0x033E ; G#5 (830 Hz)
.const SOUND_A5     0x0370  ; A5 (880 Hz)
.const SOUND_ASHARP5 0x03A4 ; A#5 (932 Hz)
.const SOUND_B5     0x03DB  ; B5 (987 Hz)
.const SOUND_C6     0x0416  ; C6 (1046 Hz)

.const DEMO_SPEED 450
.const SMALL_DEMO_SPEED 300

.const BOMB_SPRITE 0xD4
.const POISON_SPRITE 0xF0
.const DOT_SPRITE 0x85
.const BOLT_SPRITE 0xD0
.const MONEY_SPRITE 0x24
.const BLOCK_SPRITE 0xAC

.var char_sprite 0xD1
.var player_x 0xBBEF
.var player_y 0xBCEF
.var score 0xCCCC
.var temp_sprite 0xBFEF

; flags
.var player_moved_flag 0xBDEF
.var flipped_flag 0xBEEF

!init_demo
    str [PORT_B_DIR], rZ

    str [player_x], rZ
    str [player_y], rZ

    str [score], rZ
    
    str [flipped_flag], rZ
    str [player_moved_flag], rZ

    cal !draw_sprites
    jmp !demo_loop

; ANIMATIONS FOR COLLISIONS ON EACH SPRITE 
; ARE DONE HERE
!dot
    lod rA, [score]
    inc rA
    str [score], rA
    str [PORT_A_DIR], rZ
    set rE, 6000
    set rA, 0
    !dot_loop
        str [PORT_A], rA
        inc rA
        cmp rA, rE
        jne !dot_loop
    jmp !collisions_checked
!money
    jmp !collisions_checked
!bolt
    jmp !collisions_checked
!bomb
    jmp !collisions_checked
!poison
    str [LCD_CLEAR_SCREEN], rZ
    jmp !dead

!demo_loop
    cal !update_leroy
    cal !display_score ; WORKS BEST IN DECIMAL MODE
    cal !delay
    jmp !demo_loop

!display_score
    lod rA, [score]
    str [PORT_B], rA
    ret

; Check for d-pad input on port A and go to corresponding label
!check_for_input
    set rE, 0xFFFF 
    str [PORT_A_DIR], rE

    lod rE, [PORT_A]         
    and rE, 0xF    ; Bitmask to grab only d-pad buttons

    ; Down button
    cmp rE, 0x8        
    je !down_key_pressed
        
    ; Right button
    cmp rE, 0x4             
    je !right_key_pressed

    ; Up button
    cmp rE, 0x2             
    je !up_key_pressed

    ; Left button
    cmp rE, 0x1             
    je !left_key_pressed

    !key_pressed
    ret    

!erase_curr_leroy
    lod rA, [player_x]
    lod rB, [player_y]

    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB

    str [LCD_CURSOR], rZ
    ret

!check_if_moved
    lod rA, [player_moved_flag]
    cmp rA, 1
    jne !player_did_not_move
    cal !erase_curr_leroy
    str [player_moved_flag], rZ
    !player_did_not_move
    ret

!set_player_moved_flag
    cal !move_sfx
    cal !erase_curr_leroy
    lod rA, [player_moved_flag]
    set rA, 1
    str [player_moved_flag], rA
    ret

!down_key_pressed
    cal !set_player_moved_flag
    lod rB, [player_y]
    inc rB
    str [player_y], rB
    jmp !key_pressed
!right_key_pressed
    cal !set_player_moved_flag
    lod rA, [player_x]
    
    inc rA
    str [player_x], rA
    
    str [flipped_flag], rZ

    jmp !key_pressed
!up_key_pressed
    cal !set_player_moved_flag
    lod rB, [player_y]
    dec rB
    str [player_y], rB
    jmp !key_pressed
!left_key_pressed
    cal !set_player_moved_flag
    lod rA, [player_x]
    dec rA
    str [player_x], rA

    cmp rA, 0
    jle !wrap_x
    jmp !no_wrap

    !wrap_x
    set rA, 19
    str [player_x], rA

    !no_wrap
    set rA, 1
    str [flipped_flag], rA

    jmp !key_pressed

!switch_leroy_sprite

    lod rC, [char_sprite]
    cmp rC, 0xD1       ; which sprite is the current sprite
    je !use_sprite_two
    !use_sprite_one
        set rC, 0xD1    ; switch to 0xD1 sprite
        jmp !char_sprite_selected
    !use_sprite_two
        set rC, 0xD3    ; switch to 0xD3 sprite
    !char_sprite_selected

    ; Shall we flip the sprite?
    lod rE, [flipped_flag]
    cmp rE, 0             
    je !draw_normal
    !draw_flipped
        bts rC, 14        ; Set bit 14 to flip the sprite
    !draw_normal

    str [char_sprite], rC ; store the current sprite back into memory
    ret


!check_collision
    lod rA, [LCD_CURSOR]
    cmp rA, DOT_SPRITE
    je !dot

    cmp rA, MONEY_SPRITE
    je !money

    cmp rA, BOLT_SPRITE
    je !bolt

    cmp rA, BOMB_SPRITE
    je !bomb

    cmp rA, POISON_SPRITE
    je !poison

    !collisions_checked
    ret

!draw_leroy
    lod rA, [player_x]
    lod rB, [player_y] 
    lod rC, [char_sprite]

    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB
    cal !check_collision
    str [LCD_CURSOR], rC ; draw character
    ret


!update_leroy
    cal !check_if_moved
    cal !switch_leroy_sprite
    cal !check_for_input
    cal !draw_leroy
    ret

!get_random_sprite
    lod rA, [0x8010]
    mod rA, 20
    cmp rA, 8
    jle !set_dot
    cmp rA, 12
    jle !set_money
    cmp rA, 16
    jle !set_bolt
    cmp rA, 18
    jle !set_bomb
    ; 20
    jmp !set_poison

    !set_dot
        set rC, DOT_SPRITE
        str [temp_sprite], rC
        jmp !sprite_set
    !set_money
        set rC, MONEY_SPRITE
        str [temp_sprite], rC
        jmp !sprite_set
    !set_bolt
        set rC, BOLT_SPRITE
        str [temp_sprite], rC
        jmp !sprite_set
    !set_bomb
        set rC, BOMB_SPRITE
        str [temp_sprite], rC
        jmp !sprite_set
    !set_poison
        set rC, POISON_SPRITE
        str [temp_sprite], rC
    !sprite_set
    ret

!draw_sprites
    set rD, 100 ; CHANGE THIS TO MODIFY DENSITY OF OBJECTS
    set rE, 0
    lod rE, [player_x]
    lod rB, [player_y]
    !sprite_loop
        lod rA, [0x8010]
        mod rA, 20
        lod rB, [0x8010]
        mod rB, 4

        ; ensure player does not 
        ; start on sprite
        cmp rA, rE
        jne !check_clear
        cmp rB, rB
        je !draw_sprites

        !check_clear
        str [LCD_CURSOR_X], rA
        str [LCD_CURSOR_Y], rB
        cal !get_random_sprite
        lod rC, [temp_sprite]
        str [LCD_CURSOR], rC
        inc rE
        cmp rE, rD
        jne !sprite_loop
    ret

!dead_sfx
    set rE, SOUND_G4  
    str [BUZZER_RIGHT], rE
    cal !delay
    set rE, SOUND_F4  
    str [BUZZER_RIGHT], rE
    cal !delay
    set rE, SOUND_E4  
    str [BUZZER_RIGHT], rE
    cal !delay
    set rE, SOUND_D4  
    str [BUZZER_RIGHT], rE
    cal !delay
    set rE, SOUND_C4  
    str [BUZZER_RIGHT], rE
    cal !delay
    set rE, SOUND_B4  
    str [BUZZER_RIGHT], rE
    cal !delay
    set rE, SOUND_A4  
    str [BUZZER_RIGHT], rE
    cal !delay
    cal !delay
    cal !delay
    str [BUZZER_RIGHT], rZ
    cal !delay
    ret

!move_sfx
    set rE, SOUND_A4  
    str [BUZZER_RIGHT], rE
    cal !delay
    str [BUZZER_RIGHT], rZ
    ret

!dead
    cal !dead_sfx
    jmp !dead_1
    !draw_block
        str [LCD_CURSOR_X], rA
        str [LCD_CURSOR_Y], rB
        str [LCD_CURSOR], rC
        cal !small_delay
        ret
    !dead_1
    set rC, BLOCK_SPRITE
    
    set rA, 1
    set rB, 0
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB

    set rA, 2
    set rB, 0
    cal !draw_block
    inc rA
    cal !draw_block

    set rA, 2
    set rB, 2
    cal !draw_block
    inc rA
    cal !draw_block

    set rA, 5
    set rB, 1
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block

    set rA, 6
    set rB, 0
    cal !draw_block
    set rB, 2
    cal !draw_block

    set rA, 7
    set rB, 1
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block

    set rA, 10
    set rB, 0
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block

    set rA, 9
    set rB, 0
    cal !draw_block
    inc rA
    cal !draw_block
    inc rA
    cal !draw_block

    set rA, 9
    set rB, 3
    cal !draw_block
    inc rA
    cal !draw_block
    inc rA
    cal !draw_block

    set rA, 13
    set rB, 0
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block
    inc rB
    cal !draw_block

    set rA, 13
    set rB, 3
    cal !draw_block
    inc rA
    cal !draw_block
    inc rA
    cal !draw_block

    set rA, 17
    set rB, 0
    cal !draw_block
    inc rB
    cal !draw_block

    set rB, 3
    lod rC, [char_sprite]
    cal !draw_block
    cal !delay
    cal !dead_sfx
    cal !delay
    cal !delay
    cal !delay
    cal !delay
    cal !delay
    cal !delay
    cal !delay
    cal !delay
    cal !delay
    str [LCD_CLEAR_SCREEN], rZ
    jmp !dead_1

!small_delay
    set rD, SMALL_DEMO_SPEED
    set rE, 0                       
    !inner_small_delay_loop
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_small_delay_loop
    set rE, 0
    ret

; Main delay loop
; Because the virtual machine is so fast..
!delay
    set rD, DEMO_SPEED
    set rE, 0                       
    !inner_delay_loop
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_delay_loop
    set rE, 0
    ret