#include <povm-info.h>
#include <povm-gen.h>

#include <stdio.h>

void povm_gen_init_header(FILE* fd, int stack_size) {
    fprintf(fd, "PROTECT2");
    fwrite(&stack_size, 8, 1, fd);
}

void povm_put_instruction(FILE* fd, char opcode, struct povm_operand* args) {
    fputc(opcode, fd);
    enum povm_operand_type type = povm_info_get_operand_type(opcode);
    if (type == OFFSET) {
        fwrite(&args->offset, sizeof(args->offset), 1, fd);
    } else if (type == TYPE) {
        fwrite(&args->type, sizeof(args->type), 1, fd);
    } else if (type == DATUM) {
        fwrite(&args->type, sizeof(args->type), 1, fd);
        fwrite(&args->value, sizeof(args->value), 1, fd);
    }
}
