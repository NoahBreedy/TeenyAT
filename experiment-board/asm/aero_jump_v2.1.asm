; <==== AERO JUMP_v2 ====>
;  optimized for a 1MHz TeenyAT
;  uses buzzers for sfx!
;
; A simple demo to show off the
; experiment board.
; 
; You play as aero, jumping over an aero.. You ONLY
; get ONE jump, so use it wisely! Any input detected on
; PORT A will make aero jump. 
;
;
; - John Pertell
;   02.18.25 => Version 2.1 w/Sound!
;   

.const PORT_A_DIR 0x8000
.const PORT_A 0x8002
.const PORT_B 0x8003
.const LCD_CURSOR 0xA000 
.const LCD_CLEAR_SCREEN 0xA001
.const LCD_CURSOR_X 0xA00A
.const LCD_CURSOR_Y 0xA00B

.const life_count 3         ; how many lives should the player start with?
.const INITIAL_SPEED 700    ; this might need to be altered, depending on machine specs
.const MAX_SPEED 300        ; the game should not go past this threshold
.const SPEED_UP_AMT 25      ; how fast should the demo speed up by

; buzzer info
.const BUZZER_RIGHT 0xA011
.const SOUND_C4     0x0105  ; C4 (261 Hz)
.const SOUND_CSHARP4 0x0115 ; C#4 (277 Hz)
.const SOUND_G4     0x0188  ; G4 (392 Hz)
.const SOUND_C5     0x020B  ; C5 (523 Hz)

.var delay_amt 0xBAEF       ; demo should adjust delay_amt for game_speed
.var aero_x 0xBBEF          ; keep track of the aero's x position, this helps w/collision detection
.var player_y 0xBCEF        ; keep track of the player's y_pos, this helps w/collision detection
.var char_sprite 0xBDEF     ; keep track of the current player sprite, for animation
.var lives_left 0xBEEF      ; keep track of the amount of lives left
.var player_jumped 0xBFEF   ; for gameplay mechanics, the player only gets one jump per "aero passthrough".

; init_game simply loads
; desired data onto memory
!init_game
    set rA, 0xFFFF
    str [PORT_A_DIR], rA ; set PORT_A to input

    ; initial player sprite = 0xE2
    lod rA, [char_sprite]
    set rA, 0xE2
    str [char_sprite], rA

    ; how many lives should the player start with?
    lod rA, [lives_left]
    set rA, life_count
    str [lives_left], rA

    ; initial player_y(standing on ground = 2)
    lod rA, [player_y]
    set rA, 2
    str [player_y], rA

    ; initial x_coord for aero
    lod rA, [aero_x]
    set rA, 20
    str [aero_x], rA

    ; initial speed for delay loop
    lod rA, [delay_amt]
    set rA, INITIAL_SPEED
    str [delay_amt], rA

    ; player has NOT yet jumped (jump available)
    str [player_jumped], rZ ; hasn't jumped = 0
    jmp !draw_banner

!game_loop
    cal !display_ground ; draw the ground & bow
    cal !draw_lives     ; draw the hearts
    cal !draw_player    ; draw the player
    cal !check_for_jump ; check for input on PORT A = JUMP!
    cal !move_aero      ; iterate the aero by one
    cal !delay          ; delay the loop by delay_amt
    jmp !game_loop      ; loop back up to start of !game_loop

; update the player
; animate the player by 
; switching between 0xE2 and 0xE3 sprites
!draw_player
    lod rB, [player_y] ; y coord
    cmp rB, 2          ; is the player in the air?
    jl !fall_player    ; fall back down

    set rA, 4          ; x coord(always the same)
    lod rC, [char_sprite]
    cmp rC, 0xE2       ; which sprite is the current sprite
    je !use_sprite_two
    !use_sprite_one
        set rC, 0xE2    ; switch to 0xE2 sprite
        jmp !char_sprite_selected
    !use_sprite_two
        set rC, 0xE3    ; switch to 0xE3 sprite
    !char_sprite_selected
    str [char_sprite], rC ; store the current sprite back into memory
    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB
    str [LCD_CURSOR], RC ; draw character
    ret

; checks for any input on PORT_A
; will not jump player unless player_jumped = 0
!check_for_jump

    lod rA, [player_jumped]
    cmp rA, 0
    jne !jump_ignored

    lod rA, [PORT_A]
    cmp rA, 0
    jg !jump_player ; input on PORT_A => make player jump

    !jump_ignored
    ret

; iterate the aero over to the left
!move_aero
    set rB, 2 ; y coord
    set rC, 0xF2 ; aero sprite 
    bts rC, 15 ; flip aero
    lod rA, [aero_x]
    str [LCD_CURSOR_X], rA  
    str [LCD_CURSOR_Y], rB 
    str [LCD_CURSOR], rZ
    dec rA
    cmp rA, 0
    je !reset_aero_x
    !aero_reset ; this label is used in 'reset_aero_x'

    str [aero_x], rA
    str [LCD_CURSOR_X], rA  
    str [LCD_CURSOR_Y], rB 
    str [LCD_CURSOR], rC ; draw aero at new x-coord
    cal !check_collision ; has the aero collided w/the player? lets check!
    ret

!check_collision
    lod rA, [aero_x]
    cmp rA, 4 ; is the aero over where player is
    jne !collision_checked ; no collision w/aero

    lod rD, [player_y]
    cmp rD, 2
    jne !collision_checked ; no collision w/aero

    ; it hit the player!
    cal !dec_life
    cal !reset_aero_x
    cal !play_aero_sound

    !collision_checked
    ret

; if the aero hits the player
; or if it goes out of bounds..
; the bow should shoot a new aero,
; and the demo should speed up.
!reset_aero_x
    cal !delay
    cal !speed_up ; speed up the delay!
    lod rA, [aero_x]
    set rA, 19
    str [aero_x], rA

    ; resets player_jumped flag to 0
    str [player_jumped], rZ
    cal !play_aero_sound
    jmp !aero_reset

; Make the player jump when
; input is recieved from PORT_A
!jump_player

    
    lod rB, [player_jumped]
    set rB, 1
    str [player_jumped], rB

    lod rB, [player_y]
    cmp rB, 2
    jl !player_jumped ; is the player in the air?

    set rA, 4
    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB 
    str [LCD_CURSOR], rZ ; draw empty box where the player was

    dec rB
    str [player_y], rB
    set rC, 0xE4 ; jumping sprite of player
    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB
    str [LCD_CURSOR], rC ; draw jumping sprite
    cal !play_jump_sound


    cal !move_aero ; move aero by one when player is jumping
    !player_jumped
    cal !delay
    ret

; Force player to fall back down
!fall_player
    lod rB, [player_y]
    set rA, 4 ; player x-coord (always the same)
    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB
    str [LCD_CURSOR], rZ
    inc rB ; incrementiing y causes player to move down
    str [player_y], rB
    set rC, 0xE2

    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB
    str [LCD_CURSOR], rC
    jmp !char_sprite_selected ; go back to game_loop

; draw all ground blocks at the bottom of the screen
; draws the bow also!
!display_ground
    set rA, 0                   
    set rB, 3 
    set rC, 0xAC ; ground tile sprite
    !draw_one_ground  ; a loop to draw all ground tiles 
        str [LCD_CURSOR_Y], rB 
        str [LCD_CURSOR], rC      ; draw ground tile   
        inc rA                      
        cmp rA, 20               
        jne !draw_one_ground

    ; draw_bow
    set rA, 19
    set rB, 2
    set rC, 0xF1 ; bow sprite
    bts rC, 15 ; flip the bow
    str [LCD_CURSOR_X], rA  
    str [LCD_CURSOR_Y], rB 
    str [LCD_CURSOR], rC ; draw bow
    ret                          

; draw the lives as heart sprites
; based on the number of lives_left
!draw_lives
    set rA, 0
    set rB, 0
    str [LCD_CURSOR_X], rA
    str [LCD_CURSOR_Y], rB

    ; draw letters to screen
    set rC, 'L'
    str [LCD_CURSOR], rC
    set rC, 'i'
    str [LCD_CURSOR], rC
    set rC, 'v'
    str [LCD_CURSOR], rC
    set rC, 'e'
    str [LCD_CURSOR], rC
    set rC, 's'
    str [LCD_CURSOR], rC
    set rC, ':'
    str [LCD_CURSOR], rC
    set rC, ' '
    str [LCD_CURSOR], rC

    set rC, 0xCA ; heart sprite
    lod rA, [lives_left] ; how many hearts do we have to draw?
    !life_loop
        str [LCD_CURSOR], rC ; draw the heart sprite
        dec rA
        cmp rA, 1
        jge !life_loop
    ret

; Decrease lives_left by one
; erase one heart sprite
!dec_life
    ; old hearts that have been drawn have to be erased..
    ; .. took me way to long to figure that one out :(
    str [LCD_CURSOR_Y], rZ
    lod rD, [lives_left]
    cmp rD, 1 ; check how many hearts are left
    jle !game_over ; end game if lives are empty

    set rA, rD
    add rA, 6 ; last heart is at this location.. trust me
    !draw_over_old_hearts
        str [LCD_CURSOR_X], rA 
        str [LCD_CURSOR], rZ
        dec rA
        cmp rA, 6
        jge !draw_over_old_hearts

    dec rD
    str [lives_left], rD ; update lives_left in memory
    ret

; Speed the demo up!
; to enhance gameplay
!speed_up
    lod rE, [delay_amt]
    cmp rE, MAX_SPEED ; check if reached max_speed
    jl !sped_up
    sub rE, SPEED_UP_AMT ; how much should we speed up by?
    str [delay_amt], rE
    !sped_up
    ret

; draws startup banner
!draw_banner
    set rA, 11
    !move_banner
        cal !play_intro_sound
        set rB, 1
        str [LCD_CURSOR_X], rA
        str [LCD_CURSOR_Y], rB
        set rC, 'A'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, 'r'
        str [LCD_CURSOR], rC
        set rC, 'o'
        str [LCD_CURSOR], rC
        set rC, ' '
        str [LCD_CURSOR], rC
        set rC, 'J'
        str [LCD_CURSOR], rC
        set rC, 'u'
        str [LCD_CURSOR], rC
        set rC, 'm'
        str [LCD_CURSOR], rC
        set rC, 'p'
        str [LCD_CURSOR], rC

        dec rB
        set rD, 6
        str [LCD_CURSOR_X], rD
        str [LCD_CURSOR_Y], rB
        set rC, 0xD1 ; LEROY!!!
        str [LCD_CURSOR], rC
        set rC, 'T'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, 'n'
        str [LCD_CURSOR], rC
        set rC, 'y'
        str [LCD_CURSOR], rC
        set rC, 'A'
        str [LCD_CURSOR], rC
        set rC, 'T'
        str [LCD_CURSOR], rC
        cal !delay
        str [LCD_CLEAR_SCREEN], rZ
        set rD, rA
        cal !display_ground
        cal !draw_player
        set rA, rD
        dec rA
        cmp rA, 0
        jg !move_banner
    
    cal !delay
    cal !delay
    cal !play_aero_sound
    jmp !game_loop

; Displayed when game is at an end
; game ends when there is no lives remaining.
!game_over
    ; reset delay
    lod rA, [delay_amt]
    set rA, INITIAL_SPEED
    str [delay_amt], rA

    cal !display_ground
    cal !play_game_over_sound
    cal !delay
    set rA, 11
    !move_game_over
        set rB, 1
        str [LCD_CURSOR_X], rA
        str [LCD_CURSOR_Y], rB
        set rC, 'G'
        str [LCD_CURSOR], rC
        set rC, 'a'
        str [LCD_CURSOR], rC
        set rC, 'm'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, ' '
        str [LCD_CURSOR], rC
        set rC, 'O'
        str [LCD_CURSOR], rC
        set rC, 'v'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, 'r'
        str [LCD_CURSOR], rC
        dec rB ; move cursor up 1
        set rD, 6
        str [LCD_CURSOR_X], rD
        str [LCD_CURSOR_Y], rB
        set rC, 0xD1 ; LEROY
        str [LCD_CURSOR], rC
        set rC, 'T'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, 'e'
        str [LCD_CURSOR], rC
        set rC, 'n'
        str [LCD_CURSOR], rC
        set rC, 'y'
        str [LCD_CURSOR], rC
        set rC, 'A'
        str [LCD_CURSOR], rC
        set rC, 'T'
        str [LCD_CURSOR], rC
        cal !delay
        str [LCD_CLEAR_SCREEN], rZ
        set rD, rA
        cal !display_ground
        set rA, rD
        dec rA
        cmp rA, 0
        jg !move_game_over
    cal !delay
    cal !play_game_over_sound
    jmp !game_over ; loop back over

!play_aero_sound
    set rE, SOUND_G4
    str [BUZZER_RIGHT], rE
    cal !sfx_delay_teeny
    str [BUZZER_RIGHT], rZ
    ret

!play_intro_sound
    set rE, SOUND_C5
    str [BUZZER_RIGHT], rE
    cal !sfx_delay_teeny
    str [BUZZER_RIGHT], rZ
    ret

!play_game_over_sound
    set rA, SOUND_G4
    str [BUZZER_RIGHT], rA
    cal !sfx_delay_big

    set rA, SOUND_CSHARP4 
    str [BUZZER_RIGHT], rA
    cal !sfx_delay_big

    str [BUZZER_RIGHT], rZ
    ret

!play_jump_sound
    set rA, SOUND_C4  
    str [BUZZER_RIGHT], rA
    cal !sfx_delay_small

    set rA, SOUND_CSHARP4 
    str [BUZZER_RIGHT], rA
    cal !sfx_delay_big

    str [BUZZER_RIGHT], rZ
    ret

; Main delay loop
; Because the virtual machine is so fast..
!delay
    lod rD, [delay_amt]
    set rE, 0                       
    !inner_delay_loop
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_delay_loop
    set rE, 0
    ret

!sfx_delay_teeny
    set rD, 500
    set rE, 0                       
    !inner_sfx_teeny_loop
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_sfx_teeny_loop
    set rE, 0
    ret

!sfx_delay_small
    set rD, 500
    set rE, 0                       
    !inner_sfx_small_loop
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_sfx_small_loop
    set rE, 0
    ret

!sfx_delay_big
    set rD, 750
    set rE, 0                       
    !inner_sfx_big_loop
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_sfx_big_loop
    set rE, 0
    ret