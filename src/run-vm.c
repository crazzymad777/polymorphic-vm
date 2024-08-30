#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <povm.h>

int run_vm(FILE *fd, int close_fd) {
	const char* protect_field = "PROTECT2";
	char buffer[9] = {0};
	int b = fread(buffer, 8, 1, fd);
	if (b < 1) {
		fprintf(stderr, "Invalid header signature");
		return -1;
	}

	if (strcmp(buffer, protect_field) != 0) {
		fprintf(stderr, "Invalid header signature");
		return -2;
	}

	size_t stack_size = 0;
	fread(&stack_size, 8, 1, fd);

	if (stack_size == 0) {
		stack_size = 4096;
	}

	union udatum stack[stack_size / 8];
	int32_t types[stack_size / 8];

	int exit_code = povm_execute(fd, stack, types);
	if (exit_code == -42) {
		exit_code = 0;
	}

	if (stdin != fd) {
		if (close_fd) {
			fclose(fd);
		}
	}
	return exit_code;
}
