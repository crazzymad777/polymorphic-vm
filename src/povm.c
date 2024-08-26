#include <stdint.h>
#include <stdio.h>
#include <povm.h>

int povm_execute_command(FILE* fd, int64_t* stack, int32_t* types);

FILE* base_fd;
void* base_pointer;

int povm_execute(FILE* fd, int64_t* stack, int32_t* types) {
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

int povm_execute_command(FILE* fd, int64_t* stack, int32_t* types) {
    int c = fgetc(fd);
	if (c != EOF) {
        if (c == COMMAND_PUSH_ROOT_STACK) {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
            return ERROR_UNIMPLEMENTED_OPCODE;
        } else  if (c == COMMAND_PUSH) {
            int type;
            int64_t value;
            int bytes = fread(&type, 4, 1, fd);
            if (bytes == 4) {
                int bytes = fread(&value, 8, 1, fd);
                if (bytes == 8) {
                    stack += 1;
                    types += 1;

                    *stack = value;
                    *types = type;
                }
            }
        } else {
			fprintf(stderr, "Polymorhpic VM halt!\n");
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			return ERROR_UNKNOWN_OPCODE;
		}
    }
    return 0;
}
