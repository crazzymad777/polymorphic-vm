# PoVM

povm [bytecode file]

Datum types (a.k.a. primitive data types):
void
boolean
(u)int8/16/32/64
float32/64
charcode (UTF-32)

Instruction set:
push typeid:constant
drop
swap
dup
add
sub
mul
div
rem
bxor
bor
band
call offset
ret
jmp offset
cmp
je
jne
jge
jle
jg
jl
print
read (textually)
feed type (read octets)
typeof (push typeid of *sp)


Next:
Interfaces
String
Enums
Unions
Vector
Struct/Tuples

