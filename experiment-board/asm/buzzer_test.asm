.const PORT_A_DIR 0x8000
.const PORT_A 0x8002
.const PORT_B 0x8003
.const FADER_LEFT 0xA020
.const FADER_RIGHT 0xA021
.const BUZZER_LEFT 0xA010
.const BUZZER_RIGHT 0xA011


!main

; Store into the buzzers 
lod rA, [FADER_LEFT]
mpy rA, 100
str [PORT_A], rA
str [BUZZER_LEFT], rA

lod rA, [FADER_RIGHT]
mpy rA, 100
add rA, PORT_A_DIR  ; :) hey would u look at that 
str [PORT_B], rA
str [BUZZER_RIGHT], rA

jmp !main

