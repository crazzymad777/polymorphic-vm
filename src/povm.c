#include <povm.h>

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
    return 0;
}
