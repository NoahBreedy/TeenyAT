jmp !main

.variable counter 4

!main

    lod rA, [ counter ]

    dec rA

    je !end_program

    str [counter], rA

    jmp !main

!end_program