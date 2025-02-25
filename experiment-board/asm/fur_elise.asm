.const LCD_CURSOR 0xA000 

.const BUZZER_RIGHT 0xA011

.const DELAY_SHORT  750
.const DELAY_LONG   1500
.const WAIT_SHORT   1500
.const WAIT_LONG    3500

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

!main
    cal !draw_screen
    cal !play_fur_elise
    str [BUZZER_RIGHT], rZ
    !end
    jmp !end

!draw_screen
    set rA, 'F'
    str [LCD_CURSOR], rA
    set rA, 'u'
    str [LCD_CURSOR], rA
    set rA, 'r'
    str [LCD_CURSOR], rA
    set rA, ' '
    str [LCD_CURSOR], rA
    set rA, 'E'
    str [LCD_CURSOR], rA
    set rA, 'l'
    str [LCD_CURSOR], rA
    set rA, 'i'
    str [LCD_CURSOR], rA
    set rA, 's'
    str [LCD_CURSOR], rA
    set rA, 'e'
    str [LCD_CURSOR], rA
    ret

; ------------------------------------------
; "Für Elise"               
; ------------------------------------------
!play_fur_elise
    cal !phrase_1
    cal !phrase_2
    cal !phrase_3
    cal !phrase_4
    cal !phrase_5
    cal !phrase_6

    cal !phrase_7
    cal !phrase_8
    cal !phrase_9
    cal !phrase_10
    cal !phrase_11
    cal !phrase_12
    cal !phrase_13
    cal !phrase_14
    cal !phrase_15
    cal !phrase_16

    ret

    !phrase_1
        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_DSHARP5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_DSHARP5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_B4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_D5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_A4
        str [BUZZER_RIGHT], rA
        cal !delay_long
        ret

    !phrase_2
        set rA, SOUND_C4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_A4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_B4
        str [BUZZER_RIGHT], rA
        cal !delay_long
        ret

    !phrase_3
        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_A4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_B4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C5
        str [BUZZER_RIGHT], rA
        cal !delay_long
        ret
    
    !phrase_4
        cal !phrase_1
        ret
    
    !phrase_5
        cal !phrase_2
        ret
    
    !phrase_6
        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_B4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_A4
        str [BUZZER_RIGHT], rA
        cal !delay_short
        ret

    !phrase_7
        set rA, SOUND_B4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_D5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_long

        ret

    !phrase_8
        set rA, SOUND_G4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_F4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_D4
        str [BUZZER_RIGHT], rA
        cal !delay_long

        ret

    !phrase_9
        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_D4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C4
        str [BUZZER_RIGHT], rA
        cal !delay_long

        ret

    !phrase_10
        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_D4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C4
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E4
        str [BUZZER_RIGHT], rA
        cal !delay_long
        cal !delay_long

        ret
    
    !phrase_11
        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_DSHARP5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_DSHARP5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_E5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_D5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_C5
        str [BUZZER_RIGHT], rA
        cal !delay_short

        set rA, SOUND_A4
        str [BUZZER_RIGHT], rA
        cal !delay_long
        ret
        ret
    
    !phrase_12
        cal !phrase_2
        ret
    
    !phrase_13
        cal !phrase_3
        ret

    !phrase_14
        cal !phrase_1
        ret
    
    !phrase_15
        cal !phrase_2
        ret
    
    !phrase_16
        cal !phrase_6
        cal !delay_long
        ret
    

!delay_short
    set rD, DELAY_SHORT
    set rE, 0                       
    !inner_delay_short
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_delay_short
    set rE, 0
    ret

!delay_long
    set rD, DELAY_LONG
    set rE, 0                       
    !inner_delay_long
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_delay_long
    set rE, 0
    ret

!wait_long
    set rD, WAIT_LONG
    set rE, 0                       
    !inner_wait_long
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_wait_long
    set rE, 0
    ret

!wait_short
    set rD, WAIT_SHORT
    set rE, 0                       
    !inner_wait_short
        inc rE
        dly rE
        cmp rE, rD 
        jl !inner_wait_short
    set rE, 0
    ret