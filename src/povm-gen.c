#include <povm-info.h>
#include <povm-gen.h>

#include <stdio.h>

int povm_gen_init_header(FILE* fd, int64_t stack_size) {
    int bytes = fprintf(fd, "PROTECT2");
    if (bytes != 8) {
        return POVM_GEN_IO_ERROR;
    }
    int w = fwrite(&stack_size, 8, 1, fd);
    if (w != 1) {
        return POVM_GEN_IO_ERROR;
    }
    return 0;
}

int povm_put_instruction(FILE* fd, char opcode, struct povm_operand* args) {
    enum povm_operand_type type = povm_info_get_operand_type(opcode);

    if (args == NULL) {
        if (type != NO_OPERANDS) {
            return POVM_GEN_ILLEGAL_ARGUMENT;
        }
    } else {
        if (args->t != type) {
            return POVM_GEN_ILLEGAL_ARGUMENT;
        }
    }

    int c = fputc(opcode, fd);
    if (c != opcode) {
        return POVM_GEN_IO_ERROR;
    }

    if (type == OFFSET) {
        int w = fwrite(&args->offset, sizeof(args->offset), 1, fd);
        if (w != 1) {
            return POVM_GEN_IO_ERROR;
        }
    } else if (type == TYPE) {
        int w = fwrite(&args->type, sizeof(args->type), 1, fd);
        if (w != 1) {
            return POVM_GEN_IO_ERROR;
        }
    } else if (type == DATUM) {
        int w = fwrite(&args->type, sizeof(args->type), 1, fd);
        if (w != 1) {
            return POVM_GEN_IO_ERROR;
        }

        w = fwrite(&args->value, sizeof(args->value), 1, fd);
        if (w != 1) {
            return POVM_GEN_IO_ERROR;
        }
    }
    return 0;
}
