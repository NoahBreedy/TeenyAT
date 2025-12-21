@ifndef MY_MACRO
@define MY_MACRO

@include "test.S"

!test_label
    set Ra, 0
    set rA, rB
    set rA, rB + 10

@endif
