.const PORT_A_DIR 0x8000
.const PORT_B_DIR  0x8001
.const PORT_A 0x8002
.const PORT_B 0x8003
.const FADER_LEFT 0xA020
.const FADER_RIGHT 0xA021

.const SCALER 100

SET rA, 0xFFFF ; set port directions to input
STR [PORT_A_DIR], rA


STR [PORT_B_DIR], rZ

!loop
LOD rB, [FADER_LEFT]
LOD rC, [FADER_RIGHT]
MPY rB, SCALER
MPY rC, SCALER

SET rA, 0xFFFF
STR [PORT_B], rA
DLY rB
SET rA, 0x0000
STR [PORT_B], rA
DLY rC
JMP !loop


!main
JMP !main
