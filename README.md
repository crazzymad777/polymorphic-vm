# PoVM

povm [bytecode file]

Datum types (a.k.a. primitive data types):
void
boolean
(u)int8/16/32/64
float32/64
charcode (UTF-32)

Complex Data Type:
Vector or Stack...
String (Vector of charcodes?)
Enum just a number...
Union doesn't matter because every Datum has a type...
Struct/Tuples/Arrays can be described by Vector or Stack
What about Interfaces AND Functions?
Function is PoVM bytecode, can be loaded by loadstring


Instruction set:
push root Stack
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
typeof (push type of *sp)

Complex Data Type instruction set:
load uint64,{type:constant,...}
loadstring uint64,bytes

enter (Stack) STACK POINTER ALWAYS POINTS TO STACK TOP
exit (Stack)
create (Stack/Vector...)
join (Stack One union Stack Two)


Summary:
Root Stack cannot be dropped
Any other stack drops recursively

