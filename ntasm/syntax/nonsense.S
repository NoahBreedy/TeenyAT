@ifndef MY_MACRO
@define MY_MACRO "hello"

@include "test.S"

.const my_const 10

.var yes "Hello"
.var me 'Hello'

set rA, my_const
set rb, rC
set rD, -my_const + rC
set r3, r3 + my_const

.var buffer[10]

lod rA, [my_const]
lod rB, [rA]
lod rC, [-my_const + rA]
lod r6, [r4 + my_const]

str [my_const], rA
str [rA], rB
str [-my_const + rA], rC
str [r4 + my_const], r6

psh rB
psh my_const
psh -10
psh -12 + rA
psh [rC], rA
psh [rD], rB - 17
psh [rE], 4

pop rA
pop rB
pop rD, [rA]

!main
    set rA, 10
    psh !main
    psh rA
    psh rA
.var my_var 10 11 '\n'
    psh rA

.raw 11 10 10 11 "hello"

bts sP, 0
bts pc, my_const
btc rb, rC
btf rD, -my_const + rC
bts pc, r3 + my_const

cal rA
cal !main
cal -!main
cal !main + rB

shf sP, 0
shf pc, my_const
shf rb, rC
shf rD, -my_const + rC
shf pc, r3 + my_const

shr rA, !main
shr rA, my_const
shl rA, 2
shl rA, -10

rot sP, 0
rot pc, my_const
rot rb, rC
rot rD, -my_const + rC
rot pc, r3 + my_const

rol rA, !main
rol rA, my_const
ror rA, 2
ror rA, -10

neg rA
neg PC

cmp rA, rb+10
cmp PC, -10

dly rA
dly rA, rB + 10

int rA + 12
rti

jmp rA
jne rA + 12
je rA - 13
jl -13 + rB
jle rA
jg rA
jge rA

lUp rA, rB

inc rA
dec rA
inv PC
ret

set rA, rB - -10

@endif
