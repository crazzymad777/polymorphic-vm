#ifndef POVM_GEN_H
#define POVM_GEN_H

#include <povm.h>

// Byte code file structure:
// PROTECT2
// stack_size in int64
// instructions

#define POVM_GEN_ILLEGAL_ARGUMENT -1
#define POVM_GEN_IO_ERROR -2

enum povm_operand_type {
  NO_OPERANDS,
  OFFSET,
  DATUM,
  TYPE
};

struct povm_operand {
    enum povm_operand_type t; // safety enum, caller must provide proper type, callee must check proper type
    union {
        int64_t offset;
        struct {
            int32_t type;
            int64_t value;
        };
    };
};

#include <stdio.h>

int povm_gen_init_header(FILE* fd, int stack_size);
int povm_put_instruction(FILE* fd, char opcode, struct povm_operand* args);

#endif
