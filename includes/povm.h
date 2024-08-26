#ifndef POVM_H
#define POVM_H

int povm_execute(FILE* fd, void* stack);

#define COMMAND_PUSH_ROOT_STACK 0x01
#define COMMAND_PUSH 0x02
#define COMMAND_DROP 0x03
#define COMMAND_SWAP 0x04
#define COMMAND_DUP 0x05
#define COMMAND_ADD 0x06
#define COMMAND_SUB 0x07
#define COMMAND_MUL 0x08
#define COMMAND_DIV 0x09
#define COMMAND_REM 0x0a
#define COMMAND_BXOR 0x0b
#define COMMAND_BOR 0x0c
#define COMMAND_BAND 0x0d
#define COMMAND_CALL 0x0e // offset
#define COMMAND_RET 0x0f
#define COMMAND_JMP 0x10 // offset
#define COMMAND_CMP 0x11
#define COMMAND_JE 0x12
#define COMMAND_JNE 0x13
#define COMMAND_JGE 0x14
#define COMMAND_JLE 0x15
#define COMMAND_JG 0x16
#define COMMAND_JL 0x17
#define COMMAND_PRINT 0x18
#define COMMAND_READ 0x19
#define COMMAND_FEED 0x1a

#define ERROR_UNIMPLEMENTED_OPCODE -5
#define ERROR_UNKNOWN_OPCODE -4


#endif

