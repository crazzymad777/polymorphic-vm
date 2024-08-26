#include <povm.h>

int povm_execute_command(FILE* fd, void* stack);

FILE* base_fd;
void* base_pointer;

int povm_execute(FILE* fd, void* stack) {
    base_fd = fd;
    base_pointer = stack;
    while (!feof(fd)) {
		int r = povm_execute_command(fd, stack);
		if (r == -42) {
			return -42;
		}

		if (r != 0) {
			return r;
		}
	}
    return 0;
}

int povm_execute_command(FILE* fd, void* stack) {
    int c = fgetc(fd);
	if (c != EOF) {
        if (c == COMMAND_PUSH_ROOT_STACK) {
            fprintf(stderr, "Polymorhpic VM halt!\n");
            fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
            return ERROR_UNIMPLEMENTED_OPCODE;
        } else {
			fprintf(stderr, "Polymorhpic VM halt!\n");
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			return ERROR_UNKNOWN_OPCODE;
		}
    }
    return 0;
}
