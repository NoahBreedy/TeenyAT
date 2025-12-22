@ifndef MY_MACRO
@define MY_MACRO

@define reg2 rB

@include "syntax/test.S"

!test_label
    set Ra, 0
    set rA, rB
    set rA, reg2 + 10

@endif
