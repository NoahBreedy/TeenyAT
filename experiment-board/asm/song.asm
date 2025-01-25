.const PORT_A_DIR 0x8000
.const PORT_A 0x8002
.const PORT_B 0x8003
.const FADER_LEFT 0xA020
.const FADER_RIGHT 0xA021
.const BUZZER_LEFT 0xA010
.const BUZZER_RIGHT 0xA011
.const LCD_CLEAR_SCREEN 0xA001
.const LCD_CURSOR 0xA000 
.const LCD_CURSOR_XY 0xA00C

.const SOUND_LEFT_0 0x00C8
.const SOUND_RIGHT_1 0x80C8
.const SOUND_RIGHT_2 0x81F4
.const SOUND_RIGHT_3 0x8258
.const SOUND_RIGHT_4 0x8320

!init
set rA, 0xFF0F
str [PORT_A_DIR], rA
!main
    cal !reset_screen
    set rA, !BUTTON_STR
    cal !print_str

    cal !wait_for_button
    
    cal !reset_screen
    set rA, !PLAYING_STR
    cal !print_str

    cal !play_song

jmp !main

!reset_screen
    str [LCD_CLEAR_SCREEN], rZ
    str [LCD_CURSOR_XY], rZ
    ret

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


!wait_rB
    cal !kill_loop
    lup rB, !wait_rB
    ret

!kill_loop
 ;----- Edit this if your thing is tooo slowwww
        psh rA
        set rA, 0
        !delay_loop
            dly 10000
            inc rA
            cmp rA, 10
            jl !delay_loop
        pop rA
        ret
; -----------

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



;------------- MY MASTER PIECE -------------------
!play_song
    set rA, SOUND_LEFT_0
    str [BUZZER_LEFT], rA

    set rB, 20
    cal !wait_rB

    set rA, SOUND_RIGHT_1
    str [BUZZER_RIGHT], rA

    set rB, 20
    cal !wait_rB

    ; din dun din dun din dun
    set rC, 3
    !loop_0
        set rA, SOUND_RIGHT_2
        str [BUZZER_RIGHT], rA
        set rB, 5
        cal !wait_rB
        set rA, SOUND_RIGHT_3
        str [BUZZER_RIGHT], rA
        set rB, 15
        cal !wait_rB
        lup rC, !loop_0

    ; din dun din dun din dun
    set rC, 2
    !loop_1
        set rA, SOUND_RIGHT_1
        set rD, 25
    !loop_2
        str [BUZZER_RIGHT], rA
        set rB, 1
        cal !wait_rB
        add rA, 100
        lup rD, !loop_2

    set rD, 25
    !loop_3
        str [BUZZER_RIGHT], rA
        set rB, 1
        cal !wait_rB
        sub rA, 100
        lup rD, !loop_3
    lup rC, !loop_1

    set rA, SOUND_RIGHT_4
    str [BUZZER_RIGHT], rA
    set rB, 5
    cal !wait_rB

    add rA,500
    str [BUZZER_RIGHT], rA

    set rB, 5
    cal !wait_rB

    add rA,50
    str [BUZZER_RIGHT], rA
    set rB, 20
    cal !wait_rB

    set rD, 13
    !loop_4
        str [BUZZER_RIGHT], rA
        str [BUZZER_LEFT], rA
        set rB, 3
        cal !wait_rB
        sub rA, 100
        lup rD, !loop_4


    str [BUZZER_RIGHT], rA
    str [BUZZER_LEFT], rA
    set rB, 30
    cal !wait_rB
    
    str [BUZZER_RIGHT], rZ
    str [BUZZER_LEFT], rZ

    ret
;-----------------------------------------

!BUTTON_STR
'P''r' 'e' 's' 's' ' ' 'a' 'n' 'y' ' ' 'b' 'u' 't' 't' 'o' 'n' 0

!PLAYING_STR
'P''l''a''y''i''n''g' '.' '.' '.' 0