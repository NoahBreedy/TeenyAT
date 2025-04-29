; John Conways Game of Life
; Author: Noah Breedy

; TeenyAT Constants
.const PORT_A_DIR   0x8000
.const PORT_B_DIR   0x8001
.const PORT_A       0x8002
.const PORT_B       0x8003
.const RAND         0x8010
.const RAND_BITS    0x8011

; LCD Peripherals
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
.const MOUSEB 0xFFFD
.const TERM 0xFFFF
.const KEY 0xFFFE

.const TOP_COLOR 4032
.const LOW_COLOR 63
.const MUTATE_AMT 1

!main   
    cal !randomize_screen       ; fill with random green and black pixels
!conways_loop
    cal !wipe_buffer            ; clear the update buffer
    cal !game_of_life           ; run our simulation 
    str [UPDATE], rZ            ; swap our buffers
    jmp !conways_loop

;--------------------------------
!randomize_screen   
    psh rA
    set rA, 4095
    psh rB
!random_fill_loop
    lod rC, [RAND]
    mod rC, 5
    set rB, rZ
    jne !draw_pixel
    lod rB, [!COLOR_VALUE]
!draw_pixel
    str [rA + UPDATESCREEN], rB
    lup rA, !random_fill_loop
    str [UPDATESCREEN], rZ       ; just make the first one zero 
    str [UPDATE], rZ             ; render to the screen
    pop rB
    pop rA
    ret

;--------------------------------
!wipe_buffer
    psh rA
    set rA, 63
    str [X1], rZ
    str [Y1], rZ
    str [X2], rA
    str [Y2], rA
    str [RECT], rZ
    pop rA
    ret

;--------------------------------
!game_of_life
    psh rA
    psh rB
    set rA, -1
!process_pixel
    inc rA
    lod rB, [rA + LIVESCREEN]
    cmp rB, rZ
    jne !process_alive_pixle

!process_dead_pixle
    cal !get_neighbors_rB
    cmp rB, 3                   ; if dead and has 3 alive neighbors become alive
    jne !end_process
    lod rB, [!COLOR_VALUE]
    str [rA + UPDATESCREEN], rB
    jmp !end_process

!process_alive_pixle 
    cal !get_neighbors_rB
    cmp rB, 2                   ; if alive and has less than 2 kill
    jl !kill_pixel

    cmp rB, 3                   ; if alive and has more than 3 kill
    jg !kill_pixel

    jmp !keep_pixel             ; else keep it alive

!kill_pixel
    str [rA + UPDATESCREEN], rZ
    jmp !end_process

!keep_pixel
    lod rB, [!COLOR_VALUE]
    str [rA + UPDATESCREEN], rB

!end_process  
    ;cal !update_color
    cmp rA, 4096
    jl !process_pixel
    pop rB
    pop rA
    ret

;--------------------------------
; Count the number of alive neighbors putting count into rB
!get_neighbors_rB
    psh rC
    psh rD
    psh rE
    set rD , -1
    set rB, rZ
!get_neighbors_loop
    inc rD
    cmp rD, 8
    jge !end_get_neighbors

    set rC, rA
    lod rE, [rD + !neighbor_index]
    add rC, rE

; bounds checking our index so we dont get weird numbers
    cmp rC, rZ
    jl !get_neighbors_loop
    cmp rC, 4095
    jg !get_neighbors_loop

    lod rC, [rC + LIVESCREEN]   ; get the color of the neighbor
    
    cmp rC, rZ
    je !get_neighbors_loop
    inc rB
    jmp !get_neighbors_loop

!end_get_neighbors
    pop rE
    pop rD
    pop rC
    ret

;--------------------------------
!update_color
    psh rA
    lod rA, [!COLOR_VALUE]
    add rA, MUTATE_AMT
    cmp rA, TOP_COLOR
    jl !end_update_color
    set rA, LOW_COLOR
!end_update_color
    str [!COLOR_VALUE], rA
    pop rA
    ret

;------- DATA SECTION -----------
; (allows for left modulus wrapping)
!neighbor_index
.raw -1 1 64 -64 -65 65 -63 63

!COLOR_VALUE 
.raw 0xA81 

; green 1796
; LAVENDER 0xA81 