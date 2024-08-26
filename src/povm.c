#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <povm.h>

int povm_execute_command(FILE* fd, union udatum* stack, int32_t* types);

FILE* base_fd;
void* base_pointer;

int povm_execute(FILE* fd, union udatum* stack, int32_t* types) {
    base_fd = fd;
    base_pointer = stack;
    while (!feof(fd)) {
		int r = povm_execute_command(fd, stack, types);
		if (r == -42) {
			return -42;
		}

		if (r != 0) {
			return r;
		}
	}
    return 0;
}

int povm_execute_command(FILE* fd, union udatum* stack, int32_t* types) {
    int c = fgetc(fd);
    int64_t* sp = &stack->i64;
	if (c != EOF) {
        if (c == COMMAND_PUSH_ROOT_STACK) {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
            return ERROR_UNIMPLEMENTED_OPCODE;
        } else if (c == COMMAND_PUSH) {
            int type;
            int64_t value;
            int bytes = fread(&type, 4, 1, fd);
            if (bytes == 4) {
                int bytes = fread(&value, 8, 1, fd);
                if (bytes == 8) {
                    stack += 1;
                    types += 1;

                    stack->i64 = value;
                    *types = type;
                }
            }
        } else if (c == COMMAND_DROP) {
            // For CDT: must check internal counter of uses
            stack -= 1;
            types -= 1;
        } else if (c == COMMAND_SWAP) {
            int type = *(types-1);
            int64_t value = *(sp-1);
            *(types - 1) = *types;
            *(sp - 1) = stack->i64;
            *types = type;
            stack->i64 = value;
        } else if (c == COMMAND_DUP) {
            // For CDT: must increment internal counter of uses
            stack += 1;
            types += 1;
            *types = *(types - 1);
            *stack = *(stack - 1);
        } else if (c == COMMAND_ADD) {
            struct result r = datum_add(povm_datum(*(types-1), *(stack-1)), povm_datum(*(types), *stack));
            if (r.type == RESULT) {
                stack -= 1;
                types -= 1;
                *types = r.datum.type;
                *sp = r.datum.i64;
            } else {
                fprintf(stderr, "Polymorhpic VM halt!\n");
                fprintf(stderr, "Error! Datum error: 0x%x\n", r.error_code);
            }
        } else if (c == COMMAND_PRINT) {
            char buffer[32];
            datum_to_string(povm_datum(*types, *stack), buffer, 32);
            printf("%s", buffer);
        } else {
			fprintf(stderr, "Polymorhpic VM halt!\n");
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			return ERROR_UNKNOWN_OPCODE;
		}
    }
    return 0;
}
