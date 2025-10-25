.const PORT_A 0x8002

set rA, rZ
set rB, 0xFFFF
!main
    lod rA, [ 0x9099 ]
    dly rB, 0xFFFF
    jmp !main
