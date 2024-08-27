#ifndef POVM_H
#define POVM_H

#include <datum.h>

struct povm_state {
    FILE* base_fd;
    void* base_pointer;
    FILE* fd;
    union udatum* stack;
    int32_t* types;
};

// Debug features:
// Disable debug commands
// Enable debug commands
// Verbose


int povm_execute(FILE* fd, union udatum* stack, int32_t* types);

#define COMMAND_PUSH_ROOT_STACK 0x01
// implemented >>>>
#define COMMAND_PUSH 0x02 // type:value
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
// implemented <<<<
#define COMMAND_CALL 0x0e // offset
#define COMMAND_RET 0x0f
#define COMMAND_JMP 0x10 // offset
// not tested <<<<
#define COMMAND_CMP 0x11
#define COMMAND_JE 0x12
#define COMMAND_JNE 0x13
#define COMMAND_JGE 0x14
#define COMMAND_JLE 0x15
#define COMMAND_JG 0x16
#define COMMAND_JL 0x17
// implemented >>>>
#define COMMAND_PRINT 0x18
// implemented <<<<
#define COMMAND_READ 0x19 // read text from stdin and determine type
#define COMMAND_FEED 0x1a // type, binary read

// implemented >>>>
#define COMMAND_DEBUG_PRINT 0x1b
// implemented <<<<
#define COMMAND_DEBUG_ASSERT 0x1c
// not tested <<<<

// #define COMMAND_CAST 0x1d // type, must be implemented
// #define COMMAND_SET_TYPE ...0x1e // type punning
// #define COMMAND_PUSH_TYPE ...0x1f // save typeid

#define ERROR_UNIMPLEMENTED_OPCODE -5
#define ERROR_UNKNOWN_OPCODE -4


#endif

