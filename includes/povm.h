#ifndef POVM_H
#define POVM_H

int povm_execute(FILE* fd, void* stack);

#define COMMAND_PUSH_ROOT_STACK 0x01


#define ERROR_UNIMPLEMENTED_OPCODE -5
#define ERROR_UNKNOWN_OPCODE -4


#endif

